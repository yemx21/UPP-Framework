#pragma once
#include "..\..\UIFramework_Config.h"

using namespace System::UI::Graphic;
namespace System
{
	namespace UI
	{
		namespace Windows
		{
			namespace Controls
			{
				class Border;
				typedef std::shared_ptr<Border> BorderPtr;

				__uclass __uobjdef(asm = UIFramework, ns = System::UI::Windows::Controls, ver = 1.0) UIFRAMEWORK_API Decorator : public UIElement
				{
				private:
					UIElementPtr _child;
				protected:
					virtual System::Size MeasureOverride(const System::Size& availableSize) override;
					virtual System::Size ArrangeOverride(const System::Size& finalSize) override;

					virtual UIElementPtr GetChildCore() const;
					virtual void SetChildCore(UIElementPtr child);
				public:
					__udefctor Decorator();
					virtual ~Decorator();

					UIElementPtr GetChild() const;
					void SetChild(UIElementPtr child);

					__uproperty(get = GetChild, put = SetChild) UIElementPtr Child;

					REFLECT_CLASS(Decorator)
				};
			}
		}
	}
}