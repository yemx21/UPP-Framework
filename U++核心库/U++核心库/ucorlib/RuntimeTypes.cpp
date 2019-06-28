#include "RuntimeTypes.h"
#include <memory>
#include <cmath>

#pragma warning(disable: 4334)
#pragma warning(disable: 4244)
#pragma warning(disable: 4800)

#define CLAMP(V, TYPE)  (V<std::numeric_limits<TYPE>::min() ? std::numeric_limits<TYPE>::min() : V>std::numeric_limits<TYPE>::max() ? std::numeric_limits<TYPE>::max() : (TYPE)V)

#define UCLAMP(V, TYPE)  (V<0 ? 0 : V>std::numeric_limits<TYPE>::max() ? std::numeric_limits<TYPE>::max() : (TYPE)V)


#define CLAMP24(V, TYPE)  (V<std::numeric_limits<TYPE>::min().operator int() ? std::numeric_limits<TYPE>::min() : V>std::numeric_limits<TYPE>::max().operator int() ? std::numeric_limits<TYPE>::max() : (TYPE)V)

#define UCLAMP24(V, TYPE)  (V<0 ? 0 : V>std::numeric_limits<TYPE>::max().operator int() ? std::numeric_limits<TYPE>::max() : (TYPE)V)


Int24_t::Int24_t()
{
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;
}

Int24_t::Int24_t(volatile const Int24_t& val)
{
	value[0] = val.value[0];
	value[1] = val.value[1];
	value[2] = val.value[2];
}

Int24_t::Int24_t(int input)
{
#ifdef LITTLE_ENDIAN
	value[0] = ((unsigned char*)&input)[0];
	value[1] = ((unsigned char*)&input)[1];
	value[2] = ((unsigned char*)&input)[2];
#else
	value[0] = ((unsigned char*)&input)[2];
	value[1] = ((unsigned char*)&input)[1];
	value[2] = ((unsigned char*)&input)[0];
#endif
}

Int24_t::operator bool() const
{
	return (int)*this != 0;
}

Int24_t::operator int() const
{
#ifdef LITTLE_ENDIAN
	if (value[2] & 0x80) {
		return (0xff << 24) | (value[2] << 16)
			| (value[1] << 8)
			| value[0];
	}
	else {
		return (value[2] << 16)
			| (value[1] << 8)
			| value[0];
	}
#else
	if (value[0] & 0x80) {
		return (0xff << 24) | (value[0] << 16)
			| (value[1] << 8)
			| value[2];
	}
	else {
		return (value[0] << 16)
			| (value[1] << 8)
			| value[2];
	}
#endif
}

Int24_t::operator Int8() const
{
	int v = operator int();
	return CLAMP(v, Int8);
}

Int24_t::operator UInt8() const
{
	int v = operator int();
	return UCLAMP(v, UInt8);
}

Int24_t::operator Int16() const
{
	int v = operator int();
	return CLAMP(v, Int16);
}

Int24_t::operator UInt16() const
{
	int v = operator int();
	return UCLAMP(v, UInt16);
}

Int24_t::operator UInt24_t() const
{
	int v = operator int();

	return UCLAMP24(v, UInt24);
}

Int24_t::operator UInt() const
{
	Int64 v = operator Int64();
	return UCLAMP(v, UInt);
}

Int24_t::operator long() const
{
	return (long)operator int();
}

Int24_t::operator ULong() const
{
	return (ULong)operator int();
}

Int24_t::operator Int64() const
{
	return (Int64)operator int();
}

Int24_t::operator UInt64() const
{
	return (UInt64)operator int();
}

Int24_t::operator Int128_t() const
{
	return (Int128_t)operator int();
}

Int24_t::operator UInt128_t() const
{
	return (UInt128_t)operator UInt();
}

Int24_t::operator float() const
{
	return (float)operator int();
}

Int24_t::operator long double() const
{
	return (long double)operator int();
}

Int24_t& Int24_t::operator= (const Int24_t& input)
{

#ifdef LITTLE_ENDIAN
	value[0] = input.value[0];
	value[1] = input.value[1];
	value[2] = input.value[2];
#else
	value[0] = input.value[2];
	value[1] = input.value[1];
	value[2] = input.value[0];
#endif
	return *this;
}

Int24_t& Int24_t::operator= (const int input)
{
#ifdef LITTLE_ENDIAN
	value[0] = ((unsigned char*)&input)[0];
	value[1] = ((unsigned char*)&input)[1];
	value[2] = ((unsigned char*)&input)[2];
#else
	value[0] = ((unsigned char*)&input)[2];
	value[1] = ((unsigned char*)&input)[1];
	value[2] = ((unsigned char*)&input)[0];
#endif
	return *this;
}

Int24_t Int24_t::operator+ (const Int24_t& val) const
{
	return Int24_t((int)*this + (int)val);
}

Int24_t Int24_t::operator- (const Int24_t& val) const
{
	return Int24_t((int)*this - (int)val);
}

Int24_t Int24_t::operator* (const Int24_t& val) const
{
	return Int24_t((int)*this * (int)val);
}

Int24_t Int24_t::operator/ (const Int24_t& val) const
{
	return Int24_t((int)*this / (int)val);
}

Int24_t& Int24_t::operator+= (const Int24_t& val)
{
	*this = *this + val;
	return *this;
}

Int24_t& Int24_t::operator-= (const Int24_t& val)
{
	*this = *this - val;
	return *this;
}

Int24_t& Int24_t::operator*= (const Int24_t& val)
{
	*this = *this * val;
	return *this;
}

Int24_t& Int24_t::operator/= (const Int24_t& val)
{
	*this = *this / val;
	return *this;
}

Int24_t Int24_t::operator>> (const int val) const
{
	return Int24_t((int)*this >> val);
}

Int24_t Int24_t::operator<< (const int val) const
{
	return Int24_t((int)*this << val);
}

bool Int24_t::operator! () const
{
	return !((int)*this);
}

Int24_t Int24_t::operator- ()
{
	return Int24_t(-(int)*this);
}

bool Int24_t::operator== (const Int24_t& val) const
{
	return (int)*this == (int)val;
}

bool Int24_t::operator!= (const Int24_t& val) const
{
	return (int)*this != (int)val;
}

bool Int24_t::operator>= (const Int24_t& val) const
{
	return (int)*this >= (int)val;
}

bool Int24_t::operator<= (const Int24_t& val) const
{
	return (int)*this <= (int)val;
}


UInt24_t::UInt24_t()
{
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;
}

UInt24_t::UInt24_t(volatile const UInt24_t& val)
{
	value[0] = val.value[0];
	value[1] = val.value[1];
	value[2] = val.value[2];
}

UInt24_t::UInt24_t(const int input)
{
#ifdef LITTLE_ENDIAN
	value[0] = ((unsigned char*)&input)[0];
	value[1] = ((unsigned char*)&input)[1];
	value[2] = ((unsigned char*)&input)[2];
#else
	value[0] = ((unsigned char*)&input)[2];
	value[1] = ((unsigned char*)&input)[1];
	value[2] = ((unsigned char*)&input)[0];
#endif
}

UInt24_t::operator bool() const
{
	return (int)*this != 0;
}

UInt24_t::operator int() const
{
#ifdef LITTLE_ENDIAN
	return (value[2] << 16)
		| (value[1] << 8)
		| value[0];
#else
	return (value[0] << 16)
		| (value[1] << 8)
		| value[2];
#endif
}

UInt24_t::operator Int8() const
{
	int v = operator int();
	return CLAMP(v, Int8);
}

UInt24_t::operator UInt8() const
{
	int v = operator int();
	return UCLAMP(v, UInt8);
}

UInt24_t::operator Int16() const
{
	int v = operator int();
	return CLAMP(v, Int16);
}

UInt24_t::operator UInt16() const
{
	int v = operator int();
	return UCLAMP(v, UInt16);
}

UInt24_t::operator Int24_t() const
{
	int v = operator int();
	return CLAMP24(v, Int24);
}

UInt24_t::operator UInt() const
{
	Int64 v = operator Int64();
	return UCLAMP(v, UInt);
}

UInt24_t::operator long() const
{
	return (long)operator int();
}

UInt24_t::operator ULong() const
{
	return (ULong)operator int();
}

UInt24_t::operator Int64() const
{
	return (Int64)operator int();
}

UInt24_t::operator UInt64() const
{
	return (UInt64)operator int();
}

UInt24_t::operator Int128_t() const
{
	return (Int128_t)operator int();
}

UInt24_t::operator UInt128_t() const
{
	return (UInt128_t)operator UInt();
}

UInt24_t::operator float() const
{
	return (float)operator int();
}

UInt24_t::operator long double() const
{
	return (long double)operator int();
}

UInt24_t& UInt24_t::operator= (const UInt24_t& input)
{

#ifdef LITTLE_ENDIAN
	value[0] = input.value[0];
	value[1] = input.value[1];
	value[2] = input.value[2];
#else
	value[0] = input.value[2];
	value[1] = input.value[1];
	value[2] = input.value[0];
#endif
	return *this;
}

UInt24_t& UInt24_t::operator= (const int input)
{
#ifdef LITTLE_ENDIAN
	value[0] = ((unsigned char*)&input)[0];
	value[1] = ((unsigned char*)&input)[1];
	value[2] = ((unsigned char*)&input)[2];
#else
	value[0] = ((unsigned char*)&input)[2];
	value[1] = ((unsigned char*)&input)[1];
	value[2] = ((unsigned char*)&input)[0];
#endif
	return *this;
}

UInt24_t UInt24_t::operator+ (const UInt24_t& val) const
{
	return UInt24_t((int)*this + (int)val);
}

UInt24_t UInt24_t::operator- (const UInt24_t& val) const
{
	return UInt24_t((int)*this - (int)val);
}

UInt24_t UInt24_t::operator* (const UInt24_t& val) const
{
	return UInt24_t((int)*this * (int)val);
}

UInt24_t UInt24_t::operator/ (const UInt24_t& val) const
{
	return UInt24_t((int)*this / (int)val);
}

UInt24_t& UInt24_t::operator+= (const UInt24_t& val)
{
	*this = *this + val;
	return *this;
}

UInt24_t& UInt24_t::operator-= (const UInt24_t& val)
{
	*this = *this - val;
	return *this;
}

UInt24_t& UInt24_t::operator*= (const UInt24_t& val)
{
	*this = *this * val;
	return *this;
}

UInt24_t& UInt24_t::operator/= (const UInt24_t& val)
{
	*this = *this / val;
	return *this;
}

UInt24_t UInt24_t::operator>> (const int val) const
{
	return UInt24_t((int)*this >> val);
}

UInt24_t UInt24_t::operator<< (const int val) const
{
	return UInt24_t((int)*this << val);
}

bool UInt24_t::operator! () const
{
	return !((int)*this);
}

UInt24_t UInt24_t::operator- ()
{
	return UInt24_t(-(int)*this);
}

bool UInt24_t::operator== (const UInt24_t& val) const
{
	return (int)*this == (int)val;
}

bool UInt24_t::operator!= (const UInt24_t& val) const
{
	return (int)*this != (int)val;
}

bool UInt24_t::operator>= (const UInt24_t& val) const
{
	return (int)*this >= (int)val;
}

bool UInt24_t::operator<= (const UInt24_t& val) const
{
	return (int)*this <= (int)val;
}

__forceinline bool Int128_bit(const Int128_t& or, unsigned int n) throw ()
{
	n &= 0x7F;

	if (n < 64)
		return or.low & (1ull << n);
	else
		return or.high & (1ull << (n - 64));
}

__forceinline void Int128_bit(Int128_t& or, unsigned int n, bool val) throw ()
{
	n &= 0x7F;

	if (val) {
		if (n < 64) or.low |= (1ull << n);
		else or.high |= (1ull << (n - 64));
	}
	else {
		if (n < 64) or.low &= ~(1ull << n);
		else or.high &= ~(1ull << (n - 64));
	}
}

__inline Int128_t Int128_div(const Int128_t& or, const Int128_t & divisor, Int128_t & remainder) throw ()
{
	if (!divisor)
		return 1u / (unsigned int)divisor.low;

	Int128_t ds = (divisor.low < 0u) ? -divisor : divisor;
	Int128_t dd = (or.low < 0u) ? -or : or;

	if (ds > dd) {
		remainder = or;
		return 0ull;
	}

	Int128_t r = 0ull;
	Int128_t q = 0ull;

	unsigned int b = 127;
	while (r < ds) {
		r <<= 1;
		if (Int128_bit(dd, b--))
			r.low |= 1;
	}
	++b;

	while (true)
		if (r < ds) {
		if (!(b--)) break;

		r <<= 1;
		if (Int128_bit(dd, b))
			r.low |= 1;

		}
		else {
			r -= ds;
			Int128_bit(q, b, true);
		}

		if ((divisor.low < 0) ^ (or.low < 0)) q = -q;
		if (or.low < 0) r = -r;

		remainder = r;
		return q;
}

bool Int128_t::GetIsNegative() const
{
	return high < 0;
}

bool Int128_t::GetIsZero() const
{
	return high == 0 && low == 0;
}

Int128_t::Int128_t(float a) throw ()
	: low((unsigned __int64)fmodf(a, 18446744073709551616.0f)),
	high((__int64)(a / 18446744073709551616.0f))
{
}

Int128_t::Int128_t(double a) throw ()
	: low((unsigned __int64)fmod(a, 18446744073709551616.0)),
	high((__int64)(a / 18446744073709551616.0))
{
}

Int128_t::Int128_t(long double a) throw ()
	: low((unsigned __int64)fmodl(a, 18446744073709551616.0l)),
	high((__int64)(a / 18446744073709551616.0l))
{
}

Int128_t::operator bool() const throw ()
{
	return low != 0;
}

Int128_t::operator Int8() const throw ()
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP(res, Int8);
}

Int128_t::operator UInt8() const throw ()
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return UCLAMP(res, UInt8);
}

Int128_t::operator Int16() const throw ()
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP(res, Int16);
}

Int128_t::operator UInt16() const throw ()
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return UCLAMP(res, UInt16);
}

Int128_t::operator Int24() const throw ()
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP24(res, Int24);
}

Int128_t::operator UInt24() const throw ()
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return UCLAMP24(res, UInt24);
}

Int128_t::operator int() const throw ()
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP(res, int);
}

Int128_t::operator UInt() const throw ()
{
	return UCLAMP(low, UInt);
}

Int128_t::operator long() const throw ()
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP(res, long);
}

Int128_t::operator ULong() const throw ()
{
	return UCLAMP(low, ULong);
}

Int128_t::operator Int64() const throw ()
{
	return CLAMP((Int64)low, Int64);
}

Int128_t::operator UInt64() const throw ()
{
	return (UInt64)((low & 0x7FFFFFFFFFFFFFFFLL) | (high & 0x8000000000000000LL));
}

Int128_t::operator UInt128_t() const throw ()
{
	return UInt128_t(low, high);
}

Int128_t::operator float() const throw ()
{
	long double result;

	if (high >= 0) {
		result = (((long double)high) * (18446744073709551616.0l)) + ((long double)low);
	}
	else
	{
		Int64 h; 
		UInt64 l;
		h = high; l = low;

		h = ~h;
		l = ~l;
		l += 1;
		if (l == 0) 
		{
			h += 1;
		}
		result = -((((long double)h) * (18446744073709551616.0l)) + ((long double)l));
	}

	if (result > std::numeric_limits<float>::max()) return std::numeric_limits<float>::max();

	return (float)result;
}

Int128_t::operator double() const throw ()
{
	long double result;

	if (high >= 0) {
		result = (((long double)high) * (18446744073709551616.0l)) + ((long double)low);
	}
	else
	{
		Int64 h;
		UInt64 l;
		h = high; l = low;

		h = ~h;
		l = ~l;
		l += 1;
		if (l == 0)
		{
			h += 1;
		}
		result = -((((long double)h) * (18446744073709551616.0l)) + ((long double)l));
	}
	if (result > std::numeric_limits<double>::max()) return std::numeric_limits<double>::max();
	return result;
}

Int128_t::operator long double() const throw ()
{
	long double result;

	if (high >= 0) {
		result = (((long double)high) * (18446744073709551616.0l)) + ((long double)low);
	}
	else
	{
		Int64 h;
		UInt64 l;
		h = high; l = low;

		h = ~h;
		l = ~l;
		l += 1;
		if (l == 0)
		{
			h += 1;
		}
		result = -((((long double)h) * (18446744073709551616.0l)) + ((long double)l));
	}

	return result;
}

Int128_t Int128_t::operator - () const throw ()
{
	if (!high && !low)
		return *this;
	else
		return Int128_t(-(Int64)low, ~high);
}

Int128_t Int128_t::operator ~ () const throw ()
{
	return Int128_t(~low, ~high);
}

Int128_t & Int128_t::operator ++ ()
{
	++low;
	if (!low)
		++high;

	return *this;
}

Int128_t & Int128_t::operator -- ()
{
	if (!low)
		--high;
	--low;

	return *this;
}

Int128_t Int128_t::operator ++ (int)
{
	Int128_t b = *this;
	++ *this;

	return b;
}

Int128_t Int128_t::operator -- (int)
{
	Int128_t b = *this;
	-- *this;

	return b;
}

Int128_t & Int128_t::operator += (const Int128_t & b) throw ()
{
	unsigned __int64 old_lo = low;

	low += b.low;
	high += b.high + (low < old_lo);

	return *this;
}

Int128_t & Int128_t::operator *= (const Int128_t & b) throw ()
{
	if (!b)
		return *this = 0u;
	if (b.low == 1u)
		return *this;

	Int128_t a = *this;
	Int128_t t = b;

	low = 0ull;
	high = 0ll;

	for (unsigned int i = 0; i < 128; ++i) {
		if (t.low & 1)
			*this += a << i;

		t >>= 1;
	}

	return *this;
}

inline Int128_t & Int128_t::operator -= (const Int128_t & b) throw ()
{
	return *this += (-b);
};

inline Int128_t & Int128_t::operator /= (const Int128_t & b) throw ()
{
	Int128_t dummy;
	*this = Int128_div(*this, b, dummy);
	return *this;
};

inline Int128_t & Int128_t::operator %= (const Int128_t & b) throw ()
{
	Int128_div(*this, b, *this);
	return *this;
};


Int128_t & Int128_t::operator >>= (unsigned int n) throw ()
{
	n &= 0x7F;

	if (n > 63)
	{
		n -= 64;
		low = high;

		if (high < 0) high = -1ll;
		else high = 0ll;
	}

	if (n)
	{
		low >>= n;

		unsigned __int64 mask = 0ull;
		for (unsigned int i = 0; i < n; ++i) mask |= (1 << i);

		low |= (high & mask) << (64 - n);

		high >>= n;
	}

	return *this;
}

Int128_t & Int128_t::operator <<= (unsigned int n) throw ()
{
	n &= 0x7F;

	if (n > 63)
	{
		n -= 64;
		high = low;
		low = 0ull;
	}

	if (n)
	{
		high <<= n;

		unsigned __int64 mask = 0ull;
		for (unsigned int i = 0; i < n; ++i) mask |= (1 << (63 - i));

		high |= (low & mask) >> (64 - n);

		low <<= n;
	}

	return *this;
}

bool Int128_t::operator ! () const throw ()
{
	return !(high || low);
}

Int128_t & Int128_t::operator |= (const Int128_t & b) throw ()
{
	high |= b.high;
	low |= b.low;

	return *this;
}

Int128_t & Int128_t::operator &= (const Int128_t & b) throw ()
{
	high &= b.high;
	low &= b.low;

	return *this;
}

Int128_t & Int128_t::operator ^= (const Int128_t & b) throw ()
{
	high ^= b.high;
	low ^= b.low;

	return *this;
}


__forceinline bool UInt128_bit(const UInt128_t& or, unsigned int n) throw ()
{
	n &= 0x7F;

	if (n < 64)
		return or.low & (1ull << n);
	else
		return or.high & (1ull << (n - 64));
}

__forceinline void UInt128_bit(UInt128_t& or, unsigned int n, bool val) throw ()
{
	n &= 0x7F;

	if (val) {
		if (n < 64) or.low |= (1ull << n);
		else or.high |= (1ull << (n - 64));
	}
	else {
		if (n < 64) or.low &= ~(1ull << n);
		else or.high &= ~(1ull << (n - 64));
	}
}

__inline UInt128_t UInt128_div(const UInt128_t& or, const UInt128_t & ds, UInt128_t & remainder) throw ()
{
	if (!ds)
		return 1u / (unsigned int)ds.low;

	UInt128_t dd = or;

	if (ds > dd) {
		remainder = or;
		return 0ull;
	}

	UInt128_t r = 0ull;
	UInt128_t q = 0ull;

	unsigned int b = 127;
	while (r < ds) {
		r <<= 1;
		if (UInt128_bit(dd, b--))
			r.low |= 1;
	}
	++b;

	while (true)
		if (r < ds) {
		if (!(b--)) break;

		r <<= 1;
		if (UInt128_bit(dd, b))
			r.low |= 1;

		}
		else {
			r -= ds;
			UInt128_bit(q, b, true);
		}

		remainder = r;
		return q;

}

bool UInt128_t::GetIsZero() const
{
	return low == 0 && high == 0;
}

UInt128_t::UInt128_t(float a) throw ()
	: low((unsigned __int64)fmodf(a, 18446744073709551616.0f)),
	high((unsigned __int64)(a / 18446744073709551616.0f))
{
}

UInt128_t::UInt128_t(double a) throw ()
	: low((unsigned __int64)fmod(a, 18446744073709551616.0)),
	high((unsigned __int64)(a / 18446744073709551616.0))
{
}

UInt128_t::UInt128_t(long double a) throw ()
	: low((unsigned __int64)fmodl(a, 18446744073709551616.0l)),
	high((unsigned __int64)(a / 18446744073709551616.0l))
{
}

UInt128_t::operator bool() const
{
	return high || low;
}

UInt128_t::operator Int8() const
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP(res, Int8);
}

UInt128_t::operator UInt8() const throw ()
{
	return UCLAMP(low, UInt8);
}

UInt128_t::operator Int16() const
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP(res, Int16);
}

UInt128_t::operator UInt16() const throw ()
{
	return UCLAMP(low, UInt16);
}

UInt128_t::operator Int24() const
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP24(res, Int24);
}

UInt128_t::operator UInt24() const throw ()
{
	return UCLAMP24(low, UInt24);
}

UInt128_t::operator int() const
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP(res, int);
}

UInt128_t::operator UInt() const throw ()
{
	return UCLAMP(low, UInt);
}

UInt128_t::operator long() const
{
	UInt64 res = low & 0x00000000FFFFFFFFLL;
	return CLAMP(res, long);
}

UInt128_t::operator ULong() const throw ()
{
	return UCLAMP(low, ULong);
}

UInt128_t::operator Int64() const throw ()
{
	return CLAMP((Int64)low, Int64);
}

UInt128_t::operator UInt64() const throw ()
{
	return UCLAMP(low, UInt64);
}

UInt128_t::operator Int128_t() const throw ()
{
	return Int128_t(low, high);
}

UInt128_t::operator float() const throw ()
{
	long double result = (long double)high * 18446744073709551616.0l+ (long double)low;

	if (result > std::numeric_limits<float>::max()) return std::numeric_limits<float>::max();

	return (float)result;
}

UInt128_t::operator double() const throw ()
{
	long double result = (long double)high * 18446744073709551616.0l + (long double)low;

	if (result > std::numeric_limits<double>::max()) return std::numeric_limits<double>::max();

	return (double)result;
}

UInt128_t::operator long double() const throw ()
{
	return (long double) high * 18446744073709551616.0l
		+ (long double) low;
}

UInt128 UInt128::operator - () const throw ()
{
	if (!high && !low)
		return *this;
	else
		return UInt128(~low, ~high)+UInt128(1ull);
}

UInt128_t UInt128_t::operator ~() const throw ()
{
	return UInt128_t(~low, ~high);
}

UInt128_t & UInt128_t::operator ++ ()
{
	++low;
	if (!low)
		++high;

	return *this;
}

UInt128_t & UInt128_t::operator -- ()
{
	if (!low)
		--high;
	--low;

	return *this;
}

UInt128_t UInt128_t::operator ++ (int)
{
	UInt128_t b = *this;
	++ *this;

	return b;
}

UInt128_t UInt128_t::operator -- (int)
{
	UInt128_t b = *this;
	-- *this;

	return b;
}

UInt128_t & UInt128_t::operator += (const UInt128_t & b) throw ()
{
	unsigned __int64 old_lo = low;

	low += b.low;
	high += b.high + (low < old_lo);

	return *this;
}

UInt128_t & UInt128_t::operator *= (const UInt128_t & b) throw ()
{
	if (!b)
		return *this = 0u;
	if (b.low == 1u)
		return *this;

	UInt128_t a = *this;
	UInt128_t t = b;

	low = 0ull;
	high = 0ull;

	for (unsigned int i = 0; i < 128; ++i) {
		if (t.low & 1)
			*this += a << i;

		t >>= 1;
	}

	return *this;
}

inline UInt128_t & UInt128_t::operator -= (const UInt128 & b) throw ()
{
	return *this += (-b);
}

inline UInt128_t & UInt128_t::operator /= (const UInt128 & b) throw ()
{
	UInt128 dummy;
	*this = UInt128_div(*this, b, dummy);
	return *this;
}

inline UInt128 & UInt128_t::operator %= (const UInt128 & b) throw ()
{
	UInt128_div(*this, b, *this);
	return *this;
}

UInt128_t & UInt128_t::operator >>= (unsigned int n) throw ()
{
	n &= 0x7F;

	if (n > 63)
	{
		n -= 64;
		low = high;
		high = 0ull;
	}

	if (n)
	{
		low >>= n;

		unsigned __int64 mask = 0ull;
		for (unsigned __int64 i = 0; i < n; ++i) mask |= (1 << i);

		low |= (high & mask) << (64 - n);

		high >>= n;
	}

	return *this;
}

UInt128_t & UInt128_t::operator <<= (unsigned int n) throw ()
{
	n &= 0x7F;

	if (n > 63)
	{
		n -= 64;
		high = low;
		low = 0ull;
	}

	if (n)
	{
		high <<= n;

		unsigned __int64 mask = 0ull;
		for (unsigned __int64 i = 0; i < n; ++i) mask |= (1 << (63 - i));

		high |= (low & mask) >> (64 - n);

		low <<= n;
	}

	return *this;
}

bool UInt128_t::operator ! () const throw ()
{
	return !(high || low);
}

UInt128_t & UInt128_t::operator |= (const UInt128_t & b) throw ()
{
	high |= b.high;
	low |= b.low;

	return *this;
}

UInt128_t & UInt128_t::operator &= (const UInt128_t & b) throw ()
{
	high &= b.high;
	low &= b.low;

	return *this;
}

UInt128_t & UInt128_t::operator ^= (const UInt128_t & b) throw ()
{
	high ^= b.high;
	low ^= b.low;

	return *this;
}


#if CONSTEXPER_SUPPORTED
#else
uhash __uhash_runtime_time(char const* str, uhash last_value)
{
	return *str ? __uhash_runtime_time(str + 1, (*str ^ last_value) * __uhash_prime) : last_value;
}
#endif