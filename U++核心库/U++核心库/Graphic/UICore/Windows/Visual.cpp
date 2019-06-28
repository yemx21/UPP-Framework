#include "Visual.h"
#include "..\EventManager.h"
#include "HwndHost.h"
#include <string>
#include <sstream>
#include "VisualTreeHelper.h"
#include "..\Graphic\Rendering\ClipCommand.h"
#include "..\Graphic\Rendering\GeometryClipCommand.h"
#include "..\Graphic\Rendering\OpacityCommand.h"
#include "..\Graphic\Rendering\OpacityMaskCommand.h"
#include "..\CompositeTransform.h"
#include "..\Graphic\Media\WriteableBitmap.h"
#include "..\Graphic\GraphicFactory.h"
using namespace System::UI;
using namespace System::UI::Windows;

Visual::Visual() : m_transform(Matrix::Identity()),
m_realSceneTransform(Matrix::Identity()),
m_clipRect(Rect(0, 0, 0, 0)),
m_unclippedDesiredSize(-1, -1),
m_clipEnabled(false),
m_measureDirty(true),
m_arrangeDirty(true),
m_transformDirty(true),
m_bitmapCachedInvalid(true),
m_renderInvalid(true),
m_parentElement(nullptr),
m_loaded(false),
m_isRoot(false)
{

}

Visual::~Visual()
{
	
}

void Visual::AddVisualChild(const VisualPtr& element)
{
	if (element->GetParent() != nullptr)
		throw invalid_argument("Visual is already connected to a parent");

	if (!element)
		throw invalid_argument("Visual is null");

	element->SetParent(std::shared_ptr<Visual>(this, System::null_deleter()));

	m_children.push_back(element);

	element->InvalidateVisual();

	if (IsRooted())
		element->InvokeLoaded();
}

VisualPtr Visual::GetParent() const
{
	return m_parentElement;
}

bool Visual::SetParent(const VisualPtr& parent)
{
	auto oldParent = m_parentElement;
	m_parentElement = parent;

	InvalidateVisual();
	OnParentChanged(oldParent, parent);

	return S_OK;
}

void Visual::RemoveVisualChild(const VisualPtr& element)
{
	auto findIter = find(begin(m_children), end(m_children), element);

	if (*findIter == nullptr)
		return;

	m_children.erase(findIter);

	if (element->m_loaded)
		element->InvokeUnloaded();

	element->SetParent(nullptr);
}

VisualPtr Visual::GetVisualChild(int index)  const
{
	auto size = m_children.size();

	if ((size_t)index >= size)
		return nullptr;

	return m_children[index];
}

int Visual::GetVisualChildCount()  const
{
	return (int)m_children.size();
}

void Visual::RenderImpl(const RenderContextPtr& renderContext)
{
	/* dirty */
	auto contextImpl = static_cast<RenderContext*>(renderContext.get());

	auto m_renderSize = GetRenderSizeCore();
	auto m_compositeMode = GetCompositeModeCore();

	EnsureBitmapCache(m_renderSize, contextImpl->GetMaximumImageSize());

	if (m_bitmapCache)
	{
		CalculateTransform();
		if (m_bitmapCachedInvalid || m_renderInvalid)
		{
			auto oldTransform = m_transform;

			contextImpl->PushTarget(m_bitmapCache);

			{
				auto cacheDimensions = m_bitmapCache->GetPixelSize();

				m_transform = Matrix::CreateScaling(cacheDimensions.Width / m_renderSize.Width,
					cacheDimensions.Height / m_renderSize.Height);

				renderContext->PushTransform(m_transform);

				Graphic::Internal::OpacityCommand opacCommand(GetOpacityCore(), renderContext.get());
				Graphic::Internal::ClipCommand clipCommand(m_clipEnabled, m_clipRect, renderContext.get());
				Graphic::Internal::GeometryClipCommand geoclipCommand(GetClipCore(), renderContext.get());
				Graphic::Internal::OpacityMaskCommand opacmaskCommand(m_clipRect, GetOpacityMaskCore(), renderContext.get());

				RenderOverrideCore(renderContext);
				m_renderInvalid = false;
				m_bitmapCachedInvalid = false;

				for (VisualPtr& element : m_children)
				{
					element->RenderImpl(renderContext);
				}
			}

			contextImpl->PopTarget();
			m_transform = oldTransform;
		}


		contextImpl->PushTransform(m_transform);
		Graphic::Internal::OpacityCommand opacCommand(GetOpacityCore(), renderContext.get());
		Graphic::Internal::ClipCommand clipCommand(m_clipEnabled, m_clipRect, renderContext.get());
		Graphic::Internal::GeometryClipCommand geoclipCommand(GetClipCore(), renderContext.get());
		Graphic::Internal::OpacityMaskCommand opacmaskCommand(m_clipRect, GetOpacityMaskCore(), renderContext.get());

		auto bitmapSize = m_bitmapCache->GetPixelSize();

		auto elementEffect = GetEffectCore();

		if (elementEffect)
			contextImpl->DrawEffect(m_bitmapCache, elementEffect, m_compositeMode, m_renderSize);
		else
			contextImpl->DrawBitmap(m_bitmapCache, m_compositeMode, m_renderSize);
	}
	else
	{
		CalculateTransform();
		m_realSceneTransform = m_transform;
		contextImpl->PushTransform(m_transform);
		Graphic::Internal::OpacityCommand opacCommand(GetOpacityCore(), renderContext.get());
		Graphic::Internal::ClipCommand clipCommand(m_clipEnabled, m_clipRect, renderContext.get());
		Graphic::Internal::GeometryClipCommand geoclipCommand(GetClipCore(), renderContext.get());
		Graphic::Internal::OpacityMaskCommand opacmaskCommand(m_clipRect, GetOpacityMaskCore(), renderContext.get());

		RenderOverrideCore(renderContext);
		m_renderInvalid = false;

		for (VisualPtr& element : m_children)
		{
			element->RenderImpl(renderContext);
		}
	}
}

void Visual::RenderingImpl()
{

}

void Visual::UpdateImpl(LONGLONG time)
{
	auto renderTransform = GetRenderTransformCore();
	if (renderTransform && !m_transformDirty)
	{
		if (!memcmp(&m_lastRenderTransform,
			&renderTransform->GetTransformMatrix(),
			sizeof(Matrix)))
		{
			m_transformDirty = true;
		}
	}

	for (auto& element : m_children)
	{
		element->UpdateImpl(time);
	}

	if (m_transformDirty && !m_renderInvalid)
	{
		auto parent = m_parentElement;
		if (parent)
			parent->InvalidateVisual();
	}
}

void Visual::SetClipRect(const Rect& clipRect)
{
	m_clipRect = clipRect;
}

System::UI::Graphic::EffectPtr Visual::GetEffectCore() const
{
	return nullptr;
}

EffectCompositeMode Visual::GetCompositeModeCore() const
{
	return EffectCompositeMode::SourceOver;
}

GeometryPtr Visual::GetClipCore() const
{
	return nullptr;
}

double Visual::GetOpacityCore() const
{
	return 1.0;
}

BrushPtr Visual::GetOpacityMaskCore() const
{
	return nullptr;
}

bool Visual::GetClipToBoundsCore() const
{
	return false;
}

Thickness Visual::GetMarginCore() const
{
	return Thickness{};
}

Size Visual::GetMinimumSizeCore() const
{
	return Size{};
}

Size Visual::GetMaximumSizeCore() const
{
	return Size::Infitite();
}

Size Visual::GetDesiredSizeCore() const
{
	return System::Size{ 0.0,0.0 };
}

Size Visual::GetRenderSizeCore() const
{
	return System::Size{ 0.0,0.0 };
}

Size Visual::GetSizeCore() const
{
	return Size::Infitite();
}

TransformPtr Visual::GetRenderTransformCore() const
{
	return nullptr;
}

Point Visual::GetRenderTransformOriginCore() const
{
	return Point{ 0.5, 0.5 };
}

VerticalAlignment Visual::GetVerticalAlignmentCore() const
{
	return VerticalAlignment::Stretch;
}

HorizontalAlignment Visual::GetHorizontalAlignmentCore() const
{
	return HorizontalAlignment::Stretch;
}

bool Visual::GetBitmapCacheEnabledCore() const
{
	return false;
}

double Visual::GetBitmapCacheRenderScaleCore() const
{
	return 1.0;
}

void Visual::OnDesiredSizeChangedCore(const System::Size& size)
{
	
}

void Visual::OnRenderSizeChangedCore(const System::Size& size)
{
	
}

Size Visual::MeasureOverrideCore(const Size& availableSize)
{
	auto size = GetSizeCore();//SizeF(0, 0);

	if (isinf(size.Width))
	{
		size.Width = availableSize.Width;
	}

	if (isinf(size.Height))
	{
		size.Height = availableSize.Height;
	}

	for (VisualPtr& element : m_children)
	{
		element->Measure(availableSize);
	}

	return availableSize;
}

Size Visual::ArrangeOverrideCore(const Size& finalSize)
{
	for (int i = 0; i < GetVisualChildCount(); i++)
	{
		GetVisualChild(i)->Arrange(Rect(0, 0, finalSize.Width, finalSize.Height));
	}
	return finalSize;
}

void Visual::CalculateMinMax(Size& minSize, Size& maxSize)
{
	Size m_size = GetSizeCore();
	Size m_minSize = GetMinimumSizeCore();
	Size m_maxSize = GetMaximumSizeCore();

	maxSize.Height = m_maxSize.Height;
	minSize.Height = m_minSize.Height;

	auto dimensionLength = m_size.Height;

	double height = dimensionLength;

	maxSize.Height = max<double>(min<double>(height, maxSize.Height), minSize.Height);

	height = (isinf(dimensionLength) ? 0 : dimensionLength);

	minSize.Height = max<double>(min<double>(maxSize.Height, height), minSize.Height);

	maxSize.Width = m_maxSize.Width;
	minSize.Width = m_minSize.Width;

	dimensionLength = m_size.Width;

	double width = dimensionLength;

	maxSize.Width = max<double>(min<double>(width, maxSize.Width), minSize.Width);

	width = (isinf(dimensionLength) ? 0 : dimensionLength);

	minSize.Width = max<double>(min<double>(maxSize.Width, width), minSize.Width);
}

void Visual::Measure(const Size& availableSize)
{
	/* if(!m_measureDirty)
	return;*/

	auto margin = GetMarginCore();

	double marginWidth = margin.Left + margin.Right;
	double marginHeight = margin.Top + margin.Bottom;

	auto frameworkAvailableSize = Size(System::ComparisonHelper::Max(availableSize.Width - marginWidth, 0.0),
		System::ComparisonHelper::Max(availableSize.Height - marginHeight, 0.0));

	Size maxSize, minSize;
	CalculateMinMax(minSize, maxSize);

	frameworkAvailableSize.Width = max(minSize.Width, min(frameworkAvailableSize.Width, maxSize.Width));
	frameworkAvailableSize.Height = max(minSize.Height, min(frameworkAvailableSize.Height, maxSize.Height));

	Size desiredSize = MeasureOverrideCore(frameworkAvailableSize);

	desiredSize = Size(max(desiredSize.Width, minSize.Width),
		max(desiredSize.Height, minSize.Height));

	Size unclippedDesiredSize = desiredSize;

	bool clipped = false;

	if (desiredSize.Width > maxSize.Width)
	{
		desiredSize.Width = maxSize.Width;
		clipped = true;
	}

	if (desiredSize.Height > maxSize.Height)
	{
		desiredSize.Height = maxSize.Height;
		clipped = true;
	}

	double clippedDesiredWidth = desiredSize.Width + marginWidth;
	double clippedDesiredHeight = desiredSize.Height + marginHeight;

	if (clippedDesiredWidth > availableSize.Width)
	{
		clippedDesiredWidth = availableSize.Width;
		clipped = true;
	}

	/*逻辑可能有问题, 可能是 clippedDesiredHeight > availableSize.Height*/
	if (clippedDesiredHeight > availableSize.Width)
	{
		clippedDesiredHeight = availableSize.Width;
		clipped = true;
	}

	if (clipped ||
		clippedDesiredWidth  < 0 ||
		clippedDesiredHeight < 0)
	{
		m_unclippedDesiredSize = unclippedDesiredSize;
	}
	else
	{
		m_unclippedDesiredSize.Width = -1;
		m_unclippedDesiredSize.Height = -1;
	}

	OnDesiredSizeChangedCore(Size{ clippedDesiredWidth, clippedDesiredHeight });

	m_measureDirty = false;
}

void Visual::Arrange(const Rect& finalRect)
{
	auto ancestor = m_parentElement;

	bool ancestorDirty = false;

	while (ancestor)
	{
		if (ancestor->m_arrangeDirty)
		{
			ancestorDirty = true;
			break;
		}

		ancestor = ancestor->m_parentElement;
	}

	if (!m_arrangeDirty && !ancestorDirty)
		return;

	m_clipEnabled = false;

	auto arrangeSize = finalRect.GetSize();

	auto margin = GetMarginCore();

	Size marginSize;

	marginSize.Width = margin.Left + margin.Right;
	marginSize.Height = margin.Top + margin.Bottom;

	arrangeSize.Width = System::ComparisonHelper::Max(0.0, arrangeSize.Width - marginSize.Width);
	arrangeSize.Height = System::ComparisonHelper::Max(0.0, arrangeSize.Height - marginSize.Height);

	auto unclippedDesiredSize = m_unclippedDesiredSize;

	auto m_desiredSize = GetDesiredSizeCore();

	if (unclippedDesiredSize.Width == -1 ||
		unclippedDesiredSize.Height == -1)
	{
		if (isinf(m_desiredSize.Width) || isinf(m_desiredSize.Height))
		{
			unclippedDesiredSize.Width = arrangeSize.Width - marginSize.Width;
			unclippedDesiredSize.Height = arrangeSize.Height - marginSize.Height;
		}
		else
		{
			unclippedDesiredSize.Width = m_desiredSize.Width - marginSize.Width,
				unclippedDesiredSize.Height = m_desiredSize.Height - marginSize.Height;
		}
	}

	if (arrangeSize.Width < unclippedDesiredSize.Width)
	{
		m_clipEnabled = true;
		arrangeSize.Width = unclippedDesiredSize.Width;
	}

	if (arrangeSize.Height < unclippedDesiredSize.Height)
	{
		m_clipEnabled = true;
		arrangeSize.Height = unclippedDesiredSize.Height;
	}

	if (GetHorizontalAlignmentCore() != HorizontalAlignment::Stretch)
	{
		arrangeSize.Width = unclippedDesiredSize.Width;
	}

	if (GetVerticalAlignmentCore() != VerticalAlignment::Stretch)
	{
		arrangeSize.Height = unclippedDesiredSize.Height;
	}

	Size minSize, maxSize;
	CalculateMinMax(minSize, maxSize);

	double calcedMaxWidth = max(unclippedDesiredSize.Width, maxSize.Width);
	if (calcedMaxWidth < arrangeSize.Width)
	{
		m_clipEnabled = true;
		arrangeSize.Width = calcedMaxWidth;
	}

	double calcedMaxHeight = max(unclippedDesiredSize.Height, maxSize.Height);
	if (calcedMaxHeight < arrangeSize.Height)
	{
		m_clipEnabled = true;
		arrangeSize.Height = calcedMaxHeight;
	}

	Size oldRenderSize = GetRenderSizeCore();
	Size arrangeResultSize = ArrangeOverrideCore(arrangeSize);

	bool arrangeSizeChanged = arrangeResultSize != GetRenderSizeCore();
	if (arrangeSizeChanged)
	{
		RenderSizeChanged(arrangeResultSize);
	}

	OnRenderSizeChangedCore(arrangeResultSize);

	Size clippedArrangeResultSize = Size(min(arrangeResultSize.Width, maxSize.Width),
		min(arrangeResultSize.Height, maxSize.Height));

	if (!m_clipEnabled)
	{
		m_clipEnabled = clippedArrangeResultSize.Width  < arrangeResultSize.Width ||
			clippedArrangeResultSize.Height < arrangeResultSize.Height;
	}

	auto clientSize = Size(System::ComparisonHelper::Max(0.0, finalRect.Width - marginSize.Width),
		System::ComparisonHelper::Max(0.0, finalRect.Height - marginSize.Height));

	if (!m_clipEnabled)
	{
		m_clipEnabled = clientSize.Width  < clippedArrangeResultSize.Width ||
			clientSize.Height < clippedArrangeResultSize.Height;

	}

	Point layoutOffset;
	layoutOffset.X = 0;
	layoutOffset.Y = 0;

	HorizontalAlignment tempHorizontalAlignment = GetHorizontalAlignmentCore();
	VerticalAlignment tempVerticalAlignment = GetVerticalAlignmentCore();

	if (tempHorizontalAlignment == HorizontalAlignment::Stretch &&
		clippedArrangeResultSize.Width > clientSize.Width)
	{
		tempHorizontalAlignment = HorizontalAlignment::Left;
	}

	if (tempVerticalAlignment == VerticalAlignment::Stretch &&
		clippedArrangeResultSize.Height > clientSize.Height)
	{
		tempVerticalAlignment = VerticalAlignment::Top;
	}

	if (tempHorizontalAlignment == HorizontalAlignment::Center ||
		tempHorizontalAlignment == HorizontalAlignment::Stretch)
	{
		layoutOffset.X = (clientSize.Width - clippedArrangeResultSize.Width) / 2.0f;
	}
	else if (tempHorizontalAlignment == HorizontalAlignment::Right)
	{
		layoutOffset.X = clientSize.Width - clippedArrangeResultSize.Width;
	}
	else
	{
		layoutOffset.X = 0;
	}

	if (tempVerticalAlignment == VerticalAlignment::Center
		|| tempVerticalAlignment == VerticalAlignment::Stretch)
	{
		layoutOffset.Y = (clientSize.Height - clippedArrangeResultSize.Height) / 2.0f;
	}
	else if (GetVerticalAlignmentCore() == VerticalAlignment::Bottom)
	{
		layoutOffset.Y = clientSize.Height - clippedArrangeResultSize.Height;
	}
	else
	{
		layoutOffset.Y = 0;
	}

	layoutOffset.X += finalRect.Left + margin.Left;
	layoutOffset.Y += finalRect.Top + margin.Top;

	if (m_clipEnabled || GetClipToBoundsCore())
		m_clipRect = Rect(0, 0, clientSize.Width, clientSize.Height);

	if (layoutOffset.X != m_layoutOffset.X ||
		layoutOffset.Y != m_layoutOffset.Y || arrangeSizeChanged)
	{
		m_transformDirty = true;
	}

	m_arrangeDirty = false;

	m_layoutOffset = layoutOffset;
}

void Visual::OnParentChanged(VisualPtr /*oldParent*/, VisualPtr /*newParent*/)
{

}

void Visual::RemoveAllChildren()
{
	vector<VisualPtr> removeList;

	for (const VisualPtr& child : m_children)
	{
		removeList.push_back(child);
	}

	for (const VisualPtr& child : removeList)
	{
		RemoveVisualChild(child);
	}
}

bool Visual::IsRooted()
{
	if (m_isRoot)
		return true;

	auto ancestor = m_parentElement;

	while (ancestor)
	{
		if (ancestor->m_isRoot)
			return true;

		ancestor = ancestor->m_parentElement;
	}

	return false;
}

void Visual::RenderOverrideCore(const RenderContextPtr& /* renderContext */)
{

}

void Visual::InvokeUnloaded()
{
	m_loaded = false;

	OnUnloaded();
	for (const VisualPtr& child : m_children)
	{
		child->InvokeUnloaded();
	}
}

void Visual::InvokeLoaded()
{
	m_loaded = true;

	SetInheritanceContext(m_parentElement);
	OnLoaded();
	for (const VisualPtr& child : m_children)
	{
		child->InvokeLoaded();

	}
}

bool Visual::GetLoaded() const
{
	return m_loaded;
}

void Visual::OnLoaded()
{

}

void Visual::OnUnloaded()
{

}

const Matrix& Visual::RealSceneTransform()
{
	return m_realSceneTransform;
}

void Visual::Invalidate(InvalidateFlags flags) 
{
	if (Enums::Contains(flags, InvalidateFlags::Visual))
		InvalidateVisual();

	if (Enums::Contains(flags, InvalidateFlags::Arrange))
		InvalidateArrange();

	if (Enums::Contains(flags, InvalidateFlags::Measure))
		InvalidateMeasure();

	if (Enums::Contains(flags, InvalidateFlags::Transform))
	{
		m_transformDirty = true;

		auto ancestor = m_parentElement;

		while (ancestor)
		{
			ancestor->m_transformDirty = true;
			ancestor = ancestor->m_parentElement;
		}
	}
}

bool Visual::HitTest(const Point& point)
{
	try
	{
		auto renderSize = GetRenderSizeCore();
		return point.X >= 0 && point.X <= renderSize.Width && point.Y >= 0 && point.Y <= renderSize.Height;
	}
	catch (...)
	{
	}
	return false;
}

void Visual::HitTestTree(const Point& point, VisualPtr& hitElement)
{
	bool hitResult = false;
	hitElement = nullptr;

	Matrix inverted = RealSceneTransform();

	inverted.Invert();

	auto transformedPoint = inverted.Transform(point);

	hitResult = HitTest(transformedPoint);

	if (hitResult)
	{
		hitElement = std::shared_ptr<Visual>(this, null_deleter());
		return;
	}

	for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
	{
		auto& child = *it;

		child->HitTestTree(point, hitElement);
		if (hitElement)
		{
			return;
		}
	}
}

Point Visual::TranslatePoint(const Point& point, const VisualPtr& toVisual) const
{
	VisualPtr me = VisualPtr(const_cast<Visual*>(this));
	return HwndHost::Translate(point, me, toVisual);
}

Point Visual::PointToScreen(const Point& point) const
{
	VisualPtr me = VisualPtr(const_cast<Visual*>(this));
	VisualPtr root = VisualTreeHelper::FindRoot(me);
	if (root)
	{	
		HwndHostPtr host = HwndHost::FromVisual(me);
		if (host)
		{
			Point rp = root->TranslatePoint(point, me);
			return host->PointToScreen(rp);
		}
	}
	return Point();
}

Point Visual::PointFromScreen(const Point& point) const
{
	VisualPtr me = VisualPtr(const_cast<Visual*>(this));
	VisualPtr root = VisualTreeHelper::FindRoot(me);
	if (root)
	{
		HwndHostPtr host = HwndHost::FromVisual(me);
		if (host)
		{
			Point rp = host->PointFromScreen(point);
			return TranslatePoint(rp, root);
		}
	}
	return Point();
}

void Visual::push_BitmapCachedInvalid()
{
	m_bitmapCachedInvalid = true;
}

void Visual::CalculateTransform()
{
	m_transform = Matrix::CreateTranslation(m_layoutOffset.X, m_layoutOffset.Y);

	Matrix transform;
	Matrix realTransform;
	if (GetParent() == nullptr)
	{
		transform = m_transform * Matrix::Identity();
		realTransform = m_transform * Matrix::Identity();
	}
	else
	{
		transform = m_transform * GetParent()->m_transform;
		realTransform = m_transform * GetParent()->m_realSceneTransform;
	}

	auto renderTransform = GetRenderTransformCore();
	auto m_renderSize = GetRenderSizeCore();
	auto m_renderTransformOrigin = GetRenderTransformOriginCore();
	if (renderTransform)
	{
		m_transform = Matrix::CreateTranslation(-(m_renderSize.Width  *  m_renderTransformOrigin.X),
			-(m_renderSize.Height *  m_renderTransformOrigin.Y));

		m_transform = m_transform * (Matrix)renderTransform->GetTransformMatrix();

		m_transform = m_transform * Matrix::CreateTranslation((m_renderSize.Width  * m_renderTransformOrigin.X),
			(m_renderSize.Height * m_renderTransformOrigin.Y));
	}
	else
	{
		m_transform = Matrix::Identity();
	}

	m_realSceneTransform = m_transform * realTransform;
	m_transform = m_transform * transform;

	m_transformDirty = false;
}

void Visual::InvalidateVisual()
{
	m_renderInvalid = true;
	m_bitmapCachedInvalid = true;
	m_transformDirty = true;
	m_measureDirty = true;
	m_arrangeDirty = true;

	auto ancestor = m_parentElement;
	while (ancestor)
	{
		/* Optimization.  If parent is already invalid, then
		* the rest of the ancestors should be invalid also */
		if (ancestor->m_bitmapCachedInvalid &&
			ancestor->m_renderInvalid &&
			ancestor->m_transformDirty &&
			ancestor->m_measureDirty &&
			ancestor->m_arrangeDirty &&
			ancestor->m_arrangeDirty)
			return;

		ancestor->m_transformDirty = true;
		ancestor->m_bitmapCachedInvalid = true;
		ancestor->m_renderInvalid = true;
		ancestor->m_measureDirty = true;
		ancestor->m_arrangeDirty = true;
		ancestor = ancestor->m_parentElement;
	}
}

void Visual::RenderSizeChanged(const Size& /* newSize */)
{
	InvalidateVisual();
}

void Visual::EnsureBitmapCache(const Size& size, const Size& maxBitmapSize)
{
	if (size.IsOneDimensionalOrEmpty || (!GetBitmapCacheEnabledCore() && !GetEffectCore() && GetCompositeModeCore() == EffectCompositeMode::SourceOver))
	{
		m_bitmapCache = nullptr;
		return;
	}

	if (size.Width > maxBitmapSize.Width || size.Height > maxBitmapSize.Height)
		return;

	if (m_bitmapCache == nullptr)
	{
		auto scaleSize = size.Scale(GetBitmapCacheRenderScaleCore());

		scaleSize.Width = floor(scaleSize.Width + 0.5);
		scaleSize.Height = floor(scaleSize.Height + 0.5);

		if (scaleSize.Width > maxBitmapSize.Width)
			scaleSize.Width = maxBitmapSize.Width;

		if (scaleSize.Height >maxBitmapSize.Height)
			scaleSize.Height =maxBitmapSize.Height;

		m_bitmapCache = dynamic_pointer_cast<Bitmap>(make_shared<WriteableBitmap>(scaleSize));
		return;
	}

	auto cacheNaturalSize = m_bitmapCache->GetPixelSize();
	auto m_bitmapCacheRenderAtScale = GetBitmapCacheRenderScaleCore();

	if (size.Width > cacheNaturalSize.Width * m_bitmapCacheRenderAtScale ||
		size.Height > cacheNaturalSize.Height * m_bitmapCacheRenderAtScale)
	{

		auto scaleSize = size.Scale(m_bitmapCacheRenderAtScale);

		scaleSize.Width = floor(scaleSize.Width + 0.5);
		scaleSize.Height = floor(scaleSize.Height + 0.5);

		if (scaleSize.Width > maxBitmapSize.Width)
			scaleSize.Width = maxBitmapSize.Width;

		if (scaleSize.Height > maxBitmapSize.Height)
			scaleSize.Height = maxBitmapSize.Height;

		m_bitmapCache = dynamic_pointer_cast<Bitmap>(make_shared<WriteableBitmap>(scaleSize));
		m_bitmapCachedInvalid = true;
	}
}

void Visual::InvalidateArrange()
{
	m_arrangeDirty = true;

	auto ancestor = GetParent();
	while (ancestor)
	{
		ancestor->m_arrangeDirty = true;
		ancestor = ancestor->GetParent();
	}
}

void Visual::InvalidateMeasure()
{
	m_arrangeDirty = true;
	m_measureDirty = true;
	m_bitmapCachedInvalid = true;

	auto ancestor = GetParent();

	while (ancestor)
	{
		ancestor->m_measureDirty = true;
		ancestor->m_arrangeDirty = true;
		ancestor = ancestor->GetParent();
	}
}

void Visual::OnInheritedPropertyChanged(const DependencyPropertyPtr& property, const PropertyChangedEventArgs& propertyChangedArgs)
{
	SetInheritedValue(property, propertyChangedArgs.NewValue());
}

void Visual::OnPropertyChanged(const DependencyPropertyPtr& property, const PropertyChangedEventArgs& propertyChangedArgs)
{
	auto flags = property->PropertyMetadata->FrameworkPropertyMetadataOptionsFlags();

	auto invalidateFlags = 0;
	if (Enums::Contains(flags, FrameworkPropertyMetadataOptions::AffectsArrange))
	{
		invalidateFlags |= InvalidateFlags::Arrange;
	}
	if (Enums::Contains(flags, FrameworkPropertyMetadataOptions::AffectsMeasure))
	{
		invalidateFlags |= InvalidateFlags::Measure;
	}
	if (Enums::Contains(flags, FrameworkPropertyMetadataOptions::AffectsRender))
	{
		invalidateFlags |= InvalidateFlags::Visual;
	}

	if (invalidateFlags)
		Invalidate(invalidateFlags);

	if (Enums::Contains(flags, FrameworkPropertyMetadataOptions::Inherits))
	{
		for (const VisualPtr& child : m_children)
		{
			child->OnInheritedPropertyChanged(property, propertyChangedArgs);
			child->OnPropertyChanged(property, propertyChangedArgs);
		}
	}
}
