/* =====================================================================
 * Author: Ignat Korchagin <ignat.korchagin@gmail.com>
 * This file is distributed under the same license as OpenSSL
 ==================================================================== */

#ifndef DSTU_PARAMS_H_
#define DSTU_PARAMS_H_

#include <openssl/ec.h>
#include "dstu.h"

#define DEFAULT_CURVE 6
#define get_default_group() group_from_named_curve(DEFAULT_CURVE)

typedef struct dstu_named_curve_st {
    int nid;
    int poly[6];
    unsigned char *data;
} DSTU_NAMED_CURVE;

extern DSTU_NAMED_CURVE dstu_curves[];
extern unsigned char default_sbox[64];

int is_default_sbox(const unsigned char sbox[64]);
unsigned char *copy_sbox(const unsigned char sbox[64]);

EC_GROUP *group_from_named_curve(int curve_num);
EC_GROUP *group_from_nid(int nid);

int dstu_generate_key(EC_KEY *key);
int dstu_add_public_key(EC_KEY *key);

void reverse_bytes(void *mem, int size);
void reverse_bytes_copy(void *dst, const void *src, int size);

int bn_encode(const BIGNUM *bn, unsigned char *buffer, int length);
int curve_nid_from_group(const EC_GROUP *group);

void dstu_set_sbox(GOST2814789_KEY *key, const unsigned char sbox[64]);
void dstu_get_sbox(const GOST2814789_KEY *ctx, unsigned char sbox[64]);

#endif /* DSTU_PARAMS_H_ */
