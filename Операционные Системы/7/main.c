#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

int main(int argc, char *argv[])
{
	char *error;
	void *library = dlopen("build/libmath.so", RTLD_LAZY);
	if (!library) {
		fputs(dlerror(), stderr);
		return 5;
	}

	int num = 0, i = 1, s = 1, k, r, sign = 0;
	double answer;
	int proverkaNum = 0, proverkaWord = 0, priority = 0;
	
	while(1) {
		//подсчитываем кол-во чисел
		if(*argv[s] >= 48 && *argv[s] <= 57) {
			++num;
			++s;
			++proverkaNum;
		//подсчитываем кол-во знаков операций
		} else if((*argv[s] >= 42 && *argv[s] <= 43) || (*argv[s] == 45) || (*argv[s] == 47)) {
			++sign;
			++s;
			--proverkaWord;
		}//проверка, что находится в принмающем массиве
		if ((*argv[i] >= 42 && *argv[i] <= 43) || (*argv[i] == 45) || (*argv[i] >= 47 && *argv[i] <= 57)) {
			if(*argv[i] == 42 || *argv[i] == 47)
				++priority;
			++i;
		} else {
			printf("ОШИБКА!\n");
			printf("Проверьте правильность ввода выражения\nВвод знаков операций\nСумма - [+]\nРазность - [-]\nУмножение - [(кавычки)*(кавычки)]\nДеление - [/]\nПример ввода:\n./cmdcalc 3 + 4 (кавычки)*(кавычки) 7 / 10\n");
			return 0;
		}
		if(i == argc) {
			break;
		}
	}
	i = 1;
	s = 0;
	while(1) {
		if(*argv[i] >= 48 && *argv[i] <= 57) {
			++s;
		} else if((*argv[i] >= 42 && *argv[i] <= 43) || (*argv[i] == 45) || (*argv[i] == 47)) {
			s--;
		}
		if(s > 1 || s < 0) {
			printf("ОШИБКА!\n");
			printf("Проверьте правильность ввода выражения\nВвод знаков операций\nСумма - [+]\nРазность - [-]\nУмножение - [(кавычки)*(кавычки)]\nДеление - [/]\nПример ввода:\n./cmdcalc 3 + 4 (кавычки)*(кавычки) 7 / 10\n");
			return 0;
		}
		if(i == argc - 1) {
			break;
		} else if(i > argc) {
			printf("ОШИБКА!\n");
			printf("Проверьте правильность ввода выражения\nВвод знаков операций\nСумма - [+]\nРазность - [-]\nУмножение - [(кавычки)*(кавычки)]\nДеление - [/]\nПример ввода:\n./cmdcalc 3 + 4 (кавычки)*(кавычки) 7 / 10\n");
			return 0;
		}
		++i;
	}
	// проверка на корректность ввода, если кол-во знаков не соответствует кол-ву цифр, то выводит ошибку
	if(num - 1 == sign) {
		s = 0;
		// массив с числами
		double number[num + 1];
		for(i = 1; i < argc; ++i) {
			if(*argv[i] >= 48 && *argv[i] <= 57) {
				//записываем в массив переведенное число из принимающего массива argv[]
				number[s] = atof(argv[i]);
				++s;
			}
		}
		// массив со знаками операций
		char word[sign + 1];
		i = 0;
		num -= 1;
		int dop = 1;
		while(1) {
			// из принимающего массива argv[] вычленяем знаки операций и записываем в массив знаков
			if((*argv[dop] >= 42 && *argv[dop] <= 43) || (*argv[dop] == 45) || (*argv[dop] == 47)) {
				word[i] = *argv[dop];
				++i;
			}
			++dop;
			if(dop == argc) {
				break;
			}
		}
		num += 1;
		i = 0;
		for(s = 0; s <= sign + 1; ++s) {
			for(k = 0; k <= sign + 1; ++k) {
			// в соответствии какой знак в массиве word выполняется соответствующая операция
				if(word[k] == 42 || word[k] == 47) {
					switch(word[k]) {
						case '*':
							double (*cmd)(double, double) = dlsym(library, "multiplication");
							if ((error = dlerror()) != NULL) {
								fprintf(stderr, "%s\n", error);
								return 7;
							}
							number[k] = (*cmd)(number[k], number[k + 1])
							for(r = k + 1; r < num; ++r) {
								number[r] = number[r + 1];
							}
							for(r = k; r < sign; ++r) {
								word[r] = word[r + 1];
							}
							--priority;
						break;
						case '/':
							double (*cmd)(double, double) = dlsym(library, "division");
							if ((error = dlerror()) != NULL) {
								fprintf(stderr, "%s\n", error);
								return 8;
							}
							number[k] = (*cmd)(number[k], number[k + 1])
							for(r = k + 1; r < num; ++r) {
								number[r] = number[r + 1];
							}
							for(r = k; r < sign; ++r) {
								word[r] = word[r + 1];
							}
							--priority;
						break;
					}
				} else if(!priority) {
					switch(word[k]) {
						case '+':
							double (*cmd)(double, double) = dlsym(library, "summation");
							if ((error = dlerror()) != NULL) {
								fprintf(stderr, "%s\n", error);
								return 9;
							}
							number[k] = (*cmd)(number[k], number[k + 1])
							for(r = k + 1; r < num; ++r) {
								number[r] = number[r + 1];
							}
							for(r = k; r < sign; ++r) {
								word[r] = word[r + 1];
							}
						break;
						case '-':
							double (*cmd)(double, double) = dlsym(library, "difference");
							if ((error = dlerror()) != NULL) {
								fprintf(stderr, "%s\n", error);
								return 10;
							}
							number[k] = (*cmd)(number[k], number[k + 1])
							for(r = k + 1; r < num; ++r) {
								number[r] = number[r + 1];
							}
							for(r = k; r < sign; ++r) {
								word[r] = word[r + 1];
							}
						break;
					}
				}
			}
		}
		answer = number[0];
		//вывод конечного результата
		printf("\nОтвет = %.02f\n", answer);
	} else {
		printf("ОШИБКА!\n");
		printf("Проверьте правильность ввода выражения:\nСумма - [+]\nРазность - [-]\nУмножение - [\"*\"]\nДеление - [/]\nПример ввода:\n./clc 3 + 4 \"*\" 7 / 10 \n");	
	}
	return 0;
}