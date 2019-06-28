#pragma once
#include "..\UICore_Config.h"

namespace System
{
	namespace UI
	{
		namespace Graphic
		{
			struct FeatureInfo
			{
				unsigned int FeatureLevel;
				wchar_t *Version;
				wchar_t *Description;
				unsigned int OS;
			};

			typedef intptr_t GraphicObject;

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API GraphicError : __uobject
			{
				__uvalue_begin(GraphicError, UInt)
				__uvalue(OK)
				__uvalue(Failed)
				__uvalue(Unsupported)
				__uvalue(NotImplemented)
				__uvalue(InvalidParameters)
				__uvalue(CannotReleaseResource)
				__uvalue_end(GraphicError, OK)
			};

			enum class GraphicObjectTypes
			{
				Invalid,
				Device,
				RenderContext,
				Surface,
				Texture2D,
				Texture3D,
				Bitmap,
				GradientStopCollection,
				SolidColorBrush,
				LinearGradientBrush,
				RadialGradientBrush,
				BitmapBrush,
				StrokeStyle,
				Effect,
				TextLayout,
				TextFormat,
				EllipseGeometry,
				RectangleGeometry,
				RoundedRectangleGeometry,
				PathGeometry,
				StreamGeometry,
				CombinedGeometry,
				TransformedGeometry,
				GeometryGroup,
			};

			enum class GraphicEffectTypes
			{
				Invalid,
				BlurEffect,
				DropShadowEffect,
				SaturationEffect,
			};

#ifndef FEATURE_CONSTS
#define FEATURE_CONSTS
			static const wchar_t* Feature_0 = L"Graphic0.dll";
			static const wchar_t* Feature_1 = L"Graphic1.dll";
			static const wchar_t* Feature_2 = L"Graphic2.dll";
			static const wchar_t* Feature_3 = L"Graphic3.dll";
			static const wchar_t* Feature_4 = L"Graphic4.dll";
			static const wchar_t* Feature_5 = L"Graphic5.dll";
			static const wchar_t* Feature_6 = L"Graphic6.dll";
			static const wchar_t* Feature_7 = L"Graphic7.dll";
			static const wchar_t* Feature_8 = L"Graphic8.dll";
#endif


			typedef FeatureInfo* (*GetFeatureInfo)();       ///声明通用特性描述接口函数地址


#pragma region enum classes
			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API HorizontalAlignment : __uobject
			{
				__uvalue_begin(HorizontalAlignment, UInt)
				__uvalue(Left)
				__uvalue(Center)
				__uvalue(Right)
				__uvalue(Stretch)
				__uvalue_end(HorizontalAlignment, Left)
			};
			
			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API VerticalAlignment : __uobject
			{
				__uvalue_begin(VerticalAlignment, UInt)
				__uvalue(Top)
				__uvalue(Center)
				__uvalue(Bottom)
				__uvalue(Stretch)
				__uvalue_end(VerticalAlignment, Top)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API Stretch : __uobject
			{
				__uvalue_begin(Stretch, UInt)
				__uvalue(Fill)
				__uvalue(None)
				__uvalue(Uniform)
				__uvalue(UniformToFill)
				__uvalue_end(Stretch, Fill)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API SweepDirection : __uobject
			{
				__uvalue_begin(SweepDirection, UInt)
				__uvalue(Clockwise)
				__uvalue(CounterClockwise)
				__uvalue_end(SweepDirection, Clockwise)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API ArcSize : __uobject
			{
				__uvalue_begin(ArcSize, UInt)
				__uvalue(Small)
				__uvalue(Large)
				__uvalue_end(ArcSize, Small)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API StretchDirection : __uobject
			{
				__uvalue_begin(StretchDirection, UInt)
				__uvalue(Both)
				__uvalue(UpOnly)
				__uvalue(DownOnly)
				__uvalue_end(StretchDirection, Both)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API EffectCompositeMode : __uobject
			{
				__uvalue_begin(EffectCompositeMode, UInt)
				__uvalue(SourceOver)
				__uvalue(SourceIn)
				__uvalue(DestinationIn)
				__uvalue(SourceOut)
				__uvalue(DestinationOut)
				__uvalue(SourceATop)
				__uvalue(DestinationATop)
				__uvalue(XOR)
				__uvalue(Plus)
				__uvalue(SourceCopy)
				__uvalue(BoundedSourceCopy)
				__uvalue(MaskInvert)
				__uvalue_end(EffectCompositeMode, SourceOver)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API AlphaMode : __uobject
			{
				__uvalue_begin(AlphaMode, UInt)
				__uvalue(Unknown)
				__uvalue(Premultipled)
				__uvalue(Straight)
				__uvalue(Ignore)
				__uvalue_end(AlphaMode, Unknown)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API ExtendMode : __uobject
			{
				__uvalue_begin(ExtendMode, UInt)
				__uvalue(Clamp)
				__uvalue(Wrap)
				__uvalue(Mirror)
				__uvalue_end(ExtendMode, Clamp)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API CapStyle : __uobject
			{
				__uvalue_begin(CapStyle, UInt)
				__uvalue(Flat)
				__uvalue(Square)
				__uvalue(Round)
				__uvalue(Triangle)
				__uvalue_end(CapStyle, Flat)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API LineJoin : __uobject
			{
				__uvalue_begin(LineJoin, UInt)
				__uvalue(Miter)
				__uvalue(Bevel)
				__uvalue(Round)
				__uvalue(MiterOrBevel)
				__uvalue_end(LineJoin, Miter)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API DashStyle : __uobject
			{
				__uvalue_begin(DashStyle, UInt)
				__uvalue(Solid)
				__uvalue(Dash)
				__uvalue(Dot)
				__uvalue(DashDot)
				__uvalue(DashDotDot)
				__uvalue(Custom)
				__uvalue_end(DashStyle, Solid)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API InterpolationMode : __uobject
			{
				__uvalue_begin(InterpolationMode, UInt)
				__uvalue(NearestNeighbor)
				__uvalue(Linear)
				__uvalue(Cubic)
				__uvalue(MultiSampleLinear)
				__uvalue(Anisotropic)
				__uvalue(HighQualityCubic)
				__uvalue_end(InterpolationMode, NearestNeighbor)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API CombineMode : __uobject
			{
				__uvalue_begin(CombineMode, UInt)
				__uvalue(Union)
				__uvalue(Intersect)
				__uvalue(Xor)
				__uvalue(Exclude)
				__uvalue_end(CombineMode, Union)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API FillMode : __uobject
			{
				__uvalue_begin(FillMode, UInt)
				__uvalue(Alternate)
				__uvalue(Winding)
				__uvalue_end(FillMode, Alternate)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API AntialiasMode : __uobject
			{
				__uvalue_begin(AntialiasMode, UInt)
				__uvalue(PerPrimitive)
				__uvalue(Aliased)
				__uvalue_end(AntialiasMode, PerPrimitive)
			};

			ENUM_FLAGS(ExecutionContext)
			enum class ExecutionContext
			{
				InprocServer = CLSCTX_INPROC_SERVER,
				InprocHandler = CLSCTX_INPROC_HANDLER,
				LocalServer = CLSCTX_LOCAL_SERVER,
				RemoteServer = CLSCTX_REMOTE_SERVER,
				EnableCloaking = CLSCTX_ENABLE_CLOAKING,
				AppContainer = CLSCTX_APPCONTAINER,
			};

			enum class Apartment
			{
				SingleThreaded = COINIT_APARTMENTTHREADED,
				MultiThreaded = COINIT_MULTITHREADED,
			};

			enum class FeatureLevel
			{
				Invalid = -1,
				GDIWithD3D9,
				PureD3D9,
				PureD3D10,
				PureD3D10_1,
				PureD3D11,
				PureD3D11_1,
				D2DWithD3D10,
				D2DWithD3D11,
				D2D1WithD3D11,
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API FontWeight : __uobject
			{
				__uvalue_begin(FontWeight, UInt)
				__uvalue(Thin)
				__uvalue(ExtraLight)
				__uvalue(UltraLight)
				__uvalue(Light)
				__uvalue(SemiLight)
				__uvalue(Normal)
				__uvalue(Regular)
				__uvalue(Medium)
				__uvalue(DemiBold)
				__uvalue(SemiBold)
				__uvalue(Bold)
				__uvalue(ExtraBold)
				__uvalue(UltraBold)
				__uvalue(Black)
				__uvalue(Heavy)
				__uvalue(ExtraBlack)
				__uvalue(UltraBlack)
				__uvalue_end(FontWeight, Normal)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API FontStretch : __uobject
			{
				__uvalue_begin(FontStretch, UInt)
				__uvalue(Undefined)
				__uvalue(UltraCondensed)
				__uvalue(ExtraCondensed)
				__uvalue(Condensed)
				__uvalue(SemiCondensed)
				__uvalue(Normal)
				__uvalue(Medium)
				__uvalue(SemiExpanded)
				__uvalue(Expanded)
				__uvalue(ExtraExpanded)
				__uvalue(UltraExpanded)
				__uvalue_end(FontStretch, Normal)
			};
		
			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API FontStyle : __uobject
			{
				__uvalue_begin(FontStyle, UInt)
				__uvalue(Normal)
				__uvalue(Oblique)
				__uvalue(Italic)
				__uvalue_end(FontStyle, Normal)
			};

			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API BlurOptimization : __uobject
			{
				__uvalue_begin(BlurOptimization, UInt)
				__uvalue(Speed)
				__uvalue(Balanced)
				__uvalue(Quality)
				__uvalue_end(BlurOptimization, Balanced)
			};
			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API BlurBorderMode : __uobject
			{
				__uvalue_begin(BlurBorderMode, UInt)
				__uvalue(Soft)
				__uvalue(Hard)
				__uvalue_end(BlurBorderMode, Soft)
			};
			__uenum __uobjdef(asm = UICore, ns = System::UI::Graphic, ver = 1.0) UICORE_API Texture2DMode : __uobject
			{
				__uvalue_begin(Texture2DMode, UInt)
				__uvalue(Target)
				__uvalue(CpuRead)
				__uvalue_end(Texture2DMode, Target)
			};
			struct BitmapData : public ZeroStruct<BitmapData>
			{
				unsigned int RowPitch;
				unsigned int DepthPitch;
				void *Bits;
			};
#pragma endregion

		}
	}
}