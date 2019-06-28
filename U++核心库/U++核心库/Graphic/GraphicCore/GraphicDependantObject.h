#pragma once
#include "GraphicCore_Config.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			namespace Internal
			{
				struct IInvalidatable_internal;
				typedef std::weak_ptr<IInvalidatable_internal>   IInvalidatableWeakPtr_internal;
				typedef std::shared_ptr<IInvalidatable_internal> IInvalidatablePtr_internal;

				ENUM_FLAGSEX(InvalidateFlags_internal, unsigned int)
				enum class InvalidateFlags_internal : unsigned int
				{
					Measure = 0x0,
					Arrange = 0x1,
					Transform = 0x2,
					Visual = 0x4,
				};

				struct IInvalidatable_internal
				{
					virtual void Invalidate(InvalidateFlags_internal flags) = 0;
				};

				class GRAPHICCORE_API DependantObject_internal;
				typedef std::shared_ptr<DependantObject_internal> DependantObjectPtr_internal;

				class GRAPHICCORE_API DependantObject_internal
				{
				public:
					DependantObject_internal()
					{

					}

					virtual ~DependantObject_internal()
					{

					}

					std::wstring Name()
					{
						return m_name;
					}

					void Name(const std::wstring& name)
					{
						m_name = name;
					}
				protected:
					void NotifyInvalidatables(InvalidateFlags_internal flags);
				private:
					void RegisterInvalidatable(IInvalidatable_internal* invalidatable);
					void UnregisterInvalidatable(IInvalidatable_internal* invalidatable);
				private:
					std::wstring m_name;
					std::vector<IInvalidatable_internal*>  m_invalidatables;
				};
			}
		}
	}
}