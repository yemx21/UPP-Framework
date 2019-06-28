#pragma once
#include "..\UICore_Config.h"
#include "..\DependencyObject.h"
#include "..\DependencyProperty.h"
#include "..\Transform.h"
#include "..\Graphic\Graphic.h"
#include "..\Graphic\Graphics.h"
#include "Input\Inputs.h"
using namespace System;
using namespace System::UI::Media;
using namespace System::UI::Graphic; 
using namespace System::UI::Windows::Input;
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class RenderContext;
			typedef std::shared_ptr<RenderContext> RenderContextPtr;
		}

		namespace Windows
		{
			class HwndHost;
			class VisualTreeHelper;

			class Visual;
			typedef std::shared_ptr<Visual> VisualPtr;

			class UIElement;
			typedef std::shared_ptr<UIElement> UIElementPtr;

			class VisualRoot;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Windows, ver = 1.0) UICORE_API Visual : public DependencyObject, std::enable_shared_from_this<Visual>
			{
				friend HwndHost;
				friend VisualRoot;
				friend VisualTreeHelper;
			public:
				Visual();
				virtual ~Visual();
			protected:
				virtual void Invalidate(InvalidateFlags flags) final;
			public:
				__uref VisualPtr GetParent() const;
			protected:
				void AddVisualChild(const VisualPtr& visual);
				void RemoveVisualChild(const VisualPtr& visual);
				VisualPtr GetVisualChild(int index) const;
				int GetVisualChildCount() const;
				void RemoveAllChildren();
			protected:
				virtual void OnLoaded();
				virtual void OnUnloaded();
			private:
				void InvokeUnloaded();
				void InvokeLoaded();
			public:
				__uref virtual bool HitTest(const Point& point);

				__uref void HitTestTree(const Point&point, VisualPtr& hitElement);

				__uref Point PointToScreen(const Point& point) const;

				__uref Point PointFromScreen(const Point& point) const;

				__uref Point TranslatePoint(const Point& point, const VisualPtr& toVisual) const;

			private:				
				friend UIElement;
				/* only override once*/

				virtual System::UI::Graphic::EffectPtr GetEffectCore() const;
				virtual EffectCompositeMode GetCompositeModeCore() const;

				virtual GeometryPtr GetClipCore() const;
				virtual double GetOpacityCore() const;
				virtual BrushPtr GetOpacityMaskCore() const;

				virtual bool GetClipToBoundsCore() const;
				virtual Thickness GetMarginCore() const;

				virtual TransformPtr GetRenderTransformCore() const;
				virtual Point GetRenderTransformOriginCore() const;

				virtual Size GetSizeCore() const;
				virtual Size GetMinimumSizeCore() const;
				virtual Size GetMaximumSizeCore() const;

				virtual Size GetDesiredSizeCore() const;
				virtual Size GetRenderSizeCore() const;

				virtual VerticalAlignment GetVerticalAlignmentCore() const;
				virtual HorizontalAlignment GetHorizontalAlignmentCore() const;

				virtual bool GetBitmapCacheEnabledCore() const;
				virtual double GetBitmapCacheRenderScaleCore() const;

				void push_BitmapCachedInvalid();

				virtual void OnDesiredSizeChangedCore(const System::Size& size);
				virtual void OnRenderSizeChangedCore(const System::Size& size);

			public:
				void Measure(const Size& availableSize);
				void Arrange(const Rect& finalRect);

				void InvalidateArrange();
				void InvalidateMeasure();
				void InvalidateVisual();

				bool GetLoaded() const;
			protected:
				virtual void RenderOverrideCore(const RenderContextPtr& renderContext);
				virtual Size MeasureOverrideCore(const Size& availableSize);
				virtual Size ArrangeOverrideCore(const Size& finalSize);
				bool IsRooted();
			protected:
				const Matrix& RealSceneTransform();
				virtual void OnPropertyChanged(const DependencyPropertyPtr& property, const PropertyChangedEventArgs& e) override;
				void OnInheritedPropertyChanged(const DependencyPropertyPtr& property, const PropertyChangedEventArgs& e);
			private:
				Visual(const Visual&);
				Visual& operator = (const Visual&);
			private:
				void SetClipRect(const Rect& clipRect);
				void SetClipEnable(bool enabled);
				void RenderSizeChanged(const Size& newSize);
				bool SetParent(const VisualPtr& parent);
				void RenderImpl(const RenderContextPtr& renderContext);
				void RenderingImpl();
				virtual void UpdateImpl(LONGLONG time);
				void CalculateTransform();
				void EnsureBitmapCache(const Size& size, const Size& maximumBitmapSize);
				void CalculateMinMax(Size& minSize, Size& maxSize);
			protected:
				virtual void OnParentChanged(VisualPtr oldParent, VisualPtr newParent);
			private:
				std::vector<VisualPtr> m_children;
				Matrix m_transform;
				Matrix m_lastRenderTransform;
				Matrix m_realSceneTransform; //For hittesting
				VisualPtr m_parentElement;
				Rect m_clipRect;
				bool m_clipEnabled;
				Size m_unclippedDesiredSize;
				Point m_layoutOffset;

				bool m_arrangeDirty;
				bool m_measureDirty;
				bool m_transformDirty;
				bool m_renderInvalid;

				BitmapPtr m_bitmapCache;
				bool m_bitmapCachedInvalid;

				bool m_loaded;
				bool m_isRoot;
				friend VisualRoot;

				__uproperty(get = GetLoaded) bool Loaded;


				REFLECT_CLASS(Visual)
			};
		}
	}
}