#pragma once
#include "Config.h"
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

namespace System
{
	namespace Audio
	{
		class PluginCenter;
		class DSPManager;
		class AudioDSP_base;

		struct DSPOrder :ZeroStruct<DSPOrder>
		{
			bool Active;
			UInt128 ID;
			DSPOrder(bool active, const UInt128& id) { Active = active; ID = id; }
		};
		class DSPOrder_ID_Equ
		{
		public:
			UInt128 i;
			explicit DSPOrder_ID_Equ(const UInt128& s) : i(s) {}
			bool  operator ()(const DSPOrder& e) const { return e.ID == i; }
		};
		struct DSPBlock :ZeroStruct<DSPBlock>
		{
			UInt128 ID;
			AudioDSPPipe Handle;
		};
		struct DSPTree :ZeroStruct<DSPTree>
		{
			vector<DSPBlock> Block;
		};
		class DSPBlock_ID_Equ
		{
		public:
			UInt128 i;
			explicit DSPBlock_ID_Equ(const UInt128& s) : i(s) {}
			bool operator ()(const DSPBlock& e) const { return e.ID == i; }
		};
		class DSPManager
		{
		private:
			safe_void_ptr hMutex;
			std::vector<intptr_t> Handle;
			std::vector<DSPOrder> DSPs;
			PluginCenter* Center;
			EventToken Token;
		public:
			DSPManager(PluginCenter* pc);
			~DSPManager();
			intptr_t CreateTree();
			bool MoveDSP(int OldIndex, int NewIndex, int Mode);
			bool SetDSPActive(const UInt128& ID, bool Active);
			void DoDSP(intptr_t Handle, AudioBuffer& Buffer, bool ignorewinamp);
			void DestoryTree(intptr_t Handle);
		};

	}
}
