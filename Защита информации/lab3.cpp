#include "cryptography.hpp"

#define Qmin 32768
#define Qmax 65535
#define Bmin 16384
#define Bmax 65535

array<uint8, 32> sha256_to_bytes(string hash)
{
	array<uint8, 32> Bytes;
	uint8 high, low;

	for(uint8 i = 0, j = 0; i < 32; ++i, j += 2) {
		high = (hash[j] < 'A') ? hash[j] << 4 : (hash[j] - 55) << 4;
		low = (hash[j + 1] < 'A') ? hash[j + 1] & 0b00001111 : hash[j + 1] - 55;

		Bytes[i] = high | low;
	}

	return Bytes;
}

lint mod(lint n, lint d)
{
	lint result = n % d;
	if(result * d < 0) result += d;

	return result;
}

void rsa_sign(string file_name, lint &d, lint &n)
{
	lint p = gen_prime(4), q = gen_prime(4);
	n = p * q, d = gen_coprime((p - 1) * (q - 1));
	lint c = mod_inv(d, (p - 1) * (q - 1));

	string hash = sha256(read_dec(file_name));
	array<uint8, 32> H = sha256_to_bytes(hash);
	array<lint, 32> S;
	
	for(uint8 i = 0; i < 32; ++i) {
		S[i] = mod_pow(H[i], c, n);
	}

	write_ds(file_name, S);
}

bool rsa_valid(string file_name, lint d, lint n, uint64 size)
{
	array<lint, 32> S = read_ds(file_name, size);
	lint w;

	string hash = sha256(read_dec(file_name, size));
	array<uint8, 32> H = sha256_to_bytes(hash);

	for(uint8 i = 0; i < 32; ++i) {
		w = mod_pow(S[i], d, n);

		if(w != H[i]) return false;
	}

	return true;
}

void el_gamal_sign(string file_name, lint &p, lint &g, lint &y, lint &r)
{
	string hash = sha256(read_dec(file_name));
	array<uint8, 32> H = sha256_to_bytes(hash);
	array<lint, 32> S;

	p = gen_p(); g = gen_g(p);

	lint k, x, u;
	x = gen_number_max(p - 1);
	y = mod_pow(g, x, p);

	do {
		k = gen_number_max(p - 1);
	} while(gcd(k, (p - 1)) != 1);

	r = mod_pow(g, k, p);
	lint inv_k = mod_inv(k, p - 1);

	for(uint8 i = 0; i < 32; ++i) {
		u = mod(H[i] - x * r, p - 1);
		S[i] = mod(inv_k * u, p - 1);
	}

	write_ds(file_name, move(S));
}

bool el_gamal_valid(string file_name, lint p, lint g, lint y, lint r, uint64 size)
{
	string hash = sha256(read_dec(file_name, size));
	array<uint8, 32> H = sha256_to_bytes(hash);
	array<lint, 32> S = read_ds(file_name, size);
	lint a, b;

	for(uint i = 0; i < 32; ++i) {
		a = (mod_pow(y, r, p) * mod_pow(r, S[i], p)) % p;
		b = mod_pow(g, H[i], p);

		if(a != b) return false;
	}

	return true;
}

void gost_sign(string file_name, lint &p, lint &q, lint &r, lint &a, lint &y)
{
	lint b, x, k;

	do {
		q = Qmin + rand() % (Qmax - Qmin);
	} while(!is_prime(q));

	do {
		b = Bmin + rand() % (Bmax - Bmin);
		p = b * q + 1;
	} while(!is_prime(p));

	do {
		a = mod_pow(gen_number_max(p - 1), b, p);
	} while(mod_pow(a, q, p) != 1);

	string hash = sha256(read_dec(file_name));
	array<uint8, 32> H = sha256_to_bytes(hash);
	array<lint, 32> S;

	x = gen_number_max(q);
	y = mod_pow(a, x, p);
	uint8 i;

	do {
		i = 0;

		do {
			k = gen_number_max(q);
			r = mod_pow(a, k, p) % q;
		} while(!r);

		for(; i <= 31; ++i) {
			S[i] = (k * H[i] + x * r) % q;
			if(!S[i]) break;
		}
	} while(!S[i]);

	write_ds(file_name, move(S));
}

bool gost_valid(string file_name, lint p, lint q, lint r, lint a, lint y, uint64 size)
{
	string hash = sha256(read_dec(file_name, size));
	array<uint8, 32> H = sha256_to_bytes(hash);
	array<lint, 32> S = read_ds(file_name, size);
	lint u1, u2, v, inv_h;

	for(uint8 i = 0; i < 32; ++i) {
		inv_h = mod_inv(H[i], q);
		u1 = mod(S[i] * inv_h, q);
		u2 = mod(-r * inv_h, q);
		v = mod_pow(a, u1, p) * mod_pow(y, u2, p) % p % q;

		if(v != r) return false;
	}

	return true;
}
