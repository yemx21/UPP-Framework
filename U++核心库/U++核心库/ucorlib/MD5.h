#pragma once
#include "Config.h"


namespace System
{
	namespace Encryption
	{

		struct MD5Context
		{
			UInt8 buffer[64]; 
			UInt count[2];  
			UInt state[4];  
			UInt8 digest[16]; 
			MD5Context()
			{
				memset(buffer, 0, sizeof(UInt8)* 64);
				memset(digest, 0, sizeof(UInt8)* 16);
				count[0] = 0;
				count[1] = 0;
				state[0] = 0x67452301;
				state[1] = 0xefcdab89;
				state[2] = 0x98badcfe;
				state[3] = 0x10325476;
			}
			MD5Context(const MD5Context& ctx)
			{
				memcpy(buffer, ctx.buffer, sizeof(UInt8)* 64);
				memcpy(digest, ctx.digest, sizeof(UInt8)* 16);
				count[0] = ctx.count[0];
				count[1] = ctx.count[1];
				state[0] = ctx.state[0];
				state[1] = ctx.state[1];
				state[2] = ctx.state[2];
				state[3] = ctx.state[3];
			}
		};

		class MD5
		{
		public:
			static void Hash(MD5Context& ctx, const char *buf, UInt length);
		};
	}
}