#include "cryptography.hpp"

uint8 check_file(string filename)
{
	char *fname = new char [filename.length()];
	strcpy(fname, filename.c_str());

	DIR *directory;
	if((directory = opendir(fname))) {
		cout << RED << "Ошибка! Это каталог.\n" << NORMAL;
		closedir(directory);
		return 3;
	} else {
		FILE *file = fopen(fname, "r");
		if(!file) {
			cout << RED << "Ошибка! Указанный файл не существует.\n" << NORMAL;
			return 2;
		}
	}

	return 0;
}

lint *read_enc(string filename, uint64 *length)
{
	ifstream file;
	file.open(filename);

	file >> *length;
	lint *enc = new lint [*length];

	for(uint64 i = 0; i < *length; ++i) {
		file >> enc[i];
	}
	file.close();

	return enc;
}

string read_dec(string filename)
{
	ifstream file;
	file.open(filename, ios::binary);
	stringstream buffer;
	buffer << file.rdbuf();

	file.close();
	return buffer.str();
}

void write_enc(string filename, lint *enc, uint64 length)
{
	ofstream file;
	file.open(filename, ios::binary);

	file << length << " ";
	for(uint64 i = 0; i < length; ++i) {
		file << enc[i] << " ";
	}

	file.close();
}

void write_dec(string filename, lint *dec, uint64 length)
{
	ofstream file;
	file.open(filename, ios::binary);

	for(uint64 i = 0; i < length; ++i) {
		file << (char)dec[i];
	}

	file.close();
}
