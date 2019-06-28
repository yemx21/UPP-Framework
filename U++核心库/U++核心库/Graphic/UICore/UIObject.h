#pragma once
#include "DependencyObject.h"
#include "DependantObject.h"
#include "UICore_Config.h"

namespace System
{
	namespace UI
	{
		class UIObject;
		typedef std::shared_ptr<UIObject> UIObjectPtr;

		__uclass __uobjdef(asm = UICore, ns = System::UI, ver = 1.0)  UICORE_API UIObject : public DependantObject, public DependencyObject
		{
		public:
			__udefctor UIObject();
			virtual ~UIObject();

			REFLECT_CLASS(UIObject)
		};
	}
}