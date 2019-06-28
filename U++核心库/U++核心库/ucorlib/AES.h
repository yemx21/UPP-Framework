#pragma once
#include "Config.h"

namespace System
{
	namespace Encryption
	{
		#define AES_MAXNR 14
		#define AES_BLOCK_SIZE 16

		struct aes_key_st
		{
			UInt rd_key[4 * (AES_MAXNR + 1)];
			int rounds;
		};

		typedef struct aes_key_st AES_KEY;

		class AES
		{
		public:
			static int SetEncryptKey(const unsigned char *userKey, const int bits,
				AES_KEY *key);
			static int  SetDecryptKey(const unsigned char *userKey, const int bits,
				AES_KEY *key);

			/* 输入输出地址可相同 */
			static void Encrypt(const unsigned char *in, unsigned char *out,
				const AES_KEY *key);
			static void Decrypt(const unsigned char *in, unsigned char *out,
				const AES_KEY *key);

			static void CBCEncrypt(const unsigned char *in, unsigned char *out, unsigned char *ivec, unsigned long nblock, const AES_KEY *key);

			/* 输入输出地址必须不相同 */
			static void CBCDecrypt(const unsigned char *in, unsigned char *out, unsigned char *ivec, unsigned long nblock, const AES_KEY *key);
		};
	}
}