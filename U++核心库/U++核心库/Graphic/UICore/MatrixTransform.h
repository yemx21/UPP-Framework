#pragma once
#include "Transform.h"

namespace System
{
	namespace UI
	{
		namespace Media
		{
			class MatrixTransform;
			typedef std::shared_ptr<MatrixTransform> MatrixTransformPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Media, ver = 1.0) UICORE_API MatrixTransform : public Transform
			{
			protected:
				wstring CustomToString(const wchar_t* fmt, const CultureInfo& loc) const override;
			public:
				__udefctor MatrixTransform();
				__uctor MatrixTransform(const System::Matrix& matrix);
				__uctor MatrixTransform(const MatrixTransform& transform);

				__uref bool operator == (const MatrixTransform& transform) const;
				__uref bool operator != (const MatrixTransform&  transform) const;

				virtual ~MatrixTransform();
			public:
				virtual operator System::Matrix() override;
				__uref virtual System::Matrix GetTransformMatrix();
			public:
				double GetM11() const;
				void SetM11(double val);

				double GetM12() const;
				void SetM12(double val);

				double GetM21() const;
				void SetM21(double val);

				double GetM22() const;
				void SetM22(double val);

				double GetOffsetX() const;
				void SetOffsetX(double val);

				double GetOffsetY() const;
				void SetOffsetY(double val);

				System::Matrix GetMatrix() const;
				void SetMatrix(const System::Matrix& matrix);

				__uproperty(get = GetM11, put = SetM11) double M11;
				__uproperty(get = GetM12, put = SetM12) double M12;
				__uproperty(get = GetM21, put = SetM21) double M21;
				__uproperty(get = GetM22, put = SetM22) double M22;
				__uproperty(get = GetOffsetX, put = SetOffsetX) double OffsetX;
				__uproperty(get = GetOffsetY, put = SetOffsetY) double OffsetY;
				__uproperty(get = GetMatrix, put = SetMatrix) System::Matrix Matrix;

				static const DependencyPropertyPtr MatrixProperty;

				

				__usctordef(MatrixTransform)
				REFLECT_CLASS(MatrixTransform)
			};
		}
	}
}
