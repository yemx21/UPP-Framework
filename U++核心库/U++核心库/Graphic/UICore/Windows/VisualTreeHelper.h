#pragma once
#include "Visual.h"

namespace System
{
	namespace UI
	{
		namespace Windows
		{
			class VisualTreeHelper
			{
			public:
				static VisualPtr GetSameParent(const VisualPtr&, const VisualPtr&);
				static VisualPtr FindRoot(const VisualPtr&);
				static bool IsContainingFromTree(const VisualPtr& root, const VisualPtr& child);
				static VisualPtr FindChildFromTree(const VisualPtr& root, const wchar_t*);
			};
		}
	}
}