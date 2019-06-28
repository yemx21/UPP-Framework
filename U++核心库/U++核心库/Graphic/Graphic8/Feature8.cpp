#include "Header.h"
#include "Rendering\RenderContext.h"
#include "Brush\Brushes.h"
#include "Geometry\Geometries.h"
#include "Effects\Effects.h"

using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;
using namespace System::UI::Graphic::d2d1;
using namespace std;
FeatureInfo Info =
{
	8,
	L"1.0",
	L"Direct2D1.1 with Direct3D11",
	0,
};

extern "C" __declspec(dllexport) FeatureInfo *GetFeatureInfoInterface()
{
	return &Info;
}


GraphicError RenderContext_PushSurfaceTarget(GraphicObject renderContext, GraphicObject surface);
Result<GraphicObject, GraphicError> RenderContext_GetCurrentTarget(GraphicObject renderContext);
Result<Size, GraphicError> RenderContext_GetCurrentTargetSize(GraphicObject renderContext);
GraphicError RenderContext_PushTarget(GraphicObject renderContext, GraphicObject bitmap);
GraphicError RenderContext_PopTarget(GraphicObject renderContext);
GraphicError RenderContext_Clear(GraphicObject renderContext, const Color& color);
GraphicError RenderContext_PushAxisAlignedClip(GraphicObject renderContext, const Rect& clipRect, AntialiasMode mode);
GraphicError RenderContext_PopAxisAlignedClip(GraphicObject renderContext);
GraphicError RenderContext_PushGeometryClip(GraphicObject renderContext, GraphicObject geometry);
GraphicError RenderContext_PopGeometryClip(GraphicObject renderContext);
GraphicError RenderContext_PushOpacity(GraphicObject renderContext, float opacity);
GraphicError RenderContext_PopOpacity(GraphicObject renderContext);
GraphicError RenderContext_PushOpacityMask(GraphicObject renderContext, const Rect& rect, GraphicObject brush);
GraphicError RenderContext_PopOpacityMask(GraphicObject renderContext);

Result<int, GraphicError> RenderContext_GetMaximumImageSize(GraphicObject renderContext);
GraphicError RenderContext_DrawGeometry(GraphicObject renderContext, GraphicObject geometry, GraphicObject brush, GraphicObject strokeBrush, GraphicObject strokeStyle, float thickness);
GraphicError RenderContext_SetTransform(GraphicObject renderContext, const Matrix& matrix);
GraphicError RenderContext_DrawText(GraphicObject renderContext, GraphicObject textLayout, GraphicObject brush, const Point& origin);
GraphicError RenderContext_DrawBitmap(GraphicObject renderContext, GraphicObject bitmap, const EffectCompositeMode compositeMode, const Size& targetSize);
GraphicError RenderContext_DrawEffect(GraphicObject renderContext, GraphicObject bitmap, GraphicObject elementEffect, const EffectCompositeMode compositeMode, const Size& size);

Result<Size, GraphicError> Bitmap_GetSize(GraphicObject bitmap);
Result<GraphicError>Bitmap_Map(GraphicObject bitmap, GraphicObject device, BitmapData8* data);
Result<GraphicError>Bitmap_Unmap(GraphicObject bitmap, GraphicObject device);

Result<Size, GraphicError> Surface_GetSize(GraphicObject surface);
Result<GraphicError> Surface_CopyTo(GraphicObject surface, GraphicObject surfaceCPU, GraphicObject device);

Result<Size, GraphicError> Texture2D_GetSize(GraphicObject texture);
Result<GraphicError>Texture2D_Map(GraphicObject texture, GraphicObject device, BitmapData8* data);
Result<GraphicError>Texture2D_Unmap(GraphicObject texture, GraphicObject device);
Result<GraphicObject, GraphicError> Texture2D_GetSurface(GraphicObject texture);

Result<GraphicObject, GraphicError> CreateCoreObject(GraphicObjectTypes type, GraphicObject inObj0, float width, float height, Texture2DMode mode2D, const wchar_t* path);
Result<GraphicObject, GraphicError> CreateGradientStopCollection(GraphicObjectTypes type, const ObservableCollection<GradientStop>& gradientstops, ExtendMode extend);
Result<GraphicObject, GraphicError> CreateStrokeStyle(GraphicObjectTypes type, CapStyle StartCap, CapStyle EndCap, CapStyle DashCap, LineJoin LineJoinStyle, float MiterLimit, DashStyle LineDashStyle, float DashOffset, const float* dashes, unsigned int dashCount);
Result<GraphicObject, GraphicError> CreateBrushObject(GraphicObjectTypes type, const Color& color0, GraphicObject gradientstops, Point start, Point end, Point center, Point gradientOriginOffset, float radiusX, float radiusY, ExtendMode modeX, ExtendMode modeY, Rect sourceRect, InterpolationMode interpolation);
Result<GraphicObject, GraphicError> CreateEffectObject(GraphicObjectTypes type, GraphicEffectTypes effectType, const Color& color0, float value0,BlurOptimization optimization, BlurBorderMode borderMode, bool value1, const TransformPtr& transform, float value2, float value3);
Result<GraphicObject, GraphicError> CreateTextObject(GraphicObjectTypes type, const wchar_t* value0, float value1, const Size& maxSize, FontWeight fontWeight, FontStyle fontStyle, FontStretch fontStretch, GraphicObject inObj0);
Result<GraphicObject, GraphicError> CreateGeometryObject(GraphicObjectTypes type, const Point& point0, const Rect& rect, float radiusX, float radiusY, GraphicObject inObj0, GraphicObject inObj1, GraphicObject* inObjs, unsigned int inObjsCount, const TransformPtr& transform, CombineMode combineMode, FillMode fillMode);
GraphicError DeleteObject(GraphicObjectTypes type, GraphicObject obj);

GraphicError BitmapBrush_SetExtendModeX(GraphicObject bitmapBrush, ExtendMode modeX);
Result<ExtendMode, GraphicError> BitmapBrush_GetExtendModeX(GraphicObject bitmapBrush);
GraphicError BitmapBrush_SetExtendModeY(GraphicObject bitmapBrush, ExtendMode modeY);
Result<ExtendMode, GraphicError> BitmapBrush_GetExtendModeY(GraphicObject bitmapBrush);
GraphicError BitmapBrush_SetInterpolationMode(GraphicObject bitmapBrush, InterpolationMode mode);
Result<InterpolationMode, GraphicError> BitmapBrush_GetInterpolationMode(GraphicObject bitmapBrush);
GraphicError BitmapBrush_SetSourceRect(GraphicObject bitmapBrush, const Rect& sourceRect);
Result<Rect, GraphicError> BitmapBrush_GetSourceRect(GraphicObject bitmapBrush);
GraphicError BitmapBrush_SetBitmap(GraphicObject bitmapBrush, GraphicObject bitmap);
Result<GraphicObject, GraphicError> BitmapBrush_GetBitmap(GraphicObject bitmapBrush);

GraphicError GradientStopCollection_SetGradientStops(GraphicObject gradientStopCollection, const ObservableCollection<GradientStop>& gradientStops);
GraphicError GradientStopCollection_SetExtendMode(GraphicObject gradientStopCollection, ExtendMode modeX);
Result<ExtendMode, GraphicError> GradientStopCollection_GetExtendMode(GraphicObject gradientStopCollection);

GraphicError LinearGradientBrush_SetGradientStopCollection(GraphicObject linearGradientBrush, GraphicObject gradientStopCollection);
Result<GraphicObject, GraphicError> LinearGradientBrush_GetGradientStopCollection(GraphicObject linearGradientBrush, GraphicObject gradientStopCollection);
GraphicError LinearGradientBrush_SetStartPoint(GraphicObject linearGradientBrush, Point point);
Result<Point, GraphicError> LinearGradientBrush_GetStartPoint(GraphicObject linearGradientBrush);
GraphicError LinearGradientBrush_SetEndPoint(GraphicObject linearGradientBrush, Point point);
Result<Point, GraphicError> LinearGradientBrush_GetEndPoint(GraphicObject linearGradientBrush);

GraphicError RadialGradientBrush_SetGradientStopCollection(GraphicObject radialGradientBrush, GraphicObject gradientStopCollection);
Result<GraphicObject, GraphicError> RadialGradientBrush_GetGradientStopCollection(GraphicObject radialGradientBrush, GraphicObject gradientStopCollection);
GraphicError RadialGradientBrush_SetCenterPoint(GraphicObject radialGradientBrush, Point point);
Result<Point, GraphicError> RadialGradientBrush_GetCenterPoint(GraphicObject radialGradientBrush);
GraphicError RadialGradientBrush_SetGradientOriginOffset(GraphicObject radialGradientBrush, Point gradientOriginOffset);
Result<Point, GraphicError> RadialGradientBrush_GetGradientOriginOffset(GraphicObject radialGradientBrush);
GraphicError RadialGradientBrush_SetRadiusX(GraphicObject radialGradientBrush, float radiusX);
Result<float, GraphicError> RadialGradientBrush_GetRadiusX(GraphicObject radialGradientBrush);
GraphicError RadialGradientBrush_SetRadiusY(GraphicObject radialGradientBrush, float radiusY);
Result<float, GraphicError> RadialGradientBrush_GetRadiusY(GraphicObject radialGradientBrush);

GraphicError SolidColorBrush_SetColor(GraphicObject solidColorBrush, const Color& color);
Result<Color, GraphicError> SolidColorBrush_GetColor(GraphicObject solidColorBrush);

GraphicError Brush_SetOpacity(GraphicObject brush, float opacity);
Result<float, GraphicError> Brush_GetOpacity(GraphicObject brush);
GraphicError Brush_SetTransform(GraphicObject brush, const Matrix& matrix);
Result<Matrix, GraphicError> Brush_GetTransform(GraphicObject brush);


GraphicError StrokeStyle_SetStartCap(GraphicObject strokeStyle, CapStyle startCap);
Result<CapStyle, GraphicError> StrokeStyle_GetStartCap(GraphicObject strokeStyle);
GraphicError StrokeStyle_SetEndCap(GraphicObject strokeStyle, CapStyle endCap);
Result<CapStyle, GraphicError> StrokeStyle_GetEndCap(GraphicObject strokeStyle);
GraphicError StrokeStyle_SetDashCap(GraphicObject strokeStyle, CapStyle dashCap);
Result<CapStyle, GraphicError> StrokeStyle_GetDashCap(GraphicObject strokeStyle);
GraphicError StrokeStyle_SetLineJoinStyle(GraphicObject strokeStyle, LineJoin lineJoinStyle);
Result<LineJoin, GraphicError> StrokeStyle_GetLineJoinStyle(GraphicObject strokeStyle);
GraphicError StrokeStyle_SetMiterLimit(GraphicObject strokeStyle, float miterLimit);
Result<float, GraphicError> StrokeStyle_GetMiterLimit(GraphicObject strokeStyle);
GraphicError StrokeStyle_SetLineDashStyle(GraphicObject strokeStyle, DashStyle lineDashStyle);
Result<DashStyle, GraphicError> StrokeStyle_GetLineDashStyle(GraphicObject strokeStyle);
GraphicError StrokeStyle_SetDashOffset(GraphicObject strokeStyle, float dashOffset);
Result<float, GraphicError> StrokeStyle_GetDashOffset(GraphicObject strokeStyle);
GraphicError StrokeStyle_SetDashes(GraphicObject strokeStyle, const float* dashes, unsigned int dashCount);
Result<const float*, unsigned int,GraphicError> StrokeStyle_GetDashes(GraphicObject strokeStyle);

GraphicError TextFormat_SetFontFamily(GraphicObject textFormat, const wchar_t* fontFamily);
Result<const wchar_t*, GraphicError> TextFormat_GetFontFamily(GraphicObject textFormat);
GraphicError TextFormat_SetFontSize(GraphicObject textFormat, float fontSize);
Result<float, GraphicError> TextFormat_GetFontSize(GraphicObject textFormat);
GraphicError TextFormat_SetFontStyle(GraphicObject textFormat, FontStyle fontStyle);
Result<FontStyle, GraphicError> TextFormat_GetFontStyle(GraphicObject textFormat);
GraphicError TextFormat_SetFontStretch(GraphicObject textFormat, FontStretch fontStretch);
Result<FontStretch, GraphicError> TextFormat_GetFontStretch(GraphicObject textFormat);
GraphicError TextFormat_SetFontWeight(GraphicObject textFormat, FontWeight fontWeight);
Result<FontWeight, GraphicError> TextFormat_GetFontWeight(GraphicObject textFormat);

GraphicError TextLayout_SetTextFormat(GraphicObject textLayout, GraphicObject textFormat);
Result<GraphicObject, GraphicError> TextLayout_GetTextFormat(GraphicObject textLayout);
GraphicError TextLayout_SetText(GraphicObject textLayout, const wchar_t* text);
Result<const wchar_t*, GraphicError> TextLayout_GetText(GraphicObject textLayout);
GraphicError TextLayout_SetMaxSize(GraphicObject textLayout, Size maxSize);
Result<Size, GraphicError> TextLayout_GetMaxSize(GraphicObject textLayout);
Result<Rect, GraphicError> TextLayout_GetLayoutBounds(GraphicObject textLayout);
Result<bool, GraphicError> TextLayout_HitTest(GraphicObject textLayout, const Point& point);

GraphicError EllipseGeometry_SetCenterPoint(GraphicObject ellipseGeometry, const Point& point);
Result<Point, GraphicError> EllipseGeometry_GetCenterPoint(GraphicObject ellipseGeometry);
GraphicError EllipseGeometry_SetRadiusX(GraphicObject ellipseGeometry, float radiusX);
Result<float, GraphicError> EllipseGeometry_GetRadiusX(GraphicObject ellipseGeometry);
GraphicError EllipseGeometry_SetRadiusY(GraphicObject ellipseGeometry, float radiusY);
Result<float, GraphicError> EllipseGeometry_GetRadiusY(GraphicObject ellipseGeometry);

GraphicError RectangleGeometry_SetRect(GraphicObject rectangleGeometry, const Rect& rect);
Result<Rect, GraphicError> RectangleGeometry_GetRect(GraphicObject rectangleGeometry);

GraphicError RoundedRectangleGeometry_SetRect(GraphicObject roundedRectangleGeometry, const Rect& rect);
Result<Rect, GraphicError> RoundedRectangleGeometry_GetRect(GraphicObject roundedRectangleGeometry);
GraphicError RoundedRectangleGeometry_SetRadiusX(GraphicObject roundedRectangleGeometry, float radiusX);
Result<float, GraphicError> RoundedRectangleGeometry_GetRadiusX(GraphicObject roundedRectangleGeometry);
GraphicError RoundedRectangleGeometry_SetRadiusY(GraphicObject roundedRectangleGeometry, float radiusY);
Result<float, GraphicError> RoundedRectangleGeometry_GetRadiusY(GraphicObject roundedRectangleGeometry);

Result<bool, GraphicError>StreamGeometry_Open(GraphicObject streamGeometry);
Result<bool, GraphicError>StreamGeometry_GetIsOpened(GraphicObject streamGeometry);
GraphicError StreamGeometry_SetFillMode(GraphicObject streamGeometry, FillMode mode);
Result<FillMode, GraphicError> StreamGeometry_GetFillMode(GraphicObject streamGeometry);
Result<GraphicError> StreamGeometry_BeginFigure(GraphicObject streamGeometry, const Point& start, bool filled, bool closed);
Result<GraphicError> StreamGeometry_EndFigure(GraphicObject streamGeometry);
Result<GraphicError> StreamGeometry_LineTo(GraphicObject streamGeometry, const Point& point, bool isStroked, bool isSmoothJoin);
Result<GraphicError> StreamGeometry_ArcTo(GraphicObject streamGeometry, const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked, bool isSmoothJoin);
Result<GraphicError> StreamGeometry_BezierTo(GraphicObject streamGeometry, const Point& p1, const Point& p2, const Point& p3, bool isStroked, bool isSmoothJoin);
Result<GraphicError> StreamGeometry_QuadraticBezierTo(GraphicObject streamGeometry, const Point& point1, const Point& point2, bool isStroked, bool isSmoothJoin);
GraphicError StreamGeometry_Clear(GraphicObject streamGeometry);
Result<wstring, GraphicError> StreamGeometry_Decompose(GraphicObject streamGeometry);


GraphicError PathGeometry_SetFigures(GraphicObject pathGeometry, const System::UI::Graphic::PathFigurePtr* figures, unsigned int count);
GraphicError PathGeometry_SetFillMode(GraphicObject pathGeometry, FillMode mode);
Result<FillMode, GraphicError> PathGeometry_GetFillMode(GraphicObject pathGeometry);


GraphicError CombinedGeometry_SetCombineMode(GraphicObject combinedGeometry, CombineMode mode);
Result<CombineMode, GraphicError> CombinedGeometry_GetCombineMode(GraphicObject combinedGeometry);

GraphicError GeometryGroup_SetFillMode(GraphicObject geometryGroup, FillMode mode);
Result<FillMode, GraphicError> GeometryGroup_GetFillMode(GraphicObject geometryGroup);
GraphicError GeometryGroup_SetGeometries(GraphicObject geometryGroup, GraphicObject* geometries, unsigned int count);

Result<bool, GraphicError> Geometry_HitTest(GraphicObject geometry, const Point& point, const Matrix& matrix);
Result<bool, GraphicError> Geometry_HitTestStroke(GraphicObject geometry, const Point& point, GraphicObject strokeStyle, float thickness, const Matrix& matrix);
Result<Rect, GraphicError> Geometry_GetBounds(GraphicObject geometry, GraphicObject strokeStyle, float thickness, const Matrix& worldTransform);
GraphicError Geometry_PushTransform(GraphicObject geometry, const TransformPtr& transform);

GraphicError BlurEffect_SetBlurSize(GraphicObject blurEffect, float blurSize);
Result<float, GraphicError>BlurEffect_GetBlurSize(GraphicObject blurEffect);
GraphicError BlurEffect_SetOptimization(GraphicObject blurEffect, BlurOptimization optimization);
Result<BlurOptimization, GraphicError>BlurEffect_GetOptimization(GraphicObject blurEffect);
GraphicError BlurEffect_SetBorderMode(GraphicObject blurEffect, BlurBorderMode borderMode);
Result<BlurBorderMode, GraphicError>BlurEffect_GetBorderMode(GraphicObject blurEffect);

GraphicError DropShadowEffect_SetBlurSize(GraphicObject dropShadowEffect, float blurSize);
Result<float, GraphicError>DropShadowEffect_GetBlurSize(GraphicObject dropShadowEffect);
GraphicError DropShadowEffect_SetOptimization(GraphicObject dropShadowEffect, BlurOptimization optimization);
Result<BlurOptimization, GraphicError>DropShadowEffect_GetOptimization(GraphicObject dropShadowEffect);
GraphicError DropShadowEffect_SetColor(GraphicObject dropShadowEffect, const Color& color);
Result<Color, GraphicError>DropShadowEffect_GetColor(GraphicObject dropShadowEffect);
GraphicError DropShadowEffect_SetEnableTransform(GraphicObject dropShadowEffect, bool enable);
Result<bool, GraphicError>DropShadowEffect_GetEnableTransform(GraphicObject dropShadowEffect);
GraphicError DropShadowEffect_SetShadowTransform(GraphicObject dropShadowEffect, const TransformPtr& transform);
Result<TransformPtr, GraphicError>DropShadowEffect_GetShadowTransform(GraphicObject dropShadowEffect);
GraphicError DropShadowEffect_SetDirection(GraphicObject dropShadowEffect, float direction);
Result<float, GraphicError>DropShadowEffect_GetDirection(GraphicObject dropShadowEffect);
GraphicError DropShadowEffect_SetShadowDepth(GraphicObject dropShadowEffect, float shadowDepth);
Result<float, GraphicError>DropShadowEffect_GetShadowDepth(GraphicObject dropShadowEffect);

GraphicError SaturationEffect_SetAmount(GraphicObject saturationEffect, float amount);
Result<float, GraphicError>SaturationEffect_GetAmount(GraphicObject saturationEffect);


RenderContext8 Rc8=
{ 
	RenderContext_PushSurfaceTarget,
	RenderContext_GetCurrentTarget,
	RenderContext_GetCurrentTargetSize,
	RenderContext_PushTarget,
	RenderContext_PopTarget,
	RenderContext_Clear,
	RenderContext_PushAxisAlignedClip,
	RenderContext_PopAxisAlignedClip,
	RenderContext_PushGeometryClip,
	RenderContext_PopGeometryClip,
	RenderContext_PushOpacity,
	RenderContext_PopOpacity,
	RenderContext_PushOpacityMask,
	RenderContext_PopOpacityMask,
	RenderContext_GetMaximumImageSize,
	RenderContext_DrawGeometry,
	RenderContext_SetTransform,
	RenderContext_DrawText,
	RenderContext_DrawBitmap,
	RenderContext_DrawEffect,
};

Bitmap8 Bmp8
{
	Bitmap_GetSize,
	Bitmap_Map,
	Bitmap_Unmap,
};

Surface8 Sf8
{
	Surface_GetSize,
	Surface_CopyTo,
};

Texture2D8 Tx2
{
	Texture2D_GetSize,
	Texture2D_Map,
	Texture2D_Unmap,
	Texture2D_GetSurface,
};

BitmapBrush8 BmpBH8
{
	BitmapBrush_SetExtendModeX,
	BitmapBrush_GetExtendModeX,
	BitmapBrush_SetExtendModeY,
	BitmapBrush_GetExtendModeY,
	BitmapBrush_SetInterpolationMode,
	BitmapBrush_GetInterpolationMode,
	BitmapBrush_SetSourceRect,
	BitmapBrush_GetSourceRect,
	BitmapBrush_SetBitmap,
	BitmapBrush_GetBitmap,
};

GradientStopCollection8 Gsc8
{
	GradientStopCollection_SetGradientStops,
	GradientStopCollection_SetExtendMode,
	GradientStopCollection_GetExtendMode,
};

LinearGradientBrush8 Lgb8
{
	LinearGradientBrush_SetGradientStopCollection,
	LinearGradientBrush_GetGradientStopCollection,
	LinearGradientBrush_SetStartPoint,
	LinearGradientBrush_GetStartPoint,
	LinearGradientBrush_SetEndPoint,
	LinearGradientBrush_GetEndPoint,
};

RadialGradientBrush8 Rgb8
{
	RadialGradientBrush_SetGradientStopCollection,
	RadialGradientBrush_GetGradientStopCollection,
	RadialGradientBrush_SetCenterPoint,
	RadialGradientBrush_GetCenterPoint,
	RadialGradientBrush_SetGradientOriginOffset,
	RadialGradientBrush_GetGradientOriginOffset,
	RadialGradientBrush_SetRadiusX,
	RadialGradientBrush_GetRadiusX,
	RadialGradientBrush_SetRadiusY,
	RadialGradientBrush_GetRadiusY,
};

SolidColorBrush8 Scb8
{
	SolidColorBrush_SetColor,
	SolidColorBrush_GetColor,
};

Brush8 B8
{
	Brush_SetOpacity,
	Brush_GetOpacity,
	Brush_SetTransform,
	Brush_GetTransform,
};

StrokeStyle8 Ss8
{
	StrokeStyle_SetStartCap,
	StrokeStyle_GetStartCap,
	StrokeStyle_SetEndCap,
	StrokeStyle_GetEndCap,
	StrokeStyle_SetDashCap,
	StrokeStyle_GetDashCap,
	StrokeStyle_SetLineJoinStyle,
	StrokeStyle_GetLineJoinStyle,
	StrokeStyle_SetMiterLimit,
	StrokeStyle_GetMiterLimit,
	StrokeStyle_SetLineDashStyle,
	StrokeStyle_GetLineDashStyle,
	StrokeStyle_SetDashOffset,
	StrokeStyle_GetDashOffset,
	StrokeStyle_SetDashes,
	StrokeStyle_GetDashes,
};

TextFormat8 Tf8
{
	TextFormat_SetFontFamily,
	TextFormat_GetFontFamily,
	TextFormat_SetFontSize,
	TextFormat_GetFontSize,
	TextFormat_SetFontWeight,
	TextFormat_GetFontWeight,
	TextFormat_SetFontStyle,
	TextFormat_GetFontStyle,
	TextFormat_SetFontStretch,
	TextFormat_GetFontStretch,
};

TextLayout8 Tl8
{
	TextLayout_SetTextFormat,
	TextLayout_GetTextFormat,
	TextLayout_SetText,
	TextLayout_GetText,
	TextLayout_SetMaxSize,
	TextLayout_GetMaxSize,
	TextLayout_GetLayoutBounds,
	TextLayout_HitTest,
};

EllipseGeometry8 Eg8
{
	EllipseGeometry_SetCenterPoint,
	EllipseGeometry_GetCenterPoint,
	EllipseGeometry_SetRadiusX,
	EllipseGeometry_GetRadiusX,
	EllipseGeometry_SetRadiusY,
	EllipseGeometry_GetRadiusY,
};
RectangleGeometry8 Rg8
{
	RectangleGeometry_SetRect,
	RectangleGeometry_GetRect,
};

RoundedRectangleGeometry8 Rrg8
{
	RoundedRectangleGeometry_SetRect,
	RoundedRectangleGeometry_GetRect,
	RoundedRectangleGeometry_SetRadiusX,
	RoundedRectangleGeometry_GetRadiusX,
	RoundedRectangleGeometry_SetRadiusY,
	RoundedRectangleGeometry_GetRadiusY,
};

PathGeometry8 Pg8
{
	PathGeometry_SetFigures,
	PathGeometry_SetFillMode,
	PathGeometry_GetFillMode,

};

StreamGeometry8 Sg8
{
	StreamGeometry_Open,
	StreamGeometry_GetIsOpened,
	StreamGeometry_SetFillMode,
	StreamGeometry_GetFillMode,
	StreamGeometry_BeginFigure,
	StreamGeometry_EndFigure,
	StreamGeometry_LineTo,
	StreamGeometry_ArcTo,
	StreamGeometry_BezierTo,
	StreamGeometry_QuadraticBezierTo,
	StreamGeometry_Clear,
	StreamGeometry_Decompose,
};


CombinedGeometry8 Cg8
{
	CombinedGeometry_SetCombineMode,
	CombinedGeometry_GetCombineMode,
};

GeometryGroup8 Gg8
{
	GeometryGroup_SetFillMode,
	GeometryGroup_GetFillMode,
	GeometryGroup_SetGeometries,
};

Geometry8 Geo8
{
	Geometry_HitTest,
	Geometry_HitTestStroke,
	Geometry_GetBounds,
	Geometry_PushTransform,
};

BlurEffect8 BE8
{
	BlurEffect_SetBlurSize,
	BlurEffect_GetBlurSize,
	BlurEffect_SetOptimization,
	BlurEffect_GetOptimization,
	BlurEffect_SetBorderMode,
	BlurEffect_GetBorderMode,
};

DropShadowEffect8 DSE8
{
	DropShadowEffect_SetBlurSize,
	DropShadowEffect_GetBlurSize,
	DropShadowEffect_SetOptimization,
	DropShadowEffect_GetOptimization,
	DropShadowEffect_SetColor,
	DropShadowEffect_GetColor,
	DropShadowEffect_SetEnableTransform,
	DropShadowEffect_GetEnableTransform,
	DropShadowEffect_SetShadowTransform,
	DropShadowEffect_GetShadowTransform,
	DropShadowEffect_SetDirection,
	DropShadowEffect_GetDirection,
	DropShadowEffect_SetShadowDepth,
	DropShadowEffect_GetShadowDepth,
};

SaturationEffect8 SAE8
{
	SaturationEffect_SetAmount,
	SaturationEffect_GetAmount,
};

Feature8 Info8 =
{
	CreateCoreObject,
	CreateGradientStopCollection,
	CreateStrokeStyle,
	CreateBrushObject,
	CreateEffectObject,
	CreateTextObject,
	CreateGeometryObject,
	DeleteObject,
	Rc8,
	Bmp8,
	Tx2,
	Sf8,
	BmpBH8,
	Gsc8,
	Lgb8,
	Rgb8,
	Scb8,
	B8,
	Ss8,
	Tf8,
	Tl8,
	Eg8,
	Rg8,
	Rrg8,
	Pg8,
	Sg8,
	Cg8,
	Gg8,
	Geo8,
	BE8,
	DSE8,
	SAE8,
};


extern "C" __declspec(dllexport) Feature8 *GetFeatureInterface()
{
	return &Info8;
}


Result<GraphicObject, GraphicError> CreateCoreObject(GraphicObjectTypes type, GraphicObject inObj0, float width, float height, Texture2DMode mode2D, const wchar_t* path)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	switch (type)
	{
		case GraphicObjectTypes::Device:
		{
			Microsoft::WRL::ComPtr<ID3D11Device>* device = new Microsoft::WRL::ComPtr<ID3D11Device>();
#ifdef _DEBUG
			HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, device->GetAddressOf(), NULL, NULL);
			if (hr != S_OK) hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_WARP, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, device->GetAddressOf(), NULL, NULL);
			if (hr != S_OK) hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_SOFTWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, device->GetAddressOf(), NULL, NULL);			
#else
			HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, 0, D3D11_SDK_VERSION, device->GetAddressOf(), NULL, NULL);
			if (hr != S_OK) hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_WARP, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, 0, D3D11_SDK_VERSION, device->GetAddressOf(), NULL, NULL);
			if (hr != S_OK) hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_SOFTWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, 0, D3D11_SDK_VERSION, device->GetAddressOf(), NULL, NULL);
#endif
			if (hr == S_OK)
			{
				result.Value = (GraphicObject)device;
				result.State = GraphicError::OK;
			}
			else
			{
				SAFE_DELETE(device);
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		case GraphicObjectTypes::RenderContext:
		{
			try
			{
				if (inObj0)
				{
					Microsoft::WRL::ComPtr<ID3D11Device>* device = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Device>*>(inObj0);
					if (device)
					{
						RenderContextPtr* rc = new RenderContextPtr(make_shared<Graphic::d2d1::RenderContext>(*device));
						result.Value = (GraphicObject)rc;
						result.State = GraphicError::OK;
					}
					else
					{
						result.State = GraphicError::InvalidParameters;
					}
				}
			}
			catch (...)
			{
				result.State = GraphicError::Failed;
			}
			break;
		}
		case GraphicObjectTypes::Texture2D:
		{
			try
			{
				if (inObj0)
				{
					Microsoft::WRL::ComPtr<ID3D11Device>* device = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Device>*>(inObj0);
					if (device)
					{
						auto d3d = device->Get();
						if (d3d)
						{
							D3D11_TEXTURE2D_DESC desc;
							memset(&desc, 0, sizeof(D3D11_TEXTURE2D_DESC));
							desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
							desc.Width = width;
							desc.Height = height;
							desc.ArraySize = 1;
							desc.MipLevels = 1;
							desc.SampleDesc.Count = 1;
							desc.SampleDesc.Quality = 0;
							if (mode2D == Texture2DMode::Target)
							{
								desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
								desc.Usage = D3D11_USAGE_DEFAULT;
								desc.CPUAccessFlags = 0;
								desc.MiscFlags = 0;
							}
							else if (mode2D == Texture2DMode::CpuRead)
							{
								desc.BindFlags = 0;			
								desc.Usage = D3D11_USAGE_STAGING;
								desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
							}
							else
							{
								result.State = GraphicError::InvalidParameters;
								return result;
							}
							Microsoft::WRL::ComPtr<ID3D11Texture2D>* texture2d = new Microsoft::WRL::ComPtr<ID3D11Texture2D>();
							if (d3d->CreateTexture2D(&desc, nullptr, texture2d->GetAddressOf()) == S_OK)
							{
								result.Value = (GraphicObject)texture2d;
								result.State = GraphicError::OK;
							}
						}
						else
							result.State = GraphicError::InvalidParameters;
					}
					else
						result.State = GraphicError::InvalidParameters;
				}
			}
			catch (...)
			{
				result.State = GraphicError::Failed;
			}
			break;										
		}
		case GraphicObjectTypes::Bitmap:
		{
			try
			{
				if (inObj0)
				{
					Microsoft::WRL::ComPtr<IDXGISurface>* surface = reinterpret_cast<Microsoft::WRL::ComPtr<IDXGISurface>*>(inObj0);
					BitmapPtr* bitmap = new BitmapPtr(make_shared<Graphic::d2d1::Bitmap>(*surface));
					if (bitmap)
					{
						result.Value = (GraphicObject)bitmap;
						result.State = GraphicError::OK;
					}
				}
				else if (path)
				{
					auto bp = Graphic::d2d1::Bitmap::LoadFromPath(path);
					if (bp.Value)
					{
						BitmapPtr* bitmap = new BitmapPtr(bp.Value);
						if (bitmap)
						{
							result.Value = (GraphicObject)bitmap;
							result.State = GraphicError::OK;
						}
					}
					else
					{
						result.WriteMessage(bp.Message);
					}
				}
				else
				{
					BitmapPtr* bitmap = new BitmapPtr(make_shared<Graphic::d2d1::Bitmap>(Size((unsigned)width, (unsigned)height)));
					if (bitmap)
					{
						result.Value = (GraphicObject)bitmap;
						result.State = GraphicError::OK;
					}
				}
			}
			catch (...)
			{
				result.State = GraphicError::Failed;
			}
			break;
		}
		default:
		{
			result.State = GraphicError::NotImplemented;
			break;
		}
	}
	return result;
}
Result<GraphicObject, GraphicError> CreateGradientStopCollection(GraphicObjectTypes type, const ObservableCollection<GradientStop>& gradientstops, ExtendMode extend)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	switch (type)
	{
		case GraphicObjectTypes::GradientStopCollection:
		{
			GradientStopCollectionPtr* stops = new GradientStopCollectionPtr(make_shared<Graphic::d2d1::GradientStopCollection>());
			stops->get()->ExtendMode(extend);
			stops->get()->SetGradientStops(gradientstops);
			result.Value = (GraphicObject)stops;
			result.State = GraphicError::OK;
			break;
		}
		default:
		{
			result.State = GraphicError::NotImplemented;
			break;
		}
	}
	return result;
}
Result<GraphicObject, GraphicError> CreateStrokeStyle(GraphicObjectTypes type, CapStyle startCap, CapStyle endCap, CapStyle dashCap, LineJoin lineJoinStyle, float miterLimit, DashStyle lineDashStyle, float dashOffset, const float* dashes, unsigned int dashCount)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	switch (type)
	{
		case GraphicObjectTypes::StrokeStyle:
		{
			try
			{
				StrokeStylePtr* ssp = new StrokeStylePtr(make_shared<Graphic::d2d1::StrokeStyle>());
				Graphic::d2d1::StrokeStyle* ss = ssp->get();
				ss->StartCap(startCap);
				ss->EndCap(endCap);
				ss->DashCap(dashCap);
				ss->LineJoinStyle(lineJoinStyle);
				ss->MiterLimit(miterLimit);
				ss->LineDashStyle(lineDashStyle);
				ss->DashOffset(dashOffset);
				ss->Dashes(dashes,dashCount);
				result.Value = (GraphicObject)ssp;
				result.State = GraphicError::OK;
			}
			catch (...)
			{
				result.State = GraphicError::Failed;
			}
			break;
		}
		default:
		{
			result.State = GraphicError::NotImplemented;
			break;
		}
	}
	return result;
}
Result<GraphicObject, GraphicError> CreateBrushObject(GraphicObjectTypes type, const Color& color0, GraphicObject inObj0, Point start, Point end, Point center, Point gradientOriginOffset, float radiusX, float radiusY, ExtendMode modeX, ExtendMode modeY, Rect sourceRect, InterpolationMode interpolation)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	switch (type)
	{
		case GraphicObjectTypes::SolidColorBrush:
		{
			SolidColorBrushPtr*brush = new SolidColorBrushPtr(make_shared<Graphic::d2d1::SolidColorBrush>(color0));
			result.Value = (GraphicObject)brush;
			result.State = GraphicError::OK;
			break;
		}
		case GraphicObjectTypes::LinearGradientBrush:
		{
			try
			{
				GradientStopCollectionPtr* gs = reinterpret_cast<GradientStopCollectionPtr*>(inObj0);
				if (gs->get())
				{
					LinearGradientBrushPtr* brush = new LinearGradientBrushPtr(make_shared<Graphic::d2d1::LinearGradientBrush>(*gs, start, end));
					result.Value = (GraphicObject)brush;
					result.State = GraphicError::OK;
				}
			}
			catch (...)
			{
				result.State = GraphicError::Failed;
			}
			break;
		}
		case GraphicObjectTypes::RadialGradientBrush:
		{
			try
			{
				GradientStopCollectionPtr* gs = reinterpret_cast<GradientStopCollectionPtr*>(inObj0);
				if (gs->get())
				{
					RadialGradientBrushPtr* brush = new RadialGradientBrushPtr(make_shared<Graphic::d2d1::RadialGradientBrush>(*gs, center, gradientOriginOffset, radiusX, radiusY));
					result.Value = (GraphicObject)brush;
					result.State = GraphicError::OK;
				}
			}
			catch (...)
			{
				result.State = GraphicError::Failed;
			}
			break;
		}
		case GraphicObjectTypes::BitmapBrush:
		{
			try
			{
				BitmapPtr* gs = reinterpret_cast<BitmapPtr*>(inObj0);
				if (gs->get())
				{
					BitmapBrushPtr* brush = new BitmapBrushPtr(make_shared<Graphic::d2d1::BitmapBrush>(*gs, modeX, modeY, sourceRect, interpolation));
					result.Value = (GraphicObject)brush;
					result.State = GraphicError::OK;
				}
			}
			catch (...)
			{
				result.State = GraphicError::Failed;
			}
			break;
		}
		default:
		{
			result.State = GraphicError::NotImplemented;
			break;
		}
	}
	return result;
}
Result<GraphicObject, GraphicError> CreateEffectObject(GraphicObjectTypes type, GraphicEffectTypes effectType, const Color& color0, float value0, BlurOptimization optimization, BlurBorderMode borderMode, bool value1, const TransformPtr& transform, float value2, float value3)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	switch (type)
	{
		case GraphicObjectTypes::Effect:
		{										   
			switch (effectType)
			{
				case GraphicEffectTypes::BlurEffect:
				{
					BlurEffectPtr* effect = new BlurEffectPtr(make_shared<Graphic::d2d1::BlurEffect>());
					Graphic::d2d1::BlurEffect* e = effect->get();
					e->BlurSize(value0);
					e->BorderMode(borderMode);
					e->Optimization(optimization);
					result.Value = (GraphicObject)effect;
					result.State = GraphicError::OK;
				}
				break;
				case GraphicEffectTypes::DropShadowEffect:
				{
					DropShadowEffectPtr* effect = new DropShadowEffectPtr(make_shared<Graphic::d2d1::DropShadowEffect>());
					Graphic::d2d1::DropShadowEffect* e = effect->get();
					e->BlurSize(value0);
					e->ShadowColor(color0);
					e->Optimization(optimization);
					e->EnableTransform(value1);
					e->ShadowTransform(transform);
					e->Direction(value2);
					e->ShadowDepth(value3);
					result.Value = (GraphicObject)effect;
					result.State = GraphicError::OK;
				}
				break;
				case GraphicEffectTypes::SaturationEffect:
				{
					SaturationEffectPtr* effect = new SaturationEffectPtr(make_shared<Graphic::d2d1::SaturationEffect>());
					effect->get()->SaturationAmount(value0);
					result.Value = (GraphicObject)effect;
					result.State = GraphicError::OK;
				}
				break;
				default:
				{
					result.State = GraphicError::NotImplemented;
				}
				break;
			}
			break;
		}
		default:
		{
			result.State = GraphicError::NotImplemented;
			break;
		}
	}
	return result;
}
Result<GraphicObject, GraphicError> CreateTextObject(GraphicObjectTypes type, const wchar_t* value0, float value1, const Size& maxSize, FontWeight fontWeight, FontStyle fontStyle, FontStretch fontStretch, GraphicObject inObj0 )
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	switch (type)
	{
		case GraphicObjectTypes::TextFormat:
		{
			try
			{
				TextFormatPtr* format = new TextFormatPtr(make_shared<Graphic::d2d1::TextFormat>(value0, value1, fontWeight, fontStyle, fontStretch));
				result.Value = (GraphicObject)format;
				result.State = GraphicError::OK;
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		case GraphicObjectTypes::TextLayout:
		{
			try
			{
				if (inObj0)
				{
					TextFormatPtr* tf = reinterpret_cast<TextFormatPtr*>(inObj0);
					if (tf)
					{
						TextLayoutPtr* layout = new TextLayoutPtr(make_shared<Graphic::d2d1::TextLayout>(*tf, value0, maxSize));
						result.Value = (GraphicObject)layout;
						result.State = GraphicError::OK;
					}
				}
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		default:
		{
			result.State = GraphicError::NotImplemented;
			break;
		}
	}
	return result;
}
Result<GraphicObject, GraphicError> CreateGeometryObject(GraphicObjectTypes type, const Point& point0, const Rect& rect, float radiusX, float radiusY, GraphicObject inObj0, GraphicObject inObj1, GraphicObject* inObjs, unsigned int inObjsCount, const TransformPtr& transform, CombineMode combineMode, FillMode fillMode)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	switch (type)
	{
		case GraphicObjectTypes::EllipseGeometry:
		{
			try
			{
				EllipsePtr* ellipse= new EllipsePtr(make_shared<Graphic::d2d1::Ellipse>(point0, radiusX, radiusY));
				result.Value = (GraphicObject)ellipse;
				result.State = GraphicError::OK;
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		case GraphicObjectTypes::RectangleGeometry:
		{
			try
			{
				RectanglePtr* rectangle = new RectanglePtr(make_shared<Graphic::d2d1::Rectangle>(rect));
				result.Value = (GraphicObject)rectangle;
				result.State = GraphicError::OK;
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		case GraphicObjectTypes::RoundedRectangleGeometry:
		{
			try
			{
				RoundedRectanglePtr* roundedrectangle = new RoundedRectanglePtr(make_shared<Graphic::d2d1::RoundedRectangle>(rect, radiusX, radiusY));
				result.Value = (GraphicObject)roundedrectangle;
				result.State = GraphicError::OK;
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		case GraphicObjectTypes::PathGeometry:
		{
			try
			{
				PathGeometryPtr* pathGeometry = new PathGeometryPtr(make_shared<Graphic::d2d1::PathGeometry>());
				pathGeometry->get()->FillMode(fillMode);
				result.Value = (GraphicObject)pathGeometry;
				result.State = GraphicError::OK;
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}		
		case GraphicObjectTypes::StreamGeometry:
		{
			try
			{
				Graphic::d2d1::StreamGeometryPtr* streamGeometry = new StreamGeometryPtr(make_shared<Graphic::d2d1::StreamGeometry>());
				streamGeometry->get()->FillMode(fillMode);
				result.Value = (GraphicObject)streamGeometry;
				result.State = GraphicError::OK;
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		case GraphicObjectTypes::CombinedGeometry:
		{
			try
			{
				GeometryPtr* geo1 = reinterpret_cast<GeometryPtr*>(inObj0);
				GeometryPtr* geo2 = reinterpret_cast<GeometryPtr*>(inObj1);
				if (geo1->get() && geo2->get())
				{
					CombinedGeometryPtr* combinedGeometry = new CombinedGeometryPtr(make_shared<Graphic::d2d1::CombinedGeometry>(*geo1, *geo2, combineMode));
					result.Value = (GraphicObject)combinedGeometry;
					result.State = GraphicError::OK;
				}
				else
					result.State = GraphicError::InvalidParameters;
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		case GraphicObjectTypes::TransformedGeometry:
		{
			try
			{
				GeometryPtr* geo1 = reinterpret_cast<GeometryPtr*>(inObj0);
				if (geo1->get())
				{
					TransformedGeometryPtr* transformedGeometry = new TransformedGeometryPtr(make_shared<TransformedGeometry>(*geo1, transform));
					result.Value = (GraphicObject)transformedGeometry;
					result.State = GraphicError::OK;
				}
				else
					result.State = GraphicError::InvalidParameters;
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		case GraphicObjectTypes::GeometryGroup:
		{
			try
			{
				GeometryGroupPtr* geometryGroup = new GeometryGroupPtr(make_shared<Graphic::d2d1::GeometryGroup>(fillMode));
				Graphic::d2d1::GeometryGroup* gg = geometryGroup->get();
				if (gg)
				{
					if (inObjsCount > 0)
					{
						vector<intptr_t> tmp(inObjsCount);
						for (unsigned int i = 0; i < inObjsCount; i++)
							tmp.push_back(inObjs[i]);

						gg->Geometries.reset(tmp);
					}

					result.Value = (GraphicObject)geometryGroup;
					result.State = GraphicError::OK;
				}
				else
					result.State = GraphicError::InvalidParameters;
			}
			catch (...)
			{
				result.State = GraphicError::Unsupported;
			}
			break;
		}
		default:
		{
			result.State = GraphicError::NotImplemented;
			break;
		}
	}
	return result;
}

Result<Size, GraphicError> Bitmap_GetSize(GraphicObject bitmap)
{
	Result<Size, GraphicError> result = Result<Size, GraphicError>(Size(), GraphicError::Failed);
	if (!bitmap)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BitmapPtr* bb = reinterpret_cast<BitmapPtr*>(bitmap);
		Graphic::d2d1::Bitmap* bmp = bb->get();
		if (bmp)
		{
			result.Value = bmp->GetSize();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

Result<GraphicError> Bitmap_Map(GraphicObject bitmap, GraphicObject device, BitmapData8* data)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!bitmap)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		BitmapPtr* bb = reinterpret_cast<BitmapPtr*>(bitmap);
		Graphic::d2d1::Bitmap* bmp = bb->get();
		if (bmp)
		{
			Microsoft::WRL::ComPtr<ID3D11Device>* pdev = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Device>*>(device);
			if (pdev)
			{
				auto dev = pdev->Get();
				if (dev)
				{
					ID3D11DeviceContext* dec = nullptr;
					dev->GetImmediateContext(&dec);
					D3D_FEATURE_LEVEL lvl = dev->GetFeatureLevel();
					if (dec)
					{
						D3D11_MAPPED_SUBRESOURCE src;
						auto res = bmp->Map(&src, dec, data->Width, data->Height);
						if (res.Value == GraphicError::OK)
						{
							data->Bits = src.pData;
							data->DepthPitch = src.DepthPitch;
							data->RowPitch = src.RowPitch;
							data->BitsPerSize = lvl >= D3D_FEATURE_LEVEL_10_0 ? 16 : 4;
							result.Value = GraphicError::OK;
						}
						else
						{
							result = res;
						}
					}
					else
					{
						result.Value = GraphicError::Unsupported;
						result.WriteMessage(L"ID3D11DeviceContext interface is invalid");
						return result;
					}
				}
				else
					result.Value = GraphicError::InvalidParameters;
			}
			else
				result.Value = GraphicError::InvalidParameters;
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}

Result<GraphicError>Bitmap_Unmap(GraphicObject bitmap, GraphicObject device)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!bitmap)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		BitmapPtr* bb = reinterpret_cast<BitmapPtr*>(bitmap);
		Graphic::d2d1::Bitmap* bmp = bb->get();
		if (bmp)
		{
			Microsoft::WRL::ComPtr<ID3D11Device>* pdev = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Device>*>(device);
			if (pdev)
			{
				auto dev = pdev->Get();
				if (dev)
				{
					ID3D11DeviceContext* dec = nullptr;
					dev->GetImmediateContext(&dec);
					if (dec)
					{
						result = bmp->Unmap(dec);		
					}
					else
					{
						result.Value = GraphicError::Unsupported;
						result.WriteMessage(L"ID3D11DeviceContext interface is invalid");
						return result;
					}
				}
				else
					result.Value = GraphicError::InvalidParameters;
			}
			else
				result.Value = GraphicError::InvalidParameters;
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}


Result<Size, GraphicError> Surface_GetSize(GraphicObject surface)
{
	Result<Size, GraphicError> result = Result<Size, GraphicError>(Size(), GraphicError::Failed);
	if (!surface)
		result.State = GraphicError::InvalidParameters;
	else
	{
		Microsoft::WRL::ComPtr<IDXGISurface>* bb = reinterpret_cast<Microsoft::WRL::ComPtr<IDXGISurface>*>(surface);
		IDXGISurface* sf = bb->Get();
		if (sf)
		{
			DXGI_SURFACE_DESC desc;
			if (sf->GetDesc(&desc) == S_OK)
			{
				result.Value =Size(desc.Width, desc.Height) ;
				result.State = GraphicError::OK;
			}
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

Result<GraphicError> Surface_CopyTo(GraphicObject surface, GraphicObject surfaceCPU, GraphicObject device)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!surface)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		Microsoft::WRL::ComPtr<IDXGISurface>* bb = reinterpret_cast<Microsoft::WRL::ComPtr<IDXGISurface>*>(surface);
		if (bb)
		{
			IDXGISurface* sf = bb->Get();
			if (sf)
			{
				Microsoft::WRL::ComPtr<IDXGISurface>* cc = reinterpret_cast<Microsoft::WRL::ComPtr<IDXGISurface>*>(surfaceCPU);
				if (cc)
				{
					IDXGISurface* sfc = cc->Get();
					if (sfc)
					{
						Microsoft::WRL::ComPtr<ID3D11Device>* pdev = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Device>*>(device);
						if (pdev)
						{
							auto dev = pdev->Get();
							if (dev)
							{
								ID3D11DeviceContext* dec = nullptr;
								dev->GetImmediateContext(&dec);
								if (dec)
								{
									Microsoft::WRL::ComPtr<ID3D11Resource> tt;
									if (sf->QueryInterface(tt.GetAddressOf()) == S_OK)
									{
										Microsoft::WRL::ComPtr<ID3D11Resource> hh;
										if (sfc->QueryInterface(hh.GetAddressOf()) == S_OK)
										{
											dec->CopyResource(hh.Get(), tt.Get());
											result.Value = GraphicError::OK;
										}
										else
										{
											result.WriteMessage(L"can not get ID3D11Resource interface from surfaceCPU");
										}
									}
									else
									{
										result.WriteMessage(L"can not get ID3D11Resource interface from surface");
									}
								}
							}
							else
								result.Value = GraphicError::InvalidParameters;
						}
						else
							result.Value = GraphicError::InvalidParameters;
					}
					else
						result.Value = GraphicError::InvalidParameters;
				}
				else
					result.Value = GraphicError::InvalidParameters;
			}
			else
				result.Value = GraphicError::InvalidParameters;
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;

}

Result<Size, GraphicError> Texture2D_GetSize(GraphicObject texture)
{
	Result<Size, GraphicError> result = Result<Size, GraphicError>(Size(), GraphicError::Failed);
	if (!texture)
		result.State = GraphicError::InvalidParameters;
	else
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D>* bb = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Texture2D>*>(texture);
		ID3D11Texture2D* sf = bb->Get();
		if (sf)
		{
			D3D11_TEXTURE2D_DESC desc;
			sf->GetDesc(&desc);
			result.Value = Size(desc.Width, desc.Height);
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

Result<GraphicError>Texture2D_Map(GraphicObject texture, GraphicObject device, BitmapData8* data)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!texture || !data || !device)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D>* bb = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Texture2D>*>(texture);
		if (bb)
		{
			auto tx = bb->Get();
			if (tx)
			{
				Microsoft::WRL::ComPtr<ID3D11Device>* pdev = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Device>*>(device);
				if (pdev)
				{
					auto dev = pdev->Get();
					if (dev)
					{
						ID3D11DeviceContext* dec = nullptr;
						dev->GetImmediateContext(&dec);
						D3D_FEATURE_LEVEL lvl = dev->GetFeatureLevel();
						if (dec)
						{
							IDXGISurface* surface = nullptr;//dispose
							if (tx->QueryInterface(&surface) != S_OK)
							{
								result.Value = GraphicError::Unsupported;
								result.WriteMessage(L"can not get IDXGISurface interface");
								return result;
							}
							if (!surface)
							{
								result.Value = GraphicError::Unsupported;
								result.WriteMessage(L"IDXGISurface interface is invalid");
								return result;
							}

							ID3D11Resource* res = nullptr;

							if (surface->QueryInterface(&res) != S_OK)
							{
								surface->Release();//disposed
								result.Value = GraphicError::Unsupported;
								result.WriteMessage(L"ID3D11Resource interface is invalid");
								return result;
							}
							D3D11_MAPPED_SUBRESOURCE src;
							if (S_OK == dec->Map(res, 0, D3D11_MAP::D3D11_MAP_READ, 0, &src))
							{
								data->Bits = src.pData;
								data->DepthPitch = src.DepthPitch;
								data->RowPitch = src.RowPitch;
								data->BitsPerSize = lvl >= D3D_FEATURE_LEVEL_10_0 ? 16 : 4;
								DXGI_SURFACE_DESC suf;
								surface->GetDesc(&suf);
								data->Width = suf.Width;
								data->Height = suf.Height;
								res->Release();//disposed
								surface->Release();//disposed
								result.Value = GraphicError::OK;
							}
							else
							{
								res->Release();//disposed
								surface->Release();//disposed
								result.WriteMessage(L"map read failed");
							}
						}
						else
						{
							result.Value = GraphicError::Unsupported;
							result.WriteMessage(L"ID3D11DeviceContext interface is invalid");
							return result;
						}
					}
					else
						result.Value = GraphicError::InvalidParameters;
				}
				else
					result.Value = GraphicError::InvalidParameters;
			}
			else
				result.Value = GraphicError::InvalidParameters;
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}

Result<GraphicError>Texture2D_Unmap(GraphicObject texture, GraphicObject device)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!texture || !device)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D>* bb = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Texture2D>*>(texture);
		if (bb)
		{
			auto tx = bb->Get();
			if (tx)
			{
				Microsoft::WRL::ComPtr<ID3D11Device>* pdev = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Device>*>(device);
				if (pdev)
				{
					auto dev = pdev->Get();
					if (dev)
					{
						ID3D11DeviceContext* dec = nullptr;
						dev->GetImmediateContext(&dec);
						if (dec)
						{
							IDXGISurface* surface = nullptr;//dispose
							if (tx->QueryInterface(&surface) != S_OK)
							{
								result.Value = GraphicError::Unsupported;
								result.WriteMessage(L"can not get IDXGISurface interface");
								return result;
							}
							if (!surface)
							{
								result.Value = GraphicError::Unsupported;
								result.WriteMessage(L"IDXGISurface interface is invalid");
								return result;
							}

							ID3D11Resource* res = nullptr;
							if (surface->QueryInterface(&res) != S_OK)
							{
								surface->Release();//disposed
								result.Value = GraphicError::Unsupported;
								result.WriteMessage(L"ID3D11Resource interface is invalid");
								return result;
							}
							dec->Unmap(res, 0);
							res->Release();//disposed
							surface->Release();//disposed
							result.Value = GraphicError::OK;
						}
						else
						{
							result.Value = GraphicError::Unsupported;
							result.WriteMessage(L"ID3D11DeviceContext interface is invalid");
							return result;
						}
					}
					else
						result.Value = GraphicError::InvalidParameters;
				}
				else
					result.Value = GraphicError::InvalidParameters;
			}
			else
				result.Value = GraphicError::InvalidParameters;
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}

Result<GraphicObject, GraphicError> Texture2D_GetSurface(GraphicObject texture)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	if (!texture)
		result.State = GraphicError::InvalidParameters;
	else
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D>* bb = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Texture2D>*>(texture);
		ID3D11Texture2D* sf = bb->Get();
		if (sf)
		{
			Microsoft::WRL::ComPtr<IDXGISurface>* suf = new Microsoft::WRL::ComPtr<IDXGISurface>();
			if (sf->QueryInterface(suf->GetAddressOf()) == S_OK)
			{
				result.Value = (intptr_t)suf;
				result.State = GraphicError::OK;
			}
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;

}

GraphicError RenderContext_PushSurfaceTarget(GraphicObject renderContext, GraphicObject surface)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext || !surface)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			Microsoft::WRL::ComPtr<IDXGISurface>* sf = reinterpret_cast<Microsoft::WRL::ComPtr<IDXGISurface>*>(surface);
			if (sf)
			{
				if(rc->PushRenderSurface(*sf))
					result = GraphicError::OK;
			}
			else
				result = GraphicError::InvalidParameters;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}

Result<GraphicObject, GraphicError> RenderContext_GetCurrentTarget(GraphicObject renderContext)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	if (!renderContext)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		if (render)
		{
			Graphic::d2d1::RenderContext* rc = render->get();
			if (rc)
			{
				result.Value = (intptr_t)rc->GetCurrentRenderTarget();
				result.State = GraphicError::OK;
			}
			else
				result.State = GraphicError::InvalidParameters;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

Result<Size, GraphicError> RenderContext_GetCurrentTargetSize(GraphicObject renderContext)
{
	Result<Size, GraphicError> result = Result<Size, GraphicError>(Size(), GraphicError::Failed);
	if (!renderContext)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		if (render)
		{
			Graphic::d2d1::RenderContext* rc = render->get();
			if (rc)
			{
				if (rc->GetCurrentTargetSize(result.Value))
					result.State = GraphicError::OK;
			}
			else
				result.State = GraphicError::InvalidParameters;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError RenderContext_PushTarget(GraphicObject renderContext, GraphicObject bitmap)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext || !bitmap)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			BitmapPtr* bb = reinterpret_cast<BitmapPtr*>(bitmap);
			if (bb)
			{
				rc->PushRenderTarget(*bb);
				result = GraphicError::OK;
			}
			else
				result = GraphicError::InvalidParameters;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RenderContext_PopTarget(GraphicObject renderContext)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result= GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{	
			rc->PopRenderTarget();
			result = GraphicError::OK;
		}
		else
			result= GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RenderContext_Clear(GraphicObject renderContext, const Color& color)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			rc->Clear(color);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RenderContext_PushAxisAlignedClip(GraphicObject renderContext, const Rect& clipRect, AntialiasMode mode)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			rc->PushAxisAlignedClip(clipRect, mode);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RenderContext_PopAxisAlignedClip(GraphicObject renderContext)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			rc->PopAxisAlignedClip();
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError RenderContext_PushGeometryClip(GraphicObject renderContext, GraphicObject geometry)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext || !geometry)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			GeometryPtr* bb = reinterpret_cast<GeometryPtr*>(geometry);
			if (bb)
			{
				if (rc->PushGeometryClip(*bb))
				{
					result = GraphicError::OK;
				}
			}
			else
				result = GraphicError::InvalidParameters;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError RenderContext_PopGeometryClip(GraphicObject renderContext)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			rc->PopGeometryClip();
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError RenderContext_PushOpacity(GraphicObject renderContext, float opacity)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			if (rc->PushOpacity(opacity))
			{
				result = GraphicError::OK;
			}
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError RenderContext_PopOpacity(GraphicObject renderContext)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			rc->PopOpacity();
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError RenderContext_PushOpacityMask(GraphicObject renderContext, const Rect& rect, GraphicObject brush)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext || !brush)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			BrushPtr* bb = reinterpret_cast<BrushPtr*>(brush);
			if (bb)
			{
				if (rc->PushOpacityMask(rect, *bb))
				{
					result = GraphicError::OK;
				}
			}
			else
				result = GraphicError::InvalidParameters;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError RenderContext_PopOpacityMask(GraphicObject renderContext)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			rc->PopOpacityMask();
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}



Result<int, GraphicError> RenderContext_GetMaximumImageSize(GraphicObject renderContext)
{
	Result<int, GraphicError> result = Result<int, GraphicError>(0, GraphicError::Failed);
	if (!renderContext)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			result.Value = rc->MaximumImageSize();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RenderContext_DrawGeometry(GraphicObject renderContext, GraphicObject geometry, GraphicObject brush, GraphicObject strokeBrush, GraphicObject strokeStyle, float thickness)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			if (geometry)
			{
				GeometryPtr* geo = reinterpret_cast<GeometryPtr*>(geometry);
				if (geo)
				{
					BrushPtr* bh = NULL;
					if (brush) bh = reinterpret_cast<BrushPtr*>(brush);
					BrushPtr* sbh = NULL;
					if (strokeBrush) sbh = reinterpret_cast<BrushPtr*>(strokeBrush);
					StrokeStylePtr* ss = NULL;
					if (strokeStyle) ss = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
					rc->DrawGeometry(*geo, bh == NULL ? NULL : *bh, sbh == NULL ? NULL : *sbh, ss == NULL ? NULL : *ss, thickness);
					result = GraphicError::OK;
				}
			}
			else
				result = GraphicError::InvalidParameters;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RenderContext_SetTransform(GraphicObject renderContext, const Matrix& matrix)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			rc->SetTransform(matrix);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RenderContext_DrawText(GraphicObject renderContext, GraphicObject textLayout, GraphicObject brush, const Point& origin)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			if (textLayout)
			{
				TextLayoutPtr* geo = reinterpret_cast<TextLayoutPtr*>(textLayout);
				if (brush)
				{
					BrushPtr* bh = reinterpret_cast<BrushPtr*>(brush);
					rc->DrawText(*geo, *bh, origin);
					result = GraphicError::OK;
				}
				else
					result = GraphicError::InvalidParameters;
			}
			else
				result = GraphicError::InvalidParameters;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RenderContext_DrawBitmap(GraphicObject renderContext, GraphicObject bitmap, const EffectCompositeMode compositeMode, const Size& targetSize)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			if (bitmap)
			{
				BitmapPtr* bmp = reinterpret_cast<BitmapPtr*>(bitmap);
				rc->DrawBitmap(*bmp, compositeMode, targetSize);
				result = GraphicError::OK;
			}
			else
				result = GraphicError::InvalidParameters;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RenderContext_DrawEffect(GraphicObject renderContext, GraphicObject bitmap, GraphicObject elementEffect, const EffectCompositeMode compositeMode, const Size& size)
{
	GraphicError result = GraphicError::Failed;
	if (!renderContext)
		result = GraphicError::InvalidParameters;
	else
	{
		RenderContextPtr* render = reinterpret_cast<RenderContextPtr*>(renderContext);
		Graphic::d2d1::RenderContext* rc = render->get();
		if (rc)
		{
			if (bitmap)
			{
				BitmapPtr* bmp = reinterpret_cast<BitmapPtr*>(bitmap);
				if (elementEffect)
				{
					ElementEffectPtr* ee = reinterpret_cast<ElementEffectPtr*>(elementEffect);
					rc->DrawEffect(*bmp, *ee, compositeMode, size);
					result = GraphicError::OK;
				}
				else
					result = GraphicError::InvalidParameters;

			}
			else
				result = GraphicError::InvalidParameters;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;

}

GraphicError BitmapBrush_SetExtendModeX(GraphicObject bitmapBrush, ExtendMode modeX)
{
	GraphicError result = GraphicError::Failed;
	if (!bitmapBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			bmp->ExtendModeX(modeX);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<ExtendMode, GraphicError> BitmapBrush_GetExtendModeX(GraphicObject bitmapBrush)
{
	Result<ExtendMode, GraphicError> result = Result<ExtendMode, GraphicError>(ExtendMode::Clamp, GraphicError::Failed);
	if (!bitmapBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			result.Value = bmp->ExtendModeX();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError BitmapBrush_SetExtendModeY(GraphicObject bitmapBrush, ExtendMode modeY)
{
	GraphicError result = GraphicError::Failed;
	if (!bitmapBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			bmp->ExtendModeY(modeY);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<ExtendMode, GraphicError> BitmapBrush_GetExtendModeY(GraphicObject bitmapBrush)
{
	Result<ExtendMode, GraphicError> result = Result<ExtendMode, GraphicError>(ExtendMode::Clamp, GraphicError::Failed);
	if (!bitmapBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			result.Value = bmp->ExtendModeY();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError BitmapBrush_SetInterpolationMode(GraphicObject bitmapBrush, InterpolationMode mode)
{
	GraphicError result = GraphicError::Failed;
	if (!bitmapBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			bmp->Interpolation(mode);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<InterpolationMode, GraphicError> BitmapBrush_GetInterpolationMode(GraphicObject bitmapBrush)
{
	Result<InterpolationMode, GraphicError> result = Result<InterpolationMode, GraphicError>(InterpolationMode::Linear, GraphicError::Failed);
	if (!bitmapBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			result.Value = bmp->Interpolation();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError BitmapBrush_SetSourceRect(GraphicObject bitmapBrush, const Rect& sourceRect)
{
	GraphicError result = GraphicError::Failed;
	if (!bitmapBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			bmp->SourceRect(sourceRect);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Rect, GraphicError> BitmapBrush_GetSourceRect(GraphicObject bitmapBrush)
{
	Result<Rect, GraphicError> result = Result<Rect, GraphicError>(Rect::Unknown(), GraphicError::Failed);
	if (!bitmapBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			result.Value = bmp->SourceRect();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError BitmapBrush_SetBitmap(GraphicObject bitmapBrush, GraphicObject bitmap)
{
	GraphicError result = GraphicError::Failed;
	if (!bitmapBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			if (bitmap)
			{
				BitmapPtr* bb = reinterpret_cast<BitmapPtr*>(bitmap);
				bmp->Image(*bb);
				result = GraphicError::OK;
			}	
			else
				result = GraphicError::InvalidParameters;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicObject, GraphicError> BitmapBrush_GetBitmap(GraphicObject bitmapBrush)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	if (!bitmapBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BitmapBrushPtr* bb = reinterpret_cast<BitmapBrushPtr*>(bitmapBrush);
		Graphic::d2d1::BitmapBrush* bmp = bb->get();
		if (bmp)
		{
			result.Value = (GraphicObject)&bmp->Image();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError GradientStopCollection_SetGradientStops(GraphicObject gradientStopCollection, const ObservableCollection<GradientStop>& gradientStops)
{
	GraphicError result = GraphicError::Failed;
	if (!gradientStopCollection)
		result = GraphicError::InvalidParameters;
	else
	{
		GradientStopCollectionPtr* gss = reinterpret_cast<GradientStopCollectionPtr*>(gradientStopCollection);
		Graphic::d2d1::GradientStopCollection* gsc = gss->get();
		if (gsc)
		{
			gsc->SetGradientStops(gradientStops);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError GradientStopCollection_SetExtendMode(GraphicObject gradientStopCollection, ExtendMode mode){
	GraphicError result = GraphicError::Failed;
	if (!gradientStopCollection)
		result = GraphicError::InvalidParameters;
	else
	{
		GradientStopCollectionPtr* gss = reinterpret_cast<GradientStopCollectionPtr*>(gradientStopCollection);
		Graphic::d2d1::GradientStopCollection* gsc = gss->get();
		if (gsc)
		{
			gsc->ExtendMode(mode);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<ExtendMode, GraphicError> GradientStopCollection_GetExtendMode(GraphicObject gradientStopCollection)
{
	Result<ExtendMode, GraphicError> result = Result<ExtendMode, GraphicError>(ExtendMode::Clamp, GraphicError::Failed);
	if (!gradientStopCollection)
		result.State = GraphicError::InvalidParameters;
	else
	{
		GradientStopCollectionPtr* gss = reinterpret_cast<GradientStopCollectionPtr*>(gradientStopCollection);
		Graphic::d2d1::GradientStopCollection* gsc = gss->get();
		if (gsc)
		{
			result.Value = gsc->ExtendMode();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError LinearGradientBrush_SetGradientStopCollection(GraphicObject linearGradientBrush, GraphicObject gradientStopCollection)
{
	GraphicError result = GraphicError::Failed;
	if (!linearGradientBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		LinearGradientBrushPtr* lgbp = reinterpret_cast<LinearGradientBrushPtr*>(linearGradientBrush);
		Graphic::d2d1::LinearGradientBrush* lgb = lgbp->get();
		if (gradientStopCollection && lgb)
		{
			GradientStopCollectionPtr* gss = reinterpret_cast<GradientStopCollectionPtr*>(gradientStopCollection);
			lgb->GradientStopCollection(*gss);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicObject, GraphicError> LinearGradientBrush_GetGradientStopCollection(GraphicObject linearGradientBrush, GraphicObject gradientStopCollection)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	if (!linearGradientBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		LinearGradientBrushPtr* lgbp = reinterpret_cast<LinearGradientBrushPtr*>(linearGradientBrush);
		Graphic::d2d1::LinearGradientBrush* lgb = lgbp->get();
		if (lgb)
		{	
			result.Value = (GraphicObject)&lgb->GradientStopCollection();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError LinearGradientBrush_SetStartPoint(GraphicObject linearGradientBrush, Point point)
{
	GraphicError result = GraphicError::Failed;
	if (!linearGradientBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		LinearGradientBrushPtr* lgbp = reinterpret_cast<LinearGradientBrushPtr*>(linearGradientBrush);
		Graphic::d2d1::LinearGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Start(point);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Point, GraphicError> LinearGradientBrush_GetStartPoint(GraphicObject linearGradientBrush)
{
	Result<Point, GraphicError> result = Result<Point, GraphicError>(Point::Unknown(), GraphicError::Failed);
	if (!linearGradientBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		LinearGradientBrushPtr* lgbp = reinterpret_cast<LinearGradientBrushPtr*>(linearGradientBrush);
		Graphic::d2d1::LinearGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value =lgb->Start();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError LinearGradientBrush_SetEndPoint(GraphicObject linearGradientBrush, Point point)
{
	GraphicError result = GraphicError::Failed;
	if (!linearGradientBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		LinearGradientBrushPtr* lgbp = reinterpret_cast<LinearGradientBrushPtr*>(linearGradientBrush);
		Graphic::d2d1::LinearGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			lgb->End(point);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Point, GraphicError> LinearGradientBrush_GetEndPoint(GraphicObject linearGradientBrush)
{
	Result<Point, GraphicError> result = Result<Point, GraphicError>(Point::Unknown(), GraphicError::Failed);
	if (!linearGradientBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		LinearGradientBrushPtr* lgbp = reinterpret_cast<LinearGradientBrushPtr*>(linearGradientBrush);
		Graphic::d2d1::LinearGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->End();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError RadialGradientBrush_SetGradientStopCollection(GraphicObject radialGradientBrush, GraphicObject gradientStopCollection)
{
	GraphicError result = GraphicError::Failed;
	if (!radialGradientBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (gradientStopCollection && lgb)
		{
			GradientStopCollectionPtr* gss = reinterpret_cast<GradientStopCollectionPtr*>(gradientStopCollection);
			lgb->GradientStopCollection(*gss);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicObject, GraphicError> RadialGradientBrush_GetGradientStopCollection(GraphicObject radialGradientBrush, GraphicObject gradientStopCollection)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	if (!radialGradientBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = (GraphicObject)&lgb->GradientStopCollection();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RadialGradientBrush_SetCenterPoint(GraphicObject radialGradientBrush, Point point)
{
	GraphicError result = GraphicError::Failed;
	if (!radialGradientBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Center(point);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Point, GraphicError> RadialGradientBrush_GetCenterPoint(GraphicObject radialGradientBrush)
{
	Result<Point, GraphicError> result = Result<Point, GraphicError>(Point::Unknown(), GraphicError::Failed);
	if (!radialGradientBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Center();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RadialGradientBrush_SetGradientOriginOffset(GraphicObject radialGradientBrush, Point gradientOriginOffset)
{
	GraphicError result = GraphicError::Failed;
	if (!radialGradientBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			lgb->GradientOriginOffset(gradientOriginOffset);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Point, GraphicError> RadialGradientBrush_GetGradientOriginOffset(GraphicObject radialGradientBrush)
{
	Result<Point, GraphicError> result = Result<Point, GraphicError>(Point::Unknown(), GraphicError::Failed);
	if (!radialGradientBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->GradientOriginOffset();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RadialGradientBrush_SetRadiusX(GraphicObject radialGradientBrush, float radiusX)
{
	GraphicError result = GraphicError::Failed;
	if (!radialGradientBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			lgb->RadiusX(radiusX);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> RadialGradientBrush_GetRadiusX(GraphicObject radialGradientBrush)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(FP_NAN, GraphicError::Failed);
	if (!radialGradientBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->RadiusX();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RadialGradientBrush_SetRadiusY(GraphicObject radialGradientBrush, float radiusY)
{
	GraphicError result = GraphicError::Failed;
	if (!radialGradientBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			lgb->RadiusY(radiusY);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> RadialGradientBrush_GetRadiusY(GraphicObject radialGradientBrush)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(FP_NAN, GraphicError::Failed);
	if (!radialGradientBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RadialGradientBrushPtr* lgbp = reinterpret_cast<RadialGradientBrushPtr*>(radialGradientBrush);
		Graphic::d2d1::RadialGradientBrush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->RadiusY();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError SolidColorBrush_SetColor(GraphicObject solidColorBrush, const Color& color)
{
	GraphicError result = GraphicError::Failed;
	if (!solidColorBrush)
		result = GraphicError::InvalidParameters;
	else
	{
		SolidColorBrushPtr* lgbp = reinterpret_cast<SolidColorBrushPtr*>(solidColorBrush);
		Graphic::d2d1::SolidColorBrush* lgb = lgbp->get();
		if (lgb)
		{
			lgb->FillColor(color);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Color, GraphicError> SolidColorBrush_GetColor(GraphicObject solidColorBrush)
{
	Result<Color, GraphicError> result = Result<Color, GraphicError>(Color(), GraphicError::Failed);
	if (!solidColorBrush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		SolidColorBrushPtr* lgbp = reinterpret_cast<SolidColorBrushPtr*>(solidColorBrush);
		Graphic::d2d1::SolidColorBrush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->FillColor();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}


GraphicError Brush_SetOpacity(GraphicObject brush, float opacity)
{
	GraphicError result = GraphicError::Failed;
	if (!brush)
		result = GraphicError::InvalidParameters;
	else
	{
		BrushPtr* lgbp = reinterpret_cast<BrushPtr*>(brush);
		Graphic::d2d1::Brush* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Opacity(opacity);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> Brush_GetOpacity(GraphicObject brush)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(1.0f, GraphicError::Failed);
	if (!brush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BrushPtr* lgbp = reinterpret_cast<BrushPtr*>(brush);
		Graphic::d2d1::Brush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Opacity();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError Brush_SetTransform(GraphicObject brush, const Matrix& matrix)
{
	GraphicError result = GraphicError::Failed;
	if (!brush)
		result = GraphicError::InvalidParameters;
	else
	{
		BrushPtr* lgbp = reinterpret_cast<BrushPtr*>(brush);
		Graphic::d2d1::Brush* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Transform(matrix);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Matrix, GraphicError> Brush_GetTransform(GraphicObject brush)
{
	Result<Matrix, GraphicError> result = Result<Matrix, GraphicError>(Matrix::Identity(), GraphicError::Failed);
	if (!brush)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BrushPtr* lgbp = reinterpret_cast<BrushPtr*>(brush);
		Graphic::d2d1::Brush* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Transform();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}


GraphicError StrokeStyle_SetStartCap(GraphicObject strokeStyle, CapStyle startCap)
{
	GraphicError result = GraphicError::Failed;
	if (!strokeStyle)
		result = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->StartCap(startCap);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<CapStyle, GraphicError> StrokeStyle_GetStartCap(GraphicObject strokeStyle)
{
	Result<CapStyle, GraphicError> result = Result<CapStyle, GraphicError>(CapStyle::Flat, GraphicError::Failed);
	if (!strokeStyle)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->StartCap();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError StrokeStyle_SetEndCap(GraphicObject strokeStyle, CapStyle endCap)
{
	GraphicError result = GraphicError::Failed;
	if (!strokeStyle)
		result = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->EndCap(endCap);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<CapStyle, GraphicError> StrokeStyle_GetEndCap(GraphicObject strokeStyle)
{
	Result<CapStyle, GraphicError> result = Result<CapStyle, GraphicError>(CapStyle::Flat, GraphicError::Failed);
	if (!strokeStyle)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->EndCap();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError StrokeStyle_SetDashCap(GraphicObject strokeStyle, CapStyle dashCap)
{
	GraphicError result = GraphicError::Failed;
	if (!strokeStyle)
		result = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->DashCap(dashCap);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<CapStyle, GraphicError> StrokeStyle_GetDashCap(GraphicObject strokeStyle)
{
	Result<CapStyle, GraphicError> result = Result<CapStyle, GraphicError>(CapStyle::Flat, GraphicError::Failed);
	if (!strokeStyle)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->DashCap();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError StrokeStyle_SetLineJoinStyle(GraphicObject strokeStyle, LineJoin lineJoinStyle)
{
	GraphicError result = GraphicError::Failed;
	if (!strokeStyle)
		result = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->LineJoinStyle(lineJoinStyle);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<LineJoin, GraphicError> StrokeStyle_GetLineJoinStyle(GraphicObject strokeStyle)
{
	Result<LineJoin, GraphicError> result = Result<LineJoin, GraphicError>(LineJoin::Miter, GraphicError::Failed);
	if (!strokeStyle)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->LineJoinStyle();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError StrokeStyle_SetMiterLimit(GraphicObject strokeStyle, float miterLimit)
{
	GraphicError result = GraphicError::Failed;
	if (!strokeStyle)
		result = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->MiterLimit(miterLimit);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> StrokeStyle_GetMiterLimit(GraphicObject strokeStyle)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(FP_NAN, GraphicError::Failed);
	if (!strokeStyle)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->MiterLimit();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError StrokeStyle_SetLineDashStyle(GraphicObject strokeStyle, DashStyle lineDashStyle)
{
	GraphicError result = GraphicError::Failed;
	if (!strokeStyle)
		result = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->LineDashStyle(lineDashStyle);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<DashStyle, GraphicError> StrokeStyle_GetLineDashStyle(GraphicObject strokeStyle)
{
	Result<DashStyle, GraphicError> result = Result<DashStyle, GraphicError>(DashStyle::Solid, GraphicError::Failed);
	if (!strokeStyle)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->LineDashStyle();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError StrokeStyle_SetDashOffset(GraphicObject strokeStyle, float dashOffset)
{
	GraphicError result = GraphicError::Failed;
	if (!strokeStyle)
		result = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->DashOffset(dashOffset);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> StrokeStyle_GetDashOffset(GraphicObject strokeStyle)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(FP_NAN, GraphicError::Failed);
	if (!strokeStyle)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->DashOffset();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError StrokeStyle_SetDashes(GraphicObject strokeStyle, const float* dashes, unsigned int dashCount)
{
	GraphicError result = GraphicError::Failed;
	if (!strokeStyle)
		result = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Dashes(dashes,dashCount);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<const float*, unsigned int, GraphicError> StrokeStyle_GetDashes(GraphicObject strokeStyle)
{
	Result<const float*, unsigned int, GraphicError> result = Result<const float*, unsigned int, GraphicError>(NULL, 0, GraphicError::Failed);
	if (!strokeStyle)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StrokeStylePtr* lgbp = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
		Graphic::d2d1::StrokeStyle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value1 = lgb->Dashes(&result.Value2);
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}


GraphicError TextFormat_SetFontFamily(GraphicObject textFormat, const wchar_t* fontFamily)
{
	GraphicError result = GraphicError::Failed;
	if (!textFormat)
		result = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			lgb->FontFamily1(fontFamily);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<const wchar_t*, GraphicError> TextFormat_GetFontFamily(GraphicObject textFormat)
{
	Result<const wchar_t*, GraphicError> result = Result<const wchar_t*, GraphicError>(NULL, GraphicError::Failed);
	if (!textFormat)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->FontFamily1();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError TextFormat_SetFontSize(GraphicObject textFormat, float fontSize)
{
	GraphicError result = GraphicError::Failed;
	if (!textFormat)
		result = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			lgb->FontSize(fontSize);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> TextFormat_GetFontSize(GraphicObject textFormat)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(FP_NAN, GraphicError::Failed);
	if (!textFormat)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->FontSize();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError TextFormat_SetFontStyle(GraphicObject textFormat, FontStyle fontStyle)
{
	GraphicError result = GraphicError::Failed;
	if (!textFormat)
		result = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			lgb->FontStyle(fontStyle);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<FontStyle, GraphicError> TextFormat_GetFontStyle(GraphicObject textFormat)
{
	Result<FontStyle, GraphicError> result = Result<FontStyle, GraphicError>(FontStyle::Normal, GraphicError::Failed);
	if (!textFormat)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->FontStyle();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError TextFormat_SetFontStretch(GraphicObject textFormat, FontStretch fontStretch)
{
	GraphicError result = GraphicError::Failed;
	if (!textFormat)
		result = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			lgb->FontStretch(fontStretch);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<FontStretch, GraphicError> TextFormat_GetFontStretch(GraphicObject textFormat)
{
	Result<FontStretch, GraphicError> result = Result<FontStretch, GraphicError>(FontStretch::Normal, GraphicError::Failed);
	if (!textFormat)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->FontStretch();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError TextFormat_SetFontWeight(GraphicObject textFormat, FontWeight fontWeight)
{
	GraphicError result = GraphicError::Failed;
	if (!textFormat)
		result = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			lgb->FontWeight(fontWeight);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<FontWeight, GraphicError> TextFormat_GetFontWeight(GraphicObject textFormat)
{
	Result<FontWeight, GraphicError> result = Result<FontWeight, GraphicError>(FontWeight::Normal, GraphicError::Failed);
	if (!textFormat)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextFormatPtr* lgbp = reinterpret_cast<TextFormatPtr*>(textFormat);
		Graphic::d2d1::TextFormat* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->FontWeight();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}


GraphicError TextLayout_SetTextFormat(GraphicObject textLayout, GraphicObject textFormat)
{
	GraphicError result = GraphicError::Failed;
	if (!textLayout)
		result = GraphicError::InvalidParameters;
	else
	{
		TextLayoutPtr* lgbp = reinterpret_cast<TextLayoutPtr*>(textLayout);
		Graphic::d2d1::TextLayout* lgb = lgbp->get();
		if (lgb)
		{
			TextFormatPtr* tfp = reinterpret_cast<TextFormatPtr*>(textFormat);
			Graphic::d2d1::TextFormat* tf = tfp->get();
			if (tf)
			{
				lgb->FontFormat(*tfp);
				result = GraphicError::OK;
			}
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicObject, GraphicError> TextLayout_GetTextFormat(GraphicObject textLayout)
{
	Result<GraphicObject, GraphicError> result = Result<GraphicObject, GraphicError>(NULL, GraphicError::Failed);
	if (!textLayout)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextLayoutPtr* lgbp = reinterpret_cast<TextLayoutPtr*>(textLayout);
		Graphic::d2d1::TextLayout* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = (GraphicObject)&lgb->FontFormat();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError TextLayout_SetText(GraphicObject textLayout, const wchar_t* text)
{
	GraphicError result = GraphicError::Failed;
	if (!textLayout)
		result = GraphicError::InvalidParameters;
	else
	{
		TextLayoutPtr* lgbp = reinterpret_cast<TextLayoutPtr*>(textLayout);
		Graphic::d2d1::TextLayout* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Text1(text);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<const wchar_t*, GraphicError> TextLayout_GetText(GraphicObject textLayout)
{
	Result<const wchar_t*, GraphicError> result = Result<const wchar_t*, GraphicError>(NULL, GraphicError::Failed);
	if (!textLayout)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextLayoutPtr* lgbp = reinterpret_cast<TextLayoutPtr*>(textLayout);
		Graphic::d2d1::TextLayout* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Text1();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError TextLayout_SetMaxSize(GraphicObject textLayout, Size maxSize)
{
	GraphicError result = GraphicError::Failed;
	if (!textLayout)
		result = GraphicError::InvalidParameters;
	else
	{
		TextLayoutPtr* lgbp = reinterpret_cast<TextLayoutPtr*>(textLayout);
		Graphic::d2d1::TextLayout* lgb = lgbp->get();
		if (lgb)
		{
			lgb->MaxSize(maxSize);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Size, GraphicError> TextLayout_GetMaxSize(GraphicObject textLayout)
{
	Result<Size, GraphicError> result = Result<Size, GraphicError>(Size::Unknown(), GraphicError::Failed);
	if (!textLayout)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextLayoutPtr* lgbp = reinterpret_cast<TextLayoutPtr*>(textLayout);
		Graphic::d2d1::TextLayout* lgb = lgbp->get();
		if (lgb)
		{
			result.Value =lgb->MaxSize();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Rect, GraphicError> TextLayout_GetLayoutBounds(GraphicObject textLayout)
{
	Result<Rect, GraphicError> result = Result<Rect, GraphicError>(Rect::Unknown(), GraphicError::Failed);
	if (!textLayout)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextLayoutPtr* lgbp = reinterpret_cast<TextLayoutPtr*>(textLayout);
		Graphic::d2d1::TextLayout* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->GetLayoutBounds();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
Result<bool, GraphicError> TextLayout_HitTest(GraphicObject textLayout, const Point& point)
{
	Result<bool, GraphicError> result = Result<bool, GraphicError>(false, GraphicError::Failed);
	if (!textLayout)
		result.State = GraphicError::InvalidParameters;
	else
	{
		TextLayoutPtr* lgbp = reinterpret_cast<TextLayoutPtr*>(textLayout);
		Graphic::d2d1::TextLayout* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->HitTest(point);
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}


GraphicError EllipseGeometry_SetCenterPoint(GraphicObject ellipseGeometry, const Point& point)
{
	GraphicError result = GraphicError::Failed;
	if (!ellipseGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		EllipsePtr* lgbp = reinterpret_cast<EllipsePtr*>(ellipseGeometry);
		Graphic::d2d1::Ellipse* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Center(point);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Point, GraphicError> EllipseGeometry_GetCenterPoint(GraphicObject ellipseGeometry)
{
	Result<Point, GraphicError> result = Result<Point, GraphicError>(Point::Unknown(), GraphicError::Failed);
	if (!ellipseGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		EllipsePtr* lgbp = reinterpret_cast<EllipsePtr*>(ellipseGeometry);
		Graphic::d2d1::Ellipse* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Center();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError EllipseGeometry_SetRadiusX(GraphicObject ellipseGeometry, float radiusX)
{
	GraphicError result = GraphicError::Failed;
	if (!ellipseGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		EllipsePtr* lgbp = reinterpret_cast<EllipsePtr*>(ellipseGeometry);
		Graphic::d2d1::Ellipse* lgb = lgbp->get();
		if (lgb)
		{
			lgb->RadiusX(radiusX);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> EllipseGeometry_GetRadiusX(GraphicObject ellipseGeometry)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(FP_NAN, GraphicError::Failed);
	if (!ellipseGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		EllipsePtr* lgbp = reinterpret_cast<EllipsePtr*>(ellipseGeometry);
		Graphic::d2d1::Ellipse* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->RadiusX();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError EllipseGeometry_SetRadiusY(GraphicObject ellipseGeometry, float radiusY)
{
	GraphicError result = GraphicError::Failed;
	if (!ellipseGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		EllipsePtr* lgbp = reinterpret_cast<EllipsePtr*>(ellipseGeometry);
		Graphic::d2d1::Ellipse* lgb = lgbp->get();
		if (lgb)
		{
			lgb->RadiusY(radiusY);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> EllipseGeometry_GetRadiusY(GraphicObject ellipseGeometry)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(FP_NAN, GraphicError::Failed);
	if (!ellipseGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		EllipsePtr* lgbp = reinterpret_cast<EllipsePtr*>(ellipseGeometry);
		Graphic::d2d1::Ellipse* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->RadiusY();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError RectangleGeometry_SetRect(GraphicObject rectangleGeometry, const Rect& rect)
{
	GraphicError result = GraphicError::Failed;
	if (!rectangleGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		RectanglePtr* lgbp = reinterpret_cast<RectanglePtr*>(rectangleGeometry);
		Graphic::d2d1::Rectangle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Rect(rect);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Rect, GraphicError> RectangleGeometry_GetRect(GraphicObject rectangleGeometry)
{
	Result<Rect, GraphicError> result = Result<Rect, GraphicError>(Rect::Unknown(), GraphicError::Failed);
	if (!rectangleGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RectanglePtr* lgbp = reinterpret_cast<RectanglePtr*>(rectangleGeometry);
		Graphic::d2d1::Rectangle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Rect();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RoundedRectangleGeometry_SetRect(GraphicObject roundedRectangleGeometry, const Rect& rect)
{
	GraphicError result = GraphicError::Failed;
	if (!roundedRectangleGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		RoundedRectanglePtr* lgbp = reinterpret_cast<RoundedRectanglePtr*>(roundedRectangleGeometry);
		Graphic::d2d1::RoundedRectangle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Rect(rect);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Rect, GraphicError> RoundedRectangleGeometry_GetRect(GraphicObject roundedRectangleGeometry)
{
	Result<Rect, GraphicError> result = Result<Rect, GraphicError>(Rect::Unknown(), GraphicError::Failed);
	if (!roundedRectangleGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RoundedRectanglePtr* lgbp = reinterpret_cast<RoundedRectanglePtr*>(roundedRectangleGeometry);
		Graphic::d2d1::RoundedRectangle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Rect();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RoundedRectangleGeometry_SetRadiusX(GraphicObject roundedRectangleGeometry, float radiusX)
{
	GraphicError result = GraphicError::Failed;
	if (!roundedRectangleGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		RoundedRectanglePtr* lgbp = reinterpret_cast<RoundedRectanglePtr*>(roundedRectangleGeometry);
		Graphic::d2d1::RoundedRectangle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->RadiusX(radiusX);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> RoundedRectangleGeometry_GetRadiusX(GraphicObject roundedRectangleGeometry)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(FP_NAN, GraphicError::Failed);
	if (!roundedRectangleGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RoundedRectanglePtr* lgbp = reinterpret_cast<RoundedRectanglePtr*>(roundedRectangleGeometry);
		Graphic::d2d1::RoundedRectangle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->RadiusX();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError RoundedRectangleGeometry_SetRadiusY(GraphicObject roundedRectangleGeometry, float radiusY)
{
	GraphicError result = GraphicError::Failed;
	if (!roundedRectangleGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		RoundedRectanglePtr* lgbp = reinterpret_cast<RoundedRectanglePtr*>(roundedRectangleGeometry);
		Graphic::d2d1::RoundedRectangle* lgb = lgbp->get();
		if (lgb)
		{
			lgb->RadiusY(radiusY);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError> RoundedRectangleGeometry_GetRadiusY(GraphicObject roundedRectangleGeometry)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(FP_NAN, GraphicError::Failed);
	if (!roundedRectangleGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		RoundedRectanglePtr* lgbp = reinterpret_cast<RoundedRectanglePtr*>(roundedRectangleGeometry);
		Graphic::d2d1::RoundedRectangle* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->RadiusY();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError PathGeometry_SetFigures(GraphicObject pathGeometry, const System::UI::Graphic::PathFigurePtr* figures, unsigned int count)
{
	GraphicError result = GraphicError::Failed;
	if (!pathGeometry || !OSHelper::IsValidAddress(figures, sizeof(System::UI::Graphic::PathFigurePtr)*count, false))
		result = GraphicError::InvalidParameters;
	else
	{
		PathGeometryPtr* lgbp = reinterpret_cast<PathGeometryPtr*>(pathGeometry);
		Graphic::d2d1::PathGeometry* lgb = lgbp->get();
		if (lgb)
		{
			lgb->SetFigures(figures, count);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;

}

GraphicError PathGeometry_SetFillMode(GraphicObject pathGeometry, FillMode mode)
{
	GraphicError result = GraphicError::Failed;
	if (!pathGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		PathGeometryPtr* lgbp = reinterpret_cast<PathGeometryPtr*>(pathGeometry);
		Graphic::d2d1::PathGeometry* lgb = lgbp->get();
		if (lgb)
		{
			lgb->FillMode(mode);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<FillMode, GraphicError> PathGeometry_GetFillMode(GraphicObject pathGeometry)
{
	Result<FillMode, GraphicError> result = Result<FillMode, GraphicError>(FillMode::Alternate, GraphicError::Failed);
	if (!pathGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		PathGeometryPtr* lgbp = reinterpret_cast<PathGeometryPtr*>(pathGeometry);
		Graphic::d2d1::PathGeometry* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->FillMode();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

Result<bool, GraphicError>StreamGeometry_Open(GraphicObject streamGeometry)
{
	Result<bool, GraphicError> result = Result<bool, GraphicError>(false, GraphicError::Failed);
	if (!streamGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			auto res= lgb->Open();
			if (res.Value)
			{
				result.Value = true;
				result.State = GraphicError::OK;
			}
			else
			{
				result.WriteMessage(res.Message);
			}
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
Result<bool, GraphicError>StreamGeometry_GetIsOpened(GraphicObject streamGeometry)
{
	Result<bool, GraphicError> result = Result<bool, GraphicError>(false, GraphicError::Failed);
	if (!streamGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->GetIsOpened(); 
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError StreamGeometry_SetFillMode(GraphicObject streamGeometry, FillMode mode)
{
	GraphicError result = GraphicError::Failed;
	if (!streamGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			lgb->FillMode(mode);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<FillMode, GraphicError> StreamGeometry_GetFillMode(GraphicObject streamGeometry)
{
	Result<FillMode, GraphicError> result = Result<FillMode, GraphicError>(FillMode::Alternate, GraphicError::Failed);
	if (!streamGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->FillMode();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicError> StreamGeometry_BeginFigure(GraphicObject streamGeometry, const Point& start, bool filled, bool closed)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!streamGeometry)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			Result<bool> tmp=lgb->BeginFigure(start,filled, closed);
			if (tmp.Value)
			{
				result.Value = GraphicError::OK;
			}
			else
			{
				result.WriteMessage(tmp.Message);
			}
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicError> StreamGeometry_EndFigure(GraphicObject streamGeometry)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!streamGeometry)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			Result<bool> tmp = lgb->EndFigure();
			if (tmp.Value)
			{
				result.Value = GraphicError::OK;
			}
			else
			{
				result.WriteMessage(tmp.Message);
			}
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicError> StreamGeometry_LineTo(GraphicObject streamGeometry, const Point& point, bool isStroked, bool isSmoothJoin)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!streamGeometry)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			Result<bool> tmp = lgb->LineTo(point, isStroked, isSmoothJoin);
			if (tmp.Value)
			{
				result.Value = GraphicError::OK;
			}
			else
			{
				result.WriteMessage(tmp.Message);
			}
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicError> StreamGeometry_ArcTo(GraphicObject streamGeometry, const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked, bool isSmoothJoin)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!streamGeometry)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			Result<bool> tmp = lgb->ArcTo(point, size, rotation, sweepDirection, arcSize, isStroked, isSmoothJoin);
			if (tmp.Value)
			{
				result.Value = GraphicError::OK;
			}
			else
			{
				result.WriteMessage(tmp.Message);
			}
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicError> StreamGeometry_BezierTo(GraphicObject streamGeometry, const Point& p1, const Point& p2, const Point& p3, bool isStroked, bool isSmoothJoin)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!streamGeometry)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			Result<bool> tmp = lgb->BezierTo(p1, p2, p3, isStroked, isSmoothJoin);
			if (tmp.Value)
			{
				result.Value = GraphicError::OK;
			}
			else
			{
				result.WriteMessage(tmp.Message);
			}
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}
Result<GraphicError> StreamGeometry_QuadraticBezierTo(GraphicObject streamGeometry, const Point& point1, const Point& point2, bool isStroked, bool isSmoothJoin)
{
	Result<GraphicError> result = Result<GraphicError>(GraphicError::Failed);
	if (!streamGeometry)
		result.Value = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			Result<bool> tmp = lgb->QuadraticBezierTo(point1, point2, isStroked, isSmoothJoin);
			if (tmp.Value)
			{
				result.Value = GraphicError::OK;
			}
			else
			{
				result.WriteMessage(tmp.Message);
			}
		}
		else
			result.Value = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError StreamGeometry_Clear(GraphicObject streamGeometry)
{
	GraphicError result = GraphicError::Failed;
	if (!streamGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Clear();
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<wstring, GraphicError> StreamGeometry_Decompose(GraphicObject streamGeometry)
{
	Result<wstring, GraphicError> result = Result<wstring, GraphicError>(wstring(), GraphicError::Failed);
	if (streamGeometry)
	{
		StreamGeometryPtr* lgbp = reinterpret_cast<StreamGeometryPtr*>(streamGeometry);
		Graphic::d2d1::StreamGeometry* lgb = lgbp->get();
		if (lgb)
		{
			Result<bool, wstring> tmp = lgb->Decompose();
			if (tmp.Value)
			{
				result.Value = tmp.State;
				result.State = GraphicError::OK;
			}
			else
			{
				result.WriteMessage(tmp.Message);
			}
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError CombinedGeometry_SetCombineMode(GraphicObject combinedGeometry, CombineMode mode)
{
	GraphicError result = GraphicError::Failed;
	if (!combinedGeometry)
		result = GraphicError::InvalidParameters;
	else
	{
		CombinedGeometryPtr* lgbp = reinterpret_cast<CombinedGeometryPtr*>(combinedGeometry);
		Graphic::d2d1::CombinedGeometry* lgb = lgbp->get();
		if (lgb)
		{
			lgb->CombineMode(mode);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<CombineMode, GraphicError> CombinedGeometry_GetCombineMode(GraphicObject combinedGeometry)
{
	Result<CombineMode, GraphicError> result = Result<CombineMode, GraphicError>(CombineMode::Union, GraphicError::Failed);
	if (!combinedGeometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		CombinedGeometryPtr* lgbp = reinterpret_cast<CombinedGeometryPtr*>(combinedGeometry);
		Graphic::d2d1::CombinedGeometry* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->CombineMode();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}


GraphicError GeometryGroup_SetFillMode(GraphicObject geometryGroup, FillMode mode)
{
	GraphicError result = GraphicError::Failed;
	if (!geometryGroup)
		result = GraphicError::InvalidParameters;
	else
	{
		GeometryGroupPtr* lgbp = reinterpret_cast<GeometryGroupPtr*>(geometryGroup);
		Graphic::d2d1::GeometryGroup* lgb = lgbp->get();
		if (lgb)
		{
			lgb->FillMode(mode);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<FillMode, GraphicError> GeometryGroup_GetFillMode(GraphicObject geometryGroup)
{
	Result<FillMode, GraphicError> result = Result<FillMode, GraphicError>(FillMode::Alternate, GraphicError::Failed);
	if (!geometryGroup)
		result.State = GraphicError::InvalidParameters;
	else
	{
		GeometryGroupPtr* lgbp = reinterpret_cast<GeometryGroupPtr*>(geometryGroup);
		Graphic::d2d1::GeometryGroup* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->FillMode();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError GeometryGroup_SetGeometries(GraphicObject geometryGroup, GraphicObject* geometries, unsigned int count)
{
	GraphicError result = GraphicError::Failed;
	if (!geometryGroup)
		result = GraphicError::InvalidParameters;
	else
	{
		GeometryGroupPtr* lgbp = reinterpret_cast<GeometryGroupPtr*>(geometryGroup);
		Graphic::d2d1::GeometryGroup* lgb = lgbp->get();
		if (lgb)
		{
			if (geometries)
			{
				vector<intptr_t> tmp(count);
				for (unsigned int i = 0; i < count; i++) 
					tmp.push_back(geometries[i]);
				lgb->Geometries.reset(tmp);
				result = GraphicError::OK;
			}
			else
			{
				if (count == 0)
				{
					lgb->Geometries.clear();
				}
				else
					result = GraphicError::InvalidParameters;
			}
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}

Result<bool, GraphicError> Geometry_HitTest(GraphicObject geometry, const Point& point, const Matrix& matrix)
{
	Result<bool, GraphicError> result = Result<bool, GraphicError>(false, GraphicError::Failed);
	if (!geometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		GeometryPtr* lgbp = reinterpret_cast<GeometryPtr*>(geometry);
		Graphic::d2d1::Geometry* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->HitTest(point,matrix);
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
Result<bool, GraphicError> Geometry_HitTestStroke(GraphicObject geometry, const Point& point, GraphicObject strokeStyle, float thickness, const Matrix& matrix)
{
	Result<bool, GraphicError> result = Result<bool, GraphicError>(false, GraphicError::Failed);
	if (!geometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		GeometryPtr* lgbp = reinterpret_cast<GeometryPtr*>(geometry);
		Graphic::d2d1::Geometry* lgb = lgbp->get();
		if (lgb)
		{
			StrokeStylePtr* ss = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
			result.Value = lgb->HitTestStroke(point, *ss, thickness, matrix);
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Rect, GraphicError> Geometry_GetBounds(GraphicObject geometry, GraphicObject strokeStyle, float thickness, const Matrix& worldTransform)
{
	Result<Rect, GraphicError> result = Result<Rect, GraphicError>(Rect::Unknown(), GraphicError::Failed);
	if (!geometry)
		result.State = GraphicError::InvalidParameters;
	else
	{
		GeometryPtr* lgbp = reinterpret_cast<GeometryPtr*>(geometry);
		Graphic::d2d1::Geometry* lgb = lgbp->get();
		if (lgb)
		{
			StrokeStylePtr* ss = reinterpret_cast<StrokeStylePtr*>(strokeStyle);
			result.Value = lgb->GetBounds(*ss, thickness,worldTransform);
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError Geometry_PushTransform(GraphicObject geometry, const TransformPtr& transform)
{
	GraphicError result = GraphicError::Failed;
	if (!geometry)
		result = GraphicError::InvalidParameters;
	else
	{
		GeometryPtr* lgbp = reinterpret_cast<GeometryPtr*>(geometry);
		Graphic::d2d1::Geometry* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Transform(transform);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError BlurEffect_SetBlurSize(GraphicObject blurEffect, float blurSize)
{
	GraphicError result = GraphicError::Failed;
	if (!blurEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		BlurEffectPtr* lgbp = reinterpret_cast<BlurEffectPtr*>(blurEffect);
		Graphic::d2d1::BlurEffect* lgb = lgbp->get();
		if (lgb)		
		{
			lgb->BlurSize(blurSize);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError>BlurEffect_GetBlurSize(GraphicObject blurEffect)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(3.0f, GraphicError::Failed);
	if (!blurEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BlurEffectPtr* lgbp = reinterpret_cast<BlurEffectPtr*>(blurEffect);
		Graphic::d2d1::BlurEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->BlurSize();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError BlurEffect_SetOptimization(GraphicObject blurEffect, BlurOptimization optimization)
{
	GraphicError result = GraphicError::Failed;
	if (!blurEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		BlurEffectPtr* lgbp = reinterpret_cast<BlurEffectPtr*>(blurEffect);
		Graphic::d2d1::BlurEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Optimization(optimization);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<BlurOptimization, GraphicError>BlurEffect_GetOptimization(GraphicObject blurEffect)
{
	Result<BlurOptimization, GraphicError> result = Result<BlurOptimization, GraphicError>(BlurOptimization::Balanced, GraphicError::Failed);
	if (!blurEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BlurEffectPtr* lgbp = reinterpret_cast<BlurEffectPtr*>(blurEffect);
		Graphic::d2d1::BlurEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Optimization();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError BlurEffect_SetBorderMode(GraphicObject blurEffect, BlurBorderMode borderMode)
{
	GraphicError result = GraphicError::Failed;
	if (!blurEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		BlurEffectPtr* lgbp = reinterpret_cast<BlurEffectPtr*>(blurEffect);
		Graphic::d2d1::BlurEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->BorderMode(borderMode);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<BlurBorderMode, GraphicError>BlurEffect_GetBorderMode(GraphicObject blurEffect)
{
	Result<BlurBorderMode, GraphicError> result = Result<BlurBorderMode, GraphicError>(BlurBorderMode::Soft, GraphicError::Failed);
	if (!blurEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		BlurEffectPtr* lgbp = reinterpret_cast<BlurEffectPtr*>(blurEffect);
		Graphic::d2d1::BlurEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->BorderMode();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError DropShadowEffect_SetBlurSize(GraphicObject dropShadowEffect, float blurSize)
{
	GraphicError result = GraphicError::Failed;
	if (!dropShadowEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->BlurSize(blurSize);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError>DropShadowEffect_GetBlurSize(GraphicObject dropShadowEffect)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(3.0f, GraphicError::Failed);
	if (!dropShadowEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->BlurSize();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError DropShadowEffect_SetOptimization(GraphicObject dropShadowEffect, BlurOptimization optimization)
{
	GraphicError result = GraphicError::Failed;
	if (!dropShadowEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Optimization(optimization);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<BlurOptimization,GraphicError> DropShadowEffect_GetOptimization(GraphicObject dropShadowEffect)
{
	Result<BlurOptimization, GraphicError> result = Result<BlurOptimization, GraphicError>(BlurOptimization::Balanced, GraphicError::Failed);
	if (!dropShadowEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Optimization();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError DropShadowEffect_SetColor(GraphicObject dropShadowEffect, const Color& color)
{
	GraphicError result = GraphicError::Failed;
	if (!dropShadowEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->ShadowColor(color);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<Color, GraphicError>DropShadowEffect_GetColor(GraphicObject dropShadowEffect)
{
	Result<Color, GraphicError> result = Result<Color, GraphicError>(Color(), GraphicError::Failed);
	if (!dropShadowEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->ShadowColor();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError DropShadowEffect_SetEnableTransform(GraphicObject dropShadowEffect, bool enable)
{
	GraphicError result = GraphicError::Failed;
	if (!dropShadowEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->EnableTransform(enable);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<bool, GraphicError>DropShadowEffect_GetEnableTransform(GraphicObject dropShadowEffect)
{
	Result<bool, GraphicError> result = Result<bool, GraphicError>(false, GraphicError::Failed);
	if (!dropShadowEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->EnableTransform();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError DropShadowEffect_SetShadowTransform(GraphicObject dropShadowEffect, const TransformPtr& transform)
{
	GraphicError result = GraphicError::Failed;
	if (!dropShadowEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->ShadowTransform(transform);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<TransformPtr, GraphicError>DropShadowEffect_GetShadowTransform(GraphicObject dropShadowEffect)
{
	Result<TransformPtr, GraphicError> result = Result<TransformPtr, GraphicError>(NULL, GraphicError::Failed);
	if (!dropShadowEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->ShadowTransform();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError DropShadowEffect_SetDirection(GraphicObject dropShadowEffect, float direction)
{
	GraphicError result = GraphicError::Failed;
	if (!dropShadowEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->Direction(direction);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError>DropShadowEffect_GetDirection(GraphicObject dropShadowEffect)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(315.0f, GraphicError::Failed);
	if (!dropShadowEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->Direction();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}
GraphicError DropShadowEffect_SetShadowDepth(GraphicObject dropShadowEffect, float shadowDepth)
{
	GraphicError result = GraphicError::Failed;
	if (!dropShadowEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->ShadowDepth(shadowDepth);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError>DropShadowEffect_GetShadowDepth(GraphicObject dropShadowEffect)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(3.0f, GraphicError::Failed);
	if (!dropShadowEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		DropShadowEffectPtr* lgbp = reinterpret_cast<DropShadowEffectPtr*>(dropShadowEffect);
		Graphic::d2d1::DropShadowEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->ShadowDepth();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}

GraphicError SaturationEffect_SetAmount(GraphicObject saturationEffect, float amount)
{
	GraphicError result = GraphicError::Failed;
	if (!saturationEffect)
		result = GraphicError::InvalidParameters;
	else
	{
		SaturationEffectPtr* lgbp = reinterpret_cast<SaturationEffectPtr*>(saturationEffect);
		Graphic::d2d1::SaturationEffect* lgb = lgbp->get();
		if (lgb)
		{
			lgb->SaturationAmount(amount);
			result = GraphicError::OK;
		}
		else
			result = GraphicError::InvalidParameters;
	}
	return result;
}
Result<float, GraphicError>SaturationEffect_GetAmount(GraphicObject saturationEffect)
{
	Result<float, GraphicError> result = Result<float, GraphicError>(3.0f, GraphicError::Failed);
	if (!saturationEffect)
		result.State = GraphicError::InvalidParameters;
	else
	{
		SaturationEffectPtr* lgbp = reinterpret_cast<SaturationEffectPtr*>(saturationEffect);
		Graphic::d2d1::SaturationEffect* lgb = lgbp->get();
		if (lgb)
		{
			result.Value = lgb->SaturationAmount();
			result.State = GraphicError::OK;
		}
		else
			result.State = GraphicError::InvalidParameters;
	}
	return result;
}



GraphicError DeleteObject(GraphicObjectTypes type, GraphicObject obj)
{
	GraphicError result = GraphicError::Failed;
	try
	{
		switch (type)
		{
		/*case GraphicObjectTypes::Surface:
			{
				Microsoft::WRL::ComPtr<IDXGISurface>* suf = reinterpret_cast<Microsoft::WRL::ComPtr<IDXGISurface>*>(obj);
				SAFE_DELETE(suf);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::Texture2D:
			{
				Microsoft::WRL::ComPtr<ID3D11Texture2D>* tex = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Texture2D>*>(obj);
				SAFE_DELETE(tex);
				result = GraphicError::OK;
			}
			break;*/
		case GraphicObjectTypes::Bitmap:
			{
				BitmapPtr* bmp = reinterpret_cast<BitmapPtr*>(obj);
				SAFE_DELETE(bmp);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::BitmapBrush:
			{
				BitmapBrushPtr* bmpBrush = reinterpret_cast<BitmapBrushPtr*>(obj);
				SAFE_DELETE(bmpBrush);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::CombinedGeometry:
			{
				CombinedGeometryPtr* combinedGeometry = reinterpret_cast<CombinedGeometryPtr*>(obj);
				SAFE_DELETE(combinedGeometry);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::Device:
			{
				Microsoft::WRL::ComPtr<ID3D11Device>* device = reinterpret_cast<Microsoft::WRL::ComPtr<ID3D11Device>*>(obj);
				SAFE_DELETE(device);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::Effect:
			{
				ElementEffectPtr* elementEffect = reinterpret_cast<ElementEffectPtr*>(obj);
				SAFE_DELETE(elementEffect);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::EllipseGeometry:
			{
				EllipsePtr* ellipse = reinterpret_cast<EllipsePtr*>(obj);
				SAFE_DELETE(ellipse);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::GeometryGroup:
			{
				GeometryGroupPtr* geometryGroup = reinterpret_cast<GeometryGroupPtr*>(obj);
				SAFE_DELETE(geometryGroup);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::GradientStopCollection:
			{
				GradientStopCollectionPtr* gradientStopCollection = reinterpret_cast<GradientStopCollectionPtr*>(obj);
				SAFE_DELETE(gradientStopCollection);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::LinearGradientBrush:
			{
				LinearGradientBrushPtr* linearGradientBrush = reinterpret_cast<LinearGradientBrushPtr*>(obj);
				SAFE_DELETE(linearGradientBrush);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::PathGeometry:
			{
				PathGeometryPtr* pathGeometry = reinterpret_cast<PathGeometryPtr*>(obj);
				SAFE_DELETE(pathGeometry);
				result = GraphicError::OK;
			}
			break;		
		/*case GraphicObjectTypes::StreamGeometry:
			{
				StreamGeometryPtr* streamGeometry = reinterpret_cast<StreamGeometryPtr*>(obj);
				SAFE_DELETE(streamGeometry);
				result = GraphicError::OK;
			}
				break;*/
		case GraphicObjectTypes::RadialGradientBrush:
			{
				RadialGradientBrushPtr* radialGradientBrush = reinterpret_cast<RadialGradientBrushPtr*>(obj);
				SAFE_DELETE(radialGradientBrush);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::RectangleGeometry:
			{
				RectanglePtr* rectangle = reinterpret_cast<RectanglePtr*>(obj);
				SAFE_DELETE(rectangle);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::RenderContext:
			{
				RenderContextPtr* renderContext = reinterpret_cast<RenderContextPtr*>(obj);
				SAFE_DELETE(renderContext);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::RoundedRectangleGeometry:
			{
				RoundedRectanglePtr* roundedRectangleGeometry = reinterpret_cast<RoundedRectanglePtr*>(obj);
				SAFE_DELETE(roundedRectangleGeometry);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::SolidColorBrush:
			{
				SolidColorBrushPtr* solidColorBrush = reinterpret_cast<SolidColorBrushPtr*>(obj);
				SAFE_DELETE(solidColorBrush);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::StrokeStyle:
			{
				StrokeStylePtr* strokeStyle = reinterpret_cast<StrokeStylePtr*>(obj);
				SAFE_DELETE(strokeStyle);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::TextFormat:
			{
				TextFormatPtr* textFormat = reinterpret_cast<TextFormatPtr*>(obj);
				SAFE_DELETE(textFormat);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::TextLayout:
			{
				TextLayoutPtr* textLayout  = reinterpret_cast<TextLayoutPtr*>(obj);
				SAFE_DELETE(textLayout);
				result = GraphicError::OK;
			}
			break;
		case GraphicObjectTypes::TransformedGeometry:
			{
				TransformedGeometryPtr* transformedGeometry = reinterpret_cast<TransformedGeometryPtr*>(obj);
				SAFE_DELETE(transformedGeometry);
				result = GraphicError::OK;
			}
			break;
		default:
			result = GraphicError::NotImplemented;
			break;
		}
	}
	catch (...)
	{
		result = GraphicError::CannotReleaseResource;
	}
	return result;
}


