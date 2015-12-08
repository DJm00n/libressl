/* =====================================================================
 * Author: Ignat Korchagin <ignat.korchagin@gmail.com>
 * This file is distributed under the same license as OpenSSL
 ==================================================================== */

#ifndef HEADER_DSTU_H
#define HEADER_DSTU_H

#include <openssl/gost.h>

void dstu_set_sbox(GOST2814789_KEY *key, const unsigned char sbox[64]);
void dstu_get_sbox(const GOST2814789_KEY * ctx, unsigned char sbox[64]);

#endif /* HEADER_DSTU_H */
