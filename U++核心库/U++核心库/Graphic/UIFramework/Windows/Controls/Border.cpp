#include "Border.h"
using namespace System::UI;
using namespace System::UI::Windows;
using namespace System::UI::Windows::Controls;

const DependencyPropertyPtr Border::BackgroundProperty = make_dependencyproperty<Border>();

const DependencyPropertyPtr Border::BorderBrushProperty = make_dependencyproperty<Border>();

const DependencyPropertyPtr Border::StrokeStyleProperty = make_dependencyproperty<Border>();

const DependencyPropertyPtr Border::PaddingProperty = make_dependencyproperty<Border>();

const DependencyPropertyPtr Border::CornerRadiusProperty = make_dependencyproperty<Border>();

const DependencyPropertyPtr Border::BorderThicknessProperty = make_dependencyproperty<Border>();

__usctor(Border)
{
	Border::BackgroundProperty.get()->Register<Border>(L"Background", FrameworkPropertyMetadata::Create<BrushPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));

	Border::BorderBrushProperty.get()->Register<Border>(L"Foreground", FrameworkPropertyMetadata::Create<BrushPtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));

	Border::StrokeStyleProperty.get()->Register<Border>(L"StrokeStyle", FrameworkPropertyMetadata::Create<StrokeStylePtr>(nullptr, FrameworkPropertyMetadataOptions::AffectsRender));

	Border::PaddingProperty.get()->Register<Border>(L"Padding", FrameworkPropertyMetadata::Create<Thickness>(Thickness(), FrameworkPropertyMetadataOptions::AffectsRender));

	Border::CornerRadiusProperty.get()->Register<Border>(L"CornerRadius", FrameworkPropertyMetadata::Create<System::CornerRadius>(System::CornerRadius{}, FrameworkPropertyMetadataOptions::AffectsRender));

	Border::BorderThicknessProperty.get()->Register<Border>(L"BorderThickness", FrameworkPropertyMetadata::Create<double>(1.0, FrameworkPropertyMetadataOptions::AffectsRender));


	return true;
}


Border::Border(void)
{
}


Border::~Border(void)
{
}

bool Border::HitTest(const Point& point)
{
	if (m_pathGeometry) return m_pathGeometry->HitTest(point);
	return false;
}

void Border::RenderOverride(const RenderContextPtr& renderContext)
{
	auto renderSize = GetRenderSize();

	if ((!m_pathGeometry)|| m_lastSize != renderSize)
		CreateGeometry();

	m_lastSize = renderSize;

	if (m_pathGeometry)
	{
		renderContext->DrawGeometry(m_pathGeometry, Background, BorderBrush, StrokeStyle, BorderThickness);
	}
}

System::Size Border::MeasureOverride(const System::Size& availableSize)
{
	int childCount = GetVisualChildCount();

	if (childCount == 0)
		return System::Size(0, 0);

	auto borderSize = System::Size(BorderThickness + BorderThickness,
		BorderThickness + BorderThickness);

	auto pad = Padding;

	auto paddingSize = System::Size(pad.Left + pad.Right, pad.Top + pad.Bottom);

	System::Size returnSize;

	if (GetVisualChildCount() > 0)
	{
		auto totalMargin = System::Size(borderSize.Width + paddingSize.Width,
			borderSize.Height + paddingSize.Height);

		auto childAvailable = System::Size(max(0.0, availableSize.Width - totalMargin.Width),
			max(0.0, availableSize.Height - totalMargin.Height));

		auto visualChild = Child;

		visualChild->Measure(childAvailable);

		auto childMeasure = visualChild->DesiredSize;

		returnSize.Width = childMeasure.Width + totalMargin.Width;
		returnSize.Height = childMeasure.Height + totalMargin.Height;
	}
	else
	{
		returnSize = System::Size(borderSize.Width + paddingSize.Width,
			borderSize.Height + paddingSize.Height);
	}

	return returnSize;
}

Size Border::ArrangeOverride(const System::Size& finalSize)
{
	int childCount = GetVisualChildCount();

	if (childCount == 0)
		return finalSize;

	auto visualChild = GetVisualChild(0);

	auto elementRect = Rect(0, 0, finalSize.Width, finalSize.Height);
	Rect contentRect;

	contentRect.Left = elementRect.Left + BorderThickness;
	contentRect.Top = elementRect.Top + BorderThickness;
	contentRect.Right = System::ComparisonHelper::Max(0.0, elementRect.Right - BorderThickness);
	contentRect.Bottom = System::ComparisonHelper::Max(0.0, elementRect.Bottom - BorderThickness);

	auto pad = Padding;

	contentRect.Left = contentRect.Left + pad.Left;
	contentRect.Top = contentRect.Top + pad.Top;
	contentRect.Right = System::ComparisonHelper::Max(0.0, contentRect.Right - pad.Left);
	contentRect.Bottom = System::ComparisonHelper::Max(0.0, contentRect.Bottom - pad.Top);


	visualChild->Arrange(contentRect);

	return finalSize;
}

void Border::CreateGeometry()
{
	auto renderSize = GetRenderSize();

	System::CornerRadius cr = CornerRadius;
	double stride = BorderThickness * 0.5;
	
	auto rect = Rect(0, 0, renderSize.Width, renderSize.Height);

	Point p1 = Point(cr.TopLeft + stride, stride);

	Point p2 = Point(rect.Width- cr.TopRight - stride, p1.Y);

	Point p3 = Point(rect.Width - stride, cr.TopRight + stride);

	Point p4 = Point(p3.X, rect.Height - cr.BottomRight - stride);

	Point p5 = Point(rect.Width - cr.BottomRight - stride, rect.Height - stride);

	Point p6 = Point(cr.BottomLeft + stride, p5.Y);

	Point p7 = Point(stride, rect.Height-stride- cr.BottomLeft);

	Point p8 = Point(p7.X, stride + cr.TopLeft);

	m_pathGeometry = nullptr;

	if (true)
	{
		/* TOP */
		m_pathGeometry = make_shared<PathGeometry>();

		m_pathGeometry->BeginFigure(p1, true, true);

		m_pathGeometry->LineTo(p2);

		double radiusX = p3.X-p2.X;
		double radiusY = p3.Y-p2.Y;
		if (0 != radiusX || 0 != radiusY)
		{
			m_pathGeometry->ArcTo(p3,
				System::Size(radiusX, radiusY),
				0,
				SweepDirection::CounterClockwise,
				ArcSize::Small);
		}

		/* RIGHT */
		m_pathGeometry->LineTo(p4);

		/* BOTTOM RIGHT CORNER */
		radiusX = p4.X - p5.X;
		radiusY = p5.Y - p4.Y;
		if (0 != radiusX || 0 != radiusY)
		{
			m_pathGeometry->ArcTo(p5,
				System::Size(radiusX, radiusY),
				0,
				SweepDirection::CounterClockwise,
				ArcSize::Small);
		}

		/* BOTTOM */
		m_pathGeometry->LineTo(p6);

		/* BOTTOM LEFT CORNER */
		radiusX = p6.X - p7.X;
		radiusY = p6.Y - p7.Y;
		if (0 != radiusX || 0 != radiusY)
		{
			m_pathGeometry->ArcTo(p7,
				System::Size(radiusX, radiusY),
				0,
				SweepDirection::CounterClockwise,
				ArcSize::Small);
		}

		/* LEFT */
		m_pathGeometry->LineTo(p8);

		/* TOP LEFT CORNER */
		radiusX = p1.X - p8.X;
		radiusY = p8.Y - p1.Y;
		if (0 != radiusX || 0 != radiusY)
		{
			m_pathGeometry->ArcTo(p1,
				System::Size(radiusX, radiusY),
				0,
				SweepDirection::CounterClockwise,
				ArcSize::Small);
		}

		m_pathGeometry->EndFigure();
	}
}

void Border::SetBorderBrush(const BrushPtr& borderBrush)
{
	SetValue(BorderBrushProperty, borderBrush);
}

const BrushPtr Border::GetBorderBrush() const
{
	return GetValuePtr<Brush>(BorderBrushProperty);
}

void Border::SetBackground(const BrushPtr& backgroundBrush)
{
	SetValue(BackgroundProperty, backgroundBrush);
}

const BrushPtr Border::GetBackground() const
{
	return GetValuePtr<Brush>(BackgroundProperty);
}

const StrokeStylePtr Border::GetStrokeStyle() const
{
	return GetValuePtr<Graphic::StrokeStyle>(StrokeStyleProperty);	
}

void Border::SetStrokeStyle(const StrokeStylePtr& strokeStyle)
{
	SetValue(StrokeStyleProperty, strokeStyle);
}

VisualPtr Border::GetContent() const
{
	auto childCount = GetVisualChildCount();

	if (childCount > 0)
	{
		return GetVisualChild(0);
	}
	return nullptr;
}
void Border::SetContent(const VisualPtr& content)
{
	auto childCount = GetVisualChildCount();

	if (childCount > 0)
		RemoveAllChildren();

	AddVisualChild(content);
}

System::CornerRadius Border::GetCornerRadius() const
{
	return GetValue<System::CornerRadius>(CornerRadiusProperty);
}

void Border::SetCornerRadius(const System::CornerRadius& val)
{
	SetValue(CornerRadiusProperty, val);
}

double Border::GetBorderThickness() const
{
	return GetValue<double>(BorderThicknessProperty);
}

void Border::SetBorderThickness(double val)
{
	SetValue(PaddingProperty, val);
}

Thickness Border::GetPadding() const
{
	return GetValue<Thickness>(PaddingProperty);
}

void Border::SetPadding(const Thickness& thickness)
{
	SetValue(PaddingProperty, thickness);
}
