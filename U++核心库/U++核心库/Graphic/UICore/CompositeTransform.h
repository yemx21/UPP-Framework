#pragma once
#include "Transform.h"

namespace System
{
	namespace UI
	{
		namespace Media
		{
			class CompositeTransform;
			typedef std::shared_ptr<CompositeTransform> CompositeTransformPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Media, ver = 1.0) UICORE_API CompositeTransform : public Transform
			{
			public:
				__udefctor CompositeTransform();
				CompositeTransform(const CompositeTransform& transform);

				virtual ~CompositeTransform();
				
				__uref bool operator == (const CompositeTransform& transform) const;
				__uref bool operator != (const CompositeTransform&  transform) const;

				double GetCenterX() const;
				void SetCenterX(double val);

				double GetCenterY() const;
				void SetCenterY(double val);

				double GetScaleX() const;
				void SetScaleX(double val);

				double GetScaleY() const;
				void SetScaleY(double val);

				double GetSkewX() const;
				void SetSkewX(double val);

				double GetSkewY() const;
				void SetSkewY(double val);

				double GetTranslateX() const;
				void SetTranslateX(double val);

				double GetTranslateY() const;
				void SetTranslateY(double val);

				double GetRotation() const;
				void SetRotation(double val);

				System::Matrix GetMatrix() const;

				virtual operator System::Matrix() override;
				__uref virtual System::Matrix GetTransformMatrix();

				__uproperty(get = GetCenterX, put = SetCenterX) double CenterX;
				__uproperty(get = GetCenterY, put = SetCenterY) double CenterY;
				__uproperty(get = GetScaleX, put = SetScaleX) double ScaleX;
				__uproperty(get = GetScaleY, put = SetScaleY) double ScaleY;
				__uproperty(get = GetSkewX, put = SetSkewX) double SkewX;
				__uproperty(get = GetSkewY, put = SetSkewY) double SkewY;
				__uproperty(get = GetTranslateX, put = SetTranslateX) double TranslateX;
				__uproperty(get = GetTranslateY, put = SetTranslateY) double TranslateY;
				__uproperty(get = GetRotation, put = SetRotation) double Rotation;
				__uproperty(get = GetMatrix) System::Matrix Matrix;

				static const DependencyPropertyPtr MatrixProperty;
				static const DependencyPropertyPtr CenterXProperty;
				static const DependencyPropertyPtr CenterYProperty;
				static const DependencyPropertyPtr ScaleXProperty;
				static const DependencyPropertyPtr ScaleYProperty;
				static const DependencyPropertyPtr SkewXProperty;
				static const DependencyPropertyPtr SkewYProperty;
				static const DependencyPropertyPtr TranslateXProperty;
				static const DependencyPropertyPtr TranslateYProperty;
				static const DependencyPropertyPtr RotationProperty;
			private:
				static bool CenterXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool CenterYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool ScaleXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool ScaleYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool SkewXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool SkewYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool TranslateXChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool TranslateYChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
				static bool RotationChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);

				__usctordef(CompositeTransform)
				REFLECT_CLASS(CompositeTransform)
			};
		}
	}
}
