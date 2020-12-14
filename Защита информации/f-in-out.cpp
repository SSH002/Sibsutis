#include <filesystem>
#include "cryptography.hpp"
using namespace std::filesystem;

uint8 check_file(string file_name)
{
	char *fname = new char [file_name.length()];
	strcpy(fname, file_name.c_str());

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

lint *read_enc(string file_name, uint64 *length)
{
	ifstream file;
	file.open(file_name);

	file >> *length;
	lint *enc = new lint [*length];

	for(uint64 i = 0; i < *length; ++i) {
		file >> enc[i];
	}
	file.close();

	return enc;
}

string read_dec(string file_name)
{
	ifstream file;
	file.open(file_name, ios::binary);
	stringstream buffer;
	buffer << file.rdbuf();

	file.close();
	return buffer.str();
}

string read_dec(string file_name, uint64 size)
{
	FILE *file;
	file = fopen(file_name.c_str(), "r");
	unique_ptr<char[]> buffer(new char [size]);

	for(uint64 i = 0; i < size; ++i) {
		fscanf(file, "%c", &buffer[i]);
	}
	fclose(file);

	string result;
	result.resize(size);
	for(uint64 i = 0; i < size; ++i) {
		result[i] = buffer[i];
	}
	
	return result;
}

void write_enc(string file_name, lint *enc, uint64 length)
{
	ofstream file;
	file.open(file_name, ios::binary);

	file << length << " ";
	for(uint64 i = 0; i < length; ++i) {
		file << enc[i] << " ";
	}

	file.close();
}

void write_dec(string file_name, lint *dec, uint64 length)
{
	ofstream file;
	file.open(file_name, ios::binary);

	for(uint64 i = 0; i < length; ++i) {
		file << (char)dec[i];
	}

	file.close();
}

array<lint, 32> read_ds(string file_name, uint64 size)
{
	ifstream file;
	file.open(file_name);
	file.seekg(size, ios::beg);

	array<lint, 32> DS;
	for(uint64 i = 0; i < 32; ++i) {
		file >> DS[i];
	}

	file.close();

	return DS;
}

void write_ds(string file_name, array<lint, 32> S)
{
	ofstream file;
	file.open(file_name, ios::app);

	file << endl;
	for(uint8 i = 0; i < 32; ++i) {
		file << S[i] << " ";
	}
	
	file.close();
}

void delete_ds(string file_name, uint64 size)
{
	resize_file(file_name, size);
}
