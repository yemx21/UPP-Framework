#pragma once
#include "Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class TransformedGeometry;

				typedef std::shared_ptr<TransformedGeometry> TransformedGeometryPtr;


				class TransformedGeometry : public Geometry
				{
				public:
					TransformedGeometry(const GeometryPtr& geometry, const TransformPtr& transform);
					virtual ~TransformedGeometry(void);
				protected:
					virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
				private:
					GeometryPtr m_originalGeometry;
					TransformPtr m_transform;
				};
			}
		}
	}
}
