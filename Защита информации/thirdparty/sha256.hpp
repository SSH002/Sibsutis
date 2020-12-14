#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct __sha256_context {
	u32 state[8];
	u32 count[2];
	u8 buf[64];
} sha256_context, *psha256_context;

typedef union {
    char chr[2];
    u16 word;
} uchar;

string sha256(string input);
char *sha256_sprintf(u8 *buffer);
u8 *str_cpy(string input);

void be32enc_vect(u8 *dst, const u32 *src, size_t len);
static inline void be32enc(u8 *pp, u32 u);
void be32dec_vect(u32 *dst, const u8 *src, size_t len);
static inline u32 be32dec(const u8 *pp);

void sha256_init(psha256_context ctx);
void sha256_trans(u32 *state, const u8 block[64]);
void sha256_update(psha256_context ctx, const u8 *src, size_t len);
void sha256_padding(psha256_context ctx);
void sha256_final(u8 digest[32], psha256_context ctx);

#define Ch(x, y, z)			((x & (y ^ z)) ^ z)
#define Maj(x, y, z)		((x & (y | z)) | (y & z))
#define SHR(x, n)			(x >> n)
#define ROTR(x, n)			((x >> n) | (x << (32 - n)))
#define S0(x) 				(ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S1(x) 				(ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define s0(x) 				(ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define s1(x) 				(ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

#define RND(a, b, c, d, e, f, g, h, k)		\
	t0 = h + S1(e) + Ch(e, f, g) + k;		\
	t1 = S0(a) + Maj(a, b, c);				\
	d += t0;								\
	h = t0 + t1;

#define RNDr(S, W, i, k)					\
	RND(S[(64 - i) % 8], S[(65 - i) % 8],	\
	    S[(66 - i) % 8], S[(67 - i) % 8],	\
	    S[(68 - i) % 8], S[(69 - i) % 8],	\
	    S[(70 - i) % 8], S[(71 - i) % 8],	\
	    W[i] + k)
