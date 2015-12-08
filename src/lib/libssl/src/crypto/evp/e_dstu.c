/* =====================================================================
 * Author: Ignat Korchagin <ignat.korchagin@gmail.com>
 * This file is distributed under the same license as OpenSSL
 ==================================================================== */

#include <string.h>

#include <openssl/opensslconf.h>

#ifndef OPENSSL_NO_DSTU
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/gost.h>
#include "../dstu/dstu.h"
#include "evp_locl.h"

/*
 * DSTU uses Russian GOST 28147 but with different s-boxes and no key meshing
 */

/* We implement CFB mode here because it is mostly used */

#define DSTU_CIPHER_BLOCK_SIZE 8
#define DSTU_CIPHER_SBOX_SIZE 64

/*
 * 2 bytes for sequence header, 2 bytes for each octet string header and 8
 * bytes for iv and 64 bytes for dke. Total 78 < 128 so we are ok with 1 byte
 * length
 */
#define DSTU_CIPHER_ASN1_PARAM_SIZE (2 + 2 + DSTU_CIPHER_BLOCK_SIZE + 2 + DSTU_CIPHER_SBOX_SIZE)

typedef struct {
	GOST2814789_KEY ks;
} EVP_DSTU28147_CTX;

static int dstu_cipher_init(EVP_CIPHER_CTX *ctx, const unsigned char *key,
							const unsigned char *iv, int enc)
{
	EVP_DSTU28147_CTX *c = ctx->cipher_data;

	/* we do not use key meshing */
	c->ks.key_meshing = 0;

	return Gost2814789_set_key(&c->ks, key, ctx->key_len * 8);
}

static int dstu_cipher_ctrl(EVP_CIPHER_CTX *ctx, int cmd, int p1, void *p2)
{
	EVP_DSTU28147_CTX *c = ctx->cipher_data;

	switch (cmd) {
	case EVP_CTRL_INIT:
		/* Default value to have any s-box set at all */
		dstu_set_sbox(&(c->ks), NULL);
		return 1;
	case EVP_CTRL_GOST_SET_SBOX:
		/* Unlike GOST, which accepts nid, we accept pointer to sbox, so p2 is used! */
		dstu_set_sbox(&(c->ks), p2);
		return 1;
	case EVP_CTRL_PBE_PRF_NID:
		if (!p2)
			return 0;
		*((int *)(p2)) = NID_hmacWithDstu34311;
		return 1;
	}

	return 0;
}

static int dstu_cipher_set_asn1_parameters(EVP_CIPHER_CTX *ctx,
										   ASN1_TYPE *asn1_type)
{
	/*
	 * We defined params asn1 structure, but for now we will use manual
	 * composition for speed here
	 */
	EVP_DSTU28147_CTX *c = ctx->cipher_data;

	unsigned char params[DSTU_CIPHER_ASN1_PARAM_SIZE];
	ASN1_STRING seq;

	params[0] = V_ASN1_SEQUENCE | V_ASN1_CONSTRUCTED;
	params[1] = 2 + DSTU_CIPHER_BLOCK_SIZE + 2 + DSTU_CIPHER_SBOX_SIZE;
	params[2] = V_ASN1_OCTET_STRING;
	params[3] = DSTU_CIPHER_BLOCK_SIZE;

	memcpy(&(params[4]), ctx->oiv, DSTU_CIPHER_BLOCK_SIZE);

	params[4 + DSTU_CIPHER_BLOCK_SIZE] = V_ASN1_OCTET_STRING;
	params[4 + DSTU_CIPHER_BLOCK_SIZE + 1] = DSTU_CIPHER_SBOX_SIZE;

	dstu_get_sbox(&(c->ks), &(params[4 + DSTU_CIPHER_BLOCK_SIZE + 2]));

	seq.type = V_ASN1_SEQUENCE;
	seq.length = sizeof(params);
	seq.flags = 0;
	seq.data = params;

	if (ASN1_TYPE_set1(asn1_type, V_ASN1_SEQUENCE, &seq))
		return 1;

	return -1;
}

static int dstu_cipher_get_asn1_parameters(EVP_CIPHER_CTX *ctx,
										   ASN1_TYPE *asn1_type)
{
	if (V_ASN1_SEQUENCE != asn1_type->type)
		return -1;

	if (DSTU_CIPHER_ASN1_PARAM_SIZE != asn1_type->value.sequence->length)
		return -1;

	if ((V_ASN1_OCTET_STRING != asn1_type->value.sequence->data[2])
		|| (DSTU_CIPHER_BLOCK_SIZE != asn1_type->value.sequence->data[3]))
		return -1;

	if ((V_ASN1_OCTET_STRING !=
		 asn1_type->value.sequence->data[4 + DSTU_CIPHER_BLOCK_SIZE])
		|| (DSTU_CIPHER_SBOX_SIZE !=
			asn1_type->value.sequence->data[4 + DSTU_CIPHER_BLOCK_SIZE + 1]))
		return -1;

	memcpy(ctx->oiv, &(asn1_type->value.sequence->data[4]),
		   DSTU_CIPHER_BLOCK_SIZE);

	if (dstu_cipher_ctrl
		(ctx, EVP_CTRL_GOST_SET_SBOX, DSTU_CIPHER_SBOX_SIZE,
		 &(asn1_type->value.sequence->data[4 + DSTU_CIPHER_BLOCK_SIZE + 2])))
		return 1;

	return -1;
}

BLOCK_CIPHER_func_cfb(dstu28147, Gost2814789, 64, EVP_DSTU28147_CTX, ks)

#define NID_dstu89_cfb64 NID_dstu28147_cfb

BLOCK_CIPHER_def_cfb(dstu28147, EVP_DSTU28147_CTX, NID_dstu89, 32, 8, 64,
			 EVP_CIPH_NO_PADDING | EVP_CIPH_CTRL_INIT,
			 dstu_cipher_init, NULL, dstu_cipher_set_asn1_parameters,
			 dstu_cipher_get_asn1_parameters, dstu_cipher_ctrl)

#endif



