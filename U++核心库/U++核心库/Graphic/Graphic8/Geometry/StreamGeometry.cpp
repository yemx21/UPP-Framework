#include "StreamGeometry.h"
#include "LineSegment.h"
#include "ArcSegment.h"
#include "BezierSegment.h"
#include "QuadraticBezierSegment.h"

using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic::d2d1;

using namespace Microsoft::WRL;
using namespace std;

StreamGeometry::StreamGeometry(void) : m_fillMode(FillMode::Alternate), m_isOpened(false)
{
}


StreamGeometry::~StreamGeometry(void)
{
}

Result<bool> StreamGeometry::Open(bool clear)
{
	Result<bool> result(false);
	if (m_isOpened) 
	{
		result.WriteMessage(L"already opened");
		return result;
	}
	else
	{
		if (clear)
		{
			m_figures.clear();
			m_currentFigure = nullptr;
		}
		auto temp = GetResource<ID2D1Geometry>();
		if (temp)
		{
			result.Value = true;
			m_isOpened = true;
		}
		else
		{
			result.WriteMessage(L"can not open");
		}
	}
	return result;
}

bool StreamGeometry::GetIsOpened() const
{
	return m_isOpened;
}

Result<bool> StreamGeometry::Close(bool clear)
{
	Result<bool> result(false);
	if (!m_isOpened)
	{
		result.WriteMessage(L"open has not been successively called");
	}
	else
	{
		m_isOpened = false;
		if (clear)
		{
			m_figures.clear();
			m_currentFigure = nullptr;
			Invalidate();
		}
	}
	return result;
}

Microsoft::WRL::ComPtr<ID2D1Geometry> StreamGeometry::OnCreateGeometry()
{
	ComPtr<ID2D1PathGeometry> StreamGeometry;
	ComPtr<ID2D1GeometrySink> geometrySink;

	m_Factory->CreatePathGeometry(&StreamGeometry);

	StreamGeometry->Open(&geometrySink);
	geometrySink->SetFillMode((D2D1_FILL_MODE)(UInt)m_fillMode);

	for (auto figure : m_figures)
	{
		figure->Create(geometrySink);
	}

	HRESULT hr = geometrySink->Close();
	if (hr != S_OK)
	{
		geometrySink = nullptr;
		StreamGeometry = nullptr;
		return nullptr;
	}
	return StreamGeometry;
}

void StreamGeometry::Clear()
{
	m_figures.clear();
	m_currentFigure = nullptr;
	Invalidate();
}

Result<bool> StreamGeometry::LineTo(const Point& point, bool isStroked, bool isSmoothJoin)
{
	Result<bool> result = Result<bool>(false);
	if (!m_isOpened)
	{
		result.WriteMessage(L"open has not been successively called");
		return result;
	}

	if (!m_currentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}

	m_currentFigure->AddSegment(make_shared<LineSegment>(point), isStroked, isSmoothJoin);
	result.Value = true;
	return result;
}

Result<bool> StreamGeometry::ArcTo(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked, bool isSmoothJoin)
{
	Result<bool> result = Result<bool>(false);
	if (!m_isOpened)
	{
		result.WriteMessage(L"open has not been successively called");
		return result;
	}

	if (!m_currentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}
	m_currentFigure->AddSegment(make_shared<ArcSegment>(point, size, rotation, sweepDirection, arcSize), isStroked, isSmoothJoin);
	result.Value = true;
	return result;
}

Result<bool> StreamGeometry::BezierTo(const Point& p1, const Point& p2, const Point& p3, bool isStroked, bool isSmoothJoin)
{
	Result<bool> result = Result<bool>(false);
	if (!m_isOpened)
	{
		result.WriteMessage(L"open has not been successively called");
		return result;
	}

	if (!m_currentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}
	m_currentFigure->AddSegment(make_shared<BezierSegment>(p1, p2, p3), isStroked, isSmoothJoin);
	result.Value = true;
	return result;
}

Result<bool> StreamGeometry::QuadraticBezierTo(const Point& point1, const Point& point2, bool isStroked, bool isSmoothJoin)
{
	Result<bool> result = Result<bool>(false);
	if (!m_isOpened)
	{
		result.WriteMessage(L"open has not been successively called");
		return result;
	}

	if (!m_currentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}
	m_currentFigure->AddSegment(make_shared<QuadraticBezierSegment>(point1, point2), isStroked, isSmoothJoin);
	result.Value = true;
	return result;
}

Result<bool> StreamGeometry::BeginFigure(const Point& start, bool filled, bool closed)
{
	Result<bool> result = Result<bool>(false);
	if (!m_isOpened)
	{
		result.WriteMessage(L"open has not been successively called");
		return result;
	}

	if (!m_currentFigure)
	{
		result.WriteMessage(L"EndFigure not called");
		return result;
	}
	m_currentFigure = unique_ptr<Figure>(new Figure(start, filled, closed));
	result.Value = true;
	return result;
}

Result<bool> StreamGeometry::EndFigure()
{
	Result<bool> result = Result<bool>(false);
	if (!m_isOpened)
	{
		result.WriteMessage(L"open has not been successively called");
		return result;
	}

	if (!m_currentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}


	m_figures.push_back(move(m_currentFigure));

	Invalidate();
	result.Value = true;
	return result;
}

void StreamGeometry::FillMode(Graphic::FillMode mode)
{
	if (m_fillMode == mode)
		return;

	m_fillMode = mode;
	Invalidate();
}

Graphic::FillMode StreamGeometry::FillMode()
{
	return m_fillMode;
}


Result<bool, wstring> StreamGeometry::Decompose()
{
	Result<bool, wstring> result = Result<bool, wstring>(false, wstring());

	try
	{
		result.State.push_back('F');
		result.State.push_back(this->m_fillMode == Graphic::FillMode::Alternate ? '0' : '1');
		result.State.push_back(' ');
		for (FigurePtr figure : m_figures)
		{
			Figure* fig = figure.get();
			if (fig)
			{
				result.State.push_back('M');
				result.State.push_back(' ');
				result.State.append(figure->m_StartPoint.ToString());
				result.State.push_back(' ');

				for (Figure::SegmentData segment : figure->m_Segments)
				{
					ISegmentPtr segPtr = segment.segment;
					ISegment* seg = segPtr.get();
					if (seg) seg->Decompose(result.State);
				}

				if (figure->m_closed) result.State.push_back('Z');
			}
		}
		result.Value = true;
	}
	catch (...)
	{
		result.WriteMessage(L"error while decomposing");
	}

	return result;
}