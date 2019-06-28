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
				class Panel;
				typedef std::shared_ptr<Panel> PanelPtr;

				__uclass __uobjdef(asm = UIFramework, ns = System::UI::Windows::Controls, ver = 1.0) UIFRAMEWORK_API Panel : public UIElement
				{
				public:
					Panel(void);
					virtual ~Panel(void);
				public:
					int GetChildCount() const;
					const UIElementPtr GetChild(int index) const;
					void RemoveChild(const UIElementPtr& child);
					void AddChild(const UIElementPtr& child);
				protected:
					virtual bool HitTest(const Point&) override;
					virtual void RenderOverride(const RenderContextPtr&) final {}
				protected:
					virtual System::Size MeasureOverride(const System::Size& availableSize) override;
					virtual System::Size ArrangeOverride(const System::Size& finalSize) override;

					REFLECT_CLASS(Panel)
				};
			}
		}
	}
}