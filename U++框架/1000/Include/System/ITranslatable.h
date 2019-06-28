#pragma once
#include "CultureInfo.h"
#include "Reflection.h"
namespace System
{
	/// <summary>
	/// 翻译引用接口
	/// </summary>
	class RUNTIME_API ITranslatable : __uobject
	{
	public:
		/// <summary>
		/// 获取翻译源
		/// </summary>
		/// <returns></returns>
		__uref virtual InertiaString GetSource() const = 0;

		/// <summary>
		/// 获取是否启用翻译
		/// </summary>
		/// <returns></returns>
		__uref virtual bool GetEnabled() const = 0;

		/// <summary>
		/// 设置是否启用翻译
		/// </summary>
		/// <returns></returns>
		__uref virtual void SetEnabled(bool)= 0;

		/// <summary>
		/// 翻译源
		/// </summary>
		__uproperty(get = GetSource) InertiaString Source;

		/// <summary>
		/// 是否启用翻译
		/// </summary>
		__uproperty(get = GetEnabled, put = SetEnabled) bool Enabled;

		REFLECT_CLASS(ITranslatable)
	};

	/// <summary>
	/// 翻译引用接口
	/// </summary>
	class RUNTIME_API ITranslatable1:  public ITranslatable
	{
	public:
		/// <summary>
		/// 获取翻译源
		/// </summary>
		/// <returns></returns>
		__uref virtual InertiaString GetSource() const =0;

		/// <summary>
		/// 获取是否启用翻译
		/// </summary>
		/// <returns></returns>
		__uref virtual bool GetEnabled() const = 0;

		/// <summary>
		/// 设置是否启用翻译
		/// </summary>
		/// <returns></returns>
		__uref virtual void SetEnabled(bool)= 0;

		/// <summary>
		/// 获取是否独立于当前全局区域性信息
		/// </summary>
		/// <returns></returns>
		__uref virtual bool GetIsDependent() const = 0;

		/// <summary>
		/// 获取翻译关联上下文
		/// </summary>
		/// <returns></returns>
		__uref virtual InertiaString GetTranslationContext() const = 0;

		/// <summary>
		/// 翻译源
		/// </summary>
		__uproperty(get = GetSource) InertiaString Source;

		/// <summary>
		/// 是否启用翻译
		/// </summary>
		__uproperty(get = GetEnabled, put = SetEnabled) bool Enabled;

		/// <summary>
		/// 是否独立于当前全局区域性信息
		/// </summary>
		__uproperty(get = GetIsDependent) bool IsDependent;

		/// <summary>
		/// 翻译关联上下文
		/// </summary>
		__uproperty(get = GetTranslationContext) InertiaString TranslationContext;

		REFLECT_CLASS(ITranslatable1)
	};

	/// <summary>
	/// 简易翻译引用
	/// </summary>
	class RUNTIME_API SimpleTranslationReference : public ITranslatable
	{
	private:
		InertiaString _source;
		bool _isEnabled;
	public:
		/// <summary>
		/// 创建 <see cref="SimpleTranslationReference"/>实例.
		/// </summary>
		/// <param name="source">翻译源</param>
		/// <param name="isEnabled">是否启用翻译</param>
		__uctor SimpleTranslationReference(const InertiaString& source, bool isEnabled = true);

		/// <summary>
		/// 获取翻译源
		/// </summary>
		/// <returns></returns>
		__uref InertiaString GetSource() const override;

		/// <summary>
		/// 获取是否启用翻译
		/// </summary>
		/// <returns></returns>
		__uref bool GetEnabled() const override;

		/// <summary>
		/// 设置是否启用翻译
		/// </summary>
		/// <returns></returns>
		__uref void SetEnabled(bool) override;

		/// <summary>
		/// 翻译源
		/// </summary>
		__uproperty(get = GetSource) InertiaString Source;

		/// <summary>
		/// 是否启用翻译
		/// </summary>
		__uproperty(get = GetEnabled, put = SetEnabled) bool Enabled;


		REFLECT_CLASS(SimpleTranslationReference)
	};

	/// <summary>
	/// 简易翻译引用1
	/// </summary>
	class RUNTIME_API SimpleTranslationReference1 : public ITranslatable1
	{
	private:
		InertiaString _source;
		InertiaString _translationContext;
		bool _isEnabled;
		bool _isDependent;
	public:
		/// <summary>
		/// 创建 <see cref="SimpleTranslationReference1"/>实例.
		/// </summary>
		/// <param name="source">翻译源</param>
		/// <param name="translationContext">翻译关联上下文</param>
		/// <param name="isEnabled">是否启用翻译</param>
		/// <param name="isDependent">是否独立于当前全局区域性信息</param>
		__uctor SimpleTranslationReference1(const InertiaString& source, const InertiaString& translationContext, bool isEnabled = true, bool isDependent = false);

		/// <summary>
		/// 获取翻译源
		/// </summary>
		/// <returns></returns>
		__uref InertiaString GetSource() const override;

		/// <summary>
		/// 获取是否启用翻译
		/// </summary>
		/// <returns></returns>
		__uref bool GetEnabled() const override;

		/// <summary>
		/// 设置是否启用翻译
		/// </summary>
		/// <returns></returns>
		__uref void SetEnabled(bool) override;

		/// <summary>
		/// 获取是否独立于当前全局区域性信息
		/// </summary>
		/// <returns></returns>
		__uref bool GetIsDependent() const override;

		/// <summary>
		/// 获取翻译关联上下文
		/// </summary>
		/// <returns></returns>
		__uref InertiaString GetTranslationContext() const override;

		/// <summary>
		/// 翻译源
		/// </summary>
		__uproperty(get = GetSource) InertiaString Source;

		/// <summary>
		/// 是否启用翻译
		/// </summary>
		__uproperty(get = GetEnabled, put = SetEnabled) bool Enabled;

		/// <summary>
		/// 是否独立于当前全局区域性信息
		/// </summary>
		__uproperty(get = GetIsDependent) bool IsDependent;

		/// <summary>
		/// 翻译关联上下文
		/// </summary>
		__uproperty(get = GetTranslationContext) InertiaString TranslationContext;
		REFLECT_CLASS(SimpleTranslationReference1)
	};
}