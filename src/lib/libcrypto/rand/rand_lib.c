/* $OpenBSD: rand_lib.c,v 1.20 2014/10/22 13:02:04 jsing Exp $ */
/*
 * Copyright (c) 2014 Ted Unangst <tedu@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>

#include <openssl/opensslconf.h>

#include <openssl/rand.h>

#include <string.h>
#include "../gost/gost.h"
#include "../dstu/dstu_params.h"

/* DSTU RGB needs at least 40 bytes of seed to work properly */
#define DSTU_RGB_SEED_SIZE 40

/*
 * The useful functions in this file are at the bottom.
 */
int
RAND_set_rand_method(const RAND_METHOD *meth)
{
	return 1;
}

const RAND_METHOD *
RAND_get_rand_method(void)
{
	return NULL;
}

RAND_METHOD *
RAND_SSLeay(void)
{
	return NULL;
}

#ifndef OPENSSL_NO_ENGINE
int
RAND_set_rand_engine(ENGINE *engine)
{
	return 1;
}
#endif

void
RAND_seed(const void *buf, int num)
{

}

void
RAND_add(const void *buf, int num, double entropy)
{

}

int
RAND_status(void)
{
	return 1;
}

int
RAND_poll(void)
{
	return 1;
}

static uint32_t I[2];
static uint32_t s[2];
static GOST2814789_KEY cryptor;
static int initialized = 0;

static void dstu_rbg_init(void)
{
    /*
     * Since time can be 32-bit or 64-bit we will use byte array for time
     * which is always 64-bit
     */

    /*
     * For 32-bit time "garbage" in rest of the bytes will even help with
     * seeding
     */
	unsigned char curr[8];
    unsigned char seed[DSTU_RGB_SEED_SIZE];

    /* Generating seed with default arc4random */
    arc4random_buf(seed, DSTU_RGB_SEED_SIZE);

    time((time_t *)curr);

    OPENSSL_cleanse(&cryptor, sizeof(GOST2814789_KEY));
    Gost2814789_set_key(&cryptor, seed, 256);
    dstu_set_sbox(&cryptor, NULL);

    memcpy(s, seed + 32, 8);
    Gost2814789_ecb_encrypt(curr, (unsigned char *)I, &cryptor, 1);
    initialized = 1;

    OPENSSL_cleanse(seed, sizeof(seed));
}

/* DSTU RBG is bit oriented. It gives one bit at a time */
static unsigned char dstu_rbg_get_bit(void)
{
    uint32_t x[2];

    x[0] = I[0] ^ s[0];
    x[1] = I[1] ^ s[1];
    Gost2814789_ecb_encrypt((uint8_t *)x, (uint8_t *)x, &cryptor, 1);

    s[0] = x[0] ^ I[0];
    s[1] = x[1] ^ I[1];
    Gost2814789_ecb_encrypt((uint8_t *)s, (uint8_t *)s, &cryptor, 1);

    return (uint8_t) (x[0] & 1);
}

/*
 * Hurray. You've made it to the good parts.
 */
void
RAND_cleanup(void)
{
    CRYPTO_w_lock(CRYPTO_LOCK_RAND);

    OPENSSL_cleanse(I, sizeof(I));
    OPENSSL_cleanse(s, sizeof(s));
    OPENSSL_cleanse(&cryptor, sizeof(GOST2814789_KEY));
    initialized = 0;

    CRYPTO_w_unlock(CRYPTO_LOCK_RAND);
}

int
RAND_bytes(unsigned char *buf, int num)
{
    int i;
    uint8_t j;

    CRYPTO_w_lock(CRYPTO_LOCK_RAND);

    while (!initialized) {
    	dstu_rbg_init();
    }

    for (i = 0; i < num; i++) {
        *(buf + i) = 0;
        for (j = 0; j < 8; j++) {
            *(buf + i) |= dstu_rbg_get_bit() << j;
        }
    }

    CRYPTO_w_unlock(CRYPTO_LOCK_RAND);

    return 1;
}

int
RAND_pseudo_bytes(unsigned char *buf, int num)
{
	return RAND_bytes(buf, num);
}
