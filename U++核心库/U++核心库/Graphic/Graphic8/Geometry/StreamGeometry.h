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
			namespace d2d1
			{
				class StreamGeometry;
				typedef std::shared_ptr<StreamGeometry> StreamGeometryPtr;

				class StreamGeometry : public Geometry
				{
				public:
					StreamGeometry(void);
					virtual ~StreamGeometry(void);

					Result<bool> BeginFigure(const Point& start, bool filled, bool closed);
					Result<bool> EndFigure();

					Result<bool> LineTo(const Point& point, bool isStroked = true, bool isSmoothJoin = true);
					Result<bool> ArcTo(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked = true, bool isSmoothJoin = true);
					Result<bool> BezierTo(const Point& p1, const Point& p2, const Point& p3, bool isStroked = true, bool isSmoothJoin = true);
					Result<bool> QuadraticBezierTo(const Point& point1, const Point& point2, bool isStroked = true, bool isSmoothJoin = true);

					void FillMode(Graphic::FillMode mode);
					Graphic::FillMode FillMode();

					void Clear();

					Result<bool, wstring> Decompose();

					Result<bool> Open(bool clear = true);

					bool GetIsOpened() const;

					Result<bool> Close(bool clear = true);

				protected:
					virtual Microsoft::WRL::ComPtr<ID2D1Geometry> OnCreateGeometry() override;
				private:
					bool m_isOpened;
					std::vector<FigurePtr> m_figures;
					std::unique_ptr<Figure> m_currentFigure;
					Graphic::FillMode m_fillMode;
				};
			}
		}
	}
}

