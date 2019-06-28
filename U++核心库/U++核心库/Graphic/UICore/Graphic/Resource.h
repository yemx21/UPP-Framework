#pragma once

#include "..\UICore_Config.h"
#include "Graphic.h"
#include "..\DependencyObject.h"
#include "..\DependantObject.h"

#pragma warning( disable : 4275 )

#include <functional>
#include "Feature\Feature.h"
#include "Events.h"
namespace System
{
	namespace UI
	{
		class Application;

		namespace Graphic
		{
			class GraphicFactory;
			class Feature;

			class FeatureDependantResource;
			typedef std::shared_ptr<FeatureDependantResource> FeatureDependantResourcePtr;

			class FeatureIndependantResource;
			typedef std::shared_ptr<FeatureIndependantResource> FeatureIndependantResourcePtr;

			class RenderContext;
			class FeatureObject;
			typedef std::shared_ptr<FeatureObject> FeatureObjectPtr;
			class BitmapSource;
			class WriteableBitmap;
			class Surface;
			class Texture2D;


			class StrokeStyle;
			class GradientStopCollection;

			class Brush;
			class BitmapBrush;
			class SolidColorBrush;
			class LinearGradientBrush;
			class RadialGradientBrush;

			class TextFormat;
			class TextLayout;

			class Effect;
			class BlurEffect;
			class DropShadowEffect;
			class SaturationEffect;

			class Geometry;
			class EllipseGeometry;
			class RectangleGeometry;
			class RoundedRectangleGeometry;
			class PathGeometry;
			class CombinedGeometry;
			class GeometryGroup;

			class Bitmap;
			class BitmapSource;
			class WriteableBitmap;

			class FeatureOwner
			{
				friend FeatureDependantResource;
				friend FeatureIndependantResource;
			public:
				FeatureOwner();
				bool DeleteObject(GraphicObjectTypes type, GraphicObject obj, FeatureLevel level);
			protected:
				GraphicFactory* GraphicFactory_;
				void Register(GraphicFactory* GraphicFactory);
				bool HasOwner();
			};

			class FeatureObject
			{
			private:
				friend FeatureDependantResource;
				friend FeatureIndependantResource;
				friend RenderContext;
				friend Bitmap;
				friend BitmapSource;
				friend WriteableBitmap;
				friend Surface;
				friend Texture2D;
				friend GraphicFactory;

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

				FeatureObject(const FeatureObject& other);

			public:				
				FeatureObject();
				FeatureObject(GraphicObjectTypes type, GraphicObject obj, FeatureLevel level);

				~FeatureObject();

			protected:
				FeatureOwner owner_;
				GraphicObjectTypes type_;
				GraphicObject ptr_;
				FeatureLevel level_;

				virtual void InternalRelease();

				FeatureObject& operator=(decltype(__nullptr));

				FeatureObject& operator=(const FeatureObject &other);

				void Swap(_Inout_ FeatureObject& r);

				GraphicObject Get() const;

				GraphicObjectTypes GetType()const;

				FeatureLevel GetLevel()const;

				void Reset();
			};

			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0)  UICORE_API FeatureDependantResource : public UIObject, public IObservableObject <FeatureDependantResource>
			{
			public:
				FeatureDependantResource();
			protected:
				virtual bool IsResourceValid() const;

				static bool IsResourceValidFrom(const FeatureDependantResourcePtr& resource);

				static bool IsResourceValidFrom(FeatureDependantResource* const resource);

				void InvalidateResource();

				void Invalidate(InvalidateFlags flags);

				virtual bool OnNotifyChanged(const wstring& notifyname)
				{
					return false;
				}

				virtual FeatureObjectPtr OnCreateResource(RenderContext* renderContext) = 0;

				FeatureObjectPtr GetResource(RenderContext* renderContext);

				FeatureObjectPtr GetResourceFrom(const FeatureDependantResourcePtr& featureDependantResource, RenderContext* renderContext);

				FeatureObjectPtr GetResourceFrom(const FeatureDependantResourcePtr& featureDependantResource);

				virtual void Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args) override;

				GraphicFactory* GetGraphicFactoryFromApplication(System::UI::Application* app);

				void ForceResetResource(FeatureObjectPtr resource, GraphicFactory* factory);

			private:
				void SetRenderContext(RenderContext* renderContext);

				FeatureObjectPtr    m_lastResource;
				bool                  m_isDirty;
				RenderContext*   m_renderContext;
				friend GraphicFactory;
				friend RenderContext;
				friend Brush;
				friend BitmapBrush;
				friend GradientStopCollection;
				friend SolidColorBrush;
				friend LinearGradientBrush;
				friend RadialGradientBrush;
				friend Surface;
				friend Texture2D;

				REFLECT_CLASS(FeatureDependantResource)
			};


			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0)  UICORE_API FeatureIndependantResource : public UIObject, public IObservableObject <FeatureIndependantResource>
			{
			public:
				FeatureIndependantResource();
			protected:

				virtual bool IsResourceValid() const;

				static bool IsResourceValidFrom(const FeatureIndependantResourcePtr& resource);

				static bool IsResourceValidFrom(FeatureIndependantResource* const resource);

				void InvalidateResource();

				void Invalidate(InvalidateFlags flags);

				virtual FeatureObjectPtr OnCreateResource(GraphicFactory* GraphicFactory) = 0;

				FeatureObjectPtr GetResource(GraphicFactory* GraphicFactory);

				FeatureObjectPtr GetResourceFrom(const FeatureIndependantResourcePtr& featureIndependantResource, GraphicFactory* GraphicFactory);

				FeatureObjectPtr GetResourceFrom(const FeatureIndependantResourcePtr& featureIndependantResource);

				virtual void Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args) override;

				GraphicFactory* GetGraphicFactoryFromApplication(System::UI::Application* app);

				void ForceResetResource(FeatureObjectPtr resource, GraphicFactory* factory);

			private:
				void SetGraphicFactory(GraphicFactory* GraphicFactory);

				FeatureObjectPtr    m_lastResource;
				bool                  m_isDirty;
				GraphicFactory*             m_GraphicFactory;
				friend GraphicFactory;
				friend RenderContext;
				friend Geometry;
				friend EllipseGeometry;
				friend RectangleGeometry;
				friend RoundedRectangleGeometry;
				friend PathGeometry;
				friend CombinedGeometry;
				friend GeometryGroup;

				friend StrokeStyle;
				friend TextFormat;
				friend TextLayout;

				friend Effect;
				friend BlurEffect;
				friend DropShadowEffect;
				friend SaturationEffect;

				friend Brush;
				REFLECT_CLASS(FeatureIndependantResource)
			};
		}
	}
}