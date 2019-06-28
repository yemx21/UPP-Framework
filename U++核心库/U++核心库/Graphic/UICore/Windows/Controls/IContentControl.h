#pragma once
#include "..\Visual.h"
namespace System
{
	namespace UI
	{
		namespace Windows
		{
			namespace Controls
			{
				class UICORE_API IContentControl
				{
				public:
					virtual VisualPtr GetContent() const = 0;
					virtual void SetContent(const VisualPtr& content) = 0;
				};
			}
		}
	}
}