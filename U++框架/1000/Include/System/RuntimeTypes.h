#pragma once

#pragma warning(disable: 4521)

#if defined(_WIN32) || defined(WIN32) 
#define LITTLE_ENDIAN 
#define __WIN__	
#elif defined(_WIN64) || defined(WIN64) 
#define LITTLE_ENDIAN 
#define __WIN__	
#else

#endif

#ifndef RUNTIME_CAPI
#ifdef RUNTIME_EXPORTS
#define RUNTIME_CAPI extern "C" __declspec(dllexport)
#else
#define RUNTIME_CAPI extern "C" __declspec(dllimport)
#endif
#endif

#ifndef RUNTIME_API
#ifdef RUNTIME_EXPORTS
#define RUNTIME_API  __declspec(dllexport)
#else
#define RUNTIME_API  __declspec(dllimport)
#endif
#endif


#ifndef RUNTIME_TYPES
#define RUNTIME_TYPES

#include <stdint.h>
#include <vcruntime_string.h>

#pragma warning( push, 3 )
#pragma warning(disable: 4018)
#pragma warning(disable: 4244)
#pragma warning(disable: 4800)
typedef char Char;
typedef char Int8;
typedef unsigned char UChar;
typedef unsigned char UInt8;
typedef unsigned char Byte;
typedef short Int16;
typedef unsigned short UShort;
typedef unsigned short UInt16;
typedef int Int32;
typedef unsigned int UInt;
typedef unsigned int UInt32;
typedef unsigned long ULong;
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
typedef float Float32;
typedef double Float64;
typedef double Double;
typedef long double Float128;
typedef long double LongDouble;
/// <summary>
/// 事件标记类型
/// </summary>
typedef Int64 EventToken;

typedef struct u_GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char Data4[8];
} uGUID;

constexpr uGUID UGUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

class Int24_t;
class UInt24_t;
class Int128_t;
class UInt128_t;

__inline bool operator == (const u_GUID & a, const u_GUID & b) throw ()
{
	return !memcmp(&a, &b, sizeof(u_GUID));
}

__inline bool operator != (const u_GUID & a, const u_GUID & b) throw ()
{
	return !(a == b);
}


typedef class RUNTIME_API Int24_t
{
public:
	unsigned char value[3];
	Int24_t();

	explicit Int24_t(volatile const Int24_t& val);

	Int24_t(int input);

	operator bool() const;

	operator Int8() const;
	operator UInt8() const;

	operator Int16() const;
	operator UInt16() const;

	operator UInt24_t() const;

	operator int() const;
	operator UInt() const;

	operator long() const throw ();
	operator ULong() const throw ();

	operator Int64() const;
	operator UInt64() const;

	operator Int128_t() const;
	operator UInt128_t() const;

	operator float() const;
	operator long double() const;

	Int24_t& operator= (const Int24_t& input);

	Int24_t& operator= (const int input);

	Int24_t operator+ (const Int24_t& val) const;

	Int24_t operator- (const Int24_t& val) const;

	Int24_t operator* (const Int24_t& val) const;

	Int24_t operator/ (const Int24_t& val) const;

	Int24_t& operator+= (const Int24_t& val);

	Int24_t& operator-= (const Int24_t& val);

	Int24_t& operator*= (const Int24_t& val);

	Int24_t& operator/= (const Int24_t& val);

	Int24_t operator>> (const int val) const;

	Int24_t operator<< (const int val) const;

	bool operator! () const;

	Int24_t operator- ();

	bool operator== (const Int24_t& val) const;

	bool operator!= (const Int24_t& val) const;

	bool operator>= (const Int24_t& val) const;

	bool operator<= (const Int24_t& val) const;
} Int24;

inline bool operator <  (const Int24_t & a, const Int24_t & b) throw ()
{
	return a.operator int() < b.operator int();
}

inline bool operator == (const Int24_t & a, const Int24_t & b) throw ()
{
	return a.operator int() && a.operator int();
}

inline bool operator && (const Int24_t & a, const Int24_t & b) throw ()
{
	return a.operator int() && b.operator int();
}

inline bool operator || (const Int24_t & a, const Int24_t & b) throw ()
{
	return a.operator int() || b.operator int();
}

inline bool operator >  (const Int24_t & a, const Int24_t & b) throw ()
{
	return   b < a;
};
inline bool operator <= (const Int24_t & a, const Int24_t & b) throw ()
{
	return !(b < a);
};
inline bool operator >= (const Int24_t & a, const Int24_t & b) throw ()
{
	return !(a < b);
};

typedef class RUNTIME_API  UInt24_t
{
public:
	unsigned char value[3];
	UInt24_t();

	explicit UInt24_t(volatile const UInt24_t& val);

	UInt24_t(int input);

	operator bool() const;

	operator Int8() const;
	operator UInt8() const;

	operator Int16() const;
	operator UInt16() const;

	operator Int24_t() const;

	operator int() const;
	operator UInt() const;

	operator long() const throw ();
	operator ULong() const throw ();

	operator Int64() const;
	operator UInt64() const;

	operator Int128_t() const;
	operator UInt128_t() const;

	operator float() const;
	operator long double() const;

	UInt24_t& operator= (const UInt24_t& input);

	UInt24_t& operator= (const int input);

	UInt24_t operator+ (const UInt24_t& val) const;

	UInt24_t operator- (const UInt24_t& val) const;

	UInt24_t operator* (const UInt24_t& val) const;

	UInt24_t operator/ (const UInt24_t& val) const;

	UInt24_t& operator+= (const UInt24_t& val);

	UInt24_t& operator-= (const UInt24_t& val);

	UInt24_t& operator*= (const UInt24_t& val);

	UInt24_t& operator/= (const UInt24_t& val);

	UInt24_t operator>> (const int val) const;

	UInt24_t operator<< (const int val) const;

	bool operator! () const;

	UInt24_t operator- ();

	bool operator== (const UInt24_t& val) const;

	bool operator!= (const UInt24_t& val) const;

	bool operator>= (const UInt24_t& val) const;

	bool operator<= (const UInt24_t& val) const;
} UInt24;

inline bool operator <  (const UInt24_t & a, const UInt24_t & b) throw ()
{
	return a.operator int() < b.operator int();
}

inline bool operator == (const UInt24_t & a, const UInt24_t & b) throw ()
{
	return a.operator int() && a.operator int();
}

inline bool operator && (const UInt24_t & a, const UInt24_t & b) throw ()
{
	return a.operator int() && b.operator int();
}

inline bool operator || (const UInt24_t & a, const UInt24_t & b) throw ()
{
	return a.operator int() || b.operator int();
}

inline bool operator >  (const UInt24_t & a, const UInt24_t & b) throw ()
{
	return   b < a;
};
inline bool operator <= (const UInt24_t & a, const UInt24_t & b) throw ()
{
	return !(b < a);
};
inline bool operator >= (const UInt24_t & a, const UInt24_t & b) throw ()
{
	return !(a < b);
};

typedef class RUNTIME_API Int128_t
{
public:
#ifdef LITTLE_ENDIAN
	UInt64    low;
	Int64    high;
#else
	Int64    high;
	UInt64    low;
#endif
public:

	bool GetIsNegative() const;
	bool GetIsZero() const;

	_declspec(property(get = GetIsNegative)) bool IsNegative;
	_declspec(property(get = GetIsZero)) bool IsZero;

	inline Int128_t() throw () :low(0), high(0) {};
	inline Int128_t(UInt64 a, Int64 b) throw () : low(a), high(b) {}

	inline Int128_t(const Int128_t & a) throw () : low(a.low), high(a.high) {}
	inline Int128_t(volatile const Int128_t & a) throw () : low(a.low), high(a.high) {}

	inline Int128_t(UInt a) throw () : low(a), high(0ll) {}
	inline Int128_t(int a) throw () : low(a), high(0ll)
	{
		if (a < 0) this->high = -1ll;
	}

	inline Int128_t(UInt64 a) throw () : low(a), high(0ll) {}
	inline Int128_t(Int64 a) throw () : low(a), high(0ll)
	{
		if (a < 0) this->high = -1ll;
	}

	Int128_t(float a) throw ();
	Int128_t(double  a) throw ();
	Int128_t(long double  a) throw ();


	bool operator ! () const throw ();

	Int128_t operator - () const throw ();
	Int128_t operator ~ () const throw ();

	Int128_t & operator ++ ();
	Int128_t & operator -- ();
	Int128_t operator ++ (int);
	Int128_t operator -- (int);

	Int128_t & operator += (const Int128_t & b) throw ();
	Int128_t & operator *= (const Int128_t & b) throw ();

	Int128_t & operator >>= (unsigned int n) throw ();
	Int128_t & operator <<= (unsigned int n) throw ();

	Int128_t & operator |= (const Int128_t & b) throw ();
	Int128_t & operator &= (const Int128_t & b) throw ();
	Int128_t & operator ^= (const Int128_t & b) throw ();

	inline const Int128_t & operator + () const throw () { return *this; };

	Int128_t & operator -= (const Int128_t & b) throw ();

	Int128_t & operator /= (const Int128_t & b) throw ();

	Int128_t & operator %= (const Int128_t & b) throw ();

	operator bool() const throw ();

	operator Int8() const throw ();
	operator UInt8() const throw ();

	operator Int16() const throw ();
	operator UInt16() const throw ();

	operator Int24() const throw ();
	operator UInt24() const throw ();

	operator int() const throw ();
	operator UInt() const throw ();

	operator long() const throw ();
	operator ULong() const throw ();

	operator Int64() const throw ();
	operator UInt64() const throw ();

	operator UInt128_t() const throw ();

	operator float() const throw ();

	operator double() const throw ();

	operator long double() const throw ();
}Int128;

inline bool operator <  (const Int128_t & a, const Int128_t & b) throw ()
{
	if (a.high == b.high) {
		if (a.high < 0)
			return (signed __int64)a.low < (signed __int64)b.low;
		else
			return a.low < b.low;
	}
	else
		return a.high < b.high;
}

inline bool operator == (const Int128_t & a, const Int128_t & b) throw ()
{
	return a.high == b.high && a.low == b.low;
}

inline bool operator && (const Int128_t & a, const Int128_t & b) throw ()
{
	return (a.high || a.low) && (b.high || b.low);
}

inline bool operator || (const Int128_t & a, const Int128_t & b) throw ()
{
	return (a.high || a.low) || (b.high || b.low);
}

inline Int128_t operator + (const Int128_t & a, const Int128_t & b) throw ()
{
	return Int128_t(a) += b;
};

inline Int128_t operator - (const Int128_t & a, const Int128_t & b) throw ()
{
	return Int128_t(a) -= b;
};

inline Int128_t operator * (const Int128_t & a, const Int128_t & b) throw ()
{
	return Int128_t(a) *= b;
};

inline Int128_t operator / (const Int128_t & a, const Int128_t & b) throw ()
{
	return Int128_t(a) /= b;
};

inline Int128_t operator % (const Int128_t & a, const Int128_t & b) throw ()
{
	return Int128_t(a) %= b;
};

inline Int128_t operator >> (const Int128_t & a, unsigned int n) throw ()
{
	return Int128_t(a) >>= n;
};

inline Int128_t operator << (const Int128_t & a, unsigned int n) throw ()
{
	return Int128_t(a) <<= n;
};

inline Int128_t operator & (const Int128_t & a, const Int128_t & b) throw ()
{
	return Int128_t(a) &= b;
};

inline Int128_t operator | (const Int128_t & a, const Int128_t & b) throw ()
{
	return Int128_t(a) |= b;
};

inline Int128_t operator ^ (const Int128_t & a, const Int128_t & b) throw ()
{
	return Int128_t(a) ^= b;
};

inline bool operator >  (const Int128_t & a, const Int128_t & b) throw ()
{
	return   b < a;
};
inline bool operator <= (const Int128_t & a, const Int128_t & b) throw ()
{
	return !(b < a);
};
inline bool operator >= (const Int128_t & a, const Int128_t & b) throw ()
{
	return !(a < b);
};
inline bool operator != (const Int128_t & a, const Int128_t & b) throw ()
{
	return !(a == b);
};


typedef class RUNTIME_API UInt128_t
{
public:
#ifdef LITTLE_ENDIAN
	UInt64 low;
	UInt64 high;
#else
	UInt64 high;
	UInt64 low;
#endif
public:
	bool GetIsZero() const;

	_declspec(property(get = GetIsZero)) bool IsZero;

	inline UInt128_t() throw () {};

	inline UInt128_t(UInt64 a, UInt64 b) throw () : low(a), high(b) {}

	inline UInt128_t(const UInt128_t & a) throw () : low(a.low), high(a.high) {}
	inline UInt128_t(volatile const UInt128_t & a) throw () : low(a.low), high(a.high) {}

	inline UInt128_t(bool val) throw () : low(val ? 1ull : 0ull), high(0ull) {}

	inline UInt128_t(Int64 val) throw () : low(val>0ll ? val : 0ll), high(0ull) {}
	inline UInt128_t(int val) throw () : low(val > 0 ? val : 0ll), high(0ll) {}

	inline UInt128_t(UInt val) throw () : low(val), high(0ull) {}
	inline UInt128_t(UInt64 val) throw () : low(val), high(0ull) {}

	UInt128_t(float a) throw ();
	UInt128_t(double a) throw ();
	UInt128_t(long double a) throw ();

	bool operator ! () const throw ();

	UInt128_t operator - () const throw ();
	UInt128_t operator ~ () const throw ();

	UInt128_t & operator ++ ();
	UInt128_t & operator -- ();
	UInt128_t operator ++ (int);
	UInt128_t operator -- (int);

	UInt128_t & operator += (const UInt128_t & b) throw ();
	UInt128_t & operator *= (const UInt128_t & b) throw ();

	UInt128_t & operator >>= (unsigned int n) throw ();
	UInt128_t & operator <<= (unsigned int n) throw ();

	UInt128_t & operator |= (const UInt128_t & b) throw ();
	UInt128_t & operator &= (const UInt128_t & b) throw ();
	UInt128_t & operator ^= (const UInt128_t & b) throw ();

	inline const UInt128_t & operator + () const throw () { return *this; };

	UInt128_t & operator -= (const UInt128_t & b) throw ();

	UInt128_t & operator /= (const UInt128_t & b) throw ();

	UInt128_t & operator %= (const UInt128_t & b) throw ();

	operator bool() const;

	operator Int8() const;
	operator UInt8() const throw ();

	operator Int16() const;
	operator UInt16() const throw ();

	operator Int24() const throw ();
	operator UInt24() const throw ();

	operator int() const;
	operator UInt() const throw ();

	operator long() const;
	operator ULong() const throw ();

	operator Int64() const throw ();
	operator UInt64() const throw ();

	operator Int128_t() const throw ();

	operator float() const throw ();

	operator double() const throw ();

	operator long double() const throw ();
}UInt128;

inline bool operator <  (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return (a.high == b.high) ? (a.low < b.low) : (a.high < b.high);
}

inline bool operator == (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return a.high == b.high && a.low == b.low;
}

inline bool operator && (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return (a.high || a.low) && (b.high || b.low);
}

inline bool operator || (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return (a.high || a.low) || (b.high || b.low);
}

inline UInt128_t operator + (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return UInt128_t(a) += b;
};

inline UInt128_t operator - (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return UInt128_t(a) -= b;
};

inline UInt128_t operator * (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return UInt128_t(a) *= b;
};

inline UInt128_t operator / (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return UInt128_t(a) /= b;
};

inline UInt128_t operator % (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return UInt128_t(a) %= b;
};

inline UInt128_t operator >> (const UInt128_t & a, unsigned int n) throw ()
{
	return UInt128_t(a) >>= n;
};

inline UInt128_t operator << (const UInt128_t & a, unsigned int n) throw ()
{
	return UInt128_t(a) <<= n;
};

inline UInt128_t operator & (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return UInt128_t(a) &= b;
};

inline UInt128_t operator | (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return UInt128_t(a) |= b;
};

inline UInt128_t operator ^ (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return UInt128_t(a) ^= b;
};

inline bool operator >  (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return   b < a;
};
inline bool operator <= (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return !(b < a);
};
inline bool operator >= (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return !(a < b);
};
inline bool operator != (const UInt128_t & a, const UInt128_t & b) throw ()
{
	return !(a == b);
};



#define _INT24_MAX (Int24(8388607))
#define _INT24_MIN (Int24(-8388608))
#define _UINT24_MIN (UInt24(0U))
#define _UINT24_MAX (UInt24(16777215U))

#define _INT128_MAX   (Int128_t(UINT64_MAX, INT64_MAX)) 
#define _INT128_MIN   (Int128_t(1, INT64_MIN)) 

#define _UINT128_MAX   (UInt128_t(UINT64_MAX, UINT64_MAX)) 
#define _UINT128_MIN   (UInt128_t(0U, 0U)) 

#include <xhash>
#include <iostream>
#include <limits>
#include <xtr1common>
namespace std
{
	template<>
	struct is_convertible<bool, UInt128_t> : true_type
	{	
	};

	template<>
	struct is_convertible<Int8, UInt128_t> : true_type
	{
	};

	template<>
	struct is_convertible<UInt8, UInt128_t> : true_type
	{
	};

	template<>
	struct is_convertible<Int16, UInt128_t> : true_type
	{
	};

	template<>
	struct is_convertible<UInt16, UInt128_t> : true_type
	{
	};

	template<>
	struct is_convertible<Int24, UInt128_t> : true_type
	{
	};

	template<>
	struct is_convertible<UInt24, UInt128_t> : true_type
	{
	};

	template<>
	struct is_convertible<int, UInt128_t> : true_type
	{
	};

	template<>
	struct is_convertible<UInt, UInt128_t> : true_type
	{
	};

	template<>
	struct is_convertible<long, UInt128_t> : true_type
	{
	};

	template<>
	struct is_convertible<ULong, Int128_t> : true_type
	{
	};

	template<>
	struct is_convertible<ULong, UInt128_t> : true_type
	{
	};

	template<>
	struct _Is_integral<Int24>
		: true_type
	{	
	};

	template<>
	struct _Is_integral<UInt24>
		: true_type
	{
	};

	template<>
	struct _Is_integral<Int128>
		: true_type
	{
	};

	template<>
	struct _Is_integral<UInt128>
		: true_type
	{
	};

// CLASS numeric_limits<Int24>
template<> class numeric_limits<Int24>
	: public _Num_int_base
	{	// limits for type Int24
	public:
		typedef Int24 _Ty;

		static _Ty(min)() noexcept
		{	// return minimum value
			return (_INT24_MIN);
		}

		static _Ty(max)() noexcept
		{	// return maximum value
			return (_INT24_MAX);
		}

		static _Ty lowest() noexcept
		{	// return most negative value
			return ((min)());
		}

		static _Ty epsilon() noexcept
		{	// return smallest effective increment from 1.0
			return _Ty(0);
		}

		static _Ty round_error() noexcept
		{	// return largest rounding error
			return _Ty(0);
		}

		static _Ty denorm_min() noexcept
		{	// return minimum denormalized value
			return _Ty(0);
		}

		static _Ty infinity() noexcept
		{	// return positive infinity
			return _Ty(0);
		}

		static _Ty quiet_NaN() noexcept
		{	// return non-signaling NaN
			return _Ty(0);
		}

		static _Ty signaling_NaN() noexcept
		{	// return signaling NaN
			return _Ty(0);
		}

		static constexpr bool is_signed = true;
		static constexpr int digits = CHAR_BIT * sizeof(Int24) - 1;
		static constexpr int digits10 = (CHAR_BIT * sizeof(Int24) - 1) * 301L / 1000;
	};

	// CLASS numeric_limits<UInt24>
	template<> class numeric_limits<UInt24>
		: public _Num_int_base
		{	// limits for type UInt24
		public:
			typedef UInt24 _Ty;

			static _Ty(min)() noexcept
			{	// return minimum value
				return (_UINT24_MIN);
			}

			static _Ty(max)() noexcept
			{	// return maximum value
				return (_UINT24_MAX);
			}

			static _Ty lowest() noexcept
			{	// return most negative value
				return ((min)());
			}

			static _Ty epsilon() noexcept
			{	// return smallest effective increment from 1.0
				return _Ty(0);
			}

			static _Ty round_error() noexcept
			{	// return largest rounding error
				return _Ty(0);
			}

			static _Ty denorm_min() noexcept
			{	// return minimum denormalized value
				return _Ty(0);
			}

			static _Ty infinity() noexcept
			{	// return positive infinity
				return _Ty(0);
			}

			static _Ty quiet_NaN() noexcept
			{	// return non-signaling NaN
				return _Ty(0);
			}

			static _Ty signaling_NaN() noexcept
			{	// return signaling NaN
				return _Ty(0);
			}

			static constexpr bool is_signed = false;
			static constexpr int digits = CHAR_BIT * sizeof(UInt24);
			static constexpr int digits10 = CHAR_BIT * sizeof(UInt24) * 301L / 1000;
		};

		// CLASS numeric_limits<Int128>
		template<> class numeric_limits<Int128>
			: public _Num_int_base
			{	// limits for type Int128
			public:
				typedef Int128 _Ty;

				static _Ty(min)() noexcept
				{	// return minimum value
					return (_INT128_MIN);
				}

				static _Ty(max)() noexcept
				{	// return maximum value
					return (_INT128_MAX);
				}

				static _Ty lowest() noexcept
				{	// return most negative value
					return ((min)());
				}

				static _Ty epsilon() noexcept
				{	// return smallest effective increment from 1.0
					return _Ty(0);
				}

				static _Ty round_error() noexcept
				{	// return largest rounding error
					return _Ty(0);
				}

				static _Ty denorm_min() noexcept
				{	// return minimum denormalized value
					return _Ty(0);
				}

				static _Ty infinity() noexcept
				{	// return positive infinity
					return _Ty(0);
				}

				static _Ty quiet_NaN() noexcept
				{	// return non-signaling NaN
					return _Ty(0);
				}

				static _Ty signaling_NaN() noexcept
				{	// return signaling NaN
					return _Ty(0);
				}

				static constexpr bool is_signed = true;
				static constexpr int digits = CHAR_BIT * sizeof(Int128) - 1;
				static constexpr int digits10 = (CHAR_BIT * sizeof(Int128) - 1)* 301L / 1000;
			};

	// CLASS numeric_limits<UInt128>
	template<> class numeric_limits<UInt128>
		: public _Num_int_base
		{	// limits for type UInt128
		public:
			typedef UInt128 _Ty;

			static _Ty(min)() noexcept
			{	// return minimum value
				return (_UINT128_MIN);
			}

			static _Ty(max)() noexcept
			{	// return maximum value
				return (_UINT128_MAX);
			}

			static _Ty lowest() noexcept
			{	// return most negative value
				return ((min)());
			}

			static _Ty epsilon() noexcept
			{	// return smallest effective increment from 1.0
				return _Ty(0U);
			}

			static _Ty round_error() noexcept
			{	// return largest rounding error
				return _Ty(0U);
			}

			static _Ty denorm_min() noexcept
			{	// return minimum denormalized value
				return _Ty(0U);
			}

			static _Ty infinity() noexcept
			{	// return positive infinity
				return _Ty(0U);
			}

			static _Ty quiet_NaN() noexcept
			{	// return non-signaling NaN
				return _Ty(0U);
			}

			static _Ty signaling_NaN() noexcept
			{	// return signaling NaN
				return _Ty(0U);
			}

			static constexpr bool is_signed = false;
			static constexpr int digits = CHAR_BIT * sizeof(UInt128);
			static constexpr int digits10 = CHAR_BIT * sizeof(UInt128) * 301L / 1000;
		};

	template<>
	struct is_signed<Int24> : true_type
	{
	};

	template<>
	struct is_signed<Int128> : true_type
	{
	};

	template<>
	struct is_signed<UInt24> : false_type
	{
	};

	template<>
	struct is_signed<UInt128> : false_type
	{
	};

	template<>
	struct is_unsigned<Int24> : false_type
	{
	};

	template<>
	struct is_unsigned<Int128> : false_type
	{
	};

	template<>
	struct is_unsigned<UInt24> : true_type
	{
	};

	template<>
	struct is_unsigned<UInt128> : true_type
	{
	};



	template <>
	struct std::hash<Int24_t>
	{
		inline size_t operator()(const Int24_t& obj) const
		{
			return  std::hash<int>()(obj);
		}
	};

	inline ostream& operator<<(ostream& lhs, const Int24_t& rhs)
	{
		lhs << rhs.operator int();
		return lhs;
	}

	template <>
	struct std::hash<UInt24_t>
	{
		inline size_t operator()(const UInt24_t& obj) const
		{
			return  std::hash<int>()(obj);
		}
	};
	inline ostream& operator<<(ostream& lhs, const UInt24_t& rhs)
	{
		lhs << rhs.operator int();
		return lhs;

	}

#pragma warning( push )
#pragma warning( disable : 4477)
	template <>
	struct std::hash<Int128_t>
	{
		inline size_t operator()(const Int128_t& obj) const
		{
			size_t low = std::hash<UInt64>()(obj.low);
			size_t high = std::hash<Int64>()(obj.high) + 0x9e3779b9 + (low << 6) + (low >> 2);
			return  low ^ high;
		}
	};

	inline ostream& operator<<(ostream& lhs, const Int128_t& rhs)
	{
		char buffer[35];
		_snprintf_s(buffer, sizeof(buffer), "0x%016lx%016lx", rhs.high, rhs.low);
		lhs << buffer;
		return lhs;
	}

	template <>
	struct std::hash<UInt128_t>
	{
		inline size_t operator()(const UInt128_t& obj) const
		{
			size_t low = std::hash<UInt64>()(obj.low);
			size_t high = std::hash<UInt64>()(obj.high) + 0x9e3779b9 + (low << 6) + (low >> 2);
			return  low ^ high;
		}
	};

	inline ostream& operator<<(ostream& lhs, const UInt128_t& rhs)
	{
		char buffer[35];
		_snprintf_s(buffer, sizeof(buffer), "0x%016lx%016lx", rhs.high, rhs.low);
		lhs << buffer;
		return lhs;
	}
#pragma warning( pop )
}

template<typename F, typename T>
struct number_cast_impl
{
	static T Cast(F val)
	{
		return val;
	}
};

template<typename F, typename T, bool SAME=std::is_same<F,T>::value>
struct number_cast
{
	static T Cast(F val)
	{
		return number_cast_impl<F, T>::Cast(val);
	}
};

template<typename F, typename T>
struct number_cast<F, T, true>
{
	static T Cast(F val)
	{
		return val;
	}
};

#pragma region Int8
template<>
struct number_cast_impl<Int8, UInt8>
{
	static UInt8 Cast(Int8 val)
	{
		if (val < (Int8)std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		return val;
	}
};

template<>
struct number_cast_impl<Int8, UInt16>
{
	static UInt16 Cast(Int8 val)
	{
		if (val < (Int16)std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		return val;
	}
};

template<>
struct number_cast_impl <Int8, UInt24>
{
	static UInt24 Cast(Int8 val)
	{
		if (val < std::numeric_limits<UInt24>::min().operator int()) return std::numeric_limits<UInt24>::min();
		return UInt24(val);
	}
};

template<>
struct number_cast_impl <Int8, UInt>
{
	static UInt Cast(Int8 val)
	{
		if (val < (int)std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		return val;
	}
};

template<>
struct number_cast_impl <Int8, ULong>
{
	static ULong Cast(Int8 val)
	{
		if (val < (long)std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		return val;
	}
};

template<>
struct number_cast_impl <Int8, UInt64>
{
	static UInt64 Cast(Int8 val)
	{
		if (val < (Int64)std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		return val;
	}
};

template<>
struct number_cast_impl <Int8, UInt128>
{
	static UInt128 Cast(Int8 val)
	{
		if (val < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		return (UInt64)val;
	}
};
#pragma endregion

#pragma region Int16
template<>
struct number_cast_impl<Int16, Int8>
{
	static Int8 Cast(Int16 val)
	{
		if (val < std::numeric_limits<Int8>::min()) return std::numeric_limits<Int8>::min();
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return (Int8)val;
	}
};

template<>
struct number_cast_impl<Int16, UInt8>
{
	static UInt8 Cast(Int16 val)
	{
		if (val < (Int8)std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		return (UInt8)val;
	}
};

template<>
struct number_cast_impl<Int16, UInt16>
{
	static UInt16 Cast(Int16 val)
	{
		if (val < (Int16)std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		return (UInt16)val;
	}
};

template<>
struct number_cast_impl <Int16, UInt24>
{
	static UInt24 Cast(Int16 val)
	{
		if (val < std::numeric_limits<UInt24>::min().operator int()) return std::numeric_limits<UInt24>::min();
		return UInt24((int)val);
	}
};

template<>
struct number_cast_impl <Int16, UInt>
{
	static UInt Cast(Int16 val)
	{
		if (val < (int)std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		return (UInt)val;
	}
};

template<>
struct number_cast_impl <Int16, ULong>
{
	static ULong Cast(Int16 val)
	{
		if (val < (long)std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		return (ULong)val;
	}
};

template<>
struct number_cast_impl <Int16, UInt64>
{
	static UInt64 Cast(Int16 val)
	{
		if (val < (Int64) std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		return (UInt64)val;
	}
};

template<>
struct number_cast_impl <Int16, UInt128>
{
	static UInt128 Cast(Int16 val)
	{
		if (val < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		return (UInt64)val;
	}
};
#pragma endregion

#pragma region Int24
template<>
struct number_cast_impl<Int24, Int8>
{
	static Int8 Cast(Int24 val)
	{
		if (val.operator int() < std::numeric_limits<Int8>::min()) return std::numeric_limits<Int8>::min();
		if (val.operator int() > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return val;
	}
};

template<>
struct number_cast_impl<Int24, UInt8>
{
	static UInt8 Cast(Int24 val)
	{
		if (val.operator int() < (Int8)std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		return val;
	}
};

template<>
struct number_cast_impl<Int24, Int16>
{
	static Int16 Cast(Int24 val)
	{
		if (val.operator int() < std::numeric_limits<Int16>::min()) return std::numeric_limits<Int16>::min();
		if (val.operator int() > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return val;
	}
};

template<>
struct number_cast_impl<Int24, UInt16>
{
	static UInt16 Cast(Int24 val)
	{
		if (val.operator int() < (Int16) std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		return val;
	}
};

template<>
struct number_cast_impl <Int24, UInt24>
{
	static UInt24 Cast(Int24 val)
	{
		if (val.operator int() < std::numeric_limits<UInt24>::min().operator int()) return std::numeric_limits<UInt24>::min();
		return val.operator UInt24();
	}
};

template<>
struct number_cast_impl <Int24, UInt>
{
	static UInt Cast(Int24 val)
	{
		if (val.operator int() <(int) std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		return val.operator int();
	}
};

template<>
struct number_cast_impl <Int24, ULong>
{
	static ULong Cast(Int24 val)
	{
		if (val.operator int() < (long)std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		return val.operator ULong();
	}
};

template<>
struct number_cast_impl <Int24, UInt64>
{
	static UInt64 Cast(Int24 val)
	{
		if (val.operator int() < (Int64) std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		return val.operator UInt64();
	}
};

template<>
struct number_cast_impl <Int24, UInt128>
{
	static UInt128 Cast(Int24 val)
	{
		if (val.operator int() < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		return val.operator UInt128_t();
	}
};
#pragma endregion

#pragma region Int32
template<>
struct number_cast_impl<Int32, Int8>
{
	static Int8 Cast(Int32 val)
	{
		if (val < std::numeric_limits<Int8>::min()) return std::numeric_limits<Int8>::min();
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return val;
	}
};

template<>
struct number_cast_impl<Int32, UInt8>
{
	static UInt8 Cast(Int32 val)
	{
		if (val < (Int8)std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		return val;
	}
};

template<>
struct number_cast_impl<Int32, Int16>
{
	static Int16 Cast(Int32 val)
	{
		if (val < std::numeric_limits<Int16>::min()) return std::numeric_limits<Int16>::min();
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return val;
	}
};

template<>
struct number_cast_impl<Int32, UInt16>
{
	static UInt16 Cast(Int32 val)
	{
		if (val < (Int16)std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		return val;
	}
};

template<>
struct number_cast_impl<Int32, Int24>
{
	static Int24 Cast(Int32 val)
	{
		if (val < std::numeric_limits<Int24>::min().operator int()) return std::numeric_limits<Int24>::min();
		if (val > std::numeric_limits<Int24>::max().operator int()) return std::numeric_limits<Int24>::max();
		return Int24(val);
	}
};

template<>
struct number_cast_impl <Int32, UInt24>
{
	static UInt24 Cast(Int32 val)
	{
		if (val < std::numeric_limits<UInt24>::min().operator int()) return std::numeric_limits<UInt24>::min();
		return UInt24(val);
	}
};

template<>
struct number_cast_impl <Int32, UInt>
{
	static UInt Cast(Int32 val)
	{
		if (val < (int)std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		return val;
	}
};

template<>
struct number_cast_impl <Int32, ULong>
{
	static ULong Cast(Int32 val)
	{
		if (val < (long)std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		return val;
	}
};

template<>
struct number_cast_impl <Int32, UInt64>
{
	static UInt64 Cast(Int32 val)
	{
		if (val <(Int64) std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		return val;
	}
};

template<>
struct number_cast_impl <Int32, UInt128>
{
	static UInt128 Cast(Int32 val)
	{
		if (val < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		return (UInt64)val;
	}
};
#pragma endregion

#pragma region long
template<>
struct number_cast_impl<long, Int8>
{
	static Int8 Cast(long val)
	{
		if (val < std::numeric_limits<Int8>::min()) return std::numeric_limits<Int8>::min();
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return (Int8)val;
	}
};

template<>
struct number_cast_impl<long, UInt8>
{
	static UInt8 Cast(long val)
	{
		if (val < (Int8)std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		return (UInt8)val;
	}
};

template<>
struct number_cast_impl<long, Int16>
{
	static Int16 Cast(long val)
	{
		if (val < std::numeric_limits<Int16>::min()) return std::numeric_limits<Int16>::min();
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return (Int16)val;
	}
};

template<>
struct number_cast_impl<long, UInt16>
{
	static UInt16 Cast(long val)
	{
		if (val < (Int16)std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		return (UInt16)val;
	}
};

template<>
struct number_cast_impl<long, Int24>
{
	static Int24 Cast(long val)
	{
		if (val < std::numeric_limits<Int24>::min().operator int()) return std::numeric_limits<Int24>::min();
		if (val > std::numeric_limits<Int24>::max().operator int()) return std::numeric_limits<Int24>::max();
		return Int24((int)val);
	}
};

template<>
struct number_cast_impl <long, UInt24>
{
	static UInt24 Cast(long val)
	{
		if (val < std::numeric_limits<UInt24>::min().operator int()) return std::numeric_limits<UInt24>::min();
		return UInt24((int)val);
	}
};

template<>
struct number_cast_impl <long, UInt>
{
	static UInt Cast(long val)
	{
		if (val < (int)std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		return (UInt)val;
	}
};

template<>
struct number_cast_impl <long, ULong>
{
	static ULong Cast(long val)
	{
		if (val < (long)std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		return (ULong)val;
	}
};

template<>
struct number_cast_impl <long, UInt64>
{
	static UInt64 Cast(long val)
	{
		if (val <(Int64)std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		return (UInt64)val;
	}
};

template<>
struct number_cast_impl <long, UInt128>
{
	static UInt128 Cast(long val)
	{
		if (val < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		return (UInt64)val;
	}
};
#pragma endregion

#pragma region Int64
template<>
struct number_cast_impl<Int64, Int8>
{
	static Int8 Cast(Int64 val)
	{
		if (val < std::numeric_limits<Int8>::min()) return std::numeric_limits<Int8>::min();
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return (Int8)val;
	}
}; 

template<>
struct number_cast_impl<Int64, UInt8>
{
	static UInt8 Cast(Int64 val)
	{
		if (val < (Int8)std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		return (UInt8)val;
	}
};

template<>
struct number_cast_impl<Int64, Int16>
{
	static Int16 Cast(Int64 val)
	{
		if (val < std::numeric_limits<Int16>::min()) return std::numeric_limits<Int16>::min();
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return (Int16)val;
	}
};

template<>
struct number_cast_impl<Int64, UInt16>
{
	static UInt16 Cast(Int64 val)
	{
		if (val < (Int16)std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		return (UInt16)val;
	}
};

template<>
struct number_cast_impl<Int64, Int24>
{
	static Int24 Cast(Int64 val)
	{
		if (val < std::numeric_limits<Int24>::min().operator int()) return std::numeric_limits<Int24>::min();
		if (val > std::numeric_limits<Int24>::max().operator int()) return std::numeric_limits<Int24>::max();
		return Int24((int)val);
	}
};

template<>
struct number_cast_impl <Int64, UInt24>
{
	static UInt24 Cast(Int64 val)
	{
		if (val < std::numeric_limits<UInt24>::min().operator int()) return std::numeric_limits<UInt24>::min();
		return UInt24((int)val);
	}
};

template<>
struct number_cast_impl<Int64, int>
{
	static int Cast(Int64 val)
	{
		if (val < std::numeric_limits<int>::min()) return std::numeric_limits<int>::min();
		if (val > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
		return (int)val;
	}
};

template<>
struct number_cast_impl <Int64, UInt>
{
	static UInt Cast(Int64 val)
	{
		if (val < (int)std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		return (UInt)val;
	}
};

template<>
struct number_cast_impl<Int64, long>
{
	static long Cast(Int64 val)
	{
		if (val < std::numeric_limits<long>::min()) return std::numeric_limits<long>::min();
		if (val > std::numeric_limits<long>::max()) return std::numeric_limits<long>::max();
		return (long)val;
	}
};

template<>
struct number_cast_impl <Int64, ULong>
{
	static ULong Cast(Int64 val)
	{
		if (val < (long)std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		return (ULong)val;
	}
};

template<>
struct number_cast_impl <Int64, UInt64>
{
	static UInt64 Cast(Int64 val)
	{
		if (val < (Int64)std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		return (UInt64)val;
	}
};

template<>
struct number_cast_impl <Int64, UInt128>
{
	static UInt128 Cast(Int64 val)
	{
		if (val < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		return (UInt64)val;
	}
};
#pragma endregion

#pragma region Int128
template<>
struct number_cast_impl<Int128, Int8>
{
	static Int8 Cast(Int128 val)
	{
		if (val.operator long double() < std::numeric_limits<Int8>::min()) return std::numeric_limits<Int8>::min();
		if (val.operator long double()> std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return val;
	}
};

template<>
struct number_cast_impl<Int128, UInt8>
{
	static UInt8 Cast(Int128 val)
	{
		if (val.operator long double() <(Int8) std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		return val;
	}
};

template<>
struct number_cast_impl<Int128, Int16>
{
	static Int16 Cast(Int128 val)
	{
		if (val.operator long double() < std::numeric_limits<Int16>::min()) return std::numeric_limits<Int16>::min();
		if (val.operator long double()> std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return val;
	}
};

template<>
struct number_cast_impl<Int128, UInt16>
{
	static UInt16 Cast(Int128 val)
	{
		if (val.operator long double() < (Int16) std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		return val;
	}
};

template<>
struct number_cast_impl<Int128, Int24>
{
	static Int24 Cast(Int128 val)
	{
		if (val.operator long double() < std::numeric_limits<Int24>::min().operator int()) return std::numeric_limits<Int24>::min();
		if (val.operator long double()> std::numeric_limits<Int24>::max().operator int()) return std::numeric_limits<Int24>::max();
		return Int24(val.operator int());
	}
};

template<>
struct number_cast_impl <Int128, UInt24>
{
	static UInt24 Cast(Int128 val)
	{
		if (val.operator long double() < std::numeric_limits<UInt24>::min().operator int()) return std::numeric_limits<UInt24>::min();
		return UInt24(val.operator int());
	}
};

template<>
struct number_cast_impl<Int128, int>
{
	static int Cast(Int128 val)
	{
		if (val.operator long double() < std::numeric_limits<int>::min()) return std::numeric_limits<int>::min();
		if (val.operator long double()> std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
		return val;
	}
};

template<>
struct number_cast_impl <Int128, UInt>
{
	static UInt Cast(Int128 val)
	{
		if (val.operator long double() <(int) std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		return val;
	}
};

template<>
struct number_cast_impl<Int128, long>
{
	static long Cast(Int128 val)
	{
		if (val.operator long double() < std::numeric_limits<long>::min()) return std::numeric_limits<long>::min();
		if (val.operator long double()> std::numeric_limits<long>::max()) return std::numeric_limits<long>::max();
		return val;
	}
};

template<>
struct number_cast_impl <Int128, ULong>
{
	static ULong Cast(Int128 val)
	{
		if (val.operator long double() <(long) std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		return val;
	}
};

template<>
struct number_cast_impl<Int128, Int64>
{
	static Int64 Cast(Int128 val)
	{
		if (val.operator long double() < std::numeric_limits<Int64>::min()) return std::numeric_limits<Int64>::min();
		if (val.operator long double()> std::numeric_limits<Int64>::max()) return std::numeric_limits<Int64>::max();
		return val;
	}
};

template<>
struct number_cast_impl <Int128, UInt64>
{
	static UInt64 Cast(Int128 val)
	{
		if (val.operator long double() <(Int64) std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		return val;
	}
};

template<>
struct number_cast_impl <Int128, UInt128>
{
	static UInt128 Cast(Int128 val)
	{
		if (val.operator long double() < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		return UInt128(val.operator long double());
	}
};
#pragma endregion

#pragma region UInt8
template<>
struct number_cast_impl<UInt8, Int8>
{
	static Int8 Cast(UInt8 val)
	{
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return val;
	}
};

template<>
struct number_cast_impl<UInt8, UInt128>
{
	static UInt128 Cast(UInt8 val)
	{
		return (UInt64)val;
	}
};
#pragma endregion

#pragma region UInt16
template<>
struct number_cast_impl<UInt16, Int8>
{
	static Int8 Cast(UInt16 val)
	{
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return (Int8)val;
	}
};

template<>
struct number_cast_impl<UInt16, Int16>
{
	static Int16 Cast(UInt16 val)
	{
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return (Int16)val;
	}
};

template<>
struct number_cast_impl<UInt16, UInt128>
{
	static UInt128 Cast(UInt16 val)
	{
		return (UInt64)val;
	}
};
#pragma endregion

#pragma region UInt24
template<>
struct number_cast_impl<UInt24, Int8>
{
	static Int8 Cast(UInt24 val)
	{
		if (val.operator int() > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return val;
	}
};

template<>
struct number_cast_impl<UInt24, Int16>
{
	static Int16 Cast(UInt24 val)
	{
		if (val.operator int() > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return val;
	}
};

template<>
struct number_cast_impl<UInt24, Int24>
{
	static Int24 Cast(UInt24 val)
	{
		if (val.operator int() > std::numeric_limits<Int24>::max().operator int()) return std::numeric_limits<Int24>::max();
		return val.operator Int24();
	}
};

template<>
struct number_cast_impl<UInt24, UInt128>
{
	static UInt128 Cast(UInt24 val)
	{
		return (UInt64)val;
	}
};


#pragma endregion

#pragma region UInt
template<>
struct number_cast_impl<UInt, Int8>
{
	static Int8 Cast(UInt val)
	{
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return val;
	}
};

template<>
struct number_cast_impl<UInt, Int16>
{
	static Int16 Cast(UInt val)
	{
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return val;
	}
};

template<>
struct number_cast_impl<UInt, Int24>
{
	static Int24 Cast(UInt val)
	{
		if (val > std::numeric_limits<Int24>::max().operator int()) return std::numeric_limits<Int24>::max();
		return Int24(val);
	}
};

template<>
struct number_cast_impl<UInt, int>
{
	static int Cast(UInt val)
	{
		if (val > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
		return val;
	}
};

template<>
struct number_cast_impl<UInt, long>
{
	static long Cast(UInt val)
	{
		if (val > std::numeric_limits<long>::max()) return std::numeric_limits<long>::max();
		return val;
	}
};

template<>
struct number_cast_impl<UInt, UInt128>
{
	static UInt128 Cast(UInt val)
	{
		return (UInt64)val;
	}
};
#pragma endregion

#pragma region ULong
template<>
struct number_cast_impl<ULong, Int8>
{
	static Int8 Cast(ULong val)
	{
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return (Int8)val;
	}
};

template<>
struct number_cast_impl<ULong, Int16>
{
	static Int16 Cast(ULong val)
	{
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return (Int16)val;
	}
};

template<>
struct number_cast_impl<ULong, Int24>
{
	static Int24 Cast(ULong val)
	{
		if (val > std::numeric_limits<Int24>::max().operator int()) return std::numeric_limits<Int24>::max();
		return Int24(val);
	}
};

template<>
struct number_cast_impl<ULong, int>
{
	static int Cast(ULong val)
	{
		if (val > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
		return val;
	}
};

template<>
struct number_cast_impl<ULong, long>
{
	static long Cast(ULong val)
	{
		if (val > std::numeric_limits<long>::max()) return std::numeric_limits<long>::max();
		return val;
	}
};

template<>
struct number_cast_impl<ULong, Int128>
{
	static Int128 Cast(ULong val)
	{
		return (UInt64)val;
	}
};

template<>
struct number_cast_impl<ULong, UInt128>
{
	static UInt128 Cast(ULong val)
	{
		return (UInt64)val;
	}
};
#pragma endregion

#pragma region UInt64
template<>
struct number_cast_impl<UInt64, Int8>
{
	static Int8 Cast(UInt64 val)
	{
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return (Int8)val;
	}
};

template<>
struct number_cast_impl<UInt64, Int16>
{
	static Int16 Cast(UInt64 val)
	{
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return (Int16)val;
	}
};

template<>
struct number_cast_impl<UInt64, Int24>
{
	static Int24 Cast(UInt64 val)
	{
		if (val > std::numeric_limits<Int24>::max().operator int()) return std::numeric_limits<Int24>::max();
		return Int24((int)val);
	}
};

template<>
struct number_cast_impl<UInt64, int>
{
	static int Cast(UInt64 val)
	{
		if (val > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
		return (int)val;
	}
};

template<>
struct number_cast_impl<UInt64, long>
{
	static long Cast(UInt64 val)
	{
		if (val > std::numeric_limits<long>::max()) return std::numeric_limits<long>::max();
		return (long)val;
	}
};

template<>
struct number_cast_impl<UInt64, Int64>
{
	static Int64 Cast(UInt64 val)
	{
		if (val > std::numeric_limits<Int64>::max()) return std::numeric_limits<Int64>::max();
		return (Int64)val;
	}
};

template<>
struct number_cast_impl<UInt64, UInt128>
{
	static UInt128 Cast(UInt64 val)
	{
		return val;
	}
};
#pragma endregion

#pragma region UInt128
template<>
struct number_cast_impl<UInt128, Int8>
{
	static Int8 Cast(UInt128 val)
	{
		if (val.operator long double() > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return val;
	}
};

template<>
struct number_cast_impl<UInt128, Int16>
{
	static Int16 Cast(UInt128 val)
	{
		if (val.operator long double() > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return val.operator Int16();
	}
};

template<>
struct number_cast_impl<UInt128, Int24>
{
	static Int24 Cast(UInt128 val)
	{
		if (val.operator long double() > std::numeric_limits<Int24>::max().operator int()) return std::numeric_limits<Int24>::max();
		return val.operator Int24();
	}
};

template<>
struct number_cast_impl<UInt128, int>
{
	static int Cast(UInt128 val)
	{
		if (val.operator long double() > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
		return val.operator int();
	}
};

template<>
struct number_cast_impl<UInt128, long>
{
	static long Cast(UInt128 val)
	{
		if (val.operator long double() > std::numeric_limits<long>::max()) return std::numeric_limits<long>::max();
		return val.operator long();
	}
};

template<>
struct number_cast_impl<UInt128, Int64>
{
	static Int64 Cast(UInt128 val)
	{
		if (val.operator long double() > std::numeric_limits<Int64>::max()) return std::numeric_limits<Int64>::max();
		return val.operator Int64();
	}
};

template<>
struct number_cast_impl<UInt128, Int128>
{
	static Int128 Cast(UInt128 val)
	{
		return  val.operator Int128_t();
	}
};
#pragma endregion

#pragma region float
template<>
struct number_cast_impl<float, Int8>
{
	static Int8 Cast(float val)
	{
		if (val < std::numeric_limits<Int8>::min()) return std::numeric_limits<Int8>::min();
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return (Int8)val;
	}
};

template<>
struct number_cast_impl<float, UInt8>
{
	static UInt8 Cast(float val)
	{
		if (val < std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		if (val > std::numeric_limits<UInt8>::max()) return std::numeric_limits<UInt8>::max();
		return (UInt8)val;
	}
};

template<>
struct number_cast_impl<float, Int16>
{
	static Int16 Cast(float val)
	{
		if (val < std::numeric_limits<Int16>::min()) return std::numeric_limits<Int16>::min();
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return (Int16)val;
	}
};

template<>
struct number_cast_impl<float, UInt16>
{
	static UInt16 Cast(float val)
	{
		if (val < std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		if (val > std::numeric_limits<UInt16>::max()) return std::numeric_limits<UInt16>::max();
		return (UInt16)val;
	}
};

template<>
struct number_cast_impl<float, Int24>
{
	static Int24 Cast(float val)
	{
		if (val < std::numeric_limits<Int24>::min().operator long double()) return std::numeric_limits<Int24>::min();
		if (val > std::numeric_limits<Int24>::max().operator long double()) return std::numeric_limits<Int24>::max();
		return Int24((int)val);
	}
};

template<>
struct number_cast_impl<float, UInt24>
{
	static UInt24 Cast(float val)
	{
		if (val < std::numeric_limits<UInt24>::min().operator long double()) return std::numeric_limits<UInt24>::min();
		if (val > std::numeric_limits<UInt24>::max().operator long double()) return std::numeric_limits<UInt24>::max();
		return UInt24((int)val);
	}
};

template<>
struct number_cast_impl<float, int>
{
	static int Cast(float val)
	{
		if (val < std::numeric_limits<int>::min()) return std::numeric_limits<int>::min();
		if (val > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
		return (int)val;
	}
};

template<>
struct number_cast_impl<float, UInt>
{
	static UInt Cast(float val)
	{
		if (val < std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		if (val > std::numeric_limits<UInt>::max()) return std::numeric_limits<UInt>::max();
		return (UInt)val;
	}
};

template<>
struct number_cast_impl<float, long>
{
	static long Cast(float val)
	{
		if (val < std::numeric_limits<long>::min()) return std::numeric_limits<long>::min();
		if (val > std::numeric_limits<long>::max()) return std::numeric_limits<long>::max();
		return (long)val;
	}
};

template<>
struct number_cast_impl<float, ULong>
{
	static ULong Cast(float val)
	{
		if (val < std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		if (val > std::numeric_limits<ULong>::max()) return std::numeric_limits<ULong>::max();
		return (ULong)val;
	}
};

template<>
struct number_cast_impl<float, Int64>
{
	static Int64 Cast(float val)
	{
		if (val < std::numeric_limits<Int64>::min()) return std::numeric_limits<Int64>::min();
		if (val > std::numeric_limits<Int64>::max()) return std::numeric_limits<Int64>::max();
		return (Int64)val;
}
};

template<>
struct number_cast_impl<float, UInt64>
{
	static UInt64 Cast(float val)
	{
		if (val < std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		if (val > std::numeric_limits<UInt64>::max()) return std::numeric_limits<UInt64>::max();
		return (UInt64)val;
	}
};

template<>
struct number_cast_impl<float, Int128>
{
	static Int128 Cast(float val)
	{
		if (val < std::numeric_limits<Int128>::min().operator long double()) return std::numeric_limits<Int128>::min();
		if (val > std::numeric_limits<Int128>::max().operator long double()) return std::numeric_limits<Int128>::max();
		return val;
	}
};

template<>
struct number_cast_impl<float, UInt128>
{
	static UInt128 Cast(float val)
	{
		if (val < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		if (val > std::numeric_limits<UInt128>::max().operator long double()) return std::numeric_limits<UInt128>::max();
		return val;
	}
};


#pragma endregion

#pragma region double
template<>
struct number_cast_impl<double, Int8>
{
	static Int8 Cast(double val)
	{
		if (val < std::numeric_limits<Int8>::min()) return std::numeric_limits<Int8>::min();
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return (Int8)val;
	}
};

template<>
struct number_cast_impl<double, UInt8>
{
	static UInt8 Cast(double val)
	{
		if (val < std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		if (val > std::numeric_limits<UInt8>::max()) return std::numeric_limits<UInt8>::max();
		return (UInt8)val;
	}
};

template<>
struct number_cast_impl<double, Int16>
{
	static Int16 Cast(double val)
	{
		if (val < std::numeric_limits<Int16>::min()) return std::numeric_limits<Int16>::min();
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return (Int16)val;
	}
};

template<>
struct number_cast_impl<double, UInt16>
{
	static UInt16 Cast(double val)
	{
		if (val < std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		if (val > std::numeric_limits<UInt16>::max()) return std::numeric_limits<UInt16>::max();
		return (UInt16)val;
	}
};

template<>
struct number_cast_impl<double, Int24>
{
	static Int24 Cast(double val)
	{
		if (val < std::numeric_limits<Int24>::min().operator long double()) return std::numeric_limits<Int24>::min();
		if (val > std::numeric_limits<Int24>::max().operator long double()) return std::numeric_limits<Int24>::max();
		return Int24((int)val);
	}
};

template<>
struct number_cast_impl<double, UInt24>
{
	static UInt24 Cast(double val)
	{
		if (val < std::numeric_limits<UInt24>::min().operator long double()) return std::numeric_limits<UInt24>::min();
		if (val > std::numeric_limits<UInt24>::max().operator long double()) return std::numeric_limits<UInt24>::max();
		return UInt24((UInt)val);
	}
};

template<>
struct number_cast_impl<double, int>
{
	static int Cast(double val)
	{
		if (val < std::numeric_limits<int>::min()) return std::numeric_limits<int>::min();
		if (val > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
		return (int)val;
	}
};

template<>
struct number_cast_impl<double, UInt>
{
	static UInt Cast(double val)
	{
		if (val < std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		if (val > std::numeric_limits<UInt>::max()) return std::numeric_limits<UInt>::max();
		return (UInt)val;
	}
};

template<>
struct number_cast_impl<double, long>
{
	static long Cast(double val)
	{
		if (val < std::numeric_limits<long>::min()) return std::numeric_limits<long>::min();
		if (val > std::numeric_limits<long>::max()) return std::numeric_limits<long>::max();
		return (long)val;
	}
};

template<>
struct number_cast_impl<double, ULong>
{
	static ULong Cast(double val)
	{
		if (val < std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		if (val > std::numeric_limits<ULong>::max()) return std::numeric_limits<ULong>::max();
		return (ULong)val;
	}
};

template<>
struct number_cast_impl<double, Int64>
{
	static Int64 Cast(double val)
	{
		if (val < std::numeric_limits<Int64>::min()) return std::numeric_limits<Int64>::min();
		if (val > std::numeric_limits<Int64>::max()) return std::numeric_limits<Int64>::max();
		return (Int64)val;
	}
};

template<>
struct number_cast_impl<double, UInt64>
{
	static UInt64 Cast(double val)
	{
		if (val < std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		if (val > std::numeric_limits<UInt64>::max()) return std::numeric_limits<UInt64>::max();
		return (UInt64)val;
	}
};

template<>
struct number_cast_impl<double, Int128>
{
	static Int128 Cast(double val)
	{
		if (val < std::numeric_limits<Int128>::min().operator long double()) return std::numeric_limits<Int128>::min();
		if (val > std::numeric_limits<Int128>::max().operator long double()) return std::numeric_limits<Int128>::max();
		return val;
	}
};

template<>
struct number_cast_impl<double, UInt128>
{
	static UInt128 Cast(double val)
	{
		if (val < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		if (val > std::numeric_limits<UInt128>::max().operator long double()) return std::numeric_limits<UInt128>::max();
		return val;
	}
};

template<>
struct number_cast_impl<double, float>
{
	static float Cast(double val)
	{
		if (val < -std::numeric_limits<float>::max()) return -std::numeric_limits<float>::max();
		if (val > std::numeric_limits<float>::max()) return std::numeric_limits<float>::max();
		return (float)val;
	}
};

#pragma endregion

#pragma region long double
template<>
struct number_cast_impl<long double, Int8>
{
	static Int8 Cast(long double val)
	{
		if (val < std::numeric_limits<Int8>::min()) return std::numeric_limits<Int8>::min();
		if (val > std::numeric_limits<Int8>::max()) return std::numeric_limits<Int8>::max();
		return (Int8)val;
	}
};

template<>
struct number_cast_impl<long double, UInt8>
{
	static UInt8 Cast(long double val)
	{
		if (val < std::numeric_limits<UInt8>::min()) return std::numeric_limits<UInt8>::min();
		if (val > std::numeric_limits<UInt8>::max()) return std::numeric_limits<UInt8>::max();
		return (UInt8)val;
	}
};

template<>
struct number_cast_impl<long double, Int16>
{
	static Int16 Cast(long double val)
	{
		if (val < std::numeric_limits<Int16>::min()) return std::numeric_limits<Int16>::min();
		if (val > std::numeric_limits<Int16>::max()) return std::numeric_limits<Int16>::max();
		return (Int16)val;
	}
};

template<>
struct number_cast_impl<long double, UInt16>
{
	static UInt16 Cast(long double val)
	{
		if (val < std::numeric_limits<UInt16>::min()) return std::numeric_limits<UInt16>::min();
		if (val > std::numeric_limits<UInt16>::max()) return std::numeric_limits<UInt16>::max();
		return (UInt16)val;
	}
};

template<>
struct number_cast_impl<long double, Int24>
{
	static Int24 Cast(long double val)
	{
		if (val < std::numeric_limits<Int24>::min().operator long double()) return std::numeric_limits<Int24>::min();
		if (val > std::numeric_limits<Int24>::max().operator long double()) return std::numeric_limits<Int24>::max();
		return Int24((int)val);
	}
};

template<>
struct number_cast_impl<long double, UInt24>
{
	static UInt24 Cast(long double val)
	{
		if (val < std::numeric_limits<UInt24>::min().operator long double()) return std::numeric_limits<UInt24>::min();
		if (val > std::numeric_limits<UInt24>::max().operator long double()) return std::numeric_limits<UInt24>::max();
		return (UInt)val;
	}
};

template<>
struct number_cast_impl<long double, int>
{
	static int Cast(long double val)
	{
		if (val < std::numeric_limits<int>::min()) return std::numeric_limits<int>::min();
		if (val > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
		return (int)val;
	}
};

template<>
struct number_cast_impl<long double, UInt>
{
	static UInt Cast(long double val)
	{
		if (val < std::numeric_limits<UInt>::min()) return std::numeric_limits<UInt>::min();
		if (val > std::numeric_limits<UInt>::max()) return std::numeric_limits<UInt>::max();
		return (UInt)val;
	}
};

template<>
struct number_cast_impl<long double, long>
{
	static long Cast(long double val)
	{
		if (val < std::numeric_limits<long>::min()) return std::numeric_limits<long>::min();
		if (val > std::numeric_limits<long>::max()) return std::numeric_limits<long>::max();
		return (long)val;
	}
};

template<>
struct number_cast_impl<long double, ULong>
{
	static ULong Cast(long double val)
	{
		if (val < std::numeric_limits<ULong>::min()) return std::numeric_limits<ULong>::min();
		if (val > std::numeric_limits<ULong>::max()) return std::numeric_limits<ULong>::max();
		return (ULong)val;
	}
};

template<>
struct number_cast_impl<long double, Int64>
{
	static Int64 Cast(long double val)
	{
		if (val < std::numeric_limits<Int64>::min()) return std::numeric_limits<Int64>::min();
		if (val > std::numeric_limits<Int64>::max()) return std::numeric_limits<Int64>::max();
		return (Int64)val;
	}
};

template<>
struct number_cast_impl<long double, UInt64>
{
	static UInt64 Cast(long double val)
	{
		if (val < std::numeric_limits<UInt64>::min()) return std::numeric_limits<UInt64>::min();
		if (val > std::numeric_limits<UInt64>::max()) return std::numeric_limits<UInt64>::max();
		return (UInt64)val;
	}
};

template<>
struct number_cast_impl<long double, Int128>
{
	static Int128 Cast(long double val)
	{
		if (val < std::numeric_limits<Int128>::min().operator long double()) return std::numeric_limits<Int128>::min();
		if (val > std::numeric_limits<Int128>::max().operator long double()) return std::numeric_limits<Int128>::max();
		return val;
	}
};

template<>
struct number_cast_impl<long double, UInt128>
{
	static UInt128 Cast(long double val)
	{
		if (val < std::numeric_limits<UInt128>::min().operator long double()) return std::numeric_limits<UInt128>::min();
		if (val > std::numeric_limits<UInt128>::max().operator long double()) return std::numeric_limits<UInt128>::max();
		return val;
	}
};

template<>
struct number_cast_impl<long double, float>
{
	static float Cast(long double val)
	{
		if (val < -std::numeric_limits<float>::max()) return -std::numeric_limits<float>::max();
		if (val > std::numeric_limits<float>::max()) return std::numeric_limits<float>::max();
		return (float)val;
	}
};

template<>
struct number_cast_impl<long double, double>
{
	static double Cast(long double val)
	{
		if (val < -std::numeric_limits<double>::max()) return -std::numeric_limits<double>::max();
		if (val > std::numeric_limits<double>::max()) return std::numeric_limits<double>::max();
		return (double)val;
}
};
#pragma endregion
#pragma warning( pop )
#endif

#ifndef USWITCHCASE
#define USWITCHCASE
typedef UInt64 uhash;

#define CONSTEXPER_SUPPORTED _MSC_VER>1800

#if CONSTEXPER_SUPPORTED

constexpr uhash __uhash_prime = 0x100000001B3ull;
constexpr uhash __uhash_basis = 0xCBF29CE484222325ull;

constexpr uhash __uhash_compile_time(char const* str, uhash last_value = __uhash_basis)
{
	return *str ? __uhash_compile_time(str + 1, (*str ^ last_value) * __uhash_prime) : last_value;
}

#define __uhashstr(STR) __uhash_compile_time(#STR)

#else

#error need VC compiler later than 1800.

#endif

#endif