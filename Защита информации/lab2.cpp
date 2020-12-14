#include "cryptography.hpp"

lint gen_coprime(lint prime)
{
	lint key;
	uint64 order = get_order(prime) - 1;

	do {
		key = gen_number(order);
	} while(gcd(prime, key) != 1);

	return key;
}

lint mod_inv(lint priv1, lint p)
{
	lint key, y; 
	gcd(priv1, p, &key, &y);

	while(key < 0) {
		key += p;
	}

	return key;
}

void shamir_enc(string file_name)
{
	ofstream file;
	string Data = read_dec(file_name);
	lint x, *Enc = new lint [Data.length()];

	lint p, c_n, d_n, c_m;
	p = gen_prime(9);
	c_n = gen_coprime(p - 1);
	d_n = mod_inv(c_n, p - 1);
	c_m = gen_coprime(p - 1);

	file.open("keys/lab2/shamir", ios::binary);
	file << p << " " << c_n << " " << d_n << " " << c_m << " " << mod_inv(c_m, p - 1) << endl;
	file.close();

	for(uint64 i = 0; i < Data.length(); ++i) {
		x = mod_pow(Data[i], c_n, p);
		x = mod_pow(x, c_m, p);
		Enc[i] = mod_pow(x, d_n, p);
	}

	write_enc(file_name + ".sha.enc", Enc, Data.length());
	delete []Enc;
}

void shamir_dec(string file_name)
{
	uint64 length;
	lint *Dec = read_enc(file_name + ".sha.enc", &length);
	lint p, d_m;

	ifstream file;
	file.open("keys/lab2/shamir", ios::binary);
	file >> p >> d_m >> d_m >> d_m >> d_m;
	file.close();

	for(uint64 i = 0; i < length; ++i) {
		Dec[i] = mod_pow(Dec[i], d_m, p);
	}

	write_dec(file_name + ".sha.dec", Dec, length);
	delete []Dec;
}

void el_gamal_enc(string file_name)
{
	string Data = read_dec(file_name);
	lint *Enc = new lint [Data.length()];
	lint k, c, d;

	ofstream file;
	file.open("keys/lab2/el-gamal", ios::binary);
	lint p = gen_p(), g = gen_g(p);
	k = gen_number_max(p - 1);

	c = gen_number_max(p);
	d = mod_pow(g, c, p);
	file << p << " " << g << " " << k << " " << c << " " << d << " " << mod_pow(g, k, p) << endl;
	file.close();

	for(uint64 i = 0; i < Data.length(); ++i) {
		Enc[i] = Data[i] * mod_pow(d, k, p) % p;
	}

	write_enc(file_name + ".elg.enc", Enc, Data.length());
	delete []Enc;
}

void el_gamal_dec(string file_name)
{
	uint64 length;
	lint *Dec = read_enc(file_name + ".elg.enc", &length);
	lint p, c, r;

	ifstream file;
	file.open("keys/lab2/el-gamal", ios::binary);
	file >> p >> c >> c >> c >> r >> r;
	file.close();

	for(uint64 i = 0; i < length; ++i) {
		Dec[i] = Dec[i] * mod_pow(r, p - c - 1, p) % p;
	}

	write_dec(file_name + ".elg.dec", Dec, length);
	delete []Dec;
}

uint8 binary_size(uint8 byte)
{
	uint8 size = 0;

	for(; byte; byte >>= 1) {
		++size;
	}

	return size;
}

void vernam_enc(string file_name)
{
	string Data = read_dec(file_name);
	lint *Enc = new lint [Data.length()];
	lint key, low_border, up_border;
	uint8 size;

	ofstream file;
	file.open("keys/lab2/vernam", ios::binary);

	for(uint64 i = 0; i < Data.length(); ++i) {
		size = binary_size((uint8)Data[i]);

		switch(Data[i]) {
			case 0:
				key = 0;
			break;
			case 1:
				key = 1;
			break;
			default:
				low_border = 1 << (size - 1);
				up_border = (1 << size) - 1;

				if(up_border - low_border < 2) {
					key = low_border;
				} else {
					key = low_border + (rand() % (up_border - low_border));
				}
			break;
		}

		Enc[i] = Data[i] ^ key;
		file << key << endl;
	}

	file.close();
	write_enc(file_name + ".ver.enc", Enc, Data.length());
	delete []Enc;
}

void vernam_dec(string file_name)
{
	uint64 length;
	lint *Dec = read_enc(file_name + ".ver.enc", &length);
	lint key;

	ifstream file;
	file.open("keys/lab2/vernam", ios::binary);

	for(uint64 i = 0; i < length; ++i) {
		file >> key;
		Dec[i] ^= key;
	}

	file.close();
	write_dec(file_name + ".ver.dec", Dec, length);
	delete []Dec;
}

void rsa_enc(string file_name)
{
	string Data = read_dec(file_name);
	lint *Enc = new lint [Data.length()];

	ofstream file;
	file.open("keys/lab2/rsa", ios::binary);
	lint p = gen_prime(4), q = gen_prime(4);

	lint d = p * q;
	lint n1 = gen_coprime((p - 1) * (q - 1));

	file << d << " " << n1 << " " << mod_inv(n1, (p - 1) * (q - 1)) << endl;
	file.close();

	for(uint64 i = 0; i < Data.length(); ++i) {
		Enc[i] = mod_pow(Data[i], n1, d);
	}

	write_enc(file_name + ".rsa.enc", Enc, Data.length());
	delete []Enc;
}

void rsa_dec(string file_name)
{
	uint64 length;
	lint *Dec = read_enc(file_name + ".rsa.enc", &length);
	lint d, n2;

	ifstream file;
	file.open("keys/lab2/rsa", ios::binary);
	file >> d >> n2 >> n2;
	file.close();

	for(uint64 i = 0; i < length; ++i) {
		Dec[i] = mod_pow(Dec[i], n2, d);
	}

	write_dec(file_name + ".rsa.dec", Dec, length);
	delete []Dec;
}
