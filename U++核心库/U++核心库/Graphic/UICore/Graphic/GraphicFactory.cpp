#include "GraphicFactory.h"
#include "Feature\D2D1WithD3D11\Feature8.h"

using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

FeatureObjectPtr FeatureDependantResource::GetResource(RenderContext* renderContext)
{
	SetRenderContext(renderContext);

	if (!IsResourceValid())
	{
		m_lastResource = OnCreateResource(m_renderContext);
		if (m_lastResource)
		{
			m_lastResource->owner_.Register(m_renderContext->GetGraphicFactory());
			m_isDirty = false;
			NotifyChanged(L"Resource", L"");
		}
	}

	return m_lastResource;
}

bool FeatureOwner::DeleteObject(GraphicObjectTypes type, GraphicObject obj, FeatureLevel level)
{
	if (obj && GraphicFactory_)
	{
		Result<bool, GraphicFactoryError> result = GraphicFactory_->InternalDeleteObject(type, obj, level);
		return result.State == GraphicFactoryError::OK;
	}
	return false;
}

RenderContext::RenderContext() throw() :FeatureObject(GraphicObjectTypes::RenderContext, NULL, FeatureLevel::Invalid), owner(NULL)
{
}

RenderContext::RenderContext(GraphicObject renderContext, FeatureLevel level) throw() : FeatureObject(GraphicObjectTypes::RenderContext, renderContext, level), owner(NULL)
{
}

RenderContext::RenderContext(const RenderContext& other) throw() : FeatureObject(other.type_, other.ptr_, other.level_), owner(NULL)
{
}

GraphicFactory* RenderContext::GetGraphicFactory() const { return owner; }



Result<GraphicFactory*, GraphicFactoryError> GraphicFactory::CreateGraphicFactory(FeatureLevel level)
{
	Result<GraphicFactory*, GraphicFactoryError> result = Result<GraphicFactory*, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	try
	{
		GraphicFactory* temp = new GraphicFactory(level);
		result.Value = temp;
		result.State = GraphicFactoryError::OK;
	}
	catch (GraphicFactoryError err)
	{
		result.State = err;
	}
	return result;
}

GraphicFactory::GraphicFactory(FeatureLevel level) :featureLevel(level)
{
	OS = OSHelper::GetOSVersion();
	switch (OS)
	{
	case OSVersion::XP_0:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		throw GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		switch (level)
		{
			case FeatureLevel::D2D1WithD3D11:
			{
				try
				{
					Mgr8 = new FeatureManager8();
					Result<GraphicObject, GraphicError> result = Mgr8->Run.CreateCoreObject(GraphicObjectTypes::Device, NULL, 0.0f, 0.0f, Texture2DMode::Target, NULL);
					if (result.State == GraphicError::OK)
					{
						Dev8 = make_shared<FeatureObject>(GraphicObjectTypes::Device, result.Value, level);
					}
					else
						SAFE_DELETE(Mgr8);
				}
				catch (...)
				{
					Dev8->Reset();
					Dev8 = nullptr;
					SAFE_DELETE(Mgr8);
					throw GraphicFactoryError::Failed;
				}
			}
			break;
		}
		break;
	}
}

GraphicFactory::~GraphicFactory()
{

}

Result<RenderContext*, GraphicFactoryError> GraphicFactory::CreateRenderContext()
{
	Result<RenderContext*, GraphicFactoryError> result = Result<RenderContext*, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateCoreObject(GraphicObjectTypes::RenderContext, Dev8->Get(), 0.0f, 0.0f, Texture2DMode::Target, NULL);
					if (obj.State == GraphicError::OK)
					{
						RenderContext* rc = new RenderContext(obj.Value, featureLevel);
						rc->owner = this;
						result.Value = rc;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateBitmap(const Size& size)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateCoreObject(GraphicObjectTypes::Bitmap, NULL, (float)size.Width, (float)size.Height, Texture2DMode::Target, NULL);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateBitmap(const wchar_t* uri)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateCoreObject(GraphicObjectTypes::Bitmap, NULL, 0.0f, 0.0f, Texture2DMode::Target, uri);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateTexture2D(const Size& size, Texture2DMode mode2D)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateCoreObject(GraphicObjectTypes::Texture2D, Dev8->Get(), (float)size.Width, (float)size.Height, mode2D, nullptr);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}


Result<bool, GraphicFactoryError> GraphicFactory::InternalDeleteObject(GraphicObjectTypes type, GraphicObject obj, FeatureLevel level)
{
	Result<bool, GraphicFactoryError> result = Result<bool, GraphicFactoryError>(false, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					GraphicError error= Mgr8->Run.DeleteObject(type, obj);
					if (error == GraphicError::OK)
					{
						result.Value = true;
						result.State = GraphicFactoryError::OK;
					}
					else if (error == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (error != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}


Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateStrokeStyle(CapStyle startCap, CapStyle endCap, CapStyle dashCap, LineJoin lineJoinStyle, double miterLimit, DashStyle lineDashStyle, double dashOffset, const float* dashes, unsigned int dashCount)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateStrokeStyle(GraphicObjectTypes::StrokeStyle, startCap, endCap, dashCap, lineJoinStyle, (float)miterLimit, lineDashStyle, (float)dashOffset, dashes, dashCount);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateBitmapBrush(GraphicObject bitmap, ExtendMode modeX, ExtendMode modeY, const Rect& sourceRect, InterpolationMode interpolation)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateBrushObject(GraphicObjectTypes::BitmapBrush,Color(), bitmap, Point(),Point(), Point(), Point(), 0.0f,0.0f, modeX, modeY, sourceRect,interpolation);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateGradientStopCollection(ExtendMode mode, const ObservableCollection<GradientStop>& gradientStops)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateGradientStopCollection(GraphicObjectTypes::GradientStopCollection, gradientStops, mode);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateSolidColorBrush(Color color)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateBrushObject(GraphicObjectTypes::SolidColorBrush, color,NULL, Point::Unknown(), Point::Unknown(), Point::Unknown(), Point::Unknown(), 0.0f, 0.0f, ExtendMode::Clamp, ExtendMode::Clamp, Rect::Unknown(), InterpolationMode::Linear);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateLinearGradientBrush(GraphicObject gradientStopCollection, const Point& start, const Point& end)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateBrushObject(GraphicObjectTypes::LinearGradientBrush, Color(), gradientStopCollection, start, end, Point::Unknown(), Point::Unknown(), 0.0f, 0.0f, ExtendMode::Clamp, ExtendMode::Clamp, Rect::Unknown(), InterpolationMode::Linear);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateRadialGradientBrush(GraphicObject gradientStopCollection, const Point& center, const Point& gradientOriginOffset, double radiusX, double radiusY)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateBrushObject(GraphicObjectTypes::RadialGradientBrush, Color(), gradientStopCollection, Point::Unknown(), Point::Unknown(), center, gradientOriginOffset, (float)radiusX, (float)radiusY, ExtendMode::Clamp, ExtendMode::Clamp, Rect::Unknown(), InterpolationMode::Linear);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}


Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateTextFormat(const wchar_t* fontFamily, double fontSize, FontWeight fontWeight, FontStyle fontStyle, FontStretch fontStretch)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateTextObject(GraphicObjectTypes::TextFormat, fontFamily, (float)fontSize, Size(), fontWeight, fontStyle, fontStretch, NULL);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;

}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateTextLayout(GraphicObject textFormat, const Size& maxSize, const wchar_t* text)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateTextObject(GraphicObjectTypes::TextLayout, text, 0.0f, maxSize, FontWeight::Normal, FontStyle::Normal, FontStretch::Normal, textFormat);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateBlurEffect(double blurSize, BlurOptimization optimization, BlurBorderMode borderMode)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateEffectObject(GraphicObjectTypes::Effect, GraphicEffectTypes::BlurEffect, Color(), (float)blurSize, optimization, borderMode, false, NULL, 0.0f, 0.0f);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateDropShadowEffect(double blurSize, BlurOptimization optimization, const Color& color, const TransformPtr& transform, bool enableTransform, double direction, double shadowDepth)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateEffectObject(GraphicObjectTypes::Effect, GraphicEffectTypes::DropShadowEffect, Color(), (float)blurSize, optimization, BlurBorderMode::Soft, enableTransform, transform, (float)direction, (float)shadowDepth);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateSaturationEffect(double amount)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateEffectObject(GraphicObjectTypes::Effect, GraphicEffectTypes::SaturationEffect, Color(), (float)amount, BlurOptimization::Balanced, BlurBorderMode::Soft, false, NULL, 0.0f, 0.0f);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateEllipseGeometry(const Point& center, double radiusX, double radiusY)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateGeometryObject(GraphicObjectTypes::EllipseGeometry, center, Rect::Empty(), (float)radiusX, (float)radiusY, NULL, NULL, NULL, 0, nullptr, CombineMode::Union, FillMode::Alternate);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateRectangleGeometry(const Rect& rect)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateGeometryObject(GraphicObjectTypes::RectangleGeometry, Point::Unknown(), rect, 0.0f, 0.0f, NULL, NULL, NULL, 0, nullptr, CombineMode::Union, FillMode::Alternate);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateRoundedRectangleGeometry(const Rect& rect, double radiusX, double radiusY)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateGeometryObject(GraphicObjectTypes::RoundedRectangleGeometry, Point::Unknown(), rect, (float)radiusX, (float)radiusY, NULL, NULL, NULL, 0, nullptr, CombineMode::Union, FillMode::Alternate);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateCombinedGeometry(GraphicObject geometry1, GraphicObject geometry2, CombineMode mode)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateGeometryObject(GraphicObjectTypes::CombinedGeometry, Point::Unknown(), Rect::Empty(), 0.0f, 0.0f, geometry1, geometry2, NULL, 0, nullptr, mode, FillMode::Alternate);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreateGeometryGroup(GraphicObject* geometries, unsigned int count, FillMode mode)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateGeometryObject(GraphicObjectTypes::GeometryGroup, Point::Unknown(), Rect::Empty(), 0.0f, 0.0f, NULL, NULL, geometries, count, nullptr, CombineMode::Union, mode);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}

Result<GraphicObject, GraphicFactoryError> GraphicFactory::InternalCreatePathGeometry(FillMode mode)
{
	Result<GraphicObject, GraphicFactoryError> result = Result<GraphicObject, GraphicFactoryError>(NULL, GraphicFactoryError::Failed);
	switch (OS)
	{
	case OSVersion::XP_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::XP_3:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Vista_2:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win7_1:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_0:
		result.State = GraphicFactoryError::OSNotSupported;
		break;
	case OSVersion::Win8_1:
		try
		{
			if (Mgr8)
			{
				if (Dev8)
				{
					Result<GraphicObject, GraphicError> obj = Mgr8->Run.CreateGeometryObject(GraphicObjectTypes::PathGeometry, Point::Unknown(), Rect::Empty(), 0.0f, 0.0f, NULL, NULL, NULL, 0, nullptr, CombineMode::Union, mode);
					if (obj.State == GraphicError::OK)
					{
						result.Value = obj.Value;
						result.State = GraphicFactoryError::OK;
					}
					else if (obj.State == GraphicError::InvalidParameters)
						result.State = GraphicFactoryError::InvalidParameters;
					else if (obj.State != GraphicError::Failed)
						result.State = GraphicFactoryError::FeatureNotSupported;
				}
				else
					result.State = GraphicFactoryError::NotAvailable;
			}
			else
				result.State = GraphicFactoryError::NotAvailable;
		}
		catch (GraphicFactoryError err)
		{
			result.State = err;
		}
		break;
	}
	return result;
}