#pragma once
#include "Geometry.h"
#include "PathFigure.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class PathGeometry;
			typedef std::shared_ptr<PathGeometry> PathGeometryPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API PathGeometry : public Geometry
			{
			public:
				__uref static PathGeometryPtr Parse(const wstring& data);

				__uref Result<bool, wstring> Save();

				__uctor PathGeometry();
				~PathGeometry();

				__uref Result<bool> BeginFigure(const Point& start, bool filled, bool closed);
				__uref Result<bool> EndFigure();

				__uref Result<bool> LineTo(const Point& point, bool isStroked = true, bool isSmoothJoin = true);
				__uref Result<bool> ArcTo(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked = true, bool isSmoothJoin = true);
				__uref Result<bool> BezierTo(const Point& p1, const Point& p2, const Point& p3, bool isStroked = true, bool isSmoothJoin = true);
				__uref Result<bool> QuadraticBezierTo(const Point& point1, const Point& point2, bool isStroked = true, bool isSmoothJoin = true);


				void SetFillMode(Graphic::FillMode mode);
				Graphic::FillMode GetFillMode() const;

				__uref void Clear();
			protected:
				FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) override;
				EventToken FiguresChangedToken;
				static void OnFiguresChanged(PathGeometry* sender, std::shared_ptr<ObservableCollection<PathFigurePtr>> PathFigures, NotifyCollectionChangedEventArgs<PathFigurePtr> args);
				ObservableCollection<PathFigurePtr> Figures;
				std::unique_ptr<PathFigure> CurrentFigure;

			private:
				static bool FillModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs);
			public:
				static const DependencyPropertyPtr FillModeProperty;

				__uproperty(get = GetFillMode, put = SetFillMode) Graphic::FillMode FillMode;

				__usctordef(PathGeometry)
				REFLECT_CLASS(PathGeometry)
			};
		}
	}
}

