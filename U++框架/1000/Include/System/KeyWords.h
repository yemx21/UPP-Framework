#pragma once
namespace System
{
	class Object;
	class Type;
	class Assembly;
}

#define USTR(str) #str
#define __u_va_num_args_impl(_1,_2,_3,_4,_5,_6,N,...) N
#define __u_va_num_args(...) __u_va_num_args_impl_((__VA_ARGS__, 6,5,4,3,2,1))
#define __u_va_num_args_impl_(tuple) __u_va_num_args_impl tuple

#define __uref _declspec(USTR(ureflect))
#define __udefctor __uref _declspec(USTR(udefaultconstructor))
#define __uctor __uref _declspec(USTR(uconstructor))

#define __uclass class __uref
#define __uenum class __uref


#define __uobjdef(...)  _declspec(USTR(uobjdef(__VA_ARGS__)))

#pragma warning(disable: 4068)

#define __uvalue(...) __uvalue_impl1((__u_va_num_args(__VA_ARGS__), __VA_ARGS__))
#define __uvalue_impl1(tuple) __uvalue_impl2 tuple
#define __uvalue_impl2(N,...) __uvalue_impl3(N,(__VA_ARGS__))
#define __uvalue_impl3(N,tuple) __uvalue ## N tuple

#define __uvalue1(MEMBER) MEMBER,
#define __uvalue2(MEMBER, VALUE)  MEMBER = VALUE,

#define __ufield(...)  _declspec(USTR(uaccess(__VA_ARGS__)))

#define __uobject  public System::Object
#define __vuobject  virtual public System::Object
#define __uproperty(...)  __uref _declspec(property(__VA_ARGS__))
#define __uimport(...)  __pragma(ucompiler(import,__VA_ARGS__))
#define __ucompile(...)  __pragma(ucompiler(__VA_ARGS__))

typedef const System::Type* _type;
#define __umeta  static _type

/// <summary>
/// ²»¿É¸´ÖÆ
/// </summary>
struct _noncopyable
{
protected:
	_noncopyable() = default;
	~_noncopyable() = default;

	_noncopyable(const _noncopyable&) = delete;
	_noncopyable& operator=(const _noncopyable&) = delete;
};


