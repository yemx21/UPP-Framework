#pragma once
#include "ExecutableBase.h"
#include "..\Brush\Brush.h"
#include "..\Geometry\Geometry.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class ScopedCommand
				{
				protected:
					static auto IsResourceValidFrom(const DeviceDependantResourcePtr& resource) -> bool
					{
						return resource->IsResourceValid();
					}

					static auto IsResourceValidFrom(const IndependantResourcePtr& resource) -> bool
					{
						return resource->IsResourceValid();
					}

					static auto IsResourceValidFrom(DeviceDependantResource* const resource) -> bool
					{
						return resource->IsResourceValid();
					}

					static auto IsResourceValidFrom(IndependantResource* const resource) -> bool
					{
						return resource->IsResourceValid();
					}

					template<typename T>
					T* GetResourceFrom(const IndependantResourcePtr& independantResource)
					{
						return independantResource->GetResource<T>();
					}

					template<typename T>
					T* GetResourceFrom(IndependantResource* const independantResource)
					{
						return independantResource->GetResource<T>();
					}

					template<typename T>
					T* GetResourceFrom(const DeviceDependantResourcePtr& deviceDependantResource, ID2D1DeviceContext* deviceContext)
					{
						return deviceDependantResource->GetResource<T>(deviceContext);
					}

					template<typename T>
					T* GetResourceFrom(DeviceDependantResource* const deviceDependantResource, ID2D1DeviceContext* deviceContext)
					{
						return deviceDependantResource->GetResource<T>(deviceContext);
					}
				};

				class BrushTransformSwapScope : public ScopedCommand
				{
				public:
					explicit BrushTransformSwapScope() : m_pBrush(nullptr)
					{

					}

					void Enable(ID2D1Brush* brush, D2D1::Matrix3x2F& matrix)
					{
						m_pBrush = brush;
						m_pBrush->GetTransform(&m_oldTransform);
						m_pBrush->SetTransform(matrix);
					}

					~BrushTransformSwapScope()
					{
						if (m_pBrush)
							m_pBrush->SetTransform(m_oldTransform);
					}
				private:
					D2D1::Matrix3x2F m_oldTransform;
					ID2D1Brush*      m_pBrush;
				};

				class PerPrimitiveAliasingScope : ScopedCommand
				{
				public:
					explicit PerPrimitiveAliasingScope(ID2D1RenderTarget* rt) : m_RT(rt)
					{
						m_RT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
					}

					~PerPrimitiveAliasingScope()
					{
						m_RT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
					}

				private:
					ID2D1RenderTarget* m_RT;
				};

				class AliasModeScope : public ScopedCommand
				{
				public:
					explicit AliasModeScope(ID2D1RenderTarget* rt) : m_RT(rt)
					{
						m_RT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
					}

					~AliasModeScope()
					{
						m_RT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
					}

				private:
					ID2D1RenderTarget* m_RT;
				};

				class TemporaryTransformScope : public ScopedCommand
				{
				public:
					TemporaryTransformScope(ID2D1DeviceContext* rt, Brush* brush, const Matrix& tempMatrix)
						: m_RT(rt), m_Brush(brush)
					{
						m_RT->GetTransform(&m_OriginalMatrix);

						auto brushPtr = GetResourceFrom<ID2D1Brush>(m_Brush, rt);

						brushPtr->GetTransform(&m_OriginalBrushMatrix);

						D2D1_MATRIX_3X2_F matrix_temp = D2DMatrix(tempMatrix);

						D2D1_MATRIX_3X2_F tempBrushMatrix = matrix_temp;
						D2D1::Matrix3x2F::ReinterpretBaseType(&tempBrushMatrix)->Invert();

						m_RT->SetTransform(matrix_temp * m_OriginalMatrix);
						brushPtr->SetTransform(tempBrushMatrix * m_OriginalBrushMatrix);
					}

					~TemporaryTransformScope()
					{
						m_RT->SetTransform(m_OriginalMatrix);
						auto brushPtr = GetResourceFrom<ID2D1Brush>(m_Brush, m_RT);
						brushPtr->SetTransform(m_OriginalBrushMatrix);
					}

				private:
					ID2D1DeviceContext* m_RT;
					Brush*              m_Brush;
					D2D1_MATRIX_3X2_F   m_OriginalMatrix;
					D2D1_MATRIX_3X2_F   m_OriginalBrushMatrix;
				};

				class DrawCommand : public ExecutableBase
				{
				public:
					DrawCommand() : m_RT(0) {}
					virtual ~DrawCommand() {}

					void SetGeometry(Geometry* g)
					{
						m_Geometry = g;
					}
					void SetBrush(Brush* b)
					{
						m_Brush = b;
					}
					void SetRenderTarget(ID2D1DeviceContext* rt)
					{
						m_RT = rt;
					}
				protected:
					ID2D1DeviceContext*      m_RT;
					Geometry* m_Geometry;
					Brush*        m_Brush;
				};
			}
		}
	}
}