#include "VisualRoot.h"
#include "..\Graphic\GraphicFactory.h"
using namespace System::UI;
using namespace System::UI::Windows;

VisualRoot::VisualRoot(const RenderContextPtr& renderContext) : m_renderContext(renderContext), m_forceDirty(false)
{
}

Result<bool> VisualRoot::Render()
{
	Result<bool> result = Result<bool>(false);
	if (!m_elementRoot)
	{
		result.WriteMessage(L"no root element");
		return result;
	}

	Rect rect(0, 0, 0, 0);

	//m_elementRoot->Measure(Size(INFINITY, INFINITY));

	auto size = m_renderContext->GetCurrentTargetSize();

	if (size.State != GraphicError::OK)
	{
		result.WriteMessage(size.Message);
		return result;
	}

	rect.Right = size.Value.Width;
	rect.Bottom = size.Value.Height;

	m_elementRoot->Measure(size.Value);
	m_elementRoot->Arrange(rect);
	m_elementRoot->UpdateImpl(0);

	// m_elementRoot->RenderingImpl(); /* to drive frame based animations */

	bool needsRender = m_forceDirty;

	auto targetPtr = m_renderContext->GetCurrentTarget();

	if (targetPtr.State == GraphicError::OK)
	{
		if (targetPtr.State != m_lastRenderTarget)
			needsRender = true;
	}

	needsRender |= m_elementRoot->m_renderInvalid;

	if (needsRender)
	{
		m_renderContext->Clear(Colors::Transparent);
		m_elementRoot->RenderImpl(m_renderContext);
	}

	m_lastRenderTarget = targetPtr.Value;
	m_forceDirty = false;
	return S_OK;
}

void VisualRoot::SetRootElement(const VisualPtr& elementRoot)
{
	if (m_elementRoot)
	{
		m_elementRoot->InvokeUnloaded();
		m_elementRoot->m_isRoot = false;
	}

	m_elementRoot = elementRoot;

	if (m_elementRoot)
	{
		m_elementRoot->InvokeLoaded();
		m_elementRoot->m_isRoot = true;
	}
}

const RenderContextPtr& VisualRoot::GetRenderContext() const
{
	return m_renderContext;
}

bool VisualRoot::IsSceneDirty() const
{
	if (m_forceDirty)
		return true;

	if (!m_elementRoot)
	{
		return false;
	}

	m_elementRoot->RenderingImpl(); /* to drive frame based animations */

	return m_elementRoot->m_renderInvalid ||
		m_elementRoot->m_arrangeDirty ||
		m_elementRoot->m_measureDirty ||
		m_elementRoot->m_transformDirty;
}

//void VisualRoot::SendOnPointerDown(const Point& point)
//{
//	VisualPtr elem;
//	if (!m_elementRoot)
//		return;
//
//	m_elementRoot->HitTestTree(point, elem);
//
//	if (elem)
//	{
//		auto xform = elem->m_realSceneTransform;
//		xform.Invert();
//		auto translatedPoint = xform.TransformPoint(point);
//		elem->m_pointerDownEvent.Invoke(elem, Point(translatedPoint.x, translatedPoint.y));
//	}
//}

void VisualRoot::ForceDirty()
{
	m_forceDirty = true;

	if (m_elementRoot)
	{
		m_elementRoot->InvalidateMeasure();
		m_elementRoot->InvalidateArrange();
	}
}

void VisualRoot::SetRenderContext(const RenderContextPtr& context)
{
	m_renderContext = context;
}

void VisualRoot::HitTestTree(const Point& point, VisualPtr& visualElementResult)
{
	if (!m_elementRoot)
		return;

	m_elementRoot->HitTestTree(point, visualElementResult);
}


void VisualRoot::RaiseEvent(RoutedEventArgs& args)
{
	if (m_elementRoot) m_elementRoot->RaiseEvent(args);
}