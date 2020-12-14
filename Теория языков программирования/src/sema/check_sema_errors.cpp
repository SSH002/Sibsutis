#include "parser.hpp"

//Проверка, является ли "identificator" числом
bool is_number(string identificator)
{
	if(regex_search(identificator.begin(), identificator.end(), reg_10) || identificator == "0"
		|| regex_search(identificator.begin(), identificator.end(), reg_2)
		|| regex_search(identificator.begin(), identificator.end(), reg_8)
		|| regex_search(identificator.begin(), identificator.end(), reg_16)) {
		return true;
	}

	return false;
}

//Проверка, является ли "variable" переменной
bool is_variable(string variable)
{
	if(regex_search(variable.begin(), variable.end(), reg_var)) {
		return true;
	}

	return false;
}

//Возвращает тип данных переменной
string set_data_type(string identificator, map <string, string> iden_table)
{
	//Если переменная была внесена в таблицу идентификаторов
	if(is_variable(identificator)) {
		for(auto it = iden_table.begin(); it != iden_table.end(); ++it) {
			if(it->first == identificator) {
				return it->second;
			}
		}
	//Строка
	} else if(identificator[0] == '\'' || identificator[0] == '\"') {
		return "string";
	//Числовая константа
	} else if(is_number(identificator)) {
		return "int";
	//Логический тип
	} else if(identificator == "true" || identificator == "false") {
		return "bool";
	}
}


//Проверка, совпадают ли типы данных у переменных в векторе "leaves"
void Sema::check_opers_data_types(vector <string> leaves, map <string, string> iden_table, string oper)
{
	vector <string> data_types;

	for(int i = 0; i < leaves.size(); ++i) {
		//Если "leaves[i]" - переменная
		if(is_variable(leaves[i])) {
			auto it = iden_table.find(leaves[i]);
			data_types.push_back(it->second);
		//Если "leaves[i]" - константа
		} else {
			data_types.push_back(set_data_type(leaves[i], iden_table));
		}
	}

	for(int i = 1; i < data_types.size(); ++i) {
		if(data_types[i] != data_types[i - 1]) {
			cout << "For operands [" << leaves[i - 1] << "]: '" << data_types[i - 1]
			<< "' and [" << leaves[i] << "]: '" << data_types[i] << "' data types don't match\n";
			exit(1);
		}
	}
}

//
bool Sema::check_binary_oper_sign(string oper)
{
	return (oper == "+" || oper == "-" || oper == "*" || oper == "/" || oper == "==" || oper == "!=" || oper == "<=" || oper == ">="
		|| oper == ">" || oper == "<" || oper == ":" || oper == "%");
}
