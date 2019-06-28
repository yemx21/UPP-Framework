#pragma once
#include "..\Resource.h"
namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			class PathSegment;
			typedef std::shared_ptr<PathSegment> PathSegmentPtr;


			__uclass __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API PathSegment : public UIObject, public IObservableObject < PathSegment >
			{
			protected:
				PathSegment();
				PathSegment(bool isStroked, bool isSmoothJoin);
				virtual ~PathSegment();
			protected:
				virtual void Updated(const DependencyPropertyPtr& dep, const PropertyChangedEventArgs& args) override;
				virtual bool OnNotifyChanged(const wstring& notifyname) override;
			public:
				static const DependencyPropertyPtr IsStrokedProperty;
				static const DependencyPropertyPtr IsSmoothJoinProperty;

				bool GetIsStroked() const;
				void SetIsStroked(bool);

				bool GetIsSmoothJoin() const;
				void SetIsSmoothJoin(bool);

				__uproperty(get = GetIsStroked, put = SetIsStroked) bool IsStroked;
				__uproperty(get = GetIsSmoothJoin, put = SetIsSmoothJoin) bool IsSmoothJoin;

				__usctordef(PathSegment)
				REFLECT_CLASS(PathSegment)
			};
		}
	}
}