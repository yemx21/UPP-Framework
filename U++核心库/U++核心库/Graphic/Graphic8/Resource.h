#pragma once
#include "Header.h"

using namespace System::UI::Graphic::Internal;

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace d2d1
			{
				class RenderContext;
				class ElementEffect;
				class ExecutableBase;
				class ScopedCommand;

				class DeviceDependantResource;
				typedef std::shared_ptr<DeviceDependantResource> DeviceDependantResourcePtr;

				class DeviceDependantResource : public DependantObject_internal
				{
				public:
					DeviceDependantResource() : m_deviceContext(nullptr), m_isDirty(true)
					{
					}
				protected:
					virtual auto IsResourceValid() const -> bool
					{
						if (m_isDirty || !m_lastResource)
							return false;

						return true;
					}

					static auto IsResourceValidFrom(const DeviceDependantResourcePtr& resource) -> bool
					{
						if (resource == nullptr)
							return false;

						return resource->IsResourceValid();
					}

					static auto IsResourceValidFrom(DeviceDependantResource* const resource) -> bool
					{
						return resource->IsResourceValid();
					}

					void Invalidate()
					{
						m_isDirty = true;
						NotifyInvalidatables(InvalidateFlags_internal::Visual);
					}

					virtual Microsoft::WRL::ComPtr<IUnknown> OnCreateResource(ID2D1DeviceContext* deviceContext) = 0;

					template<typename T>
					T* GetResourceUnsafe()
					{
						return reinterpret_cast<T*>(m_lastResource.Get());
					}

					template<typename T>
					T* GetResourceUnsafeFrom(const DeviceDependantResourcePtr& DeviceDependantResource)
					{
						return DeviceDependantResource->GetResourceUnsafe<T>();
					}

					ID2D1DeviceContext* GetDeviceContext()
					{
						return m_deviceContext;
					}

					template<typename T>
					T* GetResource(ID2D1DeviceContext* deviceContext)
					{
						SetDeviceContext(deviceContext);

						if (!IsResourceValid())
						{
							m_lastResource = OnCreateResource(m_deviceContext);
							m_isDirty = false;
						}

						return reinterpret_cast<T*>(m_lastResource.Get());
					}

					template<typename T>
					T* GetResourceFrom(const DeviceDependantResourcePtr& DeviceDependantResource, ID2D1DeviceContext* deviceContext)
					{
						return DeviceDependantResource->GetResource<T>(deviceContext);
					}

					template<typename T>
					T* GetResourceFrom(const DeviceDependantResourcePtr& DeviceDependantResource)
					{
						return DeviceDependantResource->GetResource<T>(m_deviceContext);
					}
				private:
					void SetDeviceContext(ID2D1DeviceContext* deviceContext)
					{
						if (m_deviceContext != deviceContext)
						{
							m_deviceContext = deviceContext;
							Invalidate();
						}
					}

					Microsoft::WRL::ComPtr<IUnknown>    m_lastResource;
					ID2D1DeviceContext*                 m_deviceContext;
					bool                                m_isDirty;

					friend                              RenderContext;
					friend                              ExecutableBase;
					friend                              ScopedCommand;
					friend                              ElementEffect;

				};

				class IndependantResource;
				typedef std::shared_ptr<IndependantResource> IndependantResourcePtr;

				class IndependantResource
				{
				public:
					IndependantResource() : m_isDirty(true)
					{
					}
				protected:
					virtual auto IsResourceValid() const -> bool
					{
						if (m_isDirty || !m_lastResource)
							return false;

						return true;
					}

					static auto IsResourceValid(const IndependantResourcePtr& resource) -> bool
					{
						return resource->IsResourceValid();
					}

					void Invalidate()
					{
						m_isDirty = true;
						m_lastResource.ReleaseAndGetAddressOf();
					}

					virtual Microsoft::WRL::ComPtr<IUnknown> OnCreateResource() = 0;

					template<typename T>
					T* GetResourceUnsafe()
					{
						return reinterpret_cast<T*>(m_lastResource.Get());
					}

				protected:
					template<typename T>
					T* GetResource()
					{
						if (!IsResourceValid())
						{
							m_lastResource = OnCreateResource();
							m_isDirty = false;
						}

						return reinterpret_cast<T*>(m_lastResource.Get());
					}

					template<typename T>
					T* GetResourceFrom(const IndependantResourcePtr& IndependantResource)
					{
						return IndependantResource->GetResource<T>();
					}

				private:
					Microsoft::WRL::ComPtr<IUnknown> m_lastResource;
					bool                             m_isDirty;
					friend                           RenderContext;
					friend                           ExecutableBase;
					friend                           ScopedCommand;
				};

				_inline Size Convert(D2D1_SIZE_U size)
				{
					return Size((double)size.width, (double)size.height);
				}

				_inline Size Convert(D2D1_SIZE_F size)
				{
					return Size((double)size.width, (double)size.height);
				}

				_inline Point Convert(D2D1_POINT_2F point)
				{
					return Point((double)point.x, (double)point.y);
				}

				_inline Point Convert(D2D1_POINT_2U point)
				{
					return Point((double)point.x, (double)point.y);
				}

				_inline Rect Convert(D2D1_RECT_F rect)
				{
					return Rect((double)rect.left, (double)rect.top, (double)rect.right, (double)rect.bottom);
				}

				_inline Rect Convert(D2D1_RECT_U rect)
				{
					return Rect((double)rect.left, (double)rect.top, (double)rect.right, (double)rect.bottom);
				}

				_inline Color Convert(D2D1_COLOR_F color)
				{
					return Color::FromScArgb(color.a, color.r, color.g, color.b);
				}

				_inline D2D1_SIZE_U D2DSizeU(const Size& size)
				{
					return D2D1::SizeU((UINT32)size.Width, (UINT32)size.Height);
				}

				_inline D2D1_SIZE_F D2DSize(const Size& size)
				{
					return D2D1::SizeF((float)size.Width, (float)size.Height);
				}

				_inline D2D1_COLOR_F D2DColor(const Color& color)
				{
					return D2D1::ColorF(color.ScR, color.ScG, color.ScB, color.ScA);
				}

				_inline D2D1_RECT_F D2DRect(const Rect& rect)
				{
					return D2D1::RectF((float)rect.Left, (float)rect.Top, (float)rect.Right, (float)rect.Bottom);
				}

				_inline D2D1_RECT_U D2DRectU(const Rect& rect)
				{
					return D2D1::RectU((UINT32)rect.Left, (UINT32)rect.Top, (UINT32)rect.Right, (UINT32)rect.Bottom);
				}

				_inline D2D1_POINT_2U D2DPointU(const Point& point)
				{
					return D2D1::Point2U((UINT32)point.X, (UINT32)point.Y);
				}

				_inline D2D1_POINT_2F D2DPoint(const Point& point)
				{
					return D2D1::Point2F((float)point.X, (float)point.Y);
				}

				_inline D2D1_MATRIX_3X2_F D2DMatrix(const Matrix& matrix)
				{
					return D2D1::Matrix3x2F((float)matrix.M11, (float)matrix.M12, (float)matrix.M21, (float)matrix.M22, (float)matrix.M31, (float)matrix.M32);
				}



			}
		}
	}
}