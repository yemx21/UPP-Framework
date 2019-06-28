#include "VisualTreeHelper.h"

using namespace System::UI;
using namespace System::UI::Windows;

VisualPtr VisualTreeHelper::GetSameParent(const VisualPtr& v1, const VisualPtr& v2)
{
	VisualPtr res = nullptr;

	return nullptr;
}

VisualPtr VisualTreeHelper::FindRoot(const VisualPtr& visual)
{
	VisualPtr root = nullptr;

	auto ancestor = visual->m_parentElement;

	while (ancestor)
	{
		if (ancestor->m_isRoot)
		{
			root = ancestor;
			break;
		}
		ancestor = ancestor->m_parentElement;
	}

	VisualPtr res = VisualPtr(root);
	return res;
}

bool VisualTreeHelper::IsContainingFromTree(const VisualPtr& root, const VisualPtr& child)
{
	for (auto& item: root->m_children)
	{
		if (item != child)
		{
			if (item->m_children.size())
			{
				bool res = VisualTreeHelper::IsContainingFromTree(item, child);
				if (res)
				{
					return true;
				}
			}
		}
	}
	return false;
}

VisualPtr VisualTreeHelper::FindChildFromTree(const VisualPtr& root, const wchar_t*)
{
	return nullptr;
}