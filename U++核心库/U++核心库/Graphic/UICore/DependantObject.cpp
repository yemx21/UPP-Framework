#include "DependantObject.h"
#include <algorithm>

using namespace System::UI;
using namespace std;

DependantObject::DependantObject() {}

DependantObject::~DependantObject() {}

std::wstring DependantObject::GetName() const
{
	return name;
}

void DependantObject::SetName(const std::wstring& name)
{
	this->name = name;
}

void DependantObject::NotifyInvalidatables(InvalidateFlags flags)
{
	for (auto invalidatableItem : invalidatables)
	{
		invalidatableItem->Invalidate(flags);
	}
}

void DependantObject::RegisterInvalidatable(IInvalidatable* invalidatable)
{
	invalidatables.push_back(invalidatable);
}

void DependantObject::UnregisterInvalidatable(IInvalidatable* invalidatable)
{
	if (!invalidatable || invalidatables.empty())
		return;

	invalidatables.erase(remove_if(invalidatables.begin(), invalidatables.end(),
		[invalidatable](IInvalidatable* const invalidatableItem)
	{
		if (invalidatable == invalidatableItem)
		{
			return true;
		}

		return false;
	}), invalidatables.end());
}
