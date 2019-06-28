#pragma once
#include <System\Common.h>
#include <type_traits>
#include <memory>
using namespace std;

namespace System
{
	namespace UI
	{
		#define __upropertycoerce_paste(x,y) x ## _ ## y
		#define __upropertycoerce_evaluator(x,y)  __upropertycoerce_paste(x,y)

		#define __upropertycoerce(MODE, ...) __upropertycoerce_impl(MODE, __VA_ARGS__)
		#define __upropertycoerce_impl(MODE, ...) __upropertycoerce_evaluator(__upropertycoerce, MODE)  (__VA_ARGS__)

		#define __ucoerce_none_none_none 0
		#define __ucoerce_none_none_check 1
		#define __ucoerce_none_none_equalcheck 2
		#define __ucoerce_none_none_replace 3

		#define __ucoerce_none_check_none 10
		#define __ucoerce_none_check_check 11
		#define __ucoerce_none_check_equalcheck 12
		#define __ucoerce_none_check_replace 13

		#define __ucoerce_none_equalcheck_none 20
		#define __ucoerce_none_equalcheck_check 21
		#define __ucoerce_none_equalcheck_equalcheck 22
		#define __ucoerce_none_equalcheck_replace 23

		#define __ucoerce_none_replace_none 30
		#define __ucoerce_none_replace_check 31
		#define __ucoerce_none_replace_equalcheck 32
		#define __ucoerce_none_replace_replace 33

		#define __ucoerce_check_none_none 100
		#define __ucoerce_check_none_check 101
		#define __ucoerce_check_none_equalcheck 102
		#define __ucoerce_check_none_replace 103

		#define __ucoerce_check_check_none 110
		#define __ucoerce_check_check_check 111
		#define __ucoerce_check_check_equalcheck 112
		#define __ucoerce_check_check_replace 113

		#define __ucoerce_check_equalcheck_none 120
		#define __ucoerce_check_equalcheck_check 121
		#define __ucoerce_check_equalcheck_equalcheck 122
		#define __ucoerce_check_equalcheck_replace 123

		#define __ucoerce_check_replace_none 130
		#define __ucoerce_check_replace_check 131
		#define __ucoerce_check_replace_equalcheck 132
		#define __ucoerce_check_replace_replace 133

		#define __ucoerce_equalcheck_none_none 200
		#define __ucoerce_equalcheck_none_check 201
		#define __ucoerce_equalcheck_none_equalcheck 202
		#define __ucoerce_equalcheck_none_replace 203

		#define __ucoerce_equalcheck_check_none 210
		#define __ucoerce_equalcheck_check_check 211
		#define __ucoerce_equalcheck_check_equalcheck 212
		#define __ucoerce_equalcheck_check_replace 213

		#define __ucoerce_equalcheck_equalcheck_none 220
		#define __ucoerce_equalcheck_equalcheck_check 221
		#define __ucoerce_equalcheck_equalcheck_equalcheck 222
		#define __ucoerce_equalcheck_equalcheck_replace 223

		#define __ucoerce_equalcheck_replace_none 230
		#define __ucoerce_equalcheck_replace_check 231
		#define __ucoerce_equalcheck_replace_equalcheck 232
		#define __ucoerce_equalcheck_replace_replace 233

		#define __ucoerce_replace_none_none 300
		#define __ucoerce_replace_none_check 301
		#define __ucoerce_replace_none_equalcheck 302
		#define __ucoerce_replace_none_replace 303

		#define __ucoerce_replace_check_none 310
		#define __ucoerce_replace_check_check 311
		#define __ucoerce_replace_check_equalcheck 312
		#define __ucoerce_replace_check_replace 313

		#define __ucoerce_replace_equalcheck_none 320
		#define __ucoerce_replace_equalcheck_check 321
		#define __ucoerce_replace_equalcheck_equalcheck 322
		#define __ucoerce_replace_equalcheck_replace 323

		#define __ucoerce_replace_replace_none 330
		#define __ucoerce_replace_replace_check 331
		#define __ucoerce_replace_replace_equalcheck 332
		#define __ucoerce_replace_replace_replace 333
		class DependencyObject;

		namespace PreCoerceCallbackHelper
		{
			#define __upropertycoerce_0(T) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			return true;\
			}

			#define __upropertycoerce_1(T, VAL) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			return *newPtr < VAL;\
			}

			#define __upropertycoerce_2(T, VAL) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			return *newPtr <= VAL;\
			}

			#define __upropertycoerce_3(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr > VAL) newVal = make_shared<T>(VAL1);\
			return true;\
			}

			#define __upropertycoerce_10(T, VAL) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			return *newPtr > VAL;\
			}

			#define __upropertycoerce_11(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr <= VAL) return false;\
			return *newPtr < VAL1;\
			}

			#define __upropertycoerce_12(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr <= VAL) return false;\
			return *newPtr <= VAL1;\
			}

			#define __upropertycoerce_13(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr <= VAL) return false;\
			if (*newPtr > VAL1) newVal = make_shared<T>(VAL2);\
			return true;\
			}

			#define __upropertycoerce_20(T, VAL) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			return *newPtr >= VAL;\
			}

			#define __upropertycoerce_21(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr < VAL) return false;\
			return *newPtr < VAL1;\
			}

			#define __upropertycoerce_22(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr < VAL) return false;\
			return *newPtr <= VAL1;\
			}

			#define __upropertycoerce_23(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr < VAL) return false;\
			if (*newPtr > VAL1) newVal = make_shared<T>(VAL2);\
			return true;\
			}

			#define __upropertycoerce_30(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr < VAL) newVal = make_shared<T>(VAL1);\
			return true;\
			}

			#define __upropertycoerce_31(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr < VAL) return false;\
			if (*newPtr < VAL)\
			{\
			newVal = make_shared<T>(VAL1);\
			return true;\
			}\
			return *newPtr < VAL2;\
			}

			#define __upropertycoerce_32(T, VAL, VAL1, VAL2, VAL3) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr < VAL)\
			{\
			newVal = make_shared<T>(VAL1);\
			return true;\
			}\
			if (*newPtr > VAL2) newVal = make_shared<T>(VAL3);\
			return true;\
			}

			#define __upropertycoerce_33(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (*newPtr < VAL) return false;\
			if (*newPtr > VAL1) newVal = make_shared<T>(VAL2);\
			return true;\
			}

			#define __upropertycoerce_100(T) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			return !isnan(*newPtr);\
			}

			#define __upropertycoerce_101(T, VAL) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			return *newPtr < VAL;\
			}

			#define __upropertycoerce_102(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			return *newPtr <= VAL;\
			}

			#define __upropertycoerce_103(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr > VAL) newVal = make_shared<T>(VAL1);\
			return true;\
			}

			#define __upropertycoerce_110(T, VAL) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			return *newPtr > VAL;\
			}

			#define __upropertycoerce_111(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr <= VAL) return false;\
			return *newPtr < VAL1;\
			}

			#define __upropertycoerce_112(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr <= VAL) return false;\
			return *newPtr <= VAL1;\
			}

			#define __upropertycoerce_113(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr <= VAL) return false;\
			if (*newPtr > VAL1) newVal = make_shared<T>(VAL2);\
			return true;\
			}

			#define __upropertycoerce_120(T, VAL) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			return *newPtr >= VAL;\
			}

			#define __upropertycoerce_121(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr < VAL) return false;\
			return *newPtr < VAL1;\
			}

			#define __upropertycoerce_122(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr < VAL) return false;\
			return *newPtr <= VAL1;\
			}

			#define __upropertycoerce_123(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr < VAL) return false;\
			if (*newPtr > VAL1) newVal = make_shared<T>(VAL2);\
			return true;\
			}

			#define __upropertycoerce_130(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr < VAL) newVal = make_shared<T>(VAL1);\
			return true;\
			}

			#define __upropertycoerce_131(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr < VAL)\
			{\
			newVal = make_shared<T>(VAL1);\
			return true;\
			}\
			return *newPtr < VAL2;\
			}

			#define __upropertycoerce_132(T, VAL, VAL1, VAL2, VAL3) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr < VAL)\
			{\
			newVal = make_shared<T>(VAL1);\
			return true;\
			}\
			return *newPtr <= VAL2;\
			}

			#define __upropertycoerce_133(T, VAL, VAL1, VAL2, VAL3) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) return false;\
			if (*newPtr < VAL)\
			{\
			newVal = make_shared<T>(VAL1);\
			return true;\
			}\
			if (*newPtr > VAL2) newVal = make_shared<T>(VAL3);\
			return true;\
			}

			#define __upropertycoerce_200(T)  __upropertycoerce_100(T)

			#define __upropertycoerce_201(T, VAL)  __upropertycoerce_101(T, VAL)

			#define __upropertycoerce_202(T, VAL, VAL1)  __upropertycoerce_102(T, VAL, VAL1)

			#define __upropertycoerce_203(T, VAL, VAL1) __upropertycoerce_103(T, VAL, VAL1)

			#define __upropertycoerce_210(T, VAL) __upropertycoerce_110(T, VAL)

			#define __upropertycoerce_211(T, VAL, VAL1)  __upropertycoerce_111(T, VAL, VAL1)

			#define __upropertycoerce_212(T, VAL, VAL1)  __upropertycoerce_112(T, VAL, VAL1)

			#define __upropertycoerce_213(T, VAL, VAL1, VAL2)  __upropertycoerce_113(T, VAL, VAL1, VAL2)

			#define __upropertycoerce_220(T, VAL)  __upropertycoerce_120(T, VAL)

			#define __upropertycoerce_221(T, VAL, VAL1) __upropertycoerce_121(T, VAL, VAL1)

			#define __upropertycoerce_222(T, VAL, VAL1) __upropertycoerce_122(T, VAL, VAL1)

			#define __upropertycoerce_223(T, VAL, VAL1, VAL2)  __upropertycoerce_123(T, VAL, VAL1)

			#define __upropertycoerce_230(T, VAL, VAL1) __upropertycoerce_130(T, VAL, VAL1)

			#define __upropertycoerce_231(T, VAL, VAL1, VAL2) __upropertycoerce_131(T, VAL, VAL1, VAL2)

			#define __upropertycoerce_232(T, VAL, VAL1, VAL2, VAL3) __upropertycoerce_132(T, VAL, VAL1, VAL2, VAL3)

			#define __upropertycoerce_233(T, VAL, VAL1, VAL2, VAL3)  __upropertycoerce_133(T, VAL, VAL1, VAL2, VAL3)

			#define __upropertycoerce_300(T, VAL) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			return true;\
			}

			#define __upropertycoerce_301(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			return *newPtr < VAL1;\
			}

			#define __upropertycoerce_302(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			return *newPtr <= VAL1;\
			}

			#define __upropertycoerce_303(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr > VAL1) newVal = make_shared<T>(VAL2);\
			return true;\
			}

			#define __upropertycoerce_310(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			return *newPtr > VAL1;\
			}

			#define __upropertycoerce_311(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr <= VAL1) return false;\
			return *newPtr < VAL2;\
			}

			#define __upropertycoerce_312(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr <= VAL1) return false;\
			return *newPtr <= VAL2;\
			}

			#define __upropertycoerce_313(T, VAL, VAL1, VAL2, VAL3) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr <= VAL1) return false;\
			if (*newPtr > VAL2) newVal = make_shared<T>(VAL3);\
			return true;\
			}

			#define __upropertycoerce_320(T, VAL, VAL1) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			return *newPtr >= VAL1;\
			}

			#define __upropertycoerce_321(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr < VAL1) return false;\
			return *newPtr < VAL2;\
			}

			#define __upropertycoerce_322(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr < VAL1) return false;\
			return *newPtr <= VAL2;\
			}

			#define __upropertycoerce_323(T, VAL, VAL1, VAL2, VAL3) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr < VAL1) return false;\
			if (*newPtr > VAL2) newVal = make_shared<T>(VAL3);\
			return true;\
			}

			#define __upropertycoerce_330(T, VAL, VAL1, VAL2) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T* newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr < VAL1) newVal = make_shared<T>(VAL2);\
			return true;\
			}

			#define __upropertycoerce_331(T, VAL, VAL1, VAL2, VAL3) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr < VAL1)\
			{\
			newVal = make_shared<T>(VAL2);\
			return true;\
			}\
			return *newPtr < VAL3;\
			}

			#define __upropertycoerce_332(T, VAL, VAL1, VAL2, VAL3) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr < VAL1)\
			{\
			newVal = make_shared<T>(VAL2);\
			return true;\
			}\
			return *newPtr <= VAL3;\
			}

			#define __upropertycoerce_333(T, VAL, VAL1, VAL2, VAL3, VAL4) [](DependencyObject* const dependencyObject, const SharedVoid& oldVal, SharedVoid& newVal)->bool\
			{\
			T*  newPtr = static_cast<T*>(newVal.get());\
			if (!newPtr) return false;\
			if (isnan(*newPtr)) newVal = make_shared<T>(VAL);\
			if (*newPtr < VAL1)\
			{\
			MessageBoxA(NULL,"min",0,0);\
			newVal = make_shared<T>(VAL2);\
			return true;\
			}\
			if (*newPtr > VAL3) newVal = make_shared<T>(VAL4);\
			return true;\
			}
		}


	}
}