#pragma once
#include "..\Graphic.h"
#include "..\..\GradientStop.h"
#include "..\..\Transform.h"
using namespace System;
using namespace System::UI::Media;
using namespace System::UI::Graphic;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class PathFigure;
			typedef std::shared_ptr<PathFigure> PathFigurePtr;
		}
	}
}

struct RenderContext8
{
	GraphicError(*PushSurfaceTarget)(GraphicObject renderContext, GraphicObject surface);
	Result<GraphicObject, GraphicError>(*GetCurrentTarget)(GraphicObject renderContext);
	Result<Size, GraphicError>(*GetCurrentTargetSize)(GraphicObject renderContext);
	GraphicError(*PushTarget)(GraphicObject renderContext, GraphicObject bitmap);
	GraphicError(*PopTarget)(GraphicObject renderContext);
	GraphicError(*Clear)(GraphicObject renderContext, const Color& color);
	GraphicError(*PushAxisAlignedClip)(GraphicObject renderContext, const Rect& rect, AntialiasMode mode);
	GraphicError(*PopAxisAlignedClip)(GraphicObject renderContext);
	GraphicError(*PushGeometryClip)(GraphicObject renderContext, GraphicObject geometry);
	GraphicError(*PopGeometryClip)(GraphicObject renderContext);
	GraphicError(*PushOpacity)(GraphicObject renderContext, float opacity);
	GraphicError(*PopOpacity)(GraphicObject renderContext);
	GraphicError(*PushOpacityMask)(GraphicObject renderContext, const Rect& rect, GraphicObject brush);
	GraphicError(*PopOpacityMask)(GraphicObject renderContext);
	Result<int, GraphicError>(*GetMaximumImageSize)(GraphicObject renderContext);
	GraphicError(*DrawGeometry)(GraphicObject renderContext, GraphicObject geometry, GraphicObject brush, GraphicObject strokeBrush, GraphicObject strokeStyle, float thickness);
	GraphicError(*SetTransform)(GraphicObject renderContext, const Matrix& matrix);
	GraphicError(*DrawTextLayout)(GraphicObject renderContext, GraphicObject textLayout, GraphicObject brush, const Point& origin);
	GraphicError(*DrawBitmap)(GraphicObject renderContext, GraphicObject bitmap, const EffectCompositeMode compositeMode, const Size& targetSize);
	GraphicError(*DrawEffect)(GraphicObject renderContext, GraphicObject bitmap, GraphicObject elementEffect, const EffectCompositeMode compositeMode, const Size& size);
};

struct BitmapData8
{
	UInt8 BitsPerSize;
	unsigned int RowPitch;
	unsigned int DepthPitch;
	unsigned int Width;
	unsigned int Height;
	void *Bits;
};

struct Bitmap8
{
	Result<Size, GraphicError>(*GetSize)(GraphicObject bitmap);
	Result<GraphicError>(*Map)(GraphicObject bitmap, GraphicObject device, BitmapData8* data);
	Result<GraphicError>(*Unmap)(GraphicObject bitmap, GraphicObject device);
};

struct Surface8
{
	Result<Size, GraphicError>(*GetSize)(GraphicObject surface);
	Result<GraphicError>(*CopyTo)(GraphicObject surface, GraphicObject surfaceCPU, GraphicObject device);
};

struct Texture2D8
{
	Result<Size, GraphicError>(*GetSize)(GraphicObject texture);
	Result<GraphicError>(*Map)(GraphicObject texture, GraphicObject device, BitmapData8* data);
	Result<GraphicError>(*Unmap)(GraphicObject texture, GraphicObject device);
	Result<GraphicObject, GraphicError>(*GetSurface)(GraphicObject texture);
};


struct BitmapBrush8
{
	GraphicError(*SetExtendModeX)(GraphicObject bitmapBrush, ExtendMode modeX);
	Result<ExtendMode, GraphicError>(*GetExtendModeX)(GraphicObject bitmapBrush);
	GraphicError(*SetExtendModeY)(GraphicObject bitmapBrush, ExtendMode modeY);
	Result<ExtendMode, GraphicError>(*GetExtendModeY)(GraphicObject bitmapBrush);
	GraphicError(*SetInterpolationMode)(GraphicObject bitmapBrush, InterpolationMode mode);
	Result<InterpolationMode, GraphicError>(*GetInterpolationMode)(GraphicObject bitmapBrush);
	GraphicError(*SetSourceRect)(GraphicObject bitmapBrush, const Rect& sourceRect);
	Result<Rect, GraphicError>(*GetSourceRect)(GraphicObject bitmapBrush);
	GraphicError(*SetBitmap)(GraphicObject bitmapBrush, GraphicObject bitmap);
	Result<GraphicObject, GraphicError>(*GetBitmap)(GraphicObject bitmapBrush);
};

struct GradientStopCollection8
{
	GraphicError(*SetGradientStops)(GraphicObject gradientStopCollection, const ObservableCollection<GradientStop>& gradientStops);
	GraphicError(*SetExtendMode)(GraphicObject gradientStopCollection, ExtendMode modeX);
	Result<ExtendMode, GraphicError>(*GetExtendMode)(GraphicObject gradientStopCollection);
};

struct LinearGradientBrush8
{
	GraphicError(*SetGradientStopCollection)(GraphicObject linearGradientBrush, GraphicObject gradientStopCollection);
	Result<GraphicObject, GraphicError>(*GetGradientStopCollection)(GraphicObject linearGradientBrush, GraphicObject gradientStopCollection);
	GraphicError(*SetStartPoint)(GraphicObject linearGradientBrush, Point point);
	Result<Point, GraphicError>(*GetStartPoint)(GraphicObject linearGradientBrush);
	GraphicError(*SetEndPoint)(GraphicObject linearGradientBrush, Point point);
	Result<Point, GraphicError>(*GetEndPoint)(GraphicObject linearGradientBrush);
};

struct RadialGradientBrush8
{
	GraphicError(*SetGradientStopCollection)(GraphicObject radialGradientBrush, GraphicObject gradientStopCollection);
	Result<GraphicObject, GraphicError>(*GetGradientStopCollection)(GraphicObject radialGradientBrush, GraphicObject gradientStopCollection);
	GraphicError(*SetCenterPoint)(GraphicObject radialGradientBrush, Point point);
	Result<Point, GraphicError>(*GetCenterPoint)(GraphicObject radialGradientBrush);
	GraphicError(*SetGradientOriginOffset)(GraphicObject radialGradientBrush, Point gradientOriginOffset);
	Result<Point, GraphicError>(*GetGradientOriginOffset)(GraphicObject radialGradientBrush);
	GraphicError(*SetRadiusX)(GraphicObject radialGradientBrush, float radiusX);
	Result<float, GraphicError>(*GetRadiusX)(GraphicObject radialGradientBrush);
	GraphicError(*SetRadiusY)(GraphicObject radialGradientBrush, float radiusY);
	Result<float, GraphicError>(*GetRadiusY)(GraphicObject radialGradientBrush);
};

struct SolidColorBrush8
{
	GraphicError(*SetColor)(GraphicObject solidColorBrush, const Color& color);
	Result<Color, GraphicError>(*GetColor)(GraphicObject solidColorBrush);
};

struct Brush8
{
	GraphicError(*SetOpacity)(GraphicObject brush, float opacity);
	Result<float, GraphicError>(*GetOpacity)(GraphicObject brush);
	GraphicError(*SetTransform)(GraphicObject brush, const Matrix& matrix);
	Result<Matrix, GraphicError>(*GetTransform)(GraphicObject brush);
	Result<Matrix, GraphicError>(*GetTransformToGeometryBounds)(GraphicObject brush, GraphicObject geometry, GraphicObject strokeStyle, double thickness);
	Result<Matrix, GraphicError>(*GetTransformToBounds)(GraphicObject brush, const Rect& rect);
};

struct StrokeStyle8
{
	GraphicError(*SetStartCap)(GraphicObject strokeStyle, CapStyle startCap);
	Result<CapStyle, GraphicError>(*GetStartCap)(GraphicObject strokeStyle);
	GraphicError(*SetEndCap)(GraphicObject strokeStyle, CapStyle endCap);
	Result<CapStyle, GraphicError>(*GetEndCap)(GraphicObject strokeStyle);
	GraphicError(*SetDashCap)(GraphicObject strokeStyle, CapStyle dashCap);
	Result<CapStyle, GraphicError>(*GetDashCap)(GraphicObject strokeStyle);
	GraphicError(*SetLineJoinStyle)(GraphicObject strokeStyle, LineJoin lineJoinStyle);
	Result<LineJoin, GraphicError>(*GetLineJoinStyle)(GraphicObject strokeStyle);
	GraphicError(*SetMiterLimit)(GraphicObject strokeStyle, float miterLimit);
	Result<float, GraphicError>(*GetMiterLimit)(GraphicObject strokeStyle);
	GraphicError(*SetLineDashStyle)(GraphicObject strokeStyle, DashStyle lineDashStyle);
	Result<DashStyle, GraphicError>(*GetLineDashStyle)(GraphicObject strokeStyle);
	GraphicError(*SetDashOffset)(GraphicObject strokeStyle, float dashOffset);
	Result<float, GraphicError>(*GetDashOffset)(GraphicObject strokeStyle);
	GraphicError(*SetDashes)(GraphicObject strokeStyle, const float* dashes, unsigned int dashCount);
	Result<const float*, unsigned int, GraphicError>(*GetDashes)(GraphicObject strokeStyle);
};

struct TextFormat8
{
	GraphicError(*SetFontFamily)(GraphicObject textFormat, const wchar_t* fontFamily);
	Result<const wchar_t*, GraphicError>(*GetFontFamily)(GraphicObject textFormat);
	GraphicError(*SetFontSize)(GraphicObject textFormat, float fontSize);
	Result<float, GraphicError>(*GetFontSize)(GraphicObject textFormat);
	GraphicError(*SetFontWeight)(GraphicObject textFormat, FontWeight fontWeight);
	Result<FontWeight, GraphicError>(*GetFontWeight)(GraphicObject textFormat);
	GraphicError(*SetFontStyle)(GraphicObject textFormat, FontStyle fontStyle);
	Result<FontStyle, GraphicError>(*GetFontStyle)(GraphicObject textFormat);
	GraphicError(*SetFontStretch)(GraphicObject textFormat, FontStretch fontStretch);
	Result<FontStretch, GraphicError>(*GetFontStretch)(GraphicObject textFormat);
};

struct TextLayout8
{
	GraphicError(*SetTextFormat)(GraphicObject textLayout, GraphicObject textFormat);
	Result<GraphicObject, GraphicError>(*GetTextFormat)(GraphicObject textLayout);
	GraphicError(*SetText)(GraphicObject textLayout, const wchar_t* text);
	Result<const wchar_t*, GraphicError>(*GetText)(GraphicObject textLayout);
	GraphicError(*SetMaxSize)(GraphicObject textLayout, Size maxSize);
	Result<Size, GraphicError>(*GetMaxSize)(GraphicObject textLayout);
	Result<Rect, GraphicError>(*GetLayoutBounds)(GraphicObject textLayout);
	Result<bool, GraphicError>(*HitTest)(GraphicObject textLayout, const Point& point);
};

struct EllipseGeometry8
{
	GraphicError(*SetCenterPoint)(GraphicObject ellipseGeometry, const Point& point);
	Result<Point, GraphicError>(*GetCenterPoint)(GraphicObject ellipseGeometry);
	GraphicError(*SetRadiusX)(GraphicObject ellipseGeometry, float radiusX);
	Result<float, GraphicError>(*GetRadiusX)(GraphicObject ellipseGeometry);
	GraphicError(*SetRadiusY)(GraphicObject ellipseGeometry, float radiusY);
	Result<float, GraphicError>(*GetRadiusY)(GraphicObject ellipseGeometry);
};

struct RectangleGeometry8
{
	GraphicError(*SetRect)(GraphicObject rectangleGeometry, const Rect& rect);
	Result<Rect, GraphicError>(*GetRect)(GraphicObject rectangleGeometry);
};

struct RoundedRectangleGeometry8
{
	GraphicError(*SetRect)(GraphicObject roundedRectangleGeometry, const Rect& rect);
	Result<Rect, GraphicError>(*GetRect)(GraphicObject roundedRectangleGeometry);
	GraphicError(*SetRadiusX)(GraphicObject roundedRectangleGeometry, float radiusX);
	Result<float, GraphicError>(*GetRadiusX)(GraphicObject roundedRectangleGeometry);
	GraphicError(*SetRadiusY)(GraphicObject roundedRectangleGeometry, float radiusY);
	Result<float, GraphicError>(*GetRadiusY)(GraphicObject roundedRectangleGeometry);
};

struct PathGeometry8
{
	GraphicError(*SetFigures)(GraphicObject pathGeometry, const UI::Graphic::PathFigurePtr* figures, unsigned int count);
	GraphicError(*SetFillMode)(GraphicObject pathGeometry, FillMode mode);
	Result<FillMode, GraphicError>(*GetFillMode)(GraphicObject pathGeometry);
};

struct StreamGeometry8
{
	Result<bool, GraphicError>(*Open)(GraphicObject streamGeometry);
	Result<bool, GraphicError>(*GetIsOpened)(GraphicObject streamGeometry);
	GraphicError(*SetFillMode)(GraphicObject streamGeometry, FillMode mode);
	Result<FillMode, GraphicError>(*GetFillMode)(GraphicObject streamGeometry);
	Result<GraphicError>(*BeginFigure)(GraphicObject streamGeometry, const Point& start, bool filled, bool closed);
	Result<GraphicError>(*EndFigure)(GraphicObject streamGeometry);
	Result<GraphicError>(*LineTo)(GraphicObject streamGeometry, const Point& point, bool isStroked, bool isSmoothJoin);
	Result<GraphicError>(*ArcTo)(GraphicObject streamGeometry, const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked, bool isSmoothJoin);
	Result<GraphicError>(*BezierTo)(GraphicObject streamGeometry, const Point& p1, const Point& p2, const Point& p3, bool isStroked, bool isSmoothJoin);
	Result<GraphicError>(*QuadraticBezierTo)(GraphicObject streamGeometry, const Point& point1, const Point& point2, bool isStroked, bool isSmoothJoin);
	GraphicError(*Clear)(GraphicObject streamGeometry);
	Result<wstring, GraphicError>(*Decompose)(GraphicObject streamGeometry);
};

struct CombinedGeometry8
{
	GraphicError(*SetCombineMode)(GraphicObject combinedGeometry, CombineMode mode);
	Result<CombineMode, GraphicError>(*GetCombineMode)(GraphicObject combinedGeometry);
};

struct GeometryGroup8
{
	GraphicError(*SetFillMode)(GraphicObject geometryGroup, FillMode mode);
	Result<FillMode, GraphicError>(*GetFillMode)(GraphicObject geometryGroup);
	GraphicError(*SetGeometries)(GraphicObject geometryGroup, GraphicObject* geometries, unsigned int count);
};

struct Geometry8
{
	Result<bool, GraphicError>(*HitTest)(GraphicObject geometry, const Point& point, const Matrix& matrix);
	Result<bool, GraphicError>(*HitTestStroke)(GraphicObject geometry, const Point& point, GraphicObject strokeStyle, float thickness, const Matrix& matrix);
	Result<Rect, GraphicError>(*GetBounds)(GraphicObject geometry, GraphicObject strokeStyle, float thickness, const Matrix& worldTransform);
	GraphicError(*PushTransform)(GraphicObject geometry, const TransformPtr& transform);
};

struct BlurEffect8
{
	GraphicError(*SetBlurSize)(GraphicObject textLayout, float blurSize);
	Result<float, GraphicError>(*GetBlurSize)(GraphicObject textLayout);
	GraphicError(*SetOptimization)(GraphicObject textLayout, BlurOptimization optimization);
	Result<BlurOptimization, GraphicError>(*GetText)(GraphicObject textLayout);
	GraphicError(*SetBorderMode)(GraphicObject textLayout, BlurBorderMode borderMode);
	Result<BlurBorderMode, GraphicError>(*GetBorderMode)(GraphicObject textLayout);
};

struct DropShadowEffect8
{
	GraphicError(*SetBlurSize)(GraphicObject dropShadowEffect, float blurSize);
	Result<float, GraphicError>(*GetBlurSize)(GraphicObject dropShadowEffect);
	GraphicError(*SetOptimization)(GraphicObject dropShadowEffect, BlurOptimization optimization);
	Result<BlurOptimization, GraphicError>(*GetText)(GraphicObject dropShadowEffect);
	GraphicError(*SetColor)(GraphicObject dropShadowEffect, const Color& color);
	Result<Color, GraphicError>(*GetColor)(GraphicObject dropShadowEffect);
	GraphicError(*SetEnableTransform)(GraphicObject dropShadowEffect, bool enable);
	Result<bool, GraphicError>(*GetEnableTransform)(GraphicObject dropShadowEffect);
	GraphicError(*SetShadowTransform)(GraphicObject dropShadowEffect, const TransformPtr& transform);
	Result<TransformPtr, GraphicError>(*GetShadowTransform)(GraphicObject dropShadowEffect);
	GraphicError(*SetDirection)(GraphicObject dropShadowEffect, float direction);
	Result<float, GraphicError>(*GetDirection)(GraphicObject dropShadowEffect);
	GraphicError(*SetShadowDepth)(GraphicObject dropShadowEffect, float shadowDepth);
	Result<float, GraphicError>(*GetShadowDepth)(GraphicObject dropShadowEffect);
};
struct SaturationEffect8
{
	GraphicError(*SetAmount)(GraphicObject saturationEffect, float amount);
	Result<float, GraphicError>(*GetAmount)(GraphicObject saturationEffect);
};

struct Feature8
{

#pragma region CreateObjects	
	Result<GraphicObject, GraphicError>(*CreateCoreObject)(GraphicObjectTypes type, GraphicObject inObj0, float width, float height, Texture2DMode mode, const wchar_t* path);
	Result<GraphicObject, GraphicError>(*CreateGradientStopCollection)(GraphicObjectTypes type, const ObservableCollection<GradientStop>& gradientstops, ExtendMode extend);
	Result<GraphicObject, GraphicError>(*CreateStrokeStyle)(GraphicObjectTypes type, CapStyle StartCap, CapStyle EndCap, CapStyle DashCap, LineJoin LineJoinStyle, float MiterLimit, DashStyle LineDashStyle, float DashOffset, const float* dashes, unsigned int dashCount);
	Result<GraphicObject, GraphicError>(*CreateBrushObject)(GraphicObjectTypes type, const Color& color0, GraphicObject inObj0, Point start, Point end, Point center, Point gradientOriginOffset, float radiusX, float radiusY, ExtendMode modeX, ExtendMode modeY, Rect sourceRect, InterpolationMode interpolation);
	Result<GraphicObject, GraphicError>(*CreateEffectObject)(GraphicObjectTypes type, GraphicEffectTypes effectType, const Color& color0, float value0, BlurOptimization optimization, BlurBorderMode borderMode, bool value1, const TransformPtr& transform, float value2, float value3);
	Result<GraphicObject, GraphicError>(*CreateTextObject)(GraphicObjectTypes type, const wchar_t* value0, float value1, const Size& maxSize, FontWeight fontWeight, FontStyle fontStyle, FontStretch fontStretch, GraphicObject inObj0);
	Result<GraphicObject, GraphicError>(*CreateGeometryObject)(GraphicObjectTypes type, const Point& point0, const Rect& rect, float radiusX, float radiusY, GraphicObject inObj0, GraphicObject inObj1, GraphicObject* inObjs, unsigned int inObjsCount, const TransformPtr& transform, CombineMode combineMode, FillMode fillMode);
#pragma endregion

	GraphicError(*DeleteObject)(GraphicObjectTypes type, GraphicObject obj);


	RenderContext8  RenderContext;
	Bitmap8 Bitmap;
	Texture2D8 Texture2D;
	Surface8 Surface;
	BitmapBrush8 BitmapBrush;
	GradientStopCollection8 GradientStopCollection;
	LinearGradientBrush8 LinearGradientBrush;
	RadialGradientBrush8 RadialGradientBrush;
	SolidColorBrush8 SolidColorBrush;
	Brush8 Brush;
	StrokeStyle8 StrokeStyle;
	TextFormat8 TextFormat;
	TextLayout8 TextLayout;
	EllipseGeometry8 EllipseGeometry;
	RectangleGeometry8 RectangleGeometry;
	RoundedRectangleGeometry8  RoundedRectangleGeometry;
	PathGeometry8 PathGeometry;
	StreamGeometry8 StreamGeometry;
	CombinedGeometry8 CombinedGeometry;
	GeometryGroup8 GeometryGroup;
	Geometry8 Geometry;
	BlurEffect8 BlurEffect;
	DropShadowEffect8 DropShadowEffect;
	SaturationEffect8 SaturationEffect;
};

typedef Feature8* (*GetFeature8)();       ///声明通用特性描述接口函数地址