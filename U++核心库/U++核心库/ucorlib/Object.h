#pragma once
#include "Type.h"
#include "IFormattable.h"
#include <memory>

namespace System
{
	typedef void(*CreateMetaFunc)(Type&);


	/// <summary>
	/// 基础对象
	/// </summary>
	class RUNTIME_API Object : public IFormattable
	{
	protected:
		constexpr Object() {}
		/// <summary>
		/// 自定义字符串转换
		/// </summary>
		/// <param name="fmt">格式符</param>
		/// <param name="loc">区域性信息</param>
		/// <returns></returns>
		virtual std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const;

		/// <summary>
		/// 元数据
		/// </summary>
		__umeta Meta;

	public:

		virtual ~Object();

		/// <summary>
		/// 获取类型描述
		/// </summary>
		/// <returns></returns>
		virtual _type GetType() const;

		/// <summary>
		/// 判断是否指定类型
		/// </summary>
		/// <param name="type">指定类型</param>
		bool Is(_type type) const;

		/// <summary>
		/// 判断指定对象是否与当前对象相同
		/// </summary>
		/// <param name="ref">指定对象</param>
		/// <returns></returns>
		virtual bool Equals(const Object* ref) const;

		/// <summary>
		/// 获取当前对象哈希值
		/// </summary>
		/// <returns></returns>
		virtual uhash GetHashCode() const;
	};

	template<class T>
	inline T* SafeCast(Object* base)
	{
		if (base != NULL && base->Is(TypeHelper<T>::GetType()))
		{
			return static_cast<T*>(base);
		}
		else
		{
			return NULL;
		}
	}

	template<class T>
	inline const T* SafeCast(const Object* base)
	{
		if (base != NULL && base->Is(TypeHelper<T>::GetType()))
		{
			return static_cast<const T*>(base);
		}
		else
		{
			return NULL;
		}
	}

	typedef std::shared_ptr<Object> ObjectPtr;

}
