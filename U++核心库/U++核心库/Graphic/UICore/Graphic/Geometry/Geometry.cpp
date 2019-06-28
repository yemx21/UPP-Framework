#include "Geometry.h"
#include "..\Brush\StrokeStyle.h"
#include "..\GraphicFactory.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;
Geometry::Geometry() 
{
    
}

bool Geometry::HitTest(const Point& point)
{
	return HitTest(point, Matrix::Identity());
}

bool Geometry::HitTestStroke(const Point& point, const StrokeStylePtr& strokeStyle, float thickness)
{
	return HitTestStroke(point, strokeStyle, thickness, Matrix::Identity());
}

bool Geometry::HitTest(const Point& point, const Matrix& matrix)
{
	if (m_GraphicFactory)
	{
		FeatureObjectPtr obj = GetResource(m_GraphicFactory);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (m_GraphicFactory->featureLevel)
				{
					case FeatureLevel::D2D1WithD3D11:
					{
						Result<bool, GraphicError> result = m_GraphicFactory->Mgr8->Run.Geometry.HitTest(gobj, point, matrix);
						if (result.State == GraphicError::OK)
						{
							return result.Value;
						}
					}
					break;
				}
			}
		}
	}
	return false;
}

bool Geometry::HitTestStroke(const Point& point, const StrokeStylePtr& strokeStyle, float thickness, const Matrix& matrix)
{
	if (m_GraphicFactory)
	{
		if (strokeStyle)
		{
			FeatureObjectPtr obj = GetResource(m_GraphicFactory);
			if (obj)
			{
				GraphicObject gobj = obj->Get();
				if (gobj)
				{
					FeatureObjectPtr obj1 = strokeStyle->GetResource(m_GraphicFactory);
					if (obj1)
					{
						GraphicObject gobj1 = obj1->Get();
						if (gobj1)
						{
							switch (m_GraphicFactory->featureLevel)
							{
							case FeatureLevel::D2D1WithD3D11:
							{
								Result<bool, GraphicError> result = m_GraphicFactory->Mgr8->Run.Geometry.HitTestStroke(gobj, point, gobj1, thickness, matrix);
								if (result.State == GraphicError::OK)
								{
									return result.Value;
								}
							}
								break;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

FeatureObjectPtr Geometry::OnCreateResource(GraphicFactory* GraphicFactory)
{

	return NULL;
}

Rect Geometry::GetBounds(const StrokeStylePtr& strokeStyle, double thickness, const Matrix& worldTransform)
{
	if (m_GraphicFactory)
	{
		if (strokeStyle)
		{
			FeatureObjectPtr obj = GetResource(m_GraphicFactory);
			if (obj)
			{
				GraphicObject gobj = obj->Get();
				if (gobj)
				{
					FeatureObjectPtr obj1 = strokeStyle->GetResource(m_GraphicFactory);
					if (obj1)
					{
						GraphicObject gobj1 = obj1->Get();
						if (gobj1)
						{
							switch (m_GraphicFactory->featureLevel)
							{
							case FeatureLevel::D2D1WithD3D11:
							{
								Result<Rect, GraphicError> result = m_GraphicFactory->Mgr8->Run.Geometry.GetBounds(gobj, gobj1, (float)thickness, worldTransform);
								if (result.State == GraphicError::OK)
								{
									return result.Value;
								}
							}
								break;
							}
						}
					}
				}
			}
		}
	}
	return Rect::Unknown();

}

bool Geometry::PushTransform( const TransformPtr& transform )
{
	if (m_GraphicFactory)
	{
		FeatureObjectPtr obj = GetResource(m_GraphicFactory);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (m_GraphicFactory->featureLevel)
				{
					case FeatureLevel::D2D1WithD3D11:
					{
						return m_GraphicFactory->Mgr8->Run.Geometry.PushTransform(gobj, transform) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return false;
}