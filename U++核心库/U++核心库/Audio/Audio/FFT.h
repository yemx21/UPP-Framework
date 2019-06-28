#ifndef FFT_H
#define FFT_H
#include "Config.h"
#include <math.h>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif
#ifndef M_2PI
#define M_2PI       2*M_PI
#endif

class  FastFourier
{
public:
	int wsize;
	int wstyle;
	FastFourier();
	~FastFourier();
	float* windowfactors;
	void do_window(int windowstyle, float* data, unsigned int samples);
	void do_fft(unsigned int samples,  // must be a power of 2
    	int inverse,         // 0 = forward FFT, 1 = inverse
    	float* repipein,     // array of input's real samples
    	float* imag_in,     // array of input's imag samples
    	float* repipeout,    // array of output's reals
    	float* imag_out);   // array of output's imaginaries

	void do_fft_direct(int windowstyle, unsigned int samples, bool mono, unsigned short channels,  // must be a power of 2
    	float* repipein[CHANNELS_MAX],     // array of input's real samples
    	float* repipeout[CHANNELS_MAX], float* imag_out[CHANNELS_MAX], float* power_out[CHANNELS_MAX]);   // array of power output

	int symmetry(int size, double *freq_real, double *freq_imag);
	unsigned int samples_to_bits(unsigned int samples);
	unsigned int reverse_bits(unsigned int index, unsigned int bits);
};
#endif