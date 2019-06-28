#include "GraphicDependantObject.h"
using namespace System::UI::Graphic::Internal;

void DependantObject_internal::NotifyInvalidatables(InvalidateFlags_internal flags)
{
	for (auto invalidatableItem : m_invalidatables)
	{
		invalidatableItem->Invalidate(flags);
	}
}

void DependantObject_internal::RegisterInvalidatable(IInvalidatable_internal* invalidatable)
{
	m_invalidatables.push_back(invalidatable);
}

void DependantObject_internal::UnregisterInvalidatable(IInvalidatable_internal* invalidatable)
{
	if (!invalidatable || m_invalidatables.empty())
		return;

	m_invalidatables.erase(remove_if(m_invalidatables.begin(), m_invalidatables.end(),
		[invalidatable](IInvalidatable_internal* const invalidatableItem)
	{
		if (invalidatable == invalidatableItem)
		{
			return true;
		}

		return false;
	}), m_invalidatables.end());
}