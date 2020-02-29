#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <semaphore.h>  
#include <string.h> 
#include <inttypes.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define hertz 2500000000			//Тактовая частота вашего процессора (в герцах)

//Замеры времени
uint64_t tsc_gtime()
{
	uint32_t high, low;
	
	__asm__ __volatile__ (
	"rdtsc\n"
	"movl %%edx, %0\n"
	"movl %%eax, %1\n"
		: "=r" (high), "=r" (low)
		:: "%rax", "%rbx", "%rcx", "%rdx"
	);

	return ((uint64_t)high << 32) | low;
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

typedef struct {
	long type;
	char text[100];
} mymsg;

//3-я лаба по ОС, 1-ое задание 
void task1() 
{ 
	pid_t pid, kill; 
	int rv; 
	char str[12] = "kill \0"; 
	char number[5]; 

	switch(pid = fork()) { 
		case -1: 
			exit(1);
		break;
		case 0: 
			system("./lab2 ."); 
			exit(1);
		break;
		default:
			wait(0);
			printf("\n\n*-----------------------------------*"); 
			printf("\nКого убивать пойдём? (нужно ввести pid жертвы): "); 
			scanf("%d", &kill); 
			sprintf(number, "%d", kill); 
			system(strcat(str, number));
		break;
	} 
} 

//3-я лаба по ОС, 2-ое задание 
void task2() 
{ 
	int pid, to_child[2], to_parent[2];

	if(pipe(to_child)) {
		exit(1);
	}
	if(pipe(to_parent)) {
		exit(2);
	}

	switch (pid = fork()) {
		case -1:
			perror("Нет возможности создать процесс-потомок!");
			exit(3);
		break;
		case 0:
			close(to_parent[0]);
			close(to_child[1]);

			printf("Это процесс-потомок!\n\n");
			char download_URL[60] = "http://speedtest.ftp.otenet.gr/files/test1Mb.db";
			char filename[20] = "testFile.txt";
			char command[100];

			printf("Скачиваю с помощью 'wget' ...\n\n");
			sprintf(command, "wget -O %s %s", filename, download_URL);
			system(command); //Скачиваем файл по ссылке "download_URL" и сохраняем под именем "filename"

			long int filesize = file_size(filename);
			printf("Размер файла: %ld байт. Отправляю размер предку...\n", filesize);
			write(to_parent[1], &filesize, sizeof(long int));
			printf("\nПроцесс-потомок: завершение процесса...\n");
		break;
		default:
			close(to_child[0]);
			close(to_parent[1]);

			uint64_t time;
			char buffer[30];

			printf("\n\nПривет от предка!\n\nЗамеряю время...\n\n");
			time = tsc_gtime();

			int childExited = waitpid(pid, NULL, WNOHANG);
			printf("Затрачено времени: %lf сек\n", (double)time / hertz);
			while (!childExited) {
				sleep(3);
				time = tsc_gtime() - time;
				printf("\n%s\n", buffer);
				childExited = waitpid(pid, NULL, WNOHANG);
			}

			printf("\nРодитель: процесс-потомок успешно завершился...\n");
			long int recv_file_size = 0;
			read(to_parent[0], &recv_file_size, sizeof(long int));
			printf("Файл, принятый потомком весит: %lf Мбайт\n", (double)(recv_file_size / (1024 * 1024)));
		break;
	}
}

void task3() 
{
	mymsg m1, m2;
	key_t key1, key2;
	int id1, id2, pid, received;
	
	switch(pid = fork()){
		case -1:
				exit(1);
		break;
		case 0:
			key1 = ftok("test", 42);
			printf("My key is %d\n", key1);

			id1 = msgget(key1, IPC_CREAT | 0666);
			printf("Идентификатор сообщения: %d\n", id1);

			printf("Отправка сообщения\n");
			memset(m1.text, 0, 100); /* Clear out the space */
			strcpy(m1.text, "Hello, world!");
			m1.type = 1;
			msgsnd(id1, &m1, sizeof(m1), 0);
		break;
		default:
			key2 = ftok("test", 42);
			printf("My key is %d\n", key2);

			id2 = msgget(key2, 0);
			printf("Message identifier is %d\n", id2);

			printf("Принятие сообщения\n");
			received = msgrcv(id2, &m2, sizeof(m2), 0, 0);
			printf("%s (%d)\n", m2.text, received);
		break;
	}
}

int main() 
{
	task1(); 
	task2();
	task3();

	return 0; 
}