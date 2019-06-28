#include "FFT.h"
FastFourier::FastFourier():windowfactors(NULL)
{
	wsize=-1;
	wstyle=-1;
}
FastFourier::~FastFourier()
{
	if(windowfactors)
	{
		delete windowfactors;
		windowfactors=NULL;
	}
}
void FastFourier::do_fft(unsigned int samples,  // must be a power of 2
    	int inverse,         // 0 = forward FFT, 1 = inverse
    	float *repipein,     // array of input's real samples
    	float *imag_in,     // array of input's imag samples
    	float *repipeout,    // array of output's reals
    	float*imag_out)
{
    unsigned int num_bits;    // Number of bits needed to store indices
    register unsigned int i, j, k, n;
    unsigned int block_size, block_end;

    double angle_numerator = M_2PI;
    double tr, ti;     // temp real, temp imaginary

    if(inverse)
        angle_numerator = -angle_numerator;

    num_bits = samples_to_bits(samples);

// Do simultaneous data copy and bit-reversal ordering into outputs
    for(i = 0; i < samples; i++)
    {
        j = reverse_bits(i, num_bits);
        repipeout[j] = repipein[i];
        imag_out[j] = (imag_in == NULL) ? 0.0 : imag_in[i];
    }

// Do the FFT itself

    block_end = 1;
    double delta_angle;
    double sm2;
    double sm1;
    double cm2;
    double cm1;
    double w;
    double ar[3], ai[3];
    for(block_size = 2; block_size <= samples; block_size <<= 1)
    {
        delta_angle = angle_numerator / (double)block_size;
        sm2 = sin(-2 * delta_angle);
        sm1 = sin(-delta_angle);
        cm2 = cos(-2 * delta_angle);
        cm1 = cos(-delta_angle);
        w = 2 * cm1;

        for(i = 0; i < samples; i += block_size)
        {
            ar[2] = cm2;
            ar[1] = cm1;

            ai[2] = sm2;
            ai[1] = sm1;

            for(j = i, n = 0; n < block_end; j++, n++)
            {
                ar[0] = w * ar[1] - ar[2];
                ar[2] = ar[1];
                ar[1] = ar[0];

                ai[0] = w * ai[1] - ai[2];
                ai[2] = ai[1];
                ai[1] = ai[0];

                k = j + block_end;
                tr = ar[0] * repipeout[k] - ai[0] * imag_out[k];
                ti = ar[0] * imag_out[k] + ai[0] * repipeout[k];

                repipeout[k] = repipeout[j] - tr;
                imag_out[k] = imag_out[j] - ti;

                repipeout[j] += tr;
                imag_out[j] += ti;
            }
        }

        block_end = block_size;
    }

// Normalize if inverse transform

    if(inverse)
    {
        double denom = (double)samples;

        for (i = 0; i < samples; i++)
        {
            repipeout[i] /= denom;
            imag_out[i] /= denom;
        }
    }
}
void FastFourier::do_fft_direct(int windowstyle, unsigned int samples, bool mono, unsigned short channels,      // must be a power of 2
    float* repipein[CHANNELS_MAX],         // array of input's real samples
    float* repipeout[CHANNELS_MAX], float* imag_out[CHANNELS_MAX], float* power_out[CHANNELS_MAX])     // array of power output
{
	int n, i, i1, j, k, i2, l, l1, l2;
    float c1, c2, tx, ty, t1, t2, u1, u2, z;

	int m=samples_to_bits(samples);
    // Calculate the number of points
    n = 1;
    for (i = 0; i < m; i++) n *= 2;

	if(mono)
	{
		// Do the bit reversal
		i2 = n >> 1;
		j = 0;
		for (i = 0; i < n - 1; i++)
		{
			if (i < j)
			{
				tx=0.0f;
				repipeout[0][i]=0.0f;
				for(unsigned int ch=0;ch<channels;ch++)
				{			
					tx +=repipein[ch][i];
					repipeout[0][i]+=repipein[ch][j];
				}
				tx/=channels;
				repipeout[0][i]/=channels;
				ty = 0.0f;				
				imag_out[0][i]=0.0f;
				repipeout[0][j]=tx;
				imag_out[0][j]=0.0f;
			}
			k = i2;

			while (k <= j)
			{
				j -= k;
				k >>= 1;
			}
			j += k;
		}

		// Compute the FFT 
		c1 = -1.0f;
		c2 = 0.0f;
		l2 = 1;
		for (l = 0; l < m; l++)
		{
			l1 = l2;
			l2 <<= 1;
			u1 = 1.0f;
			u2 = 0.0f;
			for (j = 0; j < l1; j++)
			{
				for (i = j; i < n; i += l2)
				{
					i1 = i + l1;
					t1 = u1 * repipeout[0][i1] - u2 * imag_out[0][i1];
					t2 = u1 * imag_out[0][i1] + u2 * repipeout[0][i1];
					repipeout[0][i1] = repipeout[0][i] - t1;
					imag_out[0][i1] = imag_out[0][i] - t2;
					repipeout[0][i] += t1;
					imag_out[0][i] += t2;
				}
				z = u1 * c1 - u2 * c2;
				u2 = u1 * c2 + u2 * c1;
				u1 = z;
			}
			c2 =sqrtf((1.0f - c1) / 2.0f);
			c1 = sqrtf((1.0f + c1) / 2.0f);
		}

		for(j=0;j<samples/2;j++)
		{
			power_out[0][j]=sqrt(repipeout[0][j] * repipeout[0][j]+imag_out[0][j] * imag_out[0][j]);
		}
	}
	else
	{
		for(unsigned int ch=0;ch<channels;ch++)
		{	
			// Do the bit reversal
			i2 = n >> 1;
			j = 0;

			for (i = 0; i < n - 1; i++)
			{
				if (i < j)
				{
					tx =repipein[ch][i];
					repipeout[ch][i]=repipein[ch][j];
					ty = 0.0f;				
					imag_out[ch][i]=0.0f;
					repipeout[ch][j]=tx;
					imag_out[ch][j]=0.0f;
				}
				k = i2;

				while (k <= j)
				{
						j -= k;
						k >>= 1;
				}
				j += k;
			}
			// Compute the FFT 
			c1 = -1.0f;
			c2 = 0.0f;
			l2 = 1;
			for (l = 0; l < m; l++)
			{
				l1 = l2;
				l2 <<= 1;
				u1 = 1.0f;
				u2 = 0.0f;
				for (j = 0; j < l1; j++)
				{
					for (i = j; i < n; i += l2)
					{
						i1 = i + l1;
						t1 = u1 * repipeout[ch][i1] - u2 * imag_out[ch][i1];
						t2 = u1 * imag_out[ch][i1] + u2 * repipeout[ch][i1];
						repipeout[ch][i1] = repipeout[ch][i] - t1;
						imag_out[ch][i1] = imag_out[ch][i] - t2;
						repipeout[ch][i] += t1;
						imag_out[ch][i] += t2;
					}
					z = u1 * c1 - u2 * c2;
					u2 = u1 * c2 + u2 * c1;
					u1 = z;
				}
				c2 =sqrtf((1.0f - c1) / 2.0f);
				c1 = sqrtf((1.0f + c1) / 2.0f);
			}

			for(j=0;j<samples/2;j++)
			{
				power_out[ch][j]=sqrt(repipeout[ch][j] * repipeout[ch][j]+imag_out[ch][j] * imag_out[ch][j]);
			}
		}
	}
}


unsigned int FastFourier::samples_to_bits(unsigned int samples)
{
    register unsigned int i;

    for(i = 0; ; i++)
    {
        if(samples & (1 << i))
            return i;
    }
	return i;
}
unsigned int FastFourier::reverse_bits(unsigned int index, unsigned int bits)
{
    register unsigned int i, rev;

    for(i = rev = 0; i < bits; i++)
    {
        rev = (rev << 1) | (index & 1);
        index >>= 1;
    }

    return rev;
}
int FastFourier::symmetry(int size, double *freq_real, double *freq_imag)
{
    int h = size / 2;
    for(register int i = h + 1; i < size; i++)
    {
        freq_real[i] = freq_real[size - i];
        freq_imag[i] = -freq_imag[size - i];
    }
	return 0;
}
void FastFourier::do_window(int windowstyle, float* data, unsigned int samples)
{
	if(windowstyle==0)return;
	bool newcreate=true;
	if(wsize!=samples||wstyle!=windowstyle)newcreate=true;else newcreate=false;
	if(newcreate)
	{
		if(windowfactors)
		{
			delete windowfactors;
			windowfactors=NULL;
		}
		windowfactors=new float[samples];
		memset(windowfactors,0,sizeof(float)*samples);
		if(windowstyle==1)//HammingWindow
		{
			for(int i=0;i<samples;i++)
			{
				windowfactors[i]=0.54-0.46*cos(M_2PI*i/(samples-1));
				data[i]*=windowfactors[i];
			}
		}
		else if(windowstyle==2)//HannWindow
		{
			for(int i=0;i<samples;i++)
			{
				windowfactors[i]=0.5*(1-cos(M_2PI*i/(samples-1)));
				data[i]*=windowfactors[i];
			}
		}
		else if(windowstyle==3)//BlackmannHarrisWindow
		{
			for(int i=0;i<samples;i++)
			{				
				windowfactors[i]=0.35875 - (0.48829 * cos(M_2PI * i/ (samples - 1))) + (0.14128 * cos(2*M_2PI * i / (samples - 1))) - (0.01168 * cos(2*M_2PI *i / (samples - 1)));
				data[i]*=windowfactors[i];
			}
		}
	}
	else
	{
		for(int i=0;i<samples;i++)
		{
			data[i]*=windowfactors[i];
		}
	}
	if(newcreate){wsize=samples;wstyle=windowstyle;}
}