#pragma once
#include "..\UICore_Config.h"
#include "..\GradientStop.h"
#include "Resource.h"
#include <memory>
#include <queue>
__uimport("Media\\Bitmap.h")
__uimport("Brush\\Brush.h")
__uimport("Brush\\StrokeStyle.h")
__uimport("Effect\\Effect.h")
__uimport("Geometry\\Geometry.h")
__uimport("Text\\TextLayout.h")

using namespace System;
using namespace System::UI::Media;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API GraphicFactoryError : __uobject
			{
				__uvalue_begin(GraphicFactoryError, UInt)
				__uvalue(OK)
				__uvalue(Failed)
				__uvalue(FeatureNotSupported)
				__uvalue(OSNotSupported)
				__uvalue(NotAvailable)
				__uvalue(InvalidParameters)
				__uvalue_end(GraphicFactoryError, OK)
			};

			class Surface;
			typedef std::shared_ptr<Surface> SurfacePtr;

			class Texture2D;
			typedef std::shared_ptr<Texture2D> Texture2DPtr;

			class Bitmap;
			typedef std::shared_ptr<Bitmap> BitmapPtr;

			class Brush;
			typedef std::shared_ptr<Brush> BrushPtr;

			class StrokeStyle;
			typedef std::shared_ptr<StrokeStyle> StrokeStylePtr;

			class Geometry;
			typedef std::shared_ptr<Geometry> GeometryPtr;

			class TextLayout;
			typedef std::shared_ptr<TextLayout> TextLayoutPtr;

			class Effect;
			typedef std::shared_ptr<Effect> EffectPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API RenderContext : __uobject, public FeatureObject
			{
			private:
				friend GraphicFactory;
				GraphicFactory* owner;
				RenderContext();
				RenderContext(GraphicObject renderContext, FeatureLevel level);
				RenderContext(const RenderContext& other);
			public:
				__uref GraphicFactory* GetGraphicFactory() const;
				GraphicError PushSurfaceTarget(const SurfacePtr& surface);
				Result<GraphicObject, GraphicError> GetCurrentTarget();
				Result<Size, GraphicError> GetCurrentTargetSize();
				__uref GraphicError PushTarget(const BitmapPtr& bitmap);
				__uref GraphicError PopTarget();
				__uref GraphicError Clear(const Color& color);
				__uref Size GetMaximumImageSize();
				__uref GraphicError DrawGeometry(const GeometryPtr& geometry, const BrushPtr& brush, const BrushPtr& strokeBrush, const StrokeStylePtr& strokeStyle, double thickness);
				__uref GraphicError PushTransform(const Matrix& matrix);
				__uref GraphicError PopTransform();
				__uref GraphicError PushAxisAlignedClip(const Rect& rect, AntialiasMode mode);
				__uref GraphicError PopAxisAlignedClip();
				__uref GraphicError PushGeometryClip(const GeometryPtr& geometry);
				__uref GraphicError PopGeometryClip();
				__uref GraphicError PushOpacity(float opacity);
				__uref GraphicError PopOpacity();
				__uref GraphicError PushOpacityMask(const Rect& rect, const BrushPtr& brush);
				__uref GraphicError PopOpacityMask();
				__uref GraphicError DrawTextLayout(const TextLayoutPtr& textLayout, const BrushPtr& brush, const Point& origin);
				__uref GraphicError DrawBitmap(const BitmapPtr& bitmap, const EffectCompositeMode compositeMode, const Size& size);
				__uref GraphicError DrawEffect(const BitmapPtr& bitmap, const EffectPtr& effect, const EffectCompositeMode compositeMode, const Size& size);

				REFLECT_CLASS(RenderContext)
			};



			class Feature;
			typedef std::shared_ptr<Feature> FeaturePtr;
			class Feature
			{
			private:
				Feature();
				~Feature();
			protected:
				friend RenderContext;
				friend GraphicFactory;
				FeatureLevel featureLevel;
			};


			class FeatureManager8;

			class GraphicFactory;
			typedef std::shared_ptr<GraphicFactory> GraphicFactoryPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API GraphicFactory : __uobject
			{
			private:
				FeatureManager8* Mgr8;
				FeatureObjectPtr Dev8;
				OSVersion OS;
			protected:
				friend RenderContext;
				friend Bitmap;
				friend BitmapSource;
				friend WriteableBitmap;
				friend Surface;
				friend Texture2D;
				friend FeatureOwner;

				friend StrokeStyle;
				friend GradientStopCollection;

				friend Brush;
				friend BitmapBrush;
				friend SolidColorBrush;
				friend LinearGradientBrush;
				friend RadialGradientBrush;

				friend TextFormat;
				friend TextLayout;

				friend Effect;
				friend BlurEffect;
				friend DropShadowEffect;
				friend SaturationEffect;

				friend Geometry;
				friend EllipseGeometry;
				friend RectangleGeometry;
				friend RoundedRectangleGeometry;
				friend PathGeometry;
				friend CombinedGeometry;
				friend GeometryGroup;

				FeatureLevel featureLevel;
			private:
				GraphicFactory(FeatureLevel level);
			public:
				static Result<GraphicFactory*, GraphicFactoryError> CreateGraphicFactory(FeatureLevel level);
				~GraphicFactory();
				Result<RenderContext*, GraphicFactoryError> CreateRenderContext();

			protected:
				Result<bool, GraphicFactoryError> InternalDeleteObject(GraphicObjectTypes type, GraphicObject obj, FeatureLevel level);
				Result<GraphicObject, GraphicFactoryError> InternalCreateBitmap(const Size& size);
				Result<GraphicObject, GraphicFactoryError> InternalCreateBitmap(const wchar_t* uri);
				Result<GraphicObject, GraphicFactoryError> InternalCreateTexture2D(const Size& size, Texture2DMode mode2D);
				Result<GraphicObject, GraphicFactoryError> InternalCreateStrokeStyle(CapStyle startCap, CapStyle endCap, CapStyle dashCap, LineJoin lineJoinStyle, double miterLimit, DashStyle lineDashStyle, double dashOffset, const float* dashes, unsigned int dashCount);
				Result<GraphicObject, GraphicFactoryError> InternalCreateBitmapBrush(GraphicObject bitmap, ExtendMode modeX, ExtendMode modeY, const Rect& sourceRect, InterpolationMode interpolation);
				Result<GraphicObject, GraphicFactoryError> InternalCreateGradientStopCollection(ExtendMode mode, const ObservableCollection<GradientStop>& gradientStops);
				Result<GraphicObject, GraphicFactoryError> InternalCreateSolidColorBrush(Color color);
				Result<GraphicObject, GraphicFactoryError> InternalCreateLinearGradientBrush(GraphicObject gradientStopCollection, const Point& start, const Point& end);
				Result<GraphicObject, GraphicFactoryError> InternalCreateRadialGradientBrush(GraphicObject gradientStopCollection, const Point& center, const Point& gradientOriginOffset, double radiusX, double radiusY);
				Result<GraphicObject, GraphicFactoryError> InternalCreateTextFormat(const wchar_t* fontFamily, double fontSize, FontWeight fontWeight, FontStyle fontStyle, FontStretch fontStretch);
				Result<GraphicObject, GraphicFactoryError> InternalCreateTextLayout(GraphicObject textFormat, const Size& maxSize, const wchar_t* text);
				Result<GraphicObject, GraphicFactoryError> InternalCreateBlurEffect(double blurSize, BlurOptimization optimization, BlurBorderMode borderMode);
				Result<GraphicObject, GraphicFactoryError> InternalCreateDropShadowEffect(double blurSize, BlurOptimization optimization, const Color& color, const TransformPtr& transform, bool enableTransform, double direction, double shadowDepth);
				Result<GraphicObject, GraphicFactoryError> InternalCreateSaturationEffect(double amount);

				Result<GraphicObject, GraphicFactoryError> InternalCreateEllipseGeometry(const Point& center, double radiusX, double radiusY);
				Result<GraphicObject, GraphicFactoryError> InternalCreateRectangleGeometry(const Rect& rect);
				Result<GraphicObject, GraphicFactoryError> InternalCreateRoundedRectangleGeometry(const Rect& rect, double radiusX, double radiusY);
				Result<GraphicObject, GraphicFactoryError> InternalCreateCombinedGeometry(GraphicObject geometry1, GraphicObject geometry2, CombineMode mode);
				Result<GraphicObject, GraphicFactoryError> InternalCreateGeometryGroup(GraphicObject* geometries, unsigned int count, FillMode mode);
				Result<GraphicObject, GraphicFactoryError> InternalCreatePathGeometry(FillMode mode);


				REFLECT_CLASS(GraphicFactory)
			};
		}
	}
}