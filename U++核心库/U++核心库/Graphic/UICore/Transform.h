#pragma once
#include "UIObject.h"
#include "UICore_Config.h"
namespace System
{
	namespace UI
	{
		namespace Media
		{
			class Transform;
			typedef std::shared_ptr<Transform> TransformPtr;

			__uclass __uobjdef(asm = UICore, ns = System::UI::Media, ver = 1.0) UICORE_API Transform : public UIObject, public IObservableObject<Transform>
			{
			public:
				Transform();
				virtual ~Transform();

				static TransformPtr Identity();

			public:
				virtual operator Matrix() = 0;

				__uref virtual Matrix GetTransformMatrix() = 0;

				REFLECT_CLASS(Transform)
			};
		}
	}
}
