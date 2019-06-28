#pragma once
#include "..\Resource.h"
#include "..\Geometry\Geometry.h"
using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class Brush;
				typedef std::shared_ptr<Brush> BrushPtr;

				class Geometry;
				class Pen;

				class Brush : public DeviceDependantResource
				{
				public:
					Brush();
					virtual ~Brush() {}
				public:
					void Opacity(const float o);
					auto Opacity() const -> float;
				public:
					void Transform(const Matrix& transform);
					auto Transform() const -> const Matrix&;
				public:
					virtual auto GetTransformToGeometryBounds(const std::shared_ptr<Geometry>& geometry,
						const std::shared_ptr<Pen>& pen)->Matrix;
					virtual auto GetTransformToGeometryBounds(const std::shared_ptr<Geometry>& geometry,
						const std::shared_ptr<StrokeStyle>& pen, float thickness)->Matrix;

					virtual auto GetTransformToBounds(const Rect& rect)->Matrix;
				protected:
					float            m_Opacity;
					Matrix m_transform;
				};
			}
		}
	}
}