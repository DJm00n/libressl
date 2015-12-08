/* =====================================================================
 * Author: Ignat Korchagin <ignat.korchagin@gmail.com>
 * This file is distributed under the same license as OpenSSL
 ==================================================================== */

#include "dstu.h"
#include "../gost/gost_locl.h"

static unsigned char default_sbox[64] = {
	0xa9, 0xd6, 0xeb, 0x45, 0xf1, 0x3c, 0x70, 0x82,
	0x80, 0xc4, 0x96, 0x7b, 0x23, 0x1f, 0x5e, 0xad,
	0xf6, 0x58, 0xeb, 0xa4, 0xc0, 0x37, 0x29, 0x1d,
	0x38, 0xd9, 0x6b, 0xf0, 0x25, 0xca, 0x4e, 0x17,
	0xf8, 0xe9, 0x72, 0x0d, 0xc6, 0x15, 0xb4, 0x3a,
	0x28, 0x97, 0x5f, 0x0b, 0xc1, 0xde, 0xa3, 0x64,
	0x38, 0xb5, 0x64, 0xea, 0x2c, 0x17, 0x9f, 0xd0,
	0x12, 0x3e, 0x6d, 0xb8, 0xfa, 0xc5, 0x79, 0x04
};

static void unpack_sbox(const unsigned char *packed_sbox, gost_subst_block * unpacked_sbox)
{
    int i;
    for (i = 0; i < 8; i++) {
        unpacked_sbox->k1[2 * i] = 0x0f & (packed_sbox[i] >> 4);
        unpacked_sbox->k1[(2 * i) + 1] = 0x0f & packed_sbox[i];

        unpacked_sbox->k2[2 * i] = 0x0f & (packed_sbox[i + 8] >> 4);
        unpacked_sbox->k2[(2 * i) + 1] = 0x0f & packed_sbox[i + 8];

        unpacked_sbox->k3[2 * i] = 0x0f & (packed_sbox[i + 16] >> 4);
        unpacked_sbox->k3[(2 * i) + 1] = 0x0f & packed_sbox[i + 16];

        unpacked_sbox->k4[2 * i] = 0x0f & (packed_sbox[i + 24] >> 4);
        unpacked_sbox->k4[(2 * i) + 1] = 0x0f & packed_sbox[i + 24];

        unpacked_sbox->k5[2 * i] = 0x0f & (packed_sbox[i + 32] >> 4);
        unpacked_sbox->k5[(2 * i) + 1] = 0x0f & packed_sbox[i + 32];

        unpacked_sbox->k6[2 * i] = 0x0f & (packed_sbox[i + 40] >> 4);
        unpacked_sbox->k6[(2 * i) + 1] = 0x0f & packed_sbox[i + 40];

        unpacked_sbox->k7[2 * i] = 0x0f & (packed_sbox[i + 48] >> 4);
        unpacked_sbox->k7[(2 * i) + 1] = 0x0f & packed_sbox[i + 48];

        unpacked_sbox->k8[2 * i] = 0x0f & (packed_sbox[i + 56] >> 4);
        unpacked_sbox->k8[(2 * i) + 1] = 0x0f & packed_sbox[i + 56];
    }
}

static void pack_sbox(const gost_subst_block * unpacked_sbox, unsigned char *packed_sbox)
{
    int i;
    for (i = 0; i < 8; i++) {
        packed_sbox[i] =
            ((unpacked_sbox->
              k1[2 * i] << 4) & 0xf0) | (unpacked_sbox->k1[(2 * i) +
                                                           1] & 0x0f);
        packed_sbox[i + 8] =
            ((unpacked_sbox->
              k2[2 * i] << 4) & 0xf0) | (unpacked_sbox->k2[(2 * i) +
                                                           1] & 0x0f);
        packed_sbox[i + 16] =
            ((unpacked_sbox->
              k3[2 * i] << 4) & 0xf0) | (unpacked_sbox->k3[(2 * i) +
                                                           1] & 0x0f);
        packed_sbox[i + 24] =
            ((unpacked_sbox->
              k4[2 * i] << 4) & 0xf0) | (unpacked_sbox->k4[(2 * i) +
                                                           1] & 0x0f);
        packed_sbox[i + 32] =
            ((unpacked_sbox->
              k5[2 * i] << 4) & 0xf0) | (unpacked_sbox->k5[(2 * i) +
                                                           1] & 0x0f);
        packed_sbox[i + 40] =
            ((unpacked_sbox->
              k6[2 * i] << 4) & 0xf0) | (unpacked_sbox->k6[(2 * i) +
                                                           1] & 0x0f);
        packed_sbox[i + 48] =
            ((unpacked_sbox->
              k7[2 * i] << 4) & 0xf0) | (unpacked_sbox->k7[(2 * i) +
                                                           1] & 0x0f);
        packed_sbox[i + 56] =
            ((unpacked_sbox->
              k8[2 * i] << 4) & 0xf0) | (unpacked_sbox->k8[(2 * i) +
                                                           1] & 0x0f);

    }
}

void dstu_set_sbox(GOST2814789_KEY *key, const unsigned char sbox[64])
{
	gost_subst_block unpacked_sbox;
	unsigned int t;
	int i;

	if (!sbox)
	{
		/* set default */
		sbox = default_sbox;
	}

	unpack_sbox(sbox, &unpacked_sbox);

	for (i = 0; i < 256; i++) {
		t = (unsigned int)(unpacked_sbox.k8[i >> 4] <<4 | unpacked_sbox.k7 [i & 15]) << 24;
		key->k87[i] = (t << 11) | (t >> 21);
		t = (unsigned int)(unpacked_sbox.k6[i >> 4] <<4 | unpacked_sbox.k5 [i & 15]) << 16;
		key->k65[i] = (t << 11) | (t >> 21);
		t = (unsigned int)(unpacked_sbox.k4[i >> 4] <<4 | unpacked_sbox.k3 [i & 15]) << 8;
		key->k43[i] = (t << 11) | (t >> 21);
		t = (unsigned int)(unpacked_sbox.k2[i >> 4] <<4 | unpacked_sbox.k1 [i & 15]) << 0;
		key->k21[i] = (t << 11) | (t >> 21);
	}
}

void dstu_get_sbox(const GOST2814789_KEY * ctx, unsigned char sbox[64])
{
	gost_subst_block unpacked_sbox;
    int i;

    for (i = 0; i < 256; i++) {
    	unpacked_sbox.k8[i >> 4] = (ctx->k87[i] >> 28) & 0xf;
    	unpacked_sbox.k7[i & 15] = (ctx->k87[i] >> 24) & 0xf;

    	unpacked_sbox.k6[i >> 4] = (ctx->k65[i] >> 20) & 0xf;
    	unpacked_sbox.k5[i & 15] = (ctx->k65[i] >> 16) & 0xf;

    	unpacked_sbox.k4[i >> 4] = (ctx->k43[i] >> 12) & 0xf;
    	unpacked_sbox.k3[i & 15] = (ctx->k43[i] >> 8) & 0xf;

    	unpacked_sbox.k2[i >> 4] = (ctx->k21[i] >> 4) & 0xf;
    	unpacked_sbox.k1[i & 15] = ctx->k21[i] & 0xf;
    }

    pack_sbox(&unpacked_sbox, sbox);
}
