//#pragma once
//#include "..\Resource.h"
//#include "..\Brush\StrokeStyle.h"
//
//namespace System
//{
//	namespace Graphic
//	{
//		class StreamGeometryContext;
//
//		typedef std::shared_ptr<StreamGeometryContext> StreamGeometryContextPtr;
//
//		__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API StreamGeometryContext
//		{
//		private:
//			GraphicObject streamGeometry;
//		public:
//			static StreamGeometryContextPtr Parse(const wstring& data);
//
//			Result<bool, wstring> Save();
//
//			StreamGeometryContext(void);
//			virtual ~StreamGeometryContext(void);
//
//			StreamGeometryContext()
//
//			Result<bool> BeginFigure(const Point& start, bool filled, bool closed);
//			Result<bool> EndFigure();
//
//			Result<bool> LineTo(const Point& point, bool isStroked = true, bool isSmoothJoin = true);
//			Result<bool> ArcTo(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked = true, bool isSmoothJoin = true);
//			Result<bool> BezierTo(const Point& p1, const Point& p2, const Point& p3, bool isStroked = true, bool isSmoothJoin = true);
//			Result<bool> QuadraticBezierTo(const Point& point1, const Point& point2, bool isStroked = true, bool isSmoothJoin = true);
//
//
//			void SetFillMode(Graphic::FillMode mode);
//			Graphic::FillMode GetFillMode() const;
//
//			_declspec(property(get = GetFillMode, put = SetFillMode)) Graphic::FillMode FillMode;
//
//			void Clear();
//
//			Result<bool> Open(bool clear = true);
//			Result<bool> IsOpened() const;
//			Result<bool> Close(bool clear = true);
//
//		public:
//			bool HitTest(const Point& point, const Matrix& matrix);
//			bool HitTestStroke(const Point& point, const StrokeStylePtr& strokeStyle, float thickness, const Matrix& matrix);
//			Rect GetBounds(const StrokeStylePtr& strokeStyle, double thickness, const Matrix& worldTransform);
//			bool PushTransform(const TransformPtr& transform);
//		};
//	}
//}