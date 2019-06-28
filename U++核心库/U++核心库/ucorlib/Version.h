#pragma once
#include "Object.h"
#include "TypeConverter.h"
#include <mutex>
#include "ClassInfo.h"

namespace System
{
	/// <summary>
	/// 版本类
	/// </summary>
	class RUNTIME_API Version final: __uobject
	{
	private:
		int _Build;
		int _Revision;
		int _Major;
		int _Minor;
		static void DefineMeta(ClassInfo&);
		template<class T>
		friend void ClassInfo::Create(Type const*& pointer, const wchar_t* name, unsigned int size, ClassFlags flags);
	protected:
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
	public:
		/// <summary>
		/// 创建<see cref="Version"/>实例.
		/// </summary>
		/// <param name="major">主版本号</param>
		/// <param name="minor">次要版本号</param>
		/// <param name="build">内部版本号</param>
		/// <param name="revision">修订号</param>
		__uref Version(int major, int minor, int build, int revision);		

		/// <summary>
		/// 创建<see cref="Version"/>实例.
		/// </summary>
		/// <param name="major">主版本号</param>
		/// <param name="minor">次要版本号</param>
		/// <param name="build">内部版本号</param>
		__uref Version(int major, int minor, int build);

		/// <summary>
		/// 创建<see cref="Version"/>实例.
		/// </summary>
		/// <param name="major">主版本号</param>
		/// <param name="minor">次要版本号</param>
		__uref Version(int major, int minor);

		/// <summary>
		/// 创建<see cref="Version"/>实例.
		/// </summary>
		__uref Version();

		/// <summary>
		/// 创建<see cref="Version"/>的拷贝实例.
		/// </summary>
		__uref Version(const Version& ver);

		/// <summary>
		/// 获取主版本号
		/// </summary>
		/// <returns></returns>
		int GetMajor() const;

		/// <summary>
		/// 获取次要版本号
		/// </summary>
		/// <returns></returns>
		int GetMinor() const;

		/// <summary>
		/// 获取内部版本号
		/// </summary>
		/// <returns></returns>
		int GetBuild() const;

		/// <summary>
		/// 获取修订号
		/// </summary>
		/// <returns></returns>
		int GetRevision() const;

		/// <summary>
		/// 获取修订号的高16位
		/// </summary>
		/// <returns></returns>
		short GetMajorRevision() const;

		/// <summary>
		/// 获取修订号的低16位
		/// </summary>
		/// <returns></returns>
		short GetMinorRevision() const;

		/// <summary>
		/// 设置主版本号
		/// </summary>
		/// <param name="major">主版本号</param>
		void SetMajor(int major);

		/// <summary>
		/// 设置次要版本号
		/// </summary>
		/// <param name="major">次要版本号</param>
		void SetMinor(int minor);
		

		/// <summary>
		/// 设置内部版本号
		/// </summary>
		/// <param name="major">内部版本号</param>
		void SetBuild(int build);

		/// <summary>
		/// 设置修订号
		/// </summary>
		/// <param name="major">修订号</param>
		void SetRevision(int revision);

		/// <summary>
		/// 判断是否为空
		/// </summary>
		/// <returns></returns>
		bool GetIsEmpty() const;

		/// <summary>
		/// 主版本号
		/// </summary>
		__uproperty(get = GetMajor, put = SetMajor) int Major;
		/// <summary>
		/// 次要版本号
		/// </summary>
		__uproperty(get = GetMinor, put = SetMinor) int Minor;
		/// <summary>
		/// 内部版本号
		/// </summary>
		__uproperty(get = GetBuild, put = SetBuild) int Build;
		/// <summary>
		/// 修订号
		/// </summary>
		__uproperty(get = GetRevision, put = SetRevision) int Revision;
		/// <summary>
		/// 修订号的高16位
		/// </summary>
		__uproperty(get = GetMajorRevision) short MajorRevision;
		/// <summary>
		/// 修订号的低16位
		/// </summary>
		__uproperty(get = GetMinorRevision) short MinorRevision;
		/// <summary>
		/// 是否为空
		/// </summary>
		__uproperty(get = GetIsEmpty)bool IsEmpty;

		__uref bool operator == (Version& ver) const;
		__uref bool operator == (const Version& ver) const;
		__uref bool operator != (Version& ver) const;
		__uref bool operator != (const Version& ver) const;
		__uref bool operator > (Version& ver) const;
		__uref bool operator > (const Version& Ver) const;
		__uref bool operator < (Version& ver) const;
		__uref bool operator < (const Version& ver) const;
		__uref bool operator >= (Version& ver) const;
		__uref bool operator >= (const Version& ver) const;
		__uref bool operator <= (Version& ver) const;
		__uref bool operator <= (const Version& ver) const;

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, Version& result);

		_type GetType() const override;
	};

	class RUNTIME_API VersionConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<VersionConverter> _thisWeakPtr;
	protected:
		VersionConverter();
		VersionConverter(VersionConverter const&);
		void operator=(VersionConverter const&);
	public:
		virtual ~VersionConverter();

		static std::shared_ptr<VersionConverter> GetSharedInstance();

		static VersionConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};

}