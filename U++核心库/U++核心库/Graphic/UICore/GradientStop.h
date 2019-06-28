#pragma once
#include "UIObject.h"

namespace System
{
	namespace UI
	{
		namespace Media
		{
			class GradientStop;
			typedef std::shared_ptr<GradientStop> GradientStopPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Media, ver = 1.0) UICORE_API GradientStop : public UIObject, public IObservableObject < GradientStop >
			{
			protected:
				wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
			public:
				__udefctor GradientStop();
				__uctor GradientStop(double offset, const System::Color& color);
				virtual ~GradientStop() {}
				GradientStop(const GradientStop& stop);

				__uref bool operator == (const GradientStop& stop) const;
				__uref bool operator != (const GradientStop& stop) const;
			public:
				System::Color GetColor() const;
				void SetColor(const System::Color& color);
				double GetOffset() const;
				void SetOffset(double offset);
			public:
				static const DependencyPropertyPtr ColorProperty;
				static const DependencyPropertyPtr OffsetProperty;

				__uproperty(get = GetColor, put = SetColor) System::Color Color;
				__uproperty(get = GetOffset, put = SetOffset) double Offset;

				__usctordef(GradientStop)
				REFLECT_CLASS(GradientStop)
			};
		}
	}
}