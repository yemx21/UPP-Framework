#pragma once
#include "Panel.h"


using namespace System::UI::Graphic;

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			namespace Controls
			{
				class Canvas;
				typedef std::shared_ptr<Canvas> CanvasPtr;

				__uclass __uobjdef(asm = UIFramework, ns = System::UI::Windows::Controls, ver = 1.0) UIFRAMEWORK_API Canvas : public Panel
				{
				protected:
					virtual System::Size MeasureOverride(const System::Size& availableSize) override;
					virtual System::Size ArrangeOverride(const System::Size& finalSize) override;
				public:
					static const DependencyPropertyPtr LeftProperty;
					static const DependencyPropertyPtr TopProperty;
					static const DependencyPropertyPtr RightProperty;
					static const DependencyPropertyPtr BottomProperty;
				public:
					__udefctor Canvas(void);
					virtual ~Canvas(void);
				public:
					static void SetTop(const VisualPtr& element, double top);
					static void SetLeft(const VisualPtr& element, double top);
					static void SetBottom(const VisualPtr& element, double top);
					static void SetRight(const VisualPtr& element, double top);

					static double GetTop(const VisualPtr& element);
					static double GetLeft(const VisualPtr& element);
					static double GetBottom(const VisualPtr& element);
					static double GetRight(const VisualPtr& element);
				private:
					static bool LocationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);

					__usctordef(Canvas)
					REFLECT_CLASS(Canvas)
				};
			}
		}
	}
}