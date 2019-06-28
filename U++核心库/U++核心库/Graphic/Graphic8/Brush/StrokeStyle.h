#pragma once
#include "..\Resource.h"
#include "GradientStopCollection.h"

using namespace System;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class StrokeStyle;
				typedef std::shared_ptr<StrokeStyle> StrokeStylePtr;

				class StrokeStyle : public IndependantResource
				{
					typedef Microsoft::WRL::ComPtr<ID2D1Factory> ID2D1FactoryPtr;

				public:
					StrokeStyle(void);
					virtual ~StrokeStyle(void);
				public:
					CapStyle StartCap() const;
					void StartCap(CapStyle val);

					CapStyle EndCap() const;
					void EndCap(CapStyle val);

					CapStyle DashCap() const;
					void DashCap(CapStyle val);

					LineJoin LineJoinStyle() const;
					void LineJoinStyle(LineJoin val);

					float MiterLimit() const;
					void MiterLimit(float val) { m_miterLimit = val; }

					DashStyle LineDashStyle() const;
					void LineDashStyle(DashStyle val) { m_dashStyle = val; }

					float DashOffset() const;
					void DashOffset(float val);

					float* Dashes(unsigned int* count) const;
					void Dashes(const float* val, unsigned int count);

				protected:
					virtual Microsoft::WRL::ComPtr<IUnknown> OnCreateResource() override;
				private:
					CapStyle             m_startCap;
					CapStyle             m_endCap;
					CapStyle             m_dashCap;
					LineJoin             m_lineJoin;
					float                m_miterLimit;
					DashStyle            m_dashStyle;
					float                m_dashOffset;
					float*   m_dashes;
					unsigned int m_dashCount;
					unsigned int m_dashCacheSize;
					ID2D1FactoryPtr      m_factory;
					CRITICAL_SECTION cs;
				};
			}
		}
	}
}


