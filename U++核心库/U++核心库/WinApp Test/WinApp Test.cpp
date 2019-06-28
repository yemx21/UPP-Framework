#include "stdafx.h"
#include "WinApp Test.h"
#include <System\System.h>
#include "..\Graphic\UIFramework\UIFramework.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Windows;
using namespace System::UI::Windows::Controls;

TextBlockPtr tbp;

void Border_PreviewMouseMove(DependencyObjectPtr sender, const MouseEventArgs& args)
{
	try
	{
		auto me = std::dynamic_pointer_cast<Border>(sender);
		Point xy = args.GetPosition(me);
		tbp->Text = Formatter::Format(L"border move {0:d},{1:d}", CultureInfo::Current(), (VariadicAny(xy.X), xy.Y));
	}
	catch (...)
	{

	}
}

void Panel_PreviewMouseMove(DependencyObjectPtr sender, const MouseEventArgs& args)
{
	try
	{
		auto me = std::dynamic_pointer_cast<Panel>(sender);
		Point xy = args.GetPosition(me);
		tbp->Text = Formatter::Format(L"panel move {0}", CultureInfo::Current(), (VariadicAny(xy.X), xy.Y));

	}
	catch (...)
	{

	}
}

void Image_PreviewMouseMove(DependencyObjectPtr sender, const MouseEventArgs& args)
{
	try
	{
		auto me = std::dynamic_pointer_cast<Image>(sender);
		Point xy = args.GetPosition(me);
		tbp->Text = Formatter::Format(L"image move {0}", CultureInfo::Current(), (VariadicAny(xy.X), xy.Y));

	}
	catch (...)
	{

	}
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	auto res = Application::Create(hInstance, FeatureLevel::D2D1WithD3D11, CultureInfo::Current());

	Application* app = res.Value;

	Window* win = new Window(app);
	
	PanelPtr panel = make_shared<Panel>();

	panel->PreviewMouseMove+=(std::bind(&Panel_PreviewMouseMove, std::placeholders::_1, std::placeholders::_2));

	BorderPtr border = make_shared<Border>();

	border->PreviewMouseMove += (std::bind(&Border_PreviewMouseMove, std::placeholders::_1, std::placeholders::_2));

	GradientStopCollectionPtr gsc = make_shared<GradientStopCollection>();

	gsc->Items.add(GradientStop(0, Color::FromArgb(160, 255, 255, 255)));
	gsc->Items.add(GradientStop(0.3, Color::FromArgb(200, 255, 255, 0)));
	gsc->Items.add(GradientStop(0.7, Color::FromArgb(200, 255, 128, 128)));
	gsc->Items.add(GradientStop(1.0, Color::FromArgb(180, 255, 255, 255)));

	LinearGradientBrushPtr brush = make_shared<LinearGradientBrush>(gsc, Point(0, 0), Point(0, 600));

	SolidColorBrushPtr sbrush = make_shared<SolidColorBrush>(Color::FromArgb(255, 255, 255, 255));

	StrokeStylePtr sstyle = make_shared<StrokeStyle>();

	border->Background = brush;

	border->BorderBrush = sbrush;

	border->StrokeStyle = sstyle;

	border->CornerRadius = CornerRadius(5, 5, 5, 5);

	BorderPtr bk = make_shared<Border>();

	SolidColorBrushPtr tbrush1 = make_shared<SolidColorBrush>(Color::FromArgb(255, 0, 0, 0));

	SolidColorBrushPtr tbrush2 = make_shared<SolidColorBrush>(Color::FromArgb(1, 255, 255, 255));

	bk->Background = tbrush2;

	bk->BorderBrush = tbrush1;

	bk->StrokeStyle = sstyle;

	DropShadowEffectPtr dr = make_shared<DropShadowEffect>(3, BlurOptimization::Quality, Colors::Black, 315, 0);

	bk->SetEffect(dr);

	bk->CornerRadius = CornerRadius(5, 5, 5, 5);

	bk->SetMargin(Thickness(10, 10, 10, 10));

	panel->AddChild(bk);
	
	border->SetMargin(Thickness(11, 11, 11, 11));

	panel->AddChild(border);

	
	BitmapSourcePtr bitmap = make_shared<BitmapSource>(L"Background.jpg", app);

	ImagePtr image = make_shared<Image>();

	image->PreviewMouseMove += (std::bind(&Image_PreviewMouseMove, std::placeholders::_1, std::placeholders::_2));

	image->Source = bitmap;

	image->SetMargin(Thickness(100, 100, 100, 100));

	image->Stretch = Stretch::Uniform;

	DropShadowEffectPtr dse = make_shared<DropShadowEffect>(10, BlurOptimization::Quality, Colors::Red, 315, 0);

	BlurEffectPtr be = make_shared<BlurEffect>(5, BlurOptimization::Balanced, BlurBorderMode::Soft);

	image->SetEffect(be);

	panel->AddChild(image);
	

	TextBlockPtr tb = make_shared<TextBlock>();

	tbp = tb;

	SolidColorBrushPtr tsbrush = make_shared<SolidColorBrush>(Colors::White);

	tb->Foreground = tsbrush;

	tb->FontSize = 12;

	tb->Text = L"²âÊÔ";

	tb->SetVerticalAlignment(VerticalAlignment::Center);

	tb->SetHorizontalAlignment(HorizontalAlignment::Center);

	tb->SetEffect(dse);

	panel->AddChild(tb);
	
	win->Content = panel;

	Sleep(1000);

	win->Show();

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}








