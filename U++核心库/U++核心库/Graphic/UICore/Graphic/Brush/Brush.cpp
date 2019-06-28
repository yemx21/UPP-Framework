#include "Brush.h"
#include "..\GraphicFactory.h"
#include "StrokeStyle.h"
#include "Pen.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
#include "..\..\PreCoerceCallbackHelper.h"
using namespace System::UI;
using namespace System::UI::Graphic;

const DependencyPropertyPtr Brush::OpacityProperty = make_dependencyproperty<Brush>();
const DependencyPropertyPtr Brush::TransformProperty = make_dependencyproperty<Brush>();

__usctor(Brush)
{
	Brush::OpacityProperty.get()->Register<Brush>(L"Opacity",
		FrameworkPropertyMetadata::Create<double>(1.0, FrameworkPropertyMetadataOptions::AffectsRender | FrameworkPropertyMetadataOptions::Inherits, &OpacityChanged, __upropertycoerce(__ucoerce_replace_replace_replace, double, 1.0, 0.0, 0.0, 1.0, 1.0)));

	Brush::TransformProperty.get()->Register<Brush>(L"Transform",
		FrameworkPropertyMetadata::Create<Matrix>(Matrix::Identity(), FrameworkPropertyMetadataOptions::None | FrameworkPropertyMetadataOptions::Inherits, &TransformChanged));
	return true;
}



bool Brush::OpacityChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast< Brush*>(ptr);
	auto newValue = changedArgs.NewValue<float>();
	auto oldValue = changedArgs.OldValue<float>();

	if (me->m_renderContext)
	{
		GraphicFactory* GraphicFactory = me->m_renderContext->GetGraphicFactory();
		FeatureObjectPtr obj = me->GetResource(me->m_renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (GraphicFactory->featureLevel)
				{
					case FeatureLevel::D2D1WithD3D11:
					{
						return GraphicFactory->Mgr8->Run.Brush.SetOpacity(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

bool Brush::TransformChanged(DependencyObject* const ptr, const PropertyChangedEventArgs& changedArgs)
{
	auto me = static_cast< Brush*>(ptr);
	auto newValue = changedArgs.NewValue<Matrix>();
	auto oldValue = changedArgs.OldValue<Matrix>();
	if (me->m_renderContext)
	{
		GraphicFactory* GraphicFactory = me->m_renderContext->GetGraphicFactory();
		FeatureObjectPtr obj = me->GetResource(me->m_renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (GraphicFactory->featureLevel)
				{
					case FeatureLevel::D2D1WithD3D11:
					{
						return GraphicFactory->Mgr8->Run.Brush.SetTransform(gobj, newValue) == GraphicError::OK;
					}
					break;
				}
			}
		}
	}
	return true;
}

Brush::Brush()
{
}

FeatureObjectPtr Brush::OnCreateResource(RenderContext* renderContext)
{
	return nullptr;
}

void Brush::SetOpacity(double o)
{
	SetValue(OpacityProperty, o);
}

void Brush::SetTransform(const Matrix& transform)
{
	SetValue(TransformProperty, transform);
}

Matrix Brush::GetTransform() const
{
	return GetValue<Matrix>(TransformProperty);
}

Matrix Brush::GetTransformToGeometryBounds(const GeometryPtr& geometry, const PenPtr& pen)
{
	if (m_renderContext)
	{
		GraphicFactory* GraphicFactory = m_renderContext->GetGraphicFactory();
		FeatureObjectPtr obj = GetResource(m_renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				if (geometry)
				{
					FeatureObjectPtr gefobj = geometry->GetResource(GraphicFactory);
					if (gefobj)
					{
						GraphicObject geobj = gefobj->Get();
						if (geobj)
						{
							if (pen)
							{
								StrokeStylePtr ss = pen->StrokeStyle;
								if (ss)
								{
									FeatureObjectPtr sfobj = ss->GetResource(GraphicFactory);
									if (sfobj)
									{
										GraphicObject sobj = sfobj->Get();
										if (sobj)
										{
											switch (GraphicFactory->featureLevel)
											{
											case FeatureLevel::D2D1WithD3D11:
											{
												auto result = GraphicFactory->Mgr8->Run.Brush.GetTransformToGeometryBounds(gobj, geobj, sobj, pen->Thickness);
												if (result.State == GraphicError::OK)
												{
													return result.Value;
												}
												else
													return Matrix::Identity();
											}
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return Matrix::Identity();
}

Matrix Brush::GetTransformToGeometryBounds(const GeometryPtr& geometry, const StrokeStylePtr& strokeStyle, double thickness)
{
	if (m_renderContext)
	{
		GraphicFactory* GraphicFactory = m_renderContext->GetGraphicFactory();
		FeatureObjectPtr obj = GetResource(m_renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				if (geometry)
				{
					FeatureObjectPtr gefobj = geometry->GetResource(GraphicFactory);
					if (gefobj)
					{
						GraphicObject geobj = gefobj->Get();
						if (geobj)
						{
							if (strokeStyle)
							{
								FeatureObjectPtr sfobj = strokeStyle->GetResource(GraphicFactory);
								if (sfobj)
								{
									GraphicObject sobj = sfobj->Get();
									if (sobj)
									{
										switch (GraphicFactory->featureLevel)
										{
										case FeatureLevel::D2D1WithD3D11:
										{
											auto result = GraphicFactory->Mgr8->Run.Brush.GetTransformToGeometryBounds(gobj, geobj, sobj, thickness);
											if (result.State == GraphicError::OK)
											{
												return result.Value;
											}
											else
												return Matrix::Identity();
										}
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return Matrix::Identity();
}

Matrix Brush::GetTransformToBounds(const Rect& rect)
{
	if (m_renderContext)
	{
		GraphicFactory* GraphicFactory = m_renderContext->GetGraphicFactory();
		FeatureObjectPtr obj = GetResource(m_renderContext);
		if (obj)
		{
			GraphicObject gobj = obj->Get();
			if (gobj)
			{
				switch (GraphicFactory->featureLevel)
				{
				case FeatureLevel::D2D1WithD3D11:
				{
					auto result = GraphicFactory->Mgr8->Run.Brush.GetTransformToBounds(gobj, rect);
					if (result.State == GraphicError::OK)
					{
						return result.Value;
					}
					else
						return Matrix::Identity();
				}
					break;
				}
			}
		}
	}
	return Matrix::Identity();
}


double Brush::GetOpacity() const
{
	return GetValue<double>(OpacityProperty);
}
