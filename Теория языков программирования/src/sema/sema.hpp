#pragma once
#include <regex>
using namespace std;

const regex reg_var = regex("^[$][A-Za-z_][A-Za-z0-9_]*$");
const regex reg_10 = regex("^-?[1-9][0-9]*$");
const regex reg_2 = regex("^-?0[bB][01]+$");
const regex reg_8 = regex("^-?0[0-7]+$");
const regex reg_16 = regex("^-?0[xX][A-Fa-f0-9]+$");

class Sema
{
	public:
		Sema();
		//Проверка, совпадают ли типы данных у переменных
		void check_opers_data_types(vector <string> operands, map <string, string> type_id, string op);
		//Проверка, является ли оператор бинарным
		bool check_binary_oper_sign(string operation);
		//Семантический анализ программы
		void check_semantic(AST *tree);
		//Вывод таблицы идентификаторов на экран
		void print_semantic();
};

//Проверка, является ли "identificator" числом
bool is_number(string identificator);
//Проверка, является ли "variable" переменной
bool is_variable(string variable);
//По "identificator" определяем тип данных
string set_data_type(string identificator, map <string, string> iden_table);
