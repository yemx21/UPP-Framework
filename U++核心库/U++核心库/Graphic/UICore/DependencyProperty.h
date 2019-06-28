#pragma once
#include "DependantObject.h"
namespace System
{
	namespace UI
	{
		__uenum __uobjdef(asm = UICore, ns = System::UI, ver = 1.0) UICORE_API FrameworkPropertyMetadataOptions : __uobject
		{ 
			__uvalue_begin(FrameworkPropertyMetadataOptions, UInt)
			__uvalue(None, 0x0)
			__uvalue(AffectsMeasure,0x1)
			__uvalue(AffectsArrange,0x2)
			__uvalue(AffectsRender,0x4)
			__uvalue(Inherits,0x8)
			__uvalue(OverridesInheritanceBehavior,0x10)
			__uvalue(AffectsUpdate,0x20)
			__uvalue_end(FrameworkPropertyMetadataOptions, None)
		};

		class PropertyChangedEventArgs;
		typedef std::shared_ptr<PropertyChangedEventArgs> PropertyChangedEventArgsPtr;

		class UICORE_API PropertyChangedEventArgs
		{
		public:
			PropertyChangedEventArgs(const SharedVoid& oldValue, const SharedVoid& newValue);

			const SharedVoid& OldValue() const;

			const SharedVoid& NewValue() const;

			template <class T>
			std::shared_ptr<T> NewValuePtr() const
			{
				return std::dynamic_pointer_cast<T>(std::static_pointer_cast<T>(newValue));
			}

			template <class T>
			std::shared_ptr<T> OldValuePtr() const
			{
				return std::dynamic_pointer_cast<T>(std::static_pointer_cast<T>(oldValue));
			}

			template <class T>
			T NewValue() const
			{
				return *(T*)(newValue.get());
			}

			template <class T>
			T OldValue() const
			{
				return *(T*)(oldValue.get());
			}

			template <class T>
			T& NewValueRef() const
			{
				return *(T*)(newValue.get());
			}

			template <class T>
			T& OldValueRef() const
			{
				return *(T*)(oldValue.get());
			}
		private:
			SharedVoid oldValue;
			SharedVoid newValue;
		};

		template<bool _VAL>
		struct ValueComparer
		{

		};

		template<>
		struct ValueComparer<false>
		{
			template<typename T>
			inline static bool Compare(T v1, T v2)
			{
				return v1 == v2;
			}
		};

		template<>
		struct ValueComparer<true>
		{
			template<typename T>
			inline static bool Compare(T v1, T v2)
			{
				return *v1 == *v2;
			}
		};

		template <class T>
		bool ValueEqualityCallback(const SharedVoid& oldVal, const SharedVoid& newVal)
		{
			T*  oldPtr = static_cast<T*>(oldVal.get());
			T*  newPtr = static_cast<T*>(newVal.get());
			return ValueComparer<is_call_possible<T, bool(const T&, const T&)>::value>::Compare<T*>(oldPtr, newPtr);
		}


		class DependencyObject;

		typedef std::function<bool(DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)> PropertyPreCoerceCallback;
		typedef std::function<bool(DependencyObject* const dependencyObject, const PropertyChangedEventArgs& args)> PropertyChangedCallback;
		typedef std::function<bool(const SharedVoid& oldValue, const SharedVoid& newValue)> PropertyEqualityCallback;

		class FrameworkPropertyMetadata;
		typedef std::shared_ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadataPtr;

		class UICORE_API FrameworkPropertyMetadata
		{
		public:
			FrameworkPropertyMetadata(SharedVoid defaultValue,
				FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
				PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback = nullptr);
		public:
			SharedVoid DefaultValue() const;
			PropertyChangedCallback ChangeCallback() const;
			PropertyEqualityCallback EqualityCallback() const;
			PropertyPreCoerceCallback PreCoerceCallback() const;
			FrameworkPropertyMetadataOptions FrameworkPropertyMetadataOptionsFlags() const;

			bool IsDepenantObjectType() const;

			bool Inherits() const;

			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue)
			{
				return CreateTemplate<T>(defaultValue, FrameworkPropertyMetadataOptions::None, nullptr, &ValueEqualityCallback<T>, nullptr);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, FrameworkPropertyMetadataOptions::None, nullptr, &ValueEqualityCallback<T>, nullptr);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags)
			{
				return CreateTemplate<T>(defaultValue, frameworkPropertyMetadataOptionsFlags, nullptr, &ValueEqualityCallback<T>, nullptr);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, frameworkPropertyMetadataOptionsFlags, nullptr, &ValueEqualityCallback<T>, nullptr);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags, PropertyChangedCallback changeCallback)
			{
				return CreateTemplate<T>(defaultValue, frameworkPropertyMetadataOptionsFlags, changeCallback, &ValueEqualityCallback<T>, nullptr);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags, PropertyChangedCallback changeCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, frameworkPropertyMetadataOptionsFlags, changeCallback, &ValueEqualityCallback<T>, nullptr);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags, PropertyEqualityCallback propertyEqualityCallback)
			{
				return CreateTemplate<T>(defaultValue, frameworkPropertyMetadataOptionsFlags, nullptr, propertyEqualityCallback, nullptr);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags, PropertyEqualityCallback propertyEqualityCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, frameworkPropertyMetadataOptionsFlags, nullptr, propertyEqualityCallback, nullptr);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
				PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				return CreateTemplate<T>(defaultValue, frameworkPropertyMetadataOptionsFlags, nullptr, &ValueEqualityCallback<T>, propertyPreCoerceCallback);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
					PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, frameworkPropertyMetadataOptionsFlags, nullptr, &ValueEqualityCallback<T>, propertyPreCoerceCallback);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
				PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback)
			{
				return CreateTemplate<T>(defaultValue, frameworkPropertyMetadataOptionsFlags, changeCallback, propertyEqualityCallback, nullptr);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
					PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, frameworkPropertyMetadataOptionsFlags, changeCallback, propertyEqualityCallback, nullptr);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
				PropertyChangedCallback changeCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				return CreateTemplate<T>(defaultValue, frameworkPropertyMetadataOptionsFlags, changeCallback, &ValueEqualityCallback<T>, propertyPreCoerceCallback);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
					PropertyChangedCallback changeCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, frameworkPropertyMetadataOptionsFlags, changeCallback, &ValueEqualityCallback<T>, propertyPreCoerceCallback);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
				PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				return CreateTemplate<T>(defaultValue, frameworkPropertyMetadataOptionsFlags, nullptr, propertyEqualityCallback, propertyPreCoerceCallback);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
					PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, frameworkPropertyMetadataOptionsFlags, nullptr, propertyEqualityCallback, propertyPreCoerceCallback);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
				PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				return CreateTemplate<T>(defaultValue, frameworkPropertyMetadataOptionsFlags, changeCallback, propertyEqualityCallback, propertyPreCoerceCallback);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags, 
					PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, frameworkPropertyMetadataOptionsFlags, changeCallback, propertyEqualityCallback, propertyPreCoerceCallback);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue,  PropertyChangedCallback changeCallback)
			{
				return CreateTemplate<T>(defaultValue, FrameworkPropertyMetadataOptions::None, changeCallback, &ValueEqualityCallback<T>, nullptr);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue,  PropertyChangedCallback changeCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, FrameworkPropertyMetadataOptions::None, changeCallback, &ValueEqualityCallback<T>, nullptr);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, 
				PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback)
			{
				return CreateTemplate<T>(defaultValue, FrameworkPropertyMetadataOptions::None, changeCallback, propertyEqualityCallback, nullptr);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, 
					PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, FrameworkPropertyMetadataOptions::None, changeCallback, propertyEqualityCallback, nullptr);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue,
				PropertyChangedCallback changeCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				return CreateTemplate<T>(defaultValue, FrameworkPropertyMetadataOptions::None, changeCallback, &ValueEqualityCallback<T>, propertyPreCoerceCallback);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue,
					PropertyChangedCallback changeCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, FrameworkPropertyMetadataOptions::None, changeCallback, &ValueEqualityCallback<T>, propertyPreCoerceCallback);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue, 
				PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				return CreateTemplate<T>(defaultValue, FrameworkPropertyMetadataOptions::None, changeCallback, propertyEqualityCallback, propertyPreCoerceCallback);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue, 
					PropertyChangedCallback changeCallback, PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, FrameworkPropertyMetadataOptions::None, changeCallback, propertyEqualityCallback, propertyPreCoerceCallback);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue,
				PropertyEqualityCallback propertyEqualityCallback)
			{
				return CreateTemplate<T>(defaultValue, FrameworkPropertyMetadataOptions::None, nullptr, propertyEqualityCallback, nullptr);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue,
					PropertyEqualityCallback propertyEqualityCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, FrameworkPropertyMetadataOptions::None, nullptr, propertyEqualityCallback, nullptr);
			}


			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue,
				PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				return CreateTemplate<T>(defaultValue, FrameworkPropertyMetadataOptions::None, nullptr, propertyEqualityCallback, propertyPreCoerceCallback);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue,
					PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, FrameworkPropertyMetadataOptions::None, nullptr, propertyEqualityCallback, propertyPreCoerceCallback);
			}

			template <typename T>
			static FrameworkPropertyMetadataPtr Create(const std::shared_ptr<T>& defaultValue,
				PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				return CreateTemplate<T>(defaultValue, FrameworkPropertyMetadataOptions::None, nullptr, &ValueEqualityCallback<T>, propertyPreCoerceCallback);
			}

			template <typename T>
			typename std::enable_if<!std::is_assignable<SharedVoid, T>::value, FrameworkPropertyMetadataPtr>::type
				static Create(const T&& defaultValue,
					PropertyPreCoerceCallback propertyPreCoerceCallback)
			{
				auto defaultBoxedValue = make_shared<T>(defaultValue);
				return CreateTemplate<T>(defaultBoxedValue, FrameworkPropertyMetadataOptions::None, nullptr, &ValueEqualityCallback<T>, propertyPreCoerceCallback);
			}

		private:

			template <typename T>
			typename std::enable_if<!std::is_assignable<DependantObjectPtr, T>::value, FrameworkPropertyMetadataPtr>::type
				static CreateTemplate(const std::shared_ptr<T>& defaultValue,
				FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
				PropertyChangedCallback changeCallback,
				PropertyEqualityCallback propertyEqualityCallback = nullptr, PropertyPreCoerceCallback propertyPreCoerceCallback = nullptr)
			{

				return std::make_shared<FrameworkPropertyMetadata>(defaultValue,
					frameworkPropertyMetadataOptionsFlags,
					changeCallback,
					propertyEqualityCallback, propertyPreCoerceCallback);
			}

			template <typename T>
			typename std::enable_if<std::is_assignable<DependantObjectPtr, T>::value, FrameworkPropertyMetadataPtr>::type
				static CreateTemplate(const std::shared_ptr<T>& defaultValue,
				FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags,
				PropertyChangedCallback changeCallback,
				PropertyEqualityCallback propertyEqualityCallback, PropertyPreCoerceCallback propertyPreCoerceCallback = nullptr)
			{
				auto metaData = std::make_shared<FrameworkPropertyMetadata>(defaultValue,
					frameworkPropertyMetadataOptionsFlags,
					changeCallback,
					propertyEqualityCallback, propertyPreCoerceCallback);
				metaData->isDependantObjectType = true;
				return metaData;
			}
		private:
			SharedVoid defaultValue;
			FrameworkPropertyMetadataOptions frameworkPropertyMetadataOptionsFlags;
			bool isDependantObjectType;

			PropertyChangedCallback changeCallback;
			PropertyEqualityCallback propertyEqualityCallback;
			PropertyPreCoerceCallback propertyPreCoerceCallback;
		};

		class DependencyProperty;

		typedef std::function<bool(const DependencyObject* obj)> IsOfOwnerTypeCallback;

		class UICORE_API IDependencyProperty
		{
		protected:
			bool isReadOnly;
			InertiaString name;
			FrameworkPropertyMetadataPtr propertyMetadata;
		protected:
			friend DependencyObject;
			IsOfOwnerTypeCallback isofOwnerType;
		public:
			IDependencyProperty(const std::wstring& name, const IsOfOwnerTypeCallback ofOwnerTypeCallback, const FrameworkPropertyMetadataPtr& propertyMetadata, bool readOnly = false);

			bool GetIsReadOnly() const;
			const std::wstring& GetName() const;
			FrameworkPropertyMetadataPtr GetPropertyMetadata() const;

			bool IsOfOwnerType(const DependencyObject* obj)
			{
				return isofOwnerType(obj);
			}

			_declspec(property(get = GetIsReadOnly)) bool IsReadOnly;
			_declspec(property(get = GetName)) const std::wstring& Name;
			_declspec(property(get = GetPropertyMetadata)) FrameworkPropertyMetadataPtr& PropertyMetadata;
		};

		class UICORE_API DependencyProperty
		{
		private:
			typedef void(*fun__ustatic_ctor_impl)();
			fun__ustatic_ctor_impl fun_ustaticdp;
			std::shared_ptr<IDependencyProperty> impl;
		protected:
			friend DependencyObject;
		private:
			template<class T>
			static bool IsOfOwnerTypeTemplate(const DependencyObject* obj)
			{
				auto casted = dynamic_cast<const T*>(obj);
				return casted != nullptr;
			}

			void Register(const std::wstring& name, const IsOfOwnerTypeCallback ofOwnerTypeCallback, const FrameworkPropertyMetadataPtr& propertyMetadata);
			void RegisterReadOnly(const std::wstring& name, const IsOfOwnerTypeCallback ofOwnerTypeCallback, const FrameworkPropertyMetadataPtr& propertyMetadata);
		public:
			DependencyProperty(fun__ustatic_ctor_impl ustatic_ptr);

			template <class T>
			void Register(const std::wstring& name, const FrameworkPropertyMetadataPtr& propertyMetadata)
			{  
				static_assert(is_ustatic_constructor_supported<T>::value, "__usctordef is not defined in T");
				auto isofOwnerType = &IsOfOwnerTypeTemplate < T > ;
				return Register(name, isofOwnerType, propertyMetadata);
			}

			template <class T>
			void RegisterReadOnly(const std::wstring& name, const FrameworkPropertyMetadataPtr& propertyMetadata)
			{
				static_assert(is_ustatic_constructor_supported<T>::value, "__usctordef is not defined in T");
				auto isofOwnerType = &IsOfOwnerTypeTemplate < T > ;
				return RegisterReadOnly(name, isofOwnerType, propertyMetadata);
			}

			std::shared_ptr<IDependencyProperty> operator ->();

		};

		class UICORE_API DependencyPropertyPtr : public std::shared_ptr<DependencyProperty>
		{
		public:
			std::shared_ptr<IDependencyProperty> operator->() const noexcept
			{
				return (this->_Get()->operator ->());
			}
		};

		typedef DependencyPropertyPtr DependencyPropertyKeyPtr;

		template<class T>
		class DependencyPropertyCore: public DependencyProperty
		{
		public:
			DependencyPropertyCore() :DependencyProperty(&T::__ustatic_ctor_impl)
			{

			}
		};

		template<class T, class... _Types>
		inline DependencyPropertyPtr make_dependencyproperty(_Types&&... _Args)
		{
			std::_Ref_count_obj<DependencyPropertyCore<T>> *_Rx =
				new std::_Ref_count_obj<DependencyPropertyCore<T>>(std::forward<_Types>(_Args)...);

			DependencyPropertyPtr _Ret;
			_Ret._Resetp0(_Rx->_Getptr(), _Rx);
			return (_Ret);
		}

	}
}

namespace System
{
	__uenum_flag(System::UI::FrameworkPropertyMetadataOptions)
}
