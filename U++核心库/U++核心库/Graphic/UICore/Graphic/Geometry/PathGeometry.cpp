#include "PathGeometry.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "LineSegment.h"
#include "BezierSegment.h"
#include "ArcSegment.h"
#include "QuadraticBezierSegment.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;
using namespace std;

const DependencyPropertyPtr PathGeometry::FillModeProperty = make_dependencyproperty<PathGeometry>();

__usctor(PathGeometry)
{
	PathGeometry::FillModeProperty.get()->Register<PathGeometry>(L"FillMode",
		FrameworkPropertyMetadata::Create<Graphic::FillMode>((Graphic::FillMode)Graphic::FillMode::Alternate, FrameworkPropertyMetadataOptions::AffectsRender, &FillModeChanged));
	return true;
}


bool PathGeometry::FillModeChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast<PathGeometry*>(ptr);
	auto newValue = changedArgs.NewValue<Graphic::FillMode>();
	if (me->m_GraphicFactory)
	{
		FeatureObjectPtr obj = me->GetResource(me->m_GraphicFactory);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (me->m_GraphicFactory->featureLevel)
				{
				case FeatureLevel::D2D1WithD3D11:
					return me->m_GraphicFactory->Mgr8->Run.PathGeometry.SetFillMode(gobj, newValue) == GraphicError::OK;
					break;
				}
			}
		}
	}
	return true;
}

void PathGeometry::OnFiguresChanged(PathGeometry* sender, std::shared_ptr<ObservableCollection<PathFigurePtr>> PathFigures, NotifyCollectionChangedEventArgs<PathFigurePtr> args)
{
	auto me = static_cast<PathGeometry*>(sender);
	if (me->m_GraphicFactory)
	{
		FeatureObjectPtr obj = me->GetResource(me->m_GraphicFactory);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (me->m_GraphicFactory->featureLevel)
				{
				case FeatureLevel::D2D1WithD3D11:
					me->m_GraphicFactory->Mgr8->Run.PathGeometry.SetFigures(gobj, PathFigures->address, PathFigures->size);
					break;
				}
			}
		}
	}
}

PathGeometry::PathGeometry(void)
{
	FiguresChangedToken = Figures.CollectionChanged += (bind(&PathGeometry::OnFiguresChanged, this, std::placeholders::_1, std::placeholders::_2));
}


PathGeometry::~PathGeometry(void)
{
	Figures.CollectionChanged -= FiguresChangedToken;
}

FeatureObjectPtr PathGeometry::OnCreateResource(GraphicFactory* GraphicFactory)
{
	if (!GraphicFactory) return NULL;

	Result<GraphicObject, GraphicFactoryError> result = GraphicFactory->InternalCreatePathGeometry(this->FillMode);
	if (result.State == GraphicFactoryError::OK)
	{
		switch (GraphicFactory->featureLevel)
		{
		case FeatureLevel::D2D1WithD3D11:
			GraphicFactory->Mgr8->Run.PathGeometry.SetFigures(result.Value, Figures.address, Figures.size);
			break;
		}
		return make_shared<FeatureObject>(GraphicObjectTypes::PathGeometry, result.Value, GraphicFactory->featureLevel);
	}
	return NULL;
}

void PathGeometry::Clear()
{
	Figures.clear();
	CurrentFigure = nullptr;
}

Result<bool> PathGeometry::LineTo(const Point& point, bool isStroked, bool isSmoothJoin)
{
	Result<bool> result = Result<bool>(false);
	if (!CurrentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}

	CurrentFigure->Segments.add(make_shared<LineSegment>(point, isStroked, isSmoothJoin));
	result.Value = true;
	return result;
}

Result<bool> PathGeometry::ArcTo(const Point& point, const Size& size, float rotation, SweepDirection sweepDirection, ArcSize arcSize, bool isStroked, bool isSmoothJoin)
{
	Result<bool> result = Result<bool>(false);
	if (!CurrentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}

	CurrentFigure->Segments.add(make_shared<ArcSegment>(point, size, rotation, arcSize, sweepDirection, isStroked, isSmoothJoin));
	result.Value = true;
	return result;
}

Result<bool> PathGeometry::BezierTo(const Point& p1, const Point& p2, const Point& p3, bool isStroked, bool isSmoothJoin)
{
	Result<bool> result = Result<bool>(false);
	if (!CurrentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}

	CurrentFigure->Segments.add(make_shared<BezierSegment>(p1, p2, p3, isStroked, isSmoothJoin));
	result.Value = true;
	return result;
}

Result<bool> PathGeometry::QuadraticBezierTo(const Point& p1, const Point& p2, bool isStroked, bool isSmoothJoin)
{
	Result<bool> result = Result<bool>(false);
	if (!CurrentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}

	CurrentFigure->Segments.add(make_shared<QuadraticBezierSegment>(p1, p2, isStroked, isSmoothJoin));
	result.Value = true;
	return result;
}

Result<bool> PathGeometry::BeginFigure(const Point& start, bool filled, bool closed)
{
	Result<bool> result = Result<bool>(false);
	if (CurrentFigure)
	{
		result.WriteMessage(L"EndFigure not called");
		return result;
	}
	CurrentFigure = unique_ptr<PathFigure>(new PathFigure(start, filled, closed));
	result.Value = true;
	return result;
}

Result<bool> PathGeometry::EndFigure()
{
	Result<bool> result = Result<bool>(false);
	if (!CurrentFigure)
	{
		result.WriteMessage(L"BeginFigure not called");
		return result;
	}

	Figures.add(move(CurrentFigure));

	result.Value = true;
	return result;
}

Graphic::FillMode PathGeometry::GetFillMode() const
{
	return GetValue<Graphic::FillMode>(FillModeProperty);
}

void PathGeometry::SetFillMode(Graphic::FillMode mode)
{
	SetValue(FillModeProperty, mode);
}


class PathParser
{
private:
	wstring         m_pathData;
	size_t          m_currentPosition;
	bool            m_hasFigureBegan;
	Point           m_lastStartingPoint;
	Point           m_lastPoint;
	Point           m_secondToLastPoint;
	wchar_t          m_currentCommand;
	PathGeometryPtr  m_pathGeometry;
private:
	bool HasMoreToRead()
	{
		return m_currentPosition < m_pathData.size();
	}

	bool SkipWhiteSpace(bool commaPermitted)
	{
		bool foundComma = false;

		while (HasMoreToRead())
		{
			wchar_t ch = m_pathData[m_currentPosition];

			switch (ch)
			{
			case ' ':
			case '\n':
			case '\r':
			case '\t':
				break;
			case ',':
				if (commaPermitted)
				{
					foundComma = true;
					commaPermitted = false;
				}
				else
					throw exception("parsing error");
				break;

			default:
				if (!iswspace(ch))
					return foundComma;
			}

			m_currentPosition++;
		}

		return foundComma;
	}

	bool ReadNextToken()
	{
		SkipWhiteSpace(false);

		if (HasMoreToRead())
		{
			m_currentCommand = m_pathData[m_currentPosition++];

			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsNumeric(bool commaPermitted)
	{
		bool commaFound = SkipWhiteSpace(commaPermitted);

		if (HasMoreToRead())
		{
			m_currentCommand = m_pathData[m_currentPosition];

			if ((m_currentCommand == '.') ||
				(m_currentCommand == '-') ||
				(m_currentCommand == '+') ||
				((m_currentCommand >= '0') && (m_currentCommand <= '9')))
			{
				return true;
			}
		}

		if (commaFound)
		{
			throw exception("parsing error");
		}

		return false;
	}

	void SkipDigits(bool signPermitted)
	{
		if (signPermitted && HasMoreToRead() &&
			((m_pathData[m_currentPosition] == '-') || m_pathData[m_currentPosition] == '+'))
		{
			m_currentPosition++;
		}

		while (HasMoreToRead() && (m_pathData[m_currentPosition] >= '0') &&
			(m_pathData[m_currentPosition] <= '9'))
		{
			m_currentPosition++;
		}
	}

	float ParseNumber(bool commaPermitted)
	{
		if (!IsNumeric(commaPermitted))
			throw exception("bad token");

		bool simple = true;
		size_t start = m_currentPosition;

		if (HasMoreToRead() && ((m_pathData[m_currentPosition] == '-') || m_pathData[m_currentPosition] == '+'))
		{
			m_currentPosition++;
		}

		SkipDigits(false);

		if (HasMoreToRead() && (m_pathData[m_currentPosition] == '.'))
		{
			simple = false;
			m_currentPosition++;
			SkipDigits(false);
		}

		if (HasMoreToRead() && ((m_pathData[m_currentPosition] == 'E') ||
			(m_pathData[m_currentPosition] == 'e')))
		{
			simple = false;
			m_currentPosition++;
			SkipDigits(true);
		}

		if (simple && (m_currentPosition <= (start + 8)))
		{
			int sign = 1;

			if (m_pathData[start] == '+')
			{
				start++;
			}
			else if (m_pathData[start] == '-')
			{
				start++;
				sign = -1;
			}

			int newVal = 0;

			while (start < m_currentPosition)
			{
				newVal = newVal * 10 + (m_pathData[start] - '0');
				start++;
			}

			return (float)(newVal * sign);
		}
		else
		{
			wstring subString = m_pathData.substr(start, m_currentPosition - start);
			return (float)_wtof(subString.c_str());
		}
		return 0.0;
	}

	bool ParseBoolean()
	{
		SkipWhiteSpace(true);

		if (HasMoreToRead())
		{
			m_currentCommand = m_pathData[m_currentPosition++];

			if (m_currentCommand == '0')
			{
				return false;
			}
			else if (m_currentCommand == '1')
			{
				return true;
			}
		}

		throw exception("parse error");
	}

	Point ParsePoint(wchar_t cmd, bool commaPermitted)
	{
		double x = ParseNumber(commaPermitted);
		double y = ParseNumber(true);

		if (cmd >= 'a')
		{
			x += m_lastPoint.X;
			y += m_lastPoint.Y;
		}

		return Point(x, y);
	}

	Point ReflectPoint()
	{
		return Point(2 * m_lastPoint.X - m_secondToLastPoint.X,
			2 * m_lastPoint.Y - m_secondToLastPoint.Y);
	}

	void EnsureFigure()
	{
		if (!m_hasFigureBegan)
		{
			m_pathGeometry->BeginFigure(m_lastStartingPoint, true, true);
			m_hasFigureBegan = true;
		}
	}

public:
	void ParseToGeometryContext(const PathGeometryPtr& pathGeometry, const wstring& pathString)
	{
		m_pathGeometry = pathGeometry;
		m_pathData = pathString;

		m_secondToLastPoint = Point(0, 0);
		m_lastPoint = Point(0, 0);
		m_lastStartingPoint = Point(0, 0);

		m_hasFigureBegan = false;

		bool first = true;

		wchar_t lastCommandToken = ' ';

		while (ReadNextToken())
		{
			wchar_t cmd = m_currentCommand;

			if (first)
			{
				if ((cmd != 'M') && (cmd != 'm') && (cmd != 'F') && (cmd != 'f'))
				{
					throw exception("parse error");
				}

				first = false;
			}

			switch (cmd)
			{
			case 'f': case 'F':
			{
				float fillType = ParseNumber(false);
				FillMode fillMode = (fillType == 0) ? FillMode::Alternate : FillMode::Winding;
				m_pathGeometry->SetFillMode(fillMode);
			}
			break;
			case 'm': case 'M':

				m_lastPoint = ParsePoint(cmd, false);

				if (m_hasFigureBegan)
					pathGeometry->EndFigure();

				pathGeometry->BeginFigure(m_lastPoint, true, false);
				m_hasFigureBegan = true;
				m_lastStartingPoint = m_lastPoint;
				lastCommandToken = 'M';

				while (IsNumeric(true))
				{
					m_lastPoint = ParsePoint(cmd, false);

					pathGeometry->LineTo(m_lastPoint, true, false);
					lastCommandToken = 'L';
				}
				break;

			case 'l': case 'L':
			case 'h': case 'H':
			case 'v': case 'V':
				EnsureFigure();

				do
				{
					switch (cmd)
					{
					case 'l': m_lastPoint = ParsePoint(cmd, false); break;
					case 'L': m_lastPoint = ParsePoint(cmd, false); break;
					case 'h': m_lastPoint.X += ParseNumber(false); break;
					case 'H': m_lastPoint.X = ParseNumber(false); break;
					case 'v': m_lastPoint.Y += ParseNumber(false); break;
					case 'V': m_lastPoint.Y = ParseNumber(false); break;
					}

					pathGeometry->LineTo(m_lastPoint, true, false);
				} while (IsNumeric(true));

				lastCommandToken = 'L';
				break;

			case 'c': case 'C':
			case 's': case 'S':
				EnsureFigure();

				do
				{
					Point p;

					if ((cmd == 's') || (cmd == 'S'))
					{
						if (lastCommandToken == 'C')
						{
							p = ReflectPoint();
						}
						else
						{
							p = m_lastPoint;
						}

						m_secondToLastPoint = ParsePoint(cmd, false);
					}
					else
					{
						p = ParsePoint(cmd, false);

						m_secondToLastPoint = ParsePoint(cmd, true);
					}

					m_lastPoint = ParsePoint(cmd, true);

					pathGeometry->BezierTo(p, m_secondToLastPoint, m_lastPoint, true, false);

					lastCommandToken = 'C';
				} while (IsNumeric(true));

				break;

			case 'q': case 'Q':
			case 't': case 'T':
				EnsureFigure();

				do
				{
					if ((cmd == 't') || (cmd == 'T'))
					{
						if (lastCommandToken == 'Q')
						{
							m_secondToLastPoint = ReflectPoint();
						}
						else
						{
							m_secondToLastPoint = m_lastPoint;
						}

						m_lastPoint = ParsePoint(cmd, false);
					}
					else
					{
						m_secondToLastPoint = ParsePoint(cmd, false);
						m_lastPoint = ParsePoint(cmd, true);
					}

					pathGeometry->QuadraticBezierTo(m_secondToLastPoint, m_lastPoint, true, false);

					lastCommandToken = 'Q';
				} while (IsNumeric(true));

				break;

			case 'a': case 'A':
				EnsureFigure();

				do
				{
					float w = ParseNumber(false);
					float h = ParseNumber(true);
					float rotation = ParseNumber(true);
					bool  large = ParseBoolean();
					bool  sweep = ParseBoolean();

					m_lastPoint = ParsePoint(cmd, true);

					pathGeometry->ArcTo(m_lastPoint,
						Size(w, h),
						rotation,
						sweep ? SweepDirection::Clockwise : SweepDirection::CounterClockwise,
						large ? ArcSize::Large : ArcSize::Small);
				} while (IsNumeric(true));

				lastCommandToken = 'A';
				break;

			case 'z':
			case 'Z':
				EnsureFigure();
				pathGeometry->EndFigure();

				m_hasFigureBegan = false;
				lastCommandToken = 'Z';

				m_lastPoint = m_lastStartingPoint;
				break;

			default:
				throw exception("bad token");
				break;
			}
		}

		if (m_hasFigureBegan)
			pathGeometry->EndFigure();
	}

	PathParser() : m_currentPosition(0)
	{

	}
};

PathGeometryPtr PathGeometry::Parse(const wstring& data)
{
	try
	{
		PathGeometryPtr pg = make_shared<PathGeometry>();
		PathParser pp = PathParser();
		pp.ParseToGeometryContext(pg, data);
		return pg;
	}
	catch (...)
	{
		return nullptr;
	}
}

Result<bool, wstring> PathGeometry::Save()
{
	Result<bool, wstring> result = Result<bool, wstring>(false, wstring());

	try
	{
		result.State.push_back('F');
		result.State.push_back(FillMode == Graphic::FillMode::Alternate ? '0' : '1');
		result.State.push_back(' ');
		for (PathFigurePtr& figure : Figures)
		{
			PathFigure* fig = figure.get();
			if (fig)
			{
				result.State.push_back('M');
				result.State.push_back(' ');
				result.State.append(figure->StartPoint.ToString());
				result.State.push_back(' ');

				for (PathSegmentPtr& segment : figure->Segments)
				{
					PathSegment* seg = segment.get();
					//if (seg) seg->Decompose(result.State);
				}

				if (figure->IsClosed) result.State.push_back('Z');
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