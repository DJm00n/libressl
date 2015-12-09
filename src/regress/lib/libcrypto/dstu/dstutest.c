/* =====================================================================
 * Author: Ignat Korchagin <ignat.korchagin@gmail.com>
 * This file is distributed under the same license as OpenSSL
 ==================================================================== */

#include <string.h>
#include <openssl/evp.h>

typedef struct {
	unsigned char key[32];
	unsigned char iv[8];
	unsigned char pt[40];
	unsigned char ct[40];
} DSTU28147_TEST_DATA;

static DSTU28147_TEST_DATA cfb64_test_data[] = {
	{
		{0x5e, 0xdf, 0xe7, 0xe8, 0x54, 0xec, 0x97, 0x74, 0xda, 0xb3, 0xa2, 0x8d, 0x7d, 0xa5, 0x6a, 0x4d, 0x65, 0xbf, 0xee, 0xde, 0xfc, 0xa2, 0x30, 0x57, 0xf4, 0xdc, 0x95, 0x28, 0xec, 0x59, 0xf3, 0x8f},
		{0x55, 0x2e, 0xcb, 0xd6, 0x96, 0x7f, 0x2f, 0xc9},
		{0xf7, 0xfa, 0x44, 0x41, 0xee, 0x86, 0xa7, 0x80, 0xde, 0x8e, 0x1e, 0x84, 0x81, 0x2f, 0xe3, 0x42, 0x95, 0x22, 0x9f, 0x63, 0xde, 0x4e, 0x88, 0xd2, 0x88, 0xe3, 0xe0, 0x8a, 0x79, 0xea, 0xc4, 0x94, 0x6c, 0xf8, 0xba, 0x28, 0xe8, 0xd7, 0xfa, 0xc9},
		{0x32, 0x46, 0x9d, 0x10, 0x7f, 0x1c, 0x45, 0xe1, 0xc0, 0x94, 0xb8, 0xd1, 0x94, 0x74, 0x28, 0x3c, 0x53, 0x10, 0x98, 0xc3, 0x34, 0xbb, 0xf2, 0xf6, 0x4c, 0xc4, 0x17, 0x53, 0x5e, 0x47, 0x56, 0xb9, 0xab, 0xd5, 0x53, 0x61, 0xe7, 0x69, 0x3d, 0xeb}
	},
	{
		{0xfa, 0x38, 0xb2, 0xa2, 0xf2, 0xca, 0x3b, 0x02, 0x7d, 0x6c, 0x0f, 0xe3, 0xc2, 0x7a, 0x9e, 0x98, 0xd3, 0xc7, 0xc6, 0x67, 0x6e, 0x31, 0x53, 0x23, 0x0f, 0x01, 0x00, 0x16, 0x7e, 0xc1, 0x03, 0xfc},
		{0x90, 0x0d, 0xc1, 0x2d, 0x9b, 0xd0, 0xba, 0xb8},
		{0xad, 0xf5, 0x32, 0x48, 0x13, 0x58, 0xd2, 0xf9, 0xe8, 0x44, 0x57, 0xd2, 0x81, 0x0c, 0x98, 0xcf, 0x0e, 0xe2, 0x03, 0x1e, 0x22, 0x5a, 0x47, 0x2f, 0x41, 0x43, 0x3d, 0xc4, 0x6c, 0xa6, 0x7f, 0xb8, 0xcb, 0x26, 0x53, 0x8a, 0x24, 0x8a, 0xd8, 0x35},
		{0xe2, 0xa7, 0x76, 0x49, 0xd3, 0x41, 0x3d, 0x6b, 0x87, 0xc7, 0x5e, 0xb9, 0x97, 0xb8, 0xb4, 0xa6, 0xcb, 0x02, 0xa8, 0x9d, 0xab, 0x0e, 0x17, 0x57, 0xd4, 0x0d, 0xa2, 0x52, 0x43, 0x92, 0x65, 0x27, 0x7f, 0xdd, 0x9b, 0xdb, 0xcd, 0xa6, 0xbf, 0xe2}
	},
	{
		{0x5d, 0x46, 0x17, 0xf2, 0xa6, 0x61, 0x7c, 0xa5, 0xbc, 0xd4, 0x63, 0xec, 0x76, 0x3b, 0x57, 0x8f, 0xbb, 0x2b, 0xf2, 0xcd, 0x35, 0x5a, 0xe8, 0x6b, 0x18, 0x0e, 0x83, 0x25, 0xc3, 0xb8, 0xa1, 0x2f},
		{0xcc, 0xfa, 0x5b, 0x89, 0x10, 0x28, 0x75, 0x38},
		{0x31, 0x04, 0xb5, 0x25, 0x19, 0x74, 0x2f, 0x58, 0xa1, 0x51, 0x34, 0x81, 0xdb, 0x07, 0x6b, 0x98, 0xac, 0x40, 0x02, 0x4a, 0x6f, 0xd1, 0xb4, 0x2d, 0x2c, 0x35, 0xe6, 0x1a, 0xdf, 0xc6, 0xa9, 0xb4, 0x9d, 0xe7, 0x5f, 0xb0, 0x34, 0xe6, 0xf3, 0x95},
		{0xdd, 0x52, 0x6c, 0x25, 0x16, 0x09, 0x9d, 0x39, 0x54, 0x8b, 0xc4, 0x36, 0xf8, 0xf7, 0xc0, 0xd0, 0xc9, 0x31, 0x13, 0x0f, 0xf3, 0x82, 0xba, 0xd9, 0xdd, 0x9e, 0xbd, 0x03, 0xee, 0xe4, 0xe9, 0xa4, 0x9f, 0x3d, 0x0d, 0xd6, 0xd3, 0x66, 0xa0, 0x09}
	}
};

static int cfb64_test(void)
{
	EVP_CIPHER_CTX cipher_ctx;
	int outl = 0;
	int i;
	int res = 0;
	unsigned char buf[sizeof(cfb64_test_data[0].ct)];

	for (i = 0; i < (sizeof(cfb64_test_data) / sizeof(DSTU28147_TEST_DATA)); i++)
	{
		EVP_CIPHER_CTX_init(&cipher_ctx);

		if (!EVP_EncryptInit(&cipher_ctx, EVP_dstu28147_cfb64(), cfb64_test_data[i].key, cfb64_test_data[i].iv))
			res = 1;

		if (!EVP_EncryptUpdate(&cipher_ctx, buf, &outl, cfb64_test_data[i].pt, sizeof(cfb64_test_data[i].pt)))
			res = 1;

		if (!EVP_EncryptFinal(&cipher_ctx, buf + outl, &outl))
			res = 1;

		if (memcmp(buf, cfb64_test_data[i].ct, sizeof(buf)))
			res = 1;

		EVP_CIPHER_CTX_init(&cipher_ctx);

		if (!EVP_DecryptInit(&cipher_ctx, EVP_dstu28147_cfb64(), cfb64_test_data[i].key, cfb64_test_data[i].iv))
			res = 1;

		if (!EVP_DecryptUpdate(&cipher_ctx, buf, &outl, cfb64_test_data[i].ct, sizeof(cfb64_test_data[i].ct)))
			res = 1;

		if (!EVP_DecryptFinal(&cipher_ctx, buf + outl, &outl))
			res = 1;

		if (memcmp(buf, cfb64_test_data[i].pt, sizeof(buf)))
			res = 1;
	}

	return res;
}

int main(int argc, char* argv[])
{
	int err = 0;

	err |= cfb64_test();

	return err;
}
