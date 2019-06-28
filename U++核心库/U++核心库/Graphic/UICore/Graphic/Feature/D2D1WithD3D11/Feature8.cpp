#include "Feature8.h"
using namespace System::UI::Graphic;
HMODULE FeatureManager8::dll = NULL;
unsigned int FeatureManager8::ref = 0;
Feature8* FeatureManager8::feature = NULL;
FeatureInfo* FeatureManager8::info = NULL;