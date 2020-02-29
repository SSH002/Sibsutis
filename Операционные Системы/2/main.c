#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

long int file_size(char *file_name);
char *proc_name(char *direct_name);

/*1-ое задание*/
void user_interface()
{
	char buffer[30];
	char hello0[] = "Введите своё имя: ";
	char hello1[] = "Вы вошли в систему под именем: ";
	char note[] = "\nНажмите любую клавишу, чтобы продолжить...";

	write(0, hello0, strlen(hello0));
	read(0, buffer, 30);
	write(0, hello1, strlen(hello1));
	write(0, buffer, strlen(buffer));
	write(0, note, strlen(note));
	getchar();
}

/*2-е задание*/
//Получает номер процесса
int pid(char *direct_name)
{
	DIR *directory;
	struct stat st;
	struct dirent *dir;
	int i = 0;
	char name[PATH_MAX];
	char *buffer = (char*) malloc(sizeof(char) * PATH_MAX);
	char *name2 = (char*) malloc(sizeof(char) * PATH_MAX);
	char pid[PATH_MAX];

	size_t len = strlen(direct_name);

	if((directory = opendir(direct_name)) == NULL) {
		perror(direct_name);
		return 1;
	}

	strncpy(buffer, direct_name, len);

	while ((dir = readdir(directory)) != NULL) {
		buffer[len] = '/';
		buffer[len + 1] = '\0';
		strcat(buffer, dir->d_name);
		strcpy(pid, dir->d_name);
		if(lstat(buffer, &st) == -1) {
			perror(dir->d_name);
			break;
		}
		if(S_ISDIR(st.st_mode)) {
			if ((buffer[6] >= '0') && (buffer[6] <= '9')) {
				strcpy(name, buffer);
				strcat(name, "/stat");
				//printf("name: %s | ", name);
				name2 = proc_name(name);
				//strcpy(name2, pid);
				printf("\n(name: %s)", name2);
				printf(" [pid: %s]", pid);
			}
		}
	}

	closedir(directory);
	return 0;
}

//Возвращает имя процесса
char *proc_name(char *file_name)
{
	FILE *file = fopen(file_name, "r");
	short param = 1, i = 0;
	char *proc_name = (char*) malloc(sizeof(char) * 30);
	char symb;

	while(fscanf(file, "%c", &symb) != EOF) {
		if (!param) {
                        if (symb != ')') {
				proc_name[i] = symb;
				++i;
			} else {
				break;		//Завершаем цикл
			}
                }
		if (symb == '(') {
			param = 0;		//Начинаем считывание файла
		}
	}

	fclose(file);
	proc_name = (char*) realloc(proc_name, i * sizeof(char));
	proc_name[i] = '\0';

	return proc_name;
}

/*3-е задание*/
//Получеам список файлов в директории "filelist"
long int direct_list(char *filelist)
{
	long int size = 0;
	DIR *direct = opendir(filelist);
	if (!direct) {
		return 1;
	}

	FILE *file;
	struct dirent *dir;

	while ((dir = readdir(direct))) {
		if(dir->d_type != DT_DIR) {
			size += file_size(dir->d_name);
		}
	}

	closedir(direct);
	return size;
}

//Возвращает размер файла
long int file_size(char *file_name)
{
	long int save_pos, size_of_file;
	FILE *file = fopen(file_name, "r");

	save_pos = ftell(file);
	fseek(file, 0L, SEEK_END);
	size_of_file = ftell(file);
	fseek(file, save_pos, SEEK_SET);
	fclose(file);

	return size_of_file;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("\nОшибка! мало аргументов. Пример: \"./lab2 <имя_папки>\"\n");
		return 1;
	} else {
		DIR *directory;
		if ((directory = opendir(argv[1])) == NULL) {
			printf("\nОшибка! Папка \"%s\" не существует.\n", argv[1]);
			return 2;
		}
		printf("\n*-----Задание_№1-----*\n");
		user_interface();
		printf("\n*-----Задание_№2-----*");
		pid("/proc");
		printf("\n\n*-----Задание_№3-----*\n");
		printf("Размер папки: %ld байт\n", direct_list(argv[1]));
	}

	return 0;
}