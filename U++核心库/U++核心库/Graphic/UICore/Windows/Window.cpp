#include "Window.h"
#include "Window_impl.h"
#include "VisualTreeHelper.h"
#include "..\Application.h"
#include "..\Application_impl.h"
#include "..\Graphic\GraphicFactory.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Windows;

map<Application*, vector<Window*>> Window_impl::stores;
mutex Window_impl::locker;

Window_impl::Window_impl()
{
}

bool Window_impl::CreateNativeWindow(Application* ap, Window* winInstance)
{
	app = ap;
	HINSTANCE hInstance = ap->NativeNameSpaceHandle;
	WNDCLASS wc = {};
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"UI_WINDOW";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = Window::WndProc;
	RegisterClass(&wc);
	HWND win = CreateWindowEx(WS_EX_LAYERED, wc.lpszClassName, L"", WS_POPUP, 400, 200, 800, 600, nullptr, nullptr, hInstance, winInstance);
	return win!=nullptr;
}

Window_impl* Window_impl::Create(const RenderContextPtr& renderContext)
{
	HwndHostPtr host = nullptr;
	try
	{
		host = std::make_shared<HwndHost>(renderContext);				
		if (host)
		{
			Window_impl* impl = new Window_impl();
			impl->host = host;
			impl->mouseDevice = std::make_shared<MouseDevice>();
			impl->mouseDevice->Attach(host);
			impl->keyboardDevice = std::make_shared<KeyboardDevice>();
			impl->keyboardDevice->Attach(host);
			return impl;
		}
	}
	catch (const std::exception& e)
	{
		int mm = 0;
		mm++;
	}
	catch (...)
	{

	}
	return nullptr;
}

void Window_impl::Register(Window* win)
{
	lock_guard<mutex> lock(locker);
	auto iter = stores.find(win->impl->app);
	if (iter != stores.end())
	{
		iter->second.push_back(win);
	}
	else
	{
		auto ip = make_pair(win->impl->app, vector<Window*>());
		ip.second.push_back(win);
		stores.insert(ip);
	}
}

void Window_impl::Unregister(Window* win)
{
	lock_guard<mutex> lock(locker);
	auto iter = stores.find(win->impl->app);
	if (iter != stores.end())
	{
		vector<Window*>& col = iter->second;
		auto wter = std::find(col.begin(), col.end(), win);
		if (wter != col.end())
			col.erase(wter);
	}
}

HwndHostPtr Window_impl::GetHostFromVisual(const VisualPtr& visual)
{
	VisualPtr root = VisualTreeHelper::FindRoot(visual);
	lock_guard<mutex> lock(locker);
	for (auto& iter : stores)
	{
		auto& col = iter.second;
		for (auto& wter : col)
		{
			Window_impl* impl = wter->impl;
			if (impl)
			{
				HwndHostPtr host = impl->host;
				if (host)
				{
					if (host->m_visualRoot->GetRootElement() == root)
						return host;
				}
			}
		}
	}
	return nullptr;
}

void Window_impl::Show()
{
	ShowWindow(host->m_hwnd, SW_SHOW);		
}

void Window_impl::Hide()
{
	ShowWindow(host->m_hwnd, SW_HIDE);
}

void Window_impl::Minimize()
{
	ShowWindow(host->m_hwnd, SW_MINIMIZE);
}

void Window_impl::Maximize()
{
	ShowWindow(host->m_hwnd, SW_MAXIMIZE);
}

void Window_impl::Restore()
{
	ShowWindow(host->m_hwnd, SW_RESTORE);
}


LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
		case WM_NCCREATE:
		{
			LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lparam);
			void * lpCreateParam = create_struct->lpCreateParams;
			Window*  this_window = reinterpret_cast<Window*>(lpCreateParam);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_window));
			return this_window->impl->host->ProcessMessage(hwnd, message, wparam, lparam);
		}
			break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			if (LONG_PTR user_data = GetWindowLongPtr(hwnd, GWLP_USERDATA))
			{
				Window * this_window = reinterpret_cast<Window *>(user_data);
				if (this_window->impl)
				{
					if (this_window->impl->host)
					{
						return this_window->impl->host->ProcessMessage(hwnd, message, wparam, lparam);
					}
				}
			}
		}
		break;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}

Window::Window(Application* app)
{
	if (!app) throw std::exception("invalid application");
	auto res = app->impl->factory->CreateRenderContext();
	if (res.State == GraphicFactoryError::OK)
	{
		impl = Window_impl::Create( std::shared_ptr<RenderContext>(res.Value));
		if (!impl) throw std::exception("can not create rendercontext");
		if (!impl->CreateNativeWindow(app, this)) throw std::exception("can not create window");
		Window_impl::Register(this);
	}
	else
	{
		throw std::exception("can not create rendercontext");
	}
}

Window::~Window()
{
	Window_impl::Unregister(this);
	SAFE_DELETE(impl);
}

VisualPtr Window::GetContent() const
{
	return impl->host->m_visualRoot->GetRootElement();
}

void Window::SetContent(const VisualPtr& content)
{
	impl->host->SetRoot(content);
}

void Window::Show()
{
	impl->Show();
}

void Window::Hide()
{
	impl->Hide();
}

void Window::Minimize()
{
	impl->Minimize();
}

void Window::Maximize()
{
	impl->Maximize();
}

void Window::Restore()
{
	impl->Restore();
}

void Window::SetLeft(double left)
{
	
}

double Window::GetLeft()
{
	return 0.0;
}

void Window::SetTop(double top)
{

}

double Window::GetTop()
{
	return 0.0;
}

void Window::SetRight(double right)
{

}

double Window::GetRight()
{
	return 0.0;
}

void Window::SetBottom(double bottom)
{

}

double Window::GetBottom()
{
	return 0.0;
}

void Window::SetWidth(double width)
{

}

double Window::GetWidth()
{
	return 0.0;
}

void Window::SetHeight(double height)
{

}

double Window::GetHeight()
{
	return 0.0;
}

double Window::GetActualWidth()
{
	return 0.0;
}

double Window::GetActualHeight()
{
	return 0.0;
}

__usctor(Window)
{
	return true;
}