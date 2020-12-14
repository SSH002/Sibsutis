#include "sha256.hpp"
#include <iostream>
u8 *str_cpy(string input)
{
	u8 *buffer = (u8*)malloc(sizeof(u8) * input.length());

	for(size_t i = 0; i < input.length(); ++i) {
		buffer[i] = input[i];
	}
	
	return buffer;
}

string sha256(string input)
{
	sha256_context ctx;
	u8 *buffer = str_cpy(input);

	sha256_init(&ctx);    
	sha256_update(&ctx, (u8*)buffer, input.length());
	sha256_final((u8*)buffer, &ctx);
	
	strlen((char*)buffer) >= 32 ? buffer[32] = '\0' : buffer[strlen((char*)buffer) - 1] = '\0';
	
	string result(sha256_sprintf(buffer));

	free(buffer);
	return result;
}

char *sha256_sprintf(u8 *buffer)
{
	u8 length = strlen((char*)buffer);

	char *hash = (char*)malloc(sizeof(char) * length * 2);
	for(u8 i = 0; i < length; ++i) {
		sprintf(&hash[i * 2], "%s%X", ((u8)buffer[i] & 0xFF) < 0x10 ? "0" : "", (u8)buffer[i] & 0xFF);
	}

	return hash;
}

void be32enc_vect(u8 *dst, const u32 *src, size_t len)
{
	size_t i;
	uchar test;
	test.word = 1;

	//LITTLE_ENDIAN
	if(test.chr[0] == 1) {
		//printf("SS\n");
		for(i = 0; i < len / 4; i++) {
			be32enc(dst + i * 4, src[i]);
		}
	//BIG_ENDIAN(Можно удалить)
	} else {
		//printf("DD\n");
		memcpy(dst, src, len);
	}
}

static inline void be32enc(u8 *pp, u32 u)
{
	u8 *p = (u8*)pp;

	p[0] = (u >> 24) & 0xff;
	p[1] = (u >> 16) & 0xff;
	p[2] = (u >> 8) & 0xff;
	p[3] = u & 0xff;
}

void be32dec_vect(u32 *dst, const u8 *src, size_t len)
{
	size_t i;
	uchar test;
	test.word = 1;
	
	//LITTLE_ENDIAN
	if(test.chr[0] == 1) {
		//printf("FF\n");
	    for(i = 0; i < len / 4; i++) {
			dst[i] = be32dec(src + i * 4);
		}
	//BIG_ENDIAN(Можно удалить)
	} else {
		//printf("GG\n");
	    memcpy(dst, src, len);
	}
}

static inline u32 be32dec(const u8 *pp)
{
	const u8 *p = (const u8*)pp;

	return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

void sha256_init(psha256_context ctx)
{
	ctx->count[0] = ctx->count[1] = 0;

	ctx->state[0] = 0x6A09E667;
	ctx->state[1] = 0xBB67AE85;
	ctx->state[2] = 0x3C6EF372;
	ctx->state[3] = 0xA54FF53A;
	ctx->state[4] = 0x510E527F;
	ctx->state[5] = 0x9B05688C;
	ctx->state[6] = 0x1F83D9AB;
	ctx->state[7] = 0x5BE0CD19;
}

void sha256_trans(u32 *state, const u8 block[64])
{
	u32 W[64];
	u32 S[8];
	u32 t0, t1;
	size_t i;

	be32dec_vect(W, block, 64);
	for(i = 16; i < 64; i++) {
		W[i] = s1(W[i - 2]) + W[i - 7] + s0(W[i - 15]) + W[i - 16];
	}

	memcpy(S, state, 32);

	RNDr(S, W, 0,  0x428a2f98);
	RNDr(S, W, 1,  0x71374491);
	RNDr(S, W, 2,  0xb5c0fbcf);
	RNDr(S, W, 3,  0xe9b5dba5);
	RNDr(S, W, 4,  0x3956c25b);
	RNDr(S, W, 5,  0x59f111f1);
	RNDr(S, W, 6,  0x923f82a4);
	RNDr(S, W, 7,  0xab1c5ed5);
	RNDr(S, W, 8,  0xd807aa98);
	RNDr(S, W, 9,  0x12835b01);
	RNDr(S, W, 10, 0x243185be);
	RNDr(S, W, 11, 0x550c7dc3);
	RNDr(S, W, 12, 0x72be5d74);
	RNDr(S, W, 13, 0x80deb1fe);
	RNDr(S, W, 14, 0x9bdc06a7);
	RNDr(S, W, 15, 0xc19bf174);
	RNDr(S, W, 16, 0xe49b69c1);
	RNDr(S, W, 17, 0xefbe4786);
	RNDr(S, W, 18, 0x0fc19dc6);
	RNDr(S, W, 19, 0x240ca1cc);
	RNDr(S, W, 20, 0x2de92c6f);
	RNDr(S, W, 21, 0x4a7484aa);
	RNDr(S, W, 22, 0x5cb0a9dc);
	RNDr(S, W, 23, 0x76f988da);
	RNDr(S, W, 24, 0x983e5152);
	RNDr(S, W, 25, 0xa831c66d);
	RNDr(S, W, 26, 0xb00327c8);
	RNDr(S, W, 27, 0xbf597fc7);
	RNDr(S, W, 28, 0xc6e00bf3);
	RNDr(S, W, 29, 0xd5a79147);
	RNDr(S, W, 30, 0x06ca6351);
	RNDr(S, W, 31, 0x14292967);
	RNDr(S, W, 32, 0x27b70a85);
	RNDr(S, W, 33, 0x2e1b2138);
	RNDr(S, W, 34, 0x4d2c6dfc);
	RNDr(S, W, 35, 0x53380d13);
	RNDr(S, W, 36, 0x650a7354);
	RNDr(S, W, 37, 0x766a0abb);
	RNDr(S, W, 38, 0x81c2c92e);
	RNDr(S, W, 39, 0x92722c85);
	RNDr(S, W, 40, 0xa2bfe8a1);
	RNDr(S, W, 41, 0xa81a664b);
	RNDr(S, W, 42, 0xc24b8b70);
	RNDr(S, W, 43, 0xc76c51a3);
	RNDr(S, W, 44, 0xd192e819);
	RNDr(S, W, 45, 0xd6990624);
	RNDr(S, W, 46, 0xf40e3585);
	RNDr(S, W, 47, 0x106aa070);
	RNDr(S, W, 48, 0x19a4c116);
	RNDr(S, W, 49, 0x1e376c08);
	RNDr(S, W, 50, 0x2748774c);
	RNDr(S, W, 51, 0x34b0bcb5);
	RNDr(S, W, 52, 0x391c0cb3);
	RNDr(S, W, 53, 0x4ed8aa4a);
	RNDr(S, W, 54, 0x5b9cca4f);
	RNDr(S, W, 55, 0x682e6ff3);
	RNDr(S, W, 56, 0x748f82ee);
	RNDr(S, W, 57, 0x78a5636f);
	RNDr(S, W, 58, 0x84c87814);
	RNDr(S, W, 59, 0x8cc70208);
	RNDr(S, W, 60, 0x90befffa);
	RNDr(S, W, 61, 0xa4506ceb);
	RNDr(S, W, 62, 0xbef9a3f7);
	RNDr(S, W, 63, 0xc67178f2);

	for(i = 0; i < 8; i++) {
		state[i] += S[i];
	}
}

void sha256_update(psha256_context ctx, const u8 *src, size_t len)
{
	u32 bitlen[2];
	u32 r;

	r = (ctx->count[1] >> 3) & 0x3f;
	bitlen[1] = ((u32)len) << 3;
	bitlen[0] = (u32)(len >> 29);

	if((ctx->count[1] += bitlen[1]) < bitlen[1]) {
		ctx->count[0]++;
	}
	ctx->count[0] += bitlen[0];

	if(len < 64 - r) {
		memcpy(&ctx->buf[r], src, len);
		return;
	}

	memcpy(&ctx->buf[r], src, 64 - r);
	sha256_trans(ctx->state, ctx->buf);
	src += 64 - r;
	len -= 64 - r;

	while(len >= 64) {
		sha256_trans(ctx->state, src);
		src += 64;
		len -= 64;
	}

	memcpy(ctx->buf, src, len);
}

void sha256_padding(psha256_context ctx)
{
	u8 len[8];
	u32 r, plen;

	be32enc_vect(len, ctx->count, 8);
	static u8 PAD[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	r = (ctx->count[1] >> 3) & 0x3f;
	plen = (r < 56) ? (56 - r) : (120 - r);
	sha256_update(ctx, PAD, (size_t)plen);

	sha256_update(ctx, (u8*)len, 8);
}

void sha256_final(u8 digest[32], psha256_context ctx)
{
	sha256_padding(ctx);
	be32enc_vect(digest, ctx->state, 32);
	memset(ctx, 0, sizeof(sha256_context));
}
