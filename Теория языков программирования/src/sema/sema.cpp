#include "parser.hpp"

//Костыль для обработки тернарного оператора
int num = 0;
//Впервые ли объявляется переменная
bool is_left = false;

//Строка заполнялась методом "sema" у "DataTypeAST"
string data_type;
//Символ операции, используется для вывода при обнаружении ошибки
string oper;
//Идентификатор левого операнда операции присваивания
string left_operand;
//Для проверки корректности условия (есть ли в нём логические операторы)
string condition;
//Таблица идентификаторов (переменных и массивов)
map <string, string> iden_table;
//Вектор, в который попадает правая часть выражения в операции присваивания
vector <string> leaves;

Sema *semantic = new Sema();

//
void php_beginning_ast::sema()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->sema();
	}
}

//
void array_data_ast::sema()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->sema();
	}
}

//
void array_name_ast::sema()
{		
	if(is_left == false) {
		is_left = true;
		left_operand = definition;	
	} else {
		leaves.push_back(definition);
	}
	iden_table.emplace(definition, "NULL");
	
	if(identificator != NULL) {
		identificator->sema();
	}
}

//
void for_ast::sema()
{
	cond->sema();
	body->sema();
}

//
void for_body_ast::sema()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->sema();
	}
}

//
void for_cond_ast::sema()
{
	for(int i = 0; i < blocks.size(); ++i) {
		if(i == 1) {
			condition = "for cond";
			blocks[i]->sema();
			condition.clear();
		} else {
			blocks[i]->sema();
		}
	}
}

//
void if_ast::sema()
{
	condition = "if cond";

	cond->sema();
	condition.clear();
	body->sema();
}

//
void if_cond_ast::sema()
{	
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->sema();
	}
}

//
void if_body_ast::sema()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->sema();
	}
}
//
void else_ast::sema()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->sema();
	}
}

//
void assignment_ast::sema()
{	
	string buff = oper;
	oper = definition;

	l_operand->sema();
	leaves.clear();
	r_operand->sema();
	
	//Определяем тип данных переменной "l_operand"; считаем, что если в "leaves"
	//все переменные относятся к одному типу, то к нему относится и "l_operand"
	data_type = set_data_type(leaves[0], iden_table);

	//Проверяем, относятся ли переменные в "leaves" к одному типу данных
	//Если не относятся, то выполнение компиляции прерывается
	semantic->check_opers_data_types(leaves, iden_table, oper);

	//Ищем упоминание "l_operand" в таблице идентификаторов
	auto it = iden_table.begin();
	for(; it->first != left_operand; ++it) {
	}

	//Если переменная ещё не инициализирована
	if(it->second == "NULL") {
		//Задаём переменной "l_operand" тип данных
		it->second = data_type;
	//Если переменная была инициализирована...
	} else {
		//...то проверяем, совпадают ли типы данных переменной и выражения в правой части присваивания
		if(it->second != data_type) {
			cout << "Data types of variable [" << it->first << "] and right expression in assignment don't match.\n";
			exit(1);
		}
	}

	oper = buff;
	num = 0;
	is_left = false;

	left_operand.clear();
}

//
void logic_oper_ast::sema()
{
	l_operand->sema();
	r_operand->sema();
}

//
void ternar_oper_ast::sema()
{
	l_operand->sema();
	r_operand->sema();
}

//
void binary_oper_ast::sema()
{
	string buff = oper;
	oper = definition;
	
	l_operand->sema();
	r_operand->sema();
	oper = buff;
	num = 0;
}

//
void unary_oper_ast::sema()
{
	string buff = oper;
	oper = "unary_op";

	operand->sema();
	oper = buff;
	num = 0;
}

//
void printf_ast::sema()
{
	oper = "printf";

	for(int i = 0; i < params.size(); ++i) {
		params[i]->sema();
	}
}

//
void string_lexeme_ast::sema()
{
	//Вставляем в вектор листьев числовую константу
	leaves.push_back(definition);
}

//
void number_ast::sema()
{
	//В условии нет логических операторов
	if((condition == "if cond" || condition == "while cond" || condition == "for cond")  
		&&(oper != ">" && oper != "<" && oper != ">=" && oper != "<=" && oper != "==" && oper != "!=")) {
			cout << "Cannot execute arithmetic opereration in '" << condition << "'" << endl;
			exit(1);
	}
	
	//Вставляем в вектор листьев числовую константу
	leaves.push_back(definition);
}

//
void variable_ast::sema()
{
	//Тип данных по-умолчанию
	data_type = "NULL";

	//Если переменная используется в какой-либо операции (инициализируется)
	if(oper == "=" || oper == "unary_op" || oper == "return" || oper == "printf" || semantic->check_binary_oper_sign(oper)) {
		if((condition == "if cond" || condition == "while cond" || condition == "for cond") 
			&&(oper != ">" && oper != "<" && oper != ">=" && oper != "<=" && oper != "==" && oper != "%" && oper != "!=")) {
			cout << "Cannot execute arithmetic opereration in '" << condition << "'" << endl;
			exit(1);
		}
		if(!num) {
			if((iden_table.find(definition)) == iden_table.end()) {
				iden_table.emplace(definition, data_type);
				is_left = true;
				left_operand = definition;	
			}
			//Игнорим выражение тернарного оператора
			if(oper != ":") {
				left_operand = definition;
			}
		}
		if(num) {
			leaves.push_back(definition);
		}
		++num;
	}

	//Если переменной нет в таблице идентификаторов, и она не инициализирована
	if((iden_table.find(definition)) == iden_table.end()) {
		iden_table.emplace(definition, data_type);
	}
}

//
void break_ast::sema()
{
}

//
Sema::Sema()
{
}

//
void Sema::check_semantic(AST *tree)
{
	php_beginning_ast *php_beginning = static_cast <php_beginning_ast*> (tree);
	
	php_beginning->sema();
}

void Sema::print_semantic()
{

	cout << "\nIdentificator | Data type";
	cout << "\n[====================================]\n";

	for(auto it = iden_table.begin(); it != iden_table.end(); ++it) {

		cout << it->first << " \t\t| " << it->second;
		cout << "\n{------------------------------------}\n";
	}
}

