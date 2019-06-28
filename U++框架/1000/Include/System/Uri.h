#pragma once
#include "Object.h"
#include "TypeConverter.h"
#include <mutex>
#include "ClassInfo.h"
#include "EnumInfo.h"
namespace System
{
	class UriHelper;

	class RUNTIME_API Uri final: __uobject
	{
		friend class UriHelper;
	public:
#pragma region Scheme
		/// <summary>
		/// 方案
		/// </summary>
		class RUNTIME_API Scheme final : __uobject
		{
		private:
			static void DefineMeta(ClassInfo&);
			template<class T>
			friend void ClassInfo::Create(Type const*& pointer, const wchar_t* name, unsigned int size, ClassFlags flags);
		private:
			friend class UriHelper;
			std::wstring value;
		protected:
			std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
		public:
			/// <summary>
			/// 创建<see cref="Scheme"/>实例.
			/// </summary>
			__uref Scheme();
			/// <summary>
			/// 创建<see cref="Scheme"/>的拷贝实例.
			/// </summary>
			__uref Scheme(const std::wstring& scheme);
			/// <summary>
			/// 判断是否为空
			/// </summary>
			/// <returns></returns>
			__uref bool GetIsEmpty() const;
			/// <summary>
			/// 获取值
			/// </summary>
			/// <returns></returns>
			__uref const std::wstring& GetValue() const;
			/// <summary>
			/// 是否为空
			/// </summary>
			__uproperty(get = GetIsEmpty)bool IsEmpty;
			/// <summary>
			/// 值
			/// </summary>
			__uproperty(get = GetValue) const std::wstring& Value;
			/// <summary>
			/// 尝试解析
			/// </summary>
			/// <param name="input">输入值</param>
			/// <param name="loc">区域性信息</param>
			/// <param name="result">结果</param>
			/// <returns></returns>
			static bool TryParse(const std::wstring& input, const CultureInfo& loc, Scheme& result);
			_type GetType() const override;
		};

		class RUNTIME_API SchemeConverter : public TypeConverter
		{
		private:
			static std::mutex& getlocker();
			static std::weak_ptr<SchemeConverter> _thisWeakPtr;
		protected:
			SchemeConverter();
			SchemeConverter(SchemeConverter const&);
			void operator=(SchemeConverter const&);
		public:
			virtual ~SchemeConverter();

			static std::shared_ptr<SchemeConverter> GetSharedInstance();

			static SchemeConverter& GetInstance();

			bool CanConvertFrom(_type type) const;
			bool CanConvertTo(_type type) const;

			Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
			Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

			Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
			std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
		};

#pragma endregion

#pragma region HostType
		class RUNTIME_API HostType final : __uobject
		{
		public:
			typedef Int8 enumbase;
		private:
			Int8 value;
			static void DefineMeta(EnumInfo&);
			template<class T, class BASE>
			friend typename std::enable_if< std::is_integral<BASE>::value, void>::type EnumInfo::Create(System::Type const*& pointer, const wchar_t* name);
		protected:
			std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
		public:
			_type GetType() const override;
			~HostType();
			HostType(const Int8& val);
			HostType(const HostType& t);
			HostType(HostType&& t);
			HostType& operator=(const Int8& val);
			HostType& operator=(const HostType& t);
			operator Int8() const;
			uhash GetHashCode() const override;
			bool Equals(const Object* ref) const override;

		public:
			enum __uweakenums : enumbase
			{
				Null,
				DomainName,
				IPv4Address,
				IPv6Address,
			};
		public:
			HostType();
		};
#pragma endregion

#pragma region Authority
		/// <summary>
		/// 权利信息(服务器的域名系统 (DNS) 主机名或 IP 地址和端口号)
		/// </summary>
		class RUNTIME_API Authority final : __uobject
		{
		private:
			static void DefineMeta(ClassInfo&);
			template<class T>
			friend void ClassInfo::Create(Type const*& pointer, const wchar_t* name, unsigned int size, ClassFlags flags);
		protected:
			std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
		private:
			friend class UriHelper;
			HostType hostType;
			std::wstring host;
			UShort port;
		public:
			/// <summary>
			/// 创建<see cref="Authority"/>实例.
			/// </summary>
			__uref Authority();
			/// <summary>
			/// 创建<see cref="Authority"/>的拷贝实例.
			/// </summary>
			__uref Authority(const std::wstring& v);
			/// <summary>
			/// 判断是否为空
			/// </summary>
			/// <returns></returns>
			__uref bool GetIsEmpty() const;
			/// <summary>
			/// 获取主机名
			/// </summary>
			/// <returns></returns>
			__uref const std::wstring& GetHost() const;
			/// <summary>
			/// 获取主机类型
			/// </summary>
			/// <returns></returns>
			__uref HostType GetHostType() const;
			/// <summary>
			/// 获取端口号
			/// </summary>
			/// <returns></returns>
			__uref UShort GetPort() const;

			/// <summary>
			/// 获取编码的字符串表达式
			/// </summary>
			__uref std::wstring GetEncodedString() const;

			/// <summary>
			/// 是否为空
			/// </summary>
			__uproperty(get = GetIsEmpty) bool IsEmpty;
			/// <summary>
			/// 主机名
			/// </summary>
			__uproperty(get = GetHost) const std::wstring& Host;
			/// <summary>
			/// 主机类型
			/// </summary>
			__uproperty(get = GetHostType) Uri::HostType HostType;
			/// <summary>
			/// 端口号
			/// </summary>
			__uproperty(get = GetPort) UShort Port;
			/// <summary>
			/// 编码的字符串表达式
			/// </summary>
			__uproperty(get = GetEncodedString) std::wstring EncodedString;
			/// <summary>
			/// 尝试解析
			/// </summary>
			/// <param name="input">输入值</param>
			/// <param name="loc">区域性信息</param>
			/// <param name="result">结果</param>
			/// <returns></returns>
			static bool TryParse(const std::wstring& input, const CultureInfo& loc, Authority& result);
			_type GetType() const override;
		};

		class RUNTIME_API AuthorityConverter : public TypeConverter
		{
		private:
			static std::mutex& getlocker();
			static std::weak_ptr<AuthorityConverter> _thisWeakPtr;
		protected:
			AuthorityConverter();
			AuthorityConverter(AuthorityConverter const&);
			void operator=(AuthorityConverter const&);
		public:
			virtual ~AuthorityConverter();

			static std::shared_ptr<AuthorityConverter> GetSharedInstance();

			static AuthorityConverter& GetInstance();

			bool CanConvertFrom(_type type) const;
			bool CanConvertTo(_type type) const;

			Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
			Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

			Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
			std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
		};
#pragma endregion

#pragma region Path
		/// <summary>
		/// 路径
		/// </summary>
		class RUNTIME_API Path final : __uobject
		{
		private:
			static void DefineMeta(ClassInfo&);
			template<class T>
			friend void ClassInfo::Create(Type const*& pointer, const wchar_t* name, unsigned int size, ClassFlags flags);
		protected:
			std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
		public:
			typedef std::list<std::wstring> segments_type;
			typedef segments_type::const_iterator const_iterator;
		private:
			friend class UriHelper;
			bool absolute;
			bool isdirectory;
			segments_type segments;
		public:
			/// <summary>
			/// 创建<see cref="Path"/>实例.
			/// </summary>
			__uref Path();
			/// <summary>
			/// 创建<see cref="Path"/>的拷贝实例.
			/// </summary>
			/// <param name="value">字符串表达式</param>
			__uref Path(const std::wstring& value);
			/// <summary>
			/// 判断是否为空
			/// </summary>
			/// <returns></returns>
			__uref bool GetIsEmpty() const;
			/// <summary>
			/// 判断是否为绝对路径
			/// </summary>
			/// <returns></returns>
			__uref bool GetIsAbsolute() const;
			/// <summary>
			/// 设置是否为绝对路径
			/// </summary>
			__uref void SetIsAbsolute(bool v);
			/// <summary>
			/// 判断是否为文件夹路径
			/// </summary>
			/// <returns></returns>
			__uref bool GetIsDirectory() const;
			/// <summary>
			/// 设置是否为文件夹路径
			/// </summary>
			__uref void SetIsDirectory(bool);
			/// <summary>
			/// 获取首端段落
			/// </summary>
			__uref const std::wstring& front() const;
			/// <summary>
			/// 获取末端段落
			/// </summary>
			__uref const std::wstring& back() const;
			/// <summary>
			/// 移除首端段落并重置绝对路径状态
			/// </summary>
			__uref void popfront();
			/// <summary>
			/// 移除匹配的末端段落并重置绝对路径状态
			/// </summary>
			__uref bool popback(const Path& back);
			/// <summary>
			/// 判断匹配指定前缀路径
			/// </summary>
			__uref bool IsMatchPrefix(const Path& rhs) const;
			/// <summary>
			/// 清空段落并重置绝对路径和文件夹状态
			/// </summary>
			__uref void Clear();
			/// <summary>
			/// 获取段落数量
			/// </summary>
			__uref size_t GetSize() const;
			bool operator ==(const Path& rhs) const;
			bool operator !=(const Path& rhs) const;
			bool operator <(const Path& rhs) const;
			Path operator +(const std::wstring& rhs) const;
			Path& operator +=(const std::wstring& rhs);
			Path operator +(const Path& rhs) const;
			Path& operator +=(const Path& rhs);
			/// <summary>
			/// 获取编码的字符串表达式
			/// </summary>
			__uref std::wstring GetEncodedString() const;
			const_iterator begin() const;
			const_iterator end() const;

			/// <summary>
			/// 是否为空
			/// </summary>
			__uproperty(get = GetIsEmpty) bool IsEmpty;
			/// <summary>
			/// 是否为绝对路径
			/// </summary>
			__uproperty(get = GetIsAbsolute, put = SetIsAbsolute) bool IsAbsolute;
			/// <summary>
			/// 是否为文件夹路径
			/// </summary>
			__uproperty(get = GetHostType, put = SetIsDirectory) bool IsDirectory;
			/// <summary>
			/// 获取段落数量
			/// </summary>
			__uproperty(get = GetSize) size_t Size;
			/// <summary>
			/// 编码的字符串表达式
			/// </summary>
			__uproperty(get = GetEncodedString) std::wstring EncodedString;
			/// <summary>
			/// 尝试解析
			/// </summary>
			/// <param name="input">输入值</param>
			/// <param name="loc">区域性信息</param>
			/// <param name="result">结果</param>
			/// <returns></returns>
			static bool TryParse(const std::wstring& input, const CultureInfo& loc, Path& result);
			_type GetType() const override;
		};

		class RUNTIME_API PathConverter : public TypeConverter
		{
		private:
			static std::mutex& getlocker();
			static std::weak_ptr<PathConverter> _thisWeakPtr;
		protected:
			PathConverter();
			PathConverter(PathConverter const&);
			void operator=(PathConverter const&);
		public:
			virtual ~PathConverter();

			static std::shared_ptr<PathConverter> GetSharedInstance();

			static PathConverter& GetInstance();

			bool CanConvertFrom(_type type) const;
			bool CanConvertTo(_type type) const;

			Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
			Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

			Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
			std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
		};
#pragma endregion

#pragma region Query
		/// <summary>
		/// 查询信息
		/// </summary>
		class RUNTIME_API Query final : __uobject
		{
		private:
			static void DefineMeta(ClassInfo&);
			template<class T>
			friend void ClassInfo::Create(Type const*& pointer, const wchar_t* name, unsigned int size, ClassFlags flags);
		protected:
			std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
		public:
			typedef std::vector<std::pair<std::wstring, std::wstring>> valuestype;
			typedef valuestype::const_iterator const_iterator;
			typedef valuestype::iterator iterator;
			typedef valuestype::value_type value_type;
		private:
			friend class UriHelper;
			bool sorted;
			valuestype values;
		public:
			/// <summary>
			/// 创建<see cref="Query"/>实例.
			/// </summary>
			__uref Query();
			/// <summary>
			/// 创建<see cref="Query"/>的拷贝实例.
			/// </summary>
			/// <param name="value">字符串表达式</param>
			__uref Query(const std::wstring& value, bool dosort = false);

			/// <summary>
			/// 判断是否为空
			/// </summary>
			/// <returns></returns>
			__uref bool GetIsEmpty() const;
			/// <summary>
			/// 排序
			/// </summary>
			__uref void Sort();
			/// <summary>
			/// 判断是否已排序
			/// </summary>
			/// <returns></returns>
			__uref bool GetIsSorted() const;
			/// <summary>
			/// 获取编码的字符串表达式
			/// </summary>
			/// <returns></returns>
			__uref std::wstring GetEncodedString() const;
			/// <summary>
			/// 查询指定键值
			/// </summary>
			/// <returns></returns>
			const_iterator Find(const std::wstring& key) const;
			/// <summary>
			/// 查询指定键值
			/// </summary>
			/// <returns></returns>
			iterator Find(const std::wstring& key);
			/// <summary>
			/// 是否为空
			/// </summary>
			__uproperty(get = GetIsEmpty) bool IsEmpty;
			/// <summary>
			/// 是否已排序
			/// </summary>
			__uproperty(get = GetIsSorted) bool IsSorted;
			/// <summary>
			/// 编码的字符串表达式
			/// </summary>
			__uproperty(get = GetEncodedString) std::wstring EncodedString;
			/// <summary>
			/// 尝试解析
			/// </summary>
			/// <param name="input">输入值</param>
			/// <param name="loc">区域性信息</param>
			/// <param name="result">结果</param>
			/// <returns></returns>
			static bool TryParse(const std::wstring& input, const CultureInfo& loc, Query& result);
			_type GetType() const override;
		};
		class RUNTIME_API QueryConverter : public TypeConverter
		{
		private:
			static std::mutex& getlocker();
			static std::weak_ptr<QueryConverter> _thisWeakPtr;
		protected:
			QueryConverter();
			QueryConverter(QueryConverter const&);
			void operator=(QueryConverter const&);
		public:
			virtual ~QueryConverter();

			static std::shared_ptr<QueryConverter> GetSharedInstance();

			static QueryConverter& GetInstance();

			bool CanConvertFrom(_type type) const;
			bool CanConvertTo(_type type) const;

			Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
			Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

			Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
			std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
		};
#pragma endregion

#pragma region Fragment
		/// <summary>
		/// 片段信息
		/// </summary>
		class RUNTIME_API Fragment final : __uobject
		{
		private:
			static void DefineMeta(ClassInfo&);
			template<class T>
			friend void ClassInfo::Create(Type const*& pointer, const wchar_t* name, unsigned int size, ClassFlags flags);
		protected:
			std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
		private:
			friend class UriHelper;
			std::wstring value;
		public:
			/// <summary>
			/// 创建<see cref="Fragment"/>实例.
			/// </summary>
			__uref Fragment();
			/// <summary>
			/// 创建<see cref="Fragment"/>的拷贝实例.
			/// </summary>
			/// <param name="value">字符串表达式</param>
			__uref Fragment(const std::wstring& value);
			/// <summary>
			/// 判断是否为空
			/// </summary>
			/// <returns></returns>
			__uref bool GetIsEmpty() const;
			/// <summary>
			/// 获取解码的字符串表达式
			/// </summary>
			__uref const std::wstring& GetDecodedString() const;
			/// <summary>
			/// 获取编码的字符串表达式
			/// </summary>
			__uref std::wstring GetEncodedString() const;
			/// <summary>
			/// 是否为空
			/// </summary>
			__uproperty(get = GetIsEmpty) bool IsEmpty;
			/// <summary>
			/// 解码的字符串表达式
			/// </summary>
			__uproperty(get = GetDecodedString) const std::wstring& DecodedString;
			/// <summary>
			/// 编码的字符串表达式
			/// </summary>
			__uproperty(get = GetEncodedString) std::wstring EncodedString;
			/// <summary>
			/// 尝试解析
			/// </summary>
			/// <param name="input">输入值</param>
			/// <param name="loc">区域性信息</param>
			/// <param name="result">结果</param>
			/// <returns></returns>
			static bool TryParse(const std::wstring& input, const CultureInfo& loc, Fragment& result);
			_type GetType() const override;
		};
		class RUNTIME_API FragmentConverter : public TypeConverter
		{
		private:
			static std::mutex& getlocker();
			static std::weak_ptr<FragmentConverter> _thisWeakPtr;
		protected:
			FragmentConverter();
			FragmentConverter(FragmentConverter const&);
			void operator=(FragmentConverter const&);
		public:
			virtual ~FragmentConverter();

			static std::shared_ptr<FragmentConverter> GetSharedInstance();

			static FragmentConverter& GetInstance();

			bool CanConvertFrom(_type type) const;
			bool CanConvertTo(_type type) const;

			Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
			Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

			Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
			std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
		};
#pragma endregion

	private:
		static void DefineMeta(ClassInfo&);
		template<class T>
		friend void ClassInfo::Create(Type const*& pointer, const wchar_t* name, unsigned int size, ClassFlags flags);
		Scheme scheme;
		Authority authority;
		Path path;
		Query query;
		Fragment fragment;
	protected:
		std::wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
	public:
		__uref Uri();
		__uref Uri(const std::wstring& str);

		/// <summary>
		/// 判断是否为空
		/// </summary>
		/// <returns></returns>
		__uref bool GetIsEmpty() const;
		/// <summary>
		/// 判断是否相对
		/// </summary>
		/// <returns></returns>
		bool GetIsRelative() const;
		/// <summary>
		/// 获取方案
		/// </summary>
		/// <returns></returns>
		const Scheme& GetScheme() const;
		/// <summary>
		/// 获取方案引用
		/// </summary>
		/// <returns></returns>
		Scheme& GetSchemeRef();
		/// <summary>
		/// 获取权利信息
		/// </summary>
		/// <returns></returns>
		const Authority& GetAuthority() const;
		/// <summary>
		/// 获取权利信息引用
		/// </summary>
		/// <returns></returns>
		Authority& GetAuthorityRef();
		/// <summary>
		/// 获取路径
		/// </summary>
		/// <returns></returns>
		const Path& GetPath();
		/// <summary>
		/// 获取路径引用
		/// </summary>
		/// <returns></returns>
		Path& GetPathRef();
		/// <summary>
		/// 获取查询信息
		/// </summary>
		/// <returns></returns>
		const Query& GetQuery() const;
		/// <summary>
		/// 获取查询信息引用
		/// </summary>
		/// <returns></returns>
		Query& GetQueryRef();
		/// <summary>
		/// 获取片段信息
		/// </summary>
		/// <returns></returns>
		const Fragment& GetFragment() const;
		/// <summary>
		/// 获取片段信息引用
		/// </summary>
		/// <returns></returns>
		Fragment& GetFragmentRef();
		/// <summary>
		/// 获取编码的字符串表达式
		/// </summary>
		__uref std::wstring GetEncodedString() const;
		/// <summary>
		/// 是否为空
		/// </summary>
		__uproperty(get = GetIsEmpty) bool IsEmpty;
		/// <summary>
		/// 是否相对
		/// </summary>
		__uproperty(get = GetIsRelative) bool IsRelative;
		/// <summary>
		/// 方案引用
		/// </summary>
		__uproperty(get = GetSchemeRef) Uri::Scheme& SchemePropertyRef;
		/// <summary>
		/// 方案
		/// </summary>
		__uproperty(get = GetScheme) const Uri::Scheme& SchemeProperty;
		/// <summary>
		/// 权利信息引用
		/// </summary>
		__uproperty(get = GetAuthorityRef) Uri::Authority& AuthorityPropertyRef;
		/// <summary>
		/// 权利信息
		/// </summary>
		__uproperty(get = GetAuthority) const Uri::Authority& AuthorityProperty;
		/// <summary>
		/// 路径引用
		/// </summary>
		__uproperty(get = GetPathRef) Uri::Path& PathPropertyRef;
		/// <summary>
		/// 路径
		/// </summary>
		__uproperty(get = GetPath) const Uri::Path& PathProperty;
		/// <summary>
		/// 查询信息引用
		/// </summary>
		__uproperty(get = GetQueryRef) Uri::Query& QueryPropertyRef;
		/// <summary>
		/// 查询信息
		/// </summary>
		__uproperty(get = GetQuery) const Uri::Query& QueryProperty;
		/// <summary>
		/// 片段信息引用
		/// </summary>
		__uproperty(get = GetFragmentRef) Uri::Fragment& FragmentPropertyRef;
		/// <summary>
		/// 片段信息
		/// </summary>
		__uproperty(get = GetFragment) const Uri::Fragment& FragmentProperty;
		/// <summary>
		/// 编码的字符串表达式
		/// </summary>
		__uproperty(get = GetEncodedString) std::wstring EncodedString;

		/// <summary>
		/// 尝试解析
		/// </summary>
		/// <param name="input">输入值</param>
		/// <param name="loc">区域性信息</param>
		/// <param name="result">结果</param>
		/// <returns></returns>
		static bool TryParse(const std::wstring& input, const CultureInfo& loc, Uri& result);

		_type GetType() const override;
	};

	class RUNTIME_API UriConverter : public TypeConverter
	{
	private:
		static std::mutex& getlocker();
		static std::weak_ptr<UriConverter> _thisWeakPtr;
	protected:
		UriConverter();
		UriConverter(UriConverter const&);
		void operator=(UriConverter const&);
	public:
		virtual ~UriConverter();

		static std::shared_ptr<UriConverter> GetSharedInstance();

		static UriConverter& GetInstance();

		bool CanConvertFrom(_type type) const;
		bool CanConvertTo(_type type) const;

		Any ConvertFrom(const Any& obj, const CultureInfo& loc) const;
		Any ConvertTo(const Any& obj, _type type, const CultureInfo& loc) const;

		Any ConvertFromString(const std::wstring& str, const CultureInfo& loc) const;
		std::wstring ConvertToString(const Any& obj, const CultureInfo& loc) const;
	};

}