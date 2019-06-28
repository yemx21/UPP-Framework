#include "HwndHost.h"
#include "Window_impl.h"
#include "VisualTreeHelper.h"
#include "..\Graphic\Graphic.h"
#include "..\Graphic\GraphicFactory.h"
#include "Input\Inputs.h"
using namespace System::UI::Windows;
using namespace System::UI::Windows::Input;
using namespace std;

HwndHost::HwndHost(const RenderContextPtr& renderContext) : m_hwnd(nullptr)
{
	if (!renderContext)  throw std::exception("renderContext can not be null");
	
	m_renderContext = renderContext;
	m_visualRoot = make_shared<VisualRoot>(m_renderContext);

	memset(&info, 0, sizeof(info));
	info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);

	psize.cx = 0;
	psize.cy = 0;
	pptSrc.x = 0;
	pptSrc.y = 0;
	pptDst.x = 0;
	pptDst.y = 0;
	BF.AlphaFormat = AC_SRC_ALPHA;
	BF.BlendFlags = 0;
	BF.BlendOp = AC_SRC_OVER;
	BF.SourceConstantAlpha = 255;

	info.pptSrc = &pptSrc;
	info.pptDst = &pptDst;
	info.psize = &psize;
	info.pblend = &BF;
	info.dwFlags = ULW_ALPHA;

}

HwndHost::~HwndHost(void)
{
	if (m_hwnd)
		KillTimer(m_hwnd, (UINT_PTR)this);
}

void  __stdcall HwndHost::TimerCallback(HWND hwnd, UINT p1, UINT_PTR p2, DWORD p3)
{
	auto instance = static_cast<HwndHost*>((void*)p2);

	if (!instance->m_visualRoot->IsSceneDirty()) return;

	instance->m_visualRoot->ForceDirty();
	instance->Present();

}

HWND HwndHost::GetHwnd() const
{
	return m_hwnd;
}

void HwndHost::HitTestTree(const Point& point, VisualPtr& visualElementResult) const
{
	m_visualRoot->HitTestTree(point, visualElementResult);
}

void HwndHost::SetHwnd(HWND hwnd)
{
	if (m_hwnd)
	{
		KillTimer(m_hwnd, (UINT_PTR)this);
	}

	m_hwnd = hwnd;

	RECT rect;
	GetWindowRect(m_hwnd, &rect);


	hwndSize.Width = rect.right - rect.left;
	hwndSize.Height = rect.bottom - rect.top;

	SetTimer(m_hwnd, (UINT_PTR)this, 16, &HwndHost::TimerCallback);

}

void HwndHost::Present()
{
	if (!m_visualRoot->IsSceneDirty()) return;
	HDC hdc = GetDC(m_hwnd);
	if (!hdc) return;
	if (!m_texture)
	{
		m_texture = make_shared<Texture2D>(hwndSize, Texture2DMode::Target);
		m_texture_surf= make_shared<Surface>(m_texture);
	}
	else
	{
		if (hwndSize != m_texture->PixelSize)
		{
			m_texture = make_shared<Texture2D>(hwndSize, Texture2DMode::Target);
			m_texture_surf = make_shared<Surface>(m_texture);
		}
	}
	if (!m_hold)
	{
		m_hold = make_shared<Texture2D>(hwndSize, Texture2DMode::CpuRead);
		m_hold_surf = make_shared<Surface>(m_hold);
	}
	else
	{
		if (hwndSize != m_hold->PixelSize)
		{
			m_hold = make_shared<Texture2D>(hwndSize, Texture2DMode::CpuRead);
			m_hold_surf = make_shared<Surface>(m_hold);
		}
	}

	if (!m_texture_surf || !m_hold_surf) return;

	System::UI::Graphic::GraphicError gerr = m_renderContext->PushSurfaceTarget(m_texture_surf);

	auto rerr = m_visualRoot->Render();

	gerr = m_renderContext->PopTarget();

	if (!m_texture_surf->CopyTo(m_hold_surf)) return;

	BitmapData map;

	if (!m_hold->Map(&map)) return;

	Size txSize = m_texture->PixelSize;

	HDC hdcMem = CreateCompatibleDC(hdc);
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = (LONG)txSize.Width;
	bmi.bmiHeader.biHeight = -1 * (LONG)txSize.Height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD *prgbBits;
	HBITMAP hbm = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &reinterpret_cast<void*&>(prgbBits), NULL, 0);

	if (hbm)
	{
		HBITMAP hbmPrev = ((HBITMAP)SelectObject((hdcMem), (HGDIOBJ)(HBITMAP)(hbm))); //SelectBitmap(hdcMem, hbm);

		memcpy(prgbBits, map.Bits, map.DepthPitch);

		psize.cx = (LONG)txSize.Width;
		psize.cy = (LONG)txSize.Height;
		info.hdcSrc = hdcMem;

		RECT pos;
		GetWindowRect(m_hwnd, &pos);

		pptDst.x = pos.left;
		pptDst.y = pos.top;

		BOOL b = UpdateLayeredWindowIndirect(m_hwnd, &info);
		((HBITMAP)SelectObject((hdcMem), (HGDIOBJ)(HBITMAP)(hbmPrev))); // SelectBitmap(hdcMem, hbmPrev);
		DeleteObject(hbm);
	}

	DeleteDC(hdcMem);
	m_hold->Unmap();

	ReleaseDC(m_hwnd, hdc);
	DeleteDC(hdc);
}

void HwndHost::SetRoot(const VisualPtr& rootElement)
{
	m_visualRoot->SetRootElement(rootElement);
}

LRESULT HwndHost::ProcessMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (window != m_hwnd) SetHwnd(window);

	if (WM_SIZE == message)
	{
		if (SIZE_MINIMIZED != wparam)
		{
			m_visualRoot->ForceDirty();
			Present();
		}

		return 0;
	}

	if (WM_ERASEBKGND == message)
	{
		return 0;
	}

	if (WM_DISPLAYCHANGE == message)
	{
		m_visualRoot->ForceDirty();
		Present();
		return 0;
	}

	if (WM_TIMER == message)
	{
		if (!m_visualRoot->IsSceneDirty()) return 0;
		
		m_visualRoot->ForceDirty();

		Present();

		return 0;
	}

	m_inputDelegates.Invoke(window, message, wparam, lparam);

	return  DefWindowProc(window, message, wparam, lparam);
}

Point HwndHost::GetLocation() const
{
	RECT rect = {};
	if (GetWindowRect(m_hwnd, &rect))
	{
		return Point(rect.left, rect.top);
	}
	return Point();
}

bool HwndHost::SetTopMost()
{
	return SetWindowPos(m_hwnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
}

bool HwndHost::SetLocation(const Point& pt)
{
	return SetWindowPos(m_hwnd, 0, pt.X, pt.Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

Size HwndHost::GetSize() const
{
	RECT rect = {};
	if (GetWindowRect(m_hwnd, &rect))
	{
		return Size(rect.right - rect.left, rect.bottom - rect.top);
	}
	return Size();
}

bool HwndHost::SetSize(const Size& sz)
{
	return SetWindowPos(m_hwnd, 0, 0, 0, sz.Width, sz.Height, SWP_NOMOVE | SWP_NOZORDER);
}

Point HwndHost::PointFromScreen(const Point& p) const
{
	RECT rect = {};
	if (GetWindowRect(m_hwnd, &rect))
	{
		return Point(p.X - rect.left, p.Y - rect.top);
	}
	return Point();
}

Point HwndHost::PointToScreen(const Point& p) const
{
	RECT rect = {};
	if (GetWindowRect(m_hwnd, &rect))
	{
		return Point(p.X + rect.left, p.Y+ rect.top);
	}
	return Point();
}

HwndHostPtr HwndHost::FromVisual(const VisualPtr& visual)
{
	return Window_impl::GetHostFromVisual(visual);
}

Point HwndHost::Translate(const Point& fromPoint, const VisualPtr& from, const VisualPtr& to)
{
	VisualPtr root1 = nullptr;
	VisualPtr root2 = nullptr;
	if (from->IsRooted())
		root1 = from;
	else
	{
		root1 = VisualTreeHelper::FindRoot(from);
	}

	if (to->IsRooted())
		root2 = to;
	else
	{
		root2 = VisualTreeHelper::FindRoot(from);
	}

	Matrix inverted1 = from->RealSceneTransform();

	inverted1.Invert();

	auto transformedPoint1 = inverted1.Transform(fromPoint);

	Matrix inverted2 = to->RealSceneTransform();

	inverted2.Invert();

	auto transformedPoint2 = inverted2.Transform(Point(0, 0));

	if (root1==root2)
	{
		return transformedPoint1 - transformedPoint2;
	}
	else
	{
		const HwndHostPtr host1 = HwndHost::FromVisual(from);
		const HwndHostPtr host2 = HwndHost::FromVisual(to);
		if (host1 && host2)
		{
			Point p1 = host1->PointToScreen(Point());
			Point p2 = host2->PointToScreen(Point());

			Point ap = p2 - p1;

			return transformedPoint1 - transformedPoint2 + ap;

		}
	}
	return Point();
}
