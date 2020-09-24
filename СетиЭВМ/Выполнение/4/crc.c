#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define _2MiB 2097152						//2 Мегабайта (в байтах)
#define polynomial 0xEDB88320				//Реверсионный полином (Образующий многочлен)

uint64_t get_file_size(const char *file_name);
void generate_table(uint32_t table[256]);

//Генерация таблицы констант
void generate_table(uint32_t table[256])
{
	uint32_t remainder;
	uint16_t i, j;

	for (i = 0; i < 256; ++i) {
		remainder = i;
		for (j = 0; j < 8; ++j) {
			if (remainder & 1) { 			//Если отделяемый бит равен "1"
				remainder >>= 1;
				remainder ^= polynomial;
			} else {
				remainder >>= 1;
			}
		}
		table[i] = remainder;
	}
}

//Вычисление контрольной суммы
uint32_t crc32(unsigned char *data)
{
	static uint32_t table[256];
	uint32_t crc = 0xFFFFFFFF;
	uint64_t len = strlen(data);

	generate_table(table);
	for (uint64_t i = 0; i < len; ++i) {
		crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
	}

	return ~crc;
}

//Возвращает размер файла
uint64_t get_file_size(const char *file_name)
{
	uint64_t position, file_size;
	FILE *file = fopen(file_name, "r");

	position = ftell(file);
	fseek(file, 0L, SEEK_END);
	file_size = ftell(file);
	fseek(file, position, SEEK_SET);
	fclose(file);

	return file_size;
}

//Считывание файла
char *read_file(const char *file_name)
{
	char *file_content, symbol;
	FILE *file = fopen(file_name, "r");
	uint64_t i = 0;

	file_content = (char*)malloc(sizeof(char) * get_file_size(file_name));
	while (fscanf(file, "%c", &file_content[i]) != EOF) {
		++i;
	}

	fclose(file);
	return file_content;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Указан неверный параметр. Пример запуска:\n./crc --file \"имя_файла\"\n./crc --str \"некая_строка\"\n");
		return 11;
	}

	if (!strcmp(argv[1], "--file")) {
		FILE *file = fopen(argv[2], "r");
		if (file != NULL) {
			uint64_t size = get_file_size(argv[2]);
			if (size < _2MiB) {
				printf("Файл должен весить не меньше 2 МБайт!\n");
				return 21;
			}

			char *data = read_file(argv[2]);
			printf("%" PRIX32 "\n", crc32(data));
		} else {
			printf("Ошибка! Указанный файл не существует.\n");
			return 31;
		}
	} else if (!strcmp(argv[1], "--str")) {
		printf("%" PRIX32 "\n", crc32(argv[2]));
	} else {
		printf("Указан неверный параметр. Пример запуска:\n./crc --file \"имя_файла\"\n./crc --str \"некая_строка\"\n");
		return 11;
	}

	return 0;
}