#pragma once
#include "Geometry.h"
#include <vector>
#include "Figure.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class PathFigure;
			typedef std::shared_ptr<PathFigure> PathFigurePtr;
		}
		namespace Graphic
		{
			namespace d2d1
			{
				class PathGeometry;
				typedef std::shared_ptr<PathGeometry> PathGeometryPtr;

				class PathGeometry : public Geometry
				{
				public:
					PathGeometry(void);
					virtual ~PathGeometry(void);

					void SetFigures(const Graphic::PathFigurePtr* figures, unsigned int count);

					void FillMode(Graphic::FillMode mode);
					Graphic::FillMode FillMode();

					Result<bool, wstring> Decompose();

				protected:
					virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
				private:
					const Graphic::PathFigurePtr* m_figures;
					unsigned int m_count;
					Graphic::FillMode m_fillMode;
				};
			}
		}
	}
}

