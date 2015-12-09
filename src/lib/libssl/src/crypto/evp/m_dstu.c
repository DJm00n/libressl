/* =====================================================================
 * Author: Ignat Korchagin <ignat.korchagin@gmail.com>
 * This file is distributed under the same license as OpenSSL
 ==================================================================== */
#include <string.h>

#include <openssl/opensslconf.h>

#ifndef OPENSSL_NO_DSTU

#include <openssl/evp.h>
#include <openssl/gost.h>
#include <openssl/objects.h>

#include "../dstu/dstu.h"

typedef GOSTR341194_CTX DSTU34311_CTX;

static int
dstu34311_init(EVP_MD_CTX *ctx)
{
	DSTU34311_CTX *c = ctx->md_data;

	memset(c, 0, sizeof(DSTU34311_CTX));

	/* If we have pkey_ctx, it may contain custom sbox, so let's check it */

	/*if (ctx->pctx) {
		pkey = EVP_PKEY_CTX_get0_pkey(ctx->pctx);
		if (pkey) {
			dstu_key = EVP_PKEY_get0(pkey);
			if (dstu_key) {
				if (dstu_key->sbox) {
					unpack_sbox(dstu_key->sbox, &sbox);
					use_default_sbox = 0;
				}
			}
		}
	}*/

	dstu_set_sbox(&(c->cipher), NULL);
	return 1;
}

static int
dstu34311_update(EVP_MD_CTX *ctx, const void *data, size_t count)
{
	return GOSTR341194_Update(ctx->md_data, data, count);
}

static int
dstu34311_final(EVP_MD_CTX *ctx, unsigned char *md)
{
	return GOSTR341194_Final(md, ctx->md_data);
}

static const EVP_MD dstu34311_md = {
	.type = NID_dstu34311,
	.pkey_type = NID_undef,
	.md_size = GOSTR341194_LENGTH,
	.flags = EVP_MD_FLAG_PKEY_METHOD_SIGNATURE,
	.init = dstu34311_init,
	.update = dstu34311_update,
	.final = dstu34311_final,
	.block_size = GOSTR341194_CBLOCK,
	.ctx_size = sizeof(EVP_MD *) + sizeof(GOSTR341194_CTX),
};

const EVP_MD *
EVP_dstu34311(void)
{
	return (&dstu34311_md);
}
#endif
