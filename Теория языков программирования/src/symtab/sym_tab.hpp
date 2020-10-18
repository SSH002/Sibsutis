#pragma once
#include <map>

using namespace std;

class SymbolTable
{
	public:
		SymbolTable();
		//Создание таблицы символов
		void set_symtab(AST *tree);
		//Вывод таблицы символов на экран
		void print_symtab();
		//Сравнение иденификаторов областей видимости
		bool compare_levels(string prev_lvl, string new_lvl);
		//Проверяем, не были ли занесены какие-то переменные в таблицу символов несколько раз
		vector <string> check_multi_declaration(vector <string> identificators);
};
