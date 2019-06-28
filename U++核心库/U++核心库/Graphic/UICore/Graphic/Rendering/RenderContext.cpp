#include "..\GraphicFactory.h"
#include "..\Brush\Brush.h"
#include "..\Brush\StrokeStyle.h"
#include "..\Geometry\Geometry.h"
#include "..\Media\Bitmap.h"
#include "..\Effect\Effect.h"
#include "..\Text\TextLayout.h"
#include "..\Media\Surface.h"
#include  "..\Feature\D2D1WithD3D11\Feature8.h"
using namespace System;
using namespace System::UI;
using namespace System::UI::Graphic;

GraphicError RenderContext::PushSurfaceTarget(const SurfacePtr& surface)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		if (surface)
		{
			Surface* suf = surface.get();
			if (suf)
			{
				FeatureObjectPtr fsuf = suf->GetResource(this);
				if (fsuf)
				{
					GraphicObject gsuf = fsuf->Get();
					if (gsuf)
					{
						GraphicObject gobj = this->Get();
						if (gobj)
						{
							switch (owner->featureLevel)
							{
							case FeatureLevel::D2D1WithD3D11:
							{
								result = owner->Mgr8->Run.RenderContext.PushSurfaceTarget(gobj, gsuf);
							}
							break;
							}
						}
					}
				}
			}
		}
	}
	return result;
}

Result<GraphicObject, GraphicError> RenderContext::GetCurrentTarget()
{
	Result<GraphicObject, GraphicError> result(NULL, GraphicError::Failed);
	GraphicObject gobj = this->Get();
	if (gobj)
	{
		switch (owner->featureLevel)
		{
		case FeatureLevel::D2D1WithD3D11:
		{
			result = owner->Mgr8->Run.RenderContext.GetCurrentTarget(gobj);
		}
		break;
		}
	}
	return result;
}

Result<Size, GraphicError> RenderContext::GetCurrentTargetSize()
{
	Result<Size, GraphicError> result(Size(), GraphicError::Failed);
	GraphicObject gobj = this->Get();
	if (gobj)
	{
		switch (owner->featureLevel)
		{
		case FeatureLevel::D2D1WithD3D11:
		{
			result = owner->Mgr8->Run.RenderContext.GetCurrentTargetSize(gobj);
		}
		break;
		}
	}
	return result;
}

GraphicError RenderContext::PushTarget(const BitmapPtr& bitmap)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		if (bitmap)
		{

			Bitmap* bmp = bitmap.get();
			if (bmp)
			{
				FeatureObjectPtr fbmp = bmp->GetResource(this);
				if (fbmp)
				{
					GraphicObject gbmp = fbmp->Get();
					if (gbmp)
					{
						GraphicObject gobj = this->Get();
						if (gobj)
						{
							switch (owner->featureLevel)
							{
							case FeatureLevel::D2D1WithD3D11:
							{
								result = owner->Mgr8->Run.RenderContext.PushTarget(gobj, gbmp);
							}
								break;
							}
						}
					}
				}
			}
		}
	}
	return result;
}

GraphicError RenderContext::PopTarget()
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.PopTarget(gobj);
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::Clear(const Color& color)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.Clear(gobj, color);
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::PushAxisAlignedClip(const Rect& clipRect, AntialiasMode mode)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.PushAxisAlignedClip(gobj, clipRect, mode);
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::PopAxisAlignedClip()
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.PopAxisAlignedClip(gobj);
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::PushGeometryClip(const GeometryPtr& geometry)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		if (geometry)
		{

			Geometry* geo = geometry.get();
			if (geo)
			{
				FeatureObjectPtr fgeo = geo->GetResource(owner);
				if (fgeo)
				{
					GraphicObject ggeo = fgeo->Get();
					if (ggeo)
					{
						GraphicObject gobj = this->Get();
						if (gobj)
						{
							switch (owner->featureLevel)
							{
							case FeatureLevel::D2D1WithD3D11:
							{
								result = owner->Mgr8->Run.RenderContext.PushTarget(gobj, ggeo);
							}
							break;
							}
						}
					}
				}
			}
		}
	}
	return result;
}

GraphicError RenderContext::PopGeometryClip()
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.PopGeometryClip(gobj);
			}
			break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::PushOpacity(float opacity)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.PushOpacity(gobj, opacity);
			}
			break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::PopOpacity()
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.PopOpacity(gobj);
			}
			break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::PushOpacityMask(const Rect& rect, const BrushPtr& brush)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		if (brush)
		{

			Brush* bb = brush.get();
			if (bb)
			{
				FeatureObjectPtr fbb = bb->GetResource(this);
				if (fbb)
				{
					GraphicObject gbb = fbb->Get();
					if (gbb)
					{
						GraphicObject gobj = this->Get();
						if (gobj)
						{
							switch (owner->featureLevel)
							{
							case FeatureLevel::D2D1WithD3D11:
							{
								result = owner->Mgr8->Run.RenderContext.PushTarget(gobj, gbb);
							}
							break;
							}
						}
					}
				}
			}
		}
	}
	return result;
}

GraphicError RenderContext::PopOpacityMask()
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.PopOpacityMask(gobj);
			}
			break;
			}
		}
	}
	return result;
}

Size RenderContext::GetMaximumImageSize()
{
	Size result = Size(8192, 8192);
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				Result<int, GraphicError> tmp = owner->Mgr8->Run.RenderContext.GetMaximumImageSize(gobj);
				if (tmp.State == GraphicError::OK)
				{
					result = Size(tmp.Value, tmp.Value);
				}
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::DrawGeometry(const GeometryPtr& geometry, const BrushPtr& brush, const BrushPtr& strokeBrush, const StrokeStylePtr& strokeStyle, double thickness)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gg = NULL;
		GraphicObject gb0 = NULL;
		GraphicObject gb1 = NULL;
		GraphicObject gss = NULL;
		if (geometry)
		{
			Geometry* g = geometry.get();
			if (g)
			{
				FeatureObjectPtr fg = g->GetResource(owner);
				if (fg)
					gg = fg->Get();
			}
		}

		if (brush)
		{
			Brush* b0 = brush.get();
			if (b0)
			{
				FeatureObjectPtr fbmp = b0->GetResource(this);
				if (fbmp)
					gb0 = fbmp->Get();
			}
		}

		if (strokeBrush)
		{
			Brush* b1 = strokeBrush.get();
			if (b1)
			{
				FeatureObjectPtr fbmp1 = b1->GetResource(this);
				if (fbmp1)
					gb1 = fbmp1->Get();
			}
		}

		if (strokeStyle)
		{
			StrokeStyle* ss = strokeStyle.get();
			if (ss)
			{
				FeatureObjectPtr fss = ss->GetResource(owner);
				if (fss)
					gss = fss->Get();
			}
		}

		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.DrawGeometry(gobj, gg, gb0, gb1, gss, (float)thickness);
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::PushTransform(const Matrix& matrix)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.SetTransform(gobj, matrix);
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::PopTransform()
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.SetTransform(gobj, Matrix::Identity());
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::DrawTextLayout(const TextLayoutPtr& textLayout, const BrushPtr& brush, const Point& origin)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gt = NULL;
		GraphicObject gb = NULL;
		if (textLayout)
		{
			TextLayout* t = textLayout.get();
			if (t)
			{
				FeatureObjectPtr ft = t->GetResource(owner);
				if (ft)
					gt = ft->Get();
			}
		}

		if (brush)
		{
			Brush* b = brush.get();
			if (b)
			{
				FeatureObjectPtr fbmp = b->GetResource(this);
				if (fbmp)
					gb = fbmp->Get();
			}
		}
		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.DrawTextLayout(gobj, gt, gb, origin);
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::DrawBitmap(const BitmapPtr& bitmap, const EffectCompositeMode compositeMode, const Size& size)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject gb = NULL;
		if (bitmap)
		{
			Bitmap* b = bitmap.get();
			if (b)
			{
				FeatureObjectPtr fb = b->GetResource(this);
				if (fb)
					gb = fb->Get();
			}
		}

		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.DrawBitmap(gobj, gb,compositeMode, size);
			}
				break;
			}
		}
	}
	return result;
}

GraphicError RenderContext::DrawEffect(const BitmapPtr& bitmap, const EffectPtr& effect, const EffectCompositeMode compositeMode, const Size& size)
{
	GraphicError result = GraphicError::Failed;
	if (owner)
	{
		GraphicObject ge = NULL;
		GraphicObject gb = NULL;
		if (bitmap)
		{
			Bitmap* b = bitmap.get();
			if (b)
			{
				FeatureObjectPtr fb = b->GetResource(this);
				if (fb)
					gb = fb->Get();
			}
		}

		if (effect)
		{
			Effect* e = effect.get();
			if (e)
			{
				FeatureObjectPtr fe = e->GetResource(owner);
				if (fe)
					ge = fe->Get();
			}
		}

		GraphicObject gobj = this->Get();
		if (gobj)
		{
			switch (owner->featureLevel)
			{
			case FeatureLevel::D2D1WithD3D11:
			{
				result = owner->Mgr8->Run.RenderContext.DrawEffect(gobj, gb, ge, compositeMode, size);
			}
				break;
			}
		}
	}
	return result;
}
