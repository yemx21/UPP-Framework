//#include "StreamGeometry.h"
//
//using namespace System::UI::Graphic;
//
//StreamGeometryContextPtr StreamGeometryContext::Parse(const wstring& data);
//
//Result<bool, wstring> StreamGeometryContext::Save();
//
//StreamGeometryContext::StreamGeometryContext(void)
//{
//
//
//}
//
//StreamGeometryContext::~StreamGeometryContext(void)
//{
//
//}
//
//Result<bool> StreamGeometryContext::BeginFigure(const Point& start, bool filled, bool closed);
//Result<bool> StreamGeometryContext::EndFigure();
//
//Result<bool> StreamGeometryContext::LineTo(const Point& point, bool isStroked = true, bool isSmoothJoin = true);
//Result<bool> StreamGeometryContext::ArcTo(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked = true, bool isSmoothJoin = true);
//Result<bool> StreamGeometryContext::BezierTo(const Point& p1, const Point& p2, const Point& p3, bool isStroked = true, bool isSmoothJoin = true);
//Result<bool> StreamGeometryContext::QuadraticBezierTo(const Point& point1, const Point& point2, bool isStroked = true, bool isSmoothJoin = true);
//
//
//void StreamGeometryContext::SetFillMode(Graphic::FillMode mode);
//Graphic::FillMode StreamGeometryContext::GetFillMode() const;
//
//void StreamGeometryContext::Clear();
//
//Result<bool> StreamGeometryContext::Open(bool clear = true);
//Result<bool> StreamGeometryContext::IsOpened() const;
//Result<bool> StreamGeometryContext::Close(bool clear = true);
//
//bool StreamGeometryContext::HitTest(const Point& point, const Matrix& matrix);
//bool StreamGeometryContext::HitTestStroke(const Point& point, const StrokeStylePtr& strokeStyle, float thickness, const Matrix& matrix);
//Rect StreamGeometryContext::GetBounds(const StrokeStylePtr& strokeStyle, double thickness, const Matrix& worldTransform);
//bool StreamGeometryContext::PushTransform(const TransformPtr& transform);