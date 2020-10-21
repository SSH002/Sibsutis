#include "cryptography.hpp"

lint gen_coprime(lint prime)
{
	lint x, y, key;

	do {
		key = gen_number(9);
	} while(egcd(prime, key, &x, &y) != 1);

	return key;
}

lint mod_inv(lint private1, lint p)
{
	lint key, y; 
	gcd(private1, p, &key, &y);

	while(key < 0) {
		key += p;
	}

	return key;
}

void shamir_encrypt(string filename)
{
	ofstream file;
	string Data = read_dec(filename);
	lint x, *Enc = new lint [Data.length()];

	lint p, c_n, d_n, c_m;
	p = gen_prime(9);
	c_n = gen_coprime(p - 1);
	d_n = mod_inv(c_n, p - 1);
	c_m = gen_coprime(p - 1),

	file.open("keys/shamir", ios::binary);
	file << p << " " << c_n << " " << d_n << " " << c_m << " " << mod_inv(c_m, p - 1) << endl;
	file.close();

	for(uint64 i = 0; i < Data.length(); ++i) {
		x = mod_pow(Data[i], c_n, p);
		x = mod_pow(x, c_m, p);
		Enc[i] = mod_pow(x, d_n, p);
	}


	write_enc(filename + ".sha.enc", Enc, Data.length());
	delete []Enc;
}

void shamir_decrypt(string filename)
{
	uint64 length;
	lint *Dec = read_enc(filename + ".sha.enc", &length);
	lint p, d_m;

	ifstream file;
	file.open("keys/shamir", ios::binary);
	file >> p >> d_m >> d_m >> d_m >> d_m;
	file.close();

	for(uint64 i = 0; i < length; ++i) {
		Dec[i] = mod_pow(Dec[i], d_m, p);
	}

	write_dec(filename + ".sha.dec", Dec, length);
	delete []Dec;
}

void el_gamal_encrypt(string filename)
{
	string Data = read_dec(filename);
	lint *Enc = new lint [Data.length()];
	lint k, c_n, d_n;

	ofstream file;
	file.open("keys/el-gamal", ios::binary);
	lint p = gen_p(), g = gen_g(p);
	k = gen_number_max(p - 1);

	c_n = gen_number_max(p);
	d_n = mod_pow(g, c_n, p);
	file << p << " " << g << " " << k << " " << c_n << " " << d_n << " " << mod_pow(g, k, p) << endl;
	file.close();

	for(uint64 i = 0; i < Data.length(); ++i) {
		Enc[i] = Data[i] * mod_pow(d_n, k, p) % p;
	}

	write_enc(filename + ".elg.enc", Enc, Data.length());
	delete []Enc;
}

void el_gamal_decrypt(string filename)
{
	uint64 length;
	lint *Dec = read_enc(filename + ".elg.enc", &length);
	lint p, c_n, c_m;

	ifstream file;
	file.open("keys/el-gamal", ios::binary);
	file >> p >> c_n >> c_n >> c_n >> c_m >> c_m;
	file.close();

	for(uint64 i = 0; i < length; ++i) {
		Dec[i] = Dec[i] * mod_pow(c_m, p - c_n - 1, p) % p;
	}

	write_dec(filename + ".elg.dec", Dec, length);
	delete []Dec;
}

uint8 count_1bits(uint8 symbol)
{
	uint8 count = 0;

	for(; symbol; symbol &= symbol - 1) {
		++count;
	}

	return count;
	//return ((uint64)0x08040201 * symbol & 0x111111111) % 15;
}

void vernam_encrypt(string filename)
{
	string Data = read_dec(filename);
	lint *Enc = new lint [Data.length()];
	lint key, low_border, up_border;

	ofstream file;
	file.open("keys/vernam", ios::binary);

	for(uint64 i = 0; i < Data.length(); ++i) {
		uint8 count = count_1bits((uint8)Data[i]);

		switch(Data[i]) {
			case 0:
				key = 0;
			break;
			case 1:
				key = 1;
			break;
			default:
				low_border = lint(1) << (count - 1);
				up_border = (lint(1) << count) - lint(1);

				if(up_border - low_border < 2) {
					key = low_border;
				} else {
					key = low_border + gen_number_max(up_border - low_border);
				}
			break;
		}

		Enc[i] = Data[i] ^ key;
		file << key << endl;
	}

	file.close();
	write_enc(filename + ".ver.enc", Enc, Data.length());
	delete []Enc;
}

void vernam_decrypt(string filename)
{
	uint64 length;
	lint *Dec = read_enc(filename + ".ver.enc", &length);
	lint key;

	ifstream file;
	file.open("keys/vernam", ios::binary);

	for(uint64 i = 0; i < length; ++i) {
		file >> key;
		Dec[i] ^= key;
	}

	file.close();
	write_dec(filename + ".ver.dec", Dec, length);
	delete []Dec;
}

void rsa_encrypt(string filename)
{
	string Data = read_dec(filename);
	lint *Enc = new lint [Data.length()];

	ofstream file;
	file.open("keys/rsa", ios::binary);
	lint p = gen_prime(9), q = gen_prime(8);

	lint publ = p * q;
	lint priv1 = gen_coprime((p - 1) * (q - 1));
	lint priv2 = mod_inv(priv1, (p - 1) * (q - 1));

	file << publ << " " << priv1 << " "<< priv2 << endl;
	file.close();

	for(uint64 i = 0; i < Data.length(); ++i) {
		Enc[i] = mod_pow(Data[i], priv1, publ);
	}

	write_enc(filename + ".rsa.enc", Enc, Data.length());
	delete []Enc;
}

void rsa_decrypt(string filename)
{
	uint64 length;
	lint *Dec = read_enc(filename + ".rsa.enc", &length);
	lint publ, priv2;

	ifstream file;
	file.open("keys/rsa", ios::binary);
	file >> publ >> priv2 >> priv2;
	file.close();

	for(uint64 i = 0; i < length; ++i) {
		Dec[i] = mod_pow(Dec[i], priv2, publ);
	}

	write_dec(filename + ".rsa.dec", Dec, length);
	delete []Dec;
}
