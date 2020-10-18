#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Базовый класс
class AST 
{
	public:
		void print_level(int level);	
		virtual void print(int level) {}
		virtual void sema() {}
		virtual void symtab() {}
		virtual void codgen() {}
		virtual ~AST() {}
};

//Тело программы
class php_beginning_ast : public AST 
{
	public:										 
		vector <AST*> blocks;							 

		string definition;
		php_beginning_ast(vector <AST*> blocks);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

class data_types_ast : public AST 
{
	public:
		AST *identificator;

		string definition;
		data_types_ast(AST *identificator, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Вывод на экран(функция printf)
class printf_ast : public AST 
{
	public:
		vector <AST*> params;

		string definition;
		printf_ast(vector <AST*> params, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Присваивание
class assignment_ast : public AST 
{
	public:
		AST *l_operand;
		AST *r_operand;

		string definition;
		assignment_ast(AST *l_operand, AST *r_operand, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Унарный оператор
class unary_oper_ast : public AST 
{
	public:
		AST *operand;

		string definition;
		unary_oper_ast(AST *operand, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Бинарный оператор
class binary_oper_ast : public AST 
{
	public:
		AST *l_operand;
		AST *r_operand;

		string definition;
		binary_oper_ast(AST *l_operand, AST *r_operand, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Тернарный оператор
class ternar_oper_ast : public AST 
{
	public:
		AST *l_operand;
		AST *r_operand;

		string definition;
		ternar_oper_ast(AST *l_operand, AST *r_operand, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Логический оператор
class logic_oper_ast : public AST 
{
	public:
		AST *l_operand;
		AST *r_operand;

		string definition;
		logic_oper_ast(AST *l_operand, AST *r_operand, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Условное ветвление
class if_ast : public AST 
{
	public:
		AST *cond;
		AST *body;

		string definition;
		if_ast(AST *cond, AST *body, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Условие условного ветвления
class if_cond_ast : public AST 
{
	public:
		vector <AST*> blocks;
		string definition;

		if_cond_ast(vector <AST*> blocks);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Тело условного ветвления
class if_body_ast : public AST 
{
	public:
		vector <AST*> blocks;

		string definition;
		if_body_ast(vector <AST*> blocks);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Тело ветвления, если не выполненяется условие
class else_ast : public AST 
{
	public:
		vector <AST*> blocks;
		string definition;

		else_ast(vector <AST*> blocks, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Цикл for
class for_ast : public AST 
{
	public:
		AST *cond;
		AST *body;
		string definition;

		for_ast(AST *cond, AST *body, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Условие цикла for
class for_cond_ast : public AST 
{
	public:
		vector <AST*> blocks;
		string definition;

		for_cond_ast(vector <AST*> blocks);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Тело цикла for
class for_body_ast : public AST 
{
	public:
		vector <AST*> blocks;
		string definition;

		for_body_ast(vector <AST*> blocks);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Название массива
class array_name_ast : public AST 
{
	public:
		AST *identificator;
		string definition;

		array_name_ast(AST *identificator, Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Данные, храняшиеся в массиве
class array_data_ast : public AST 
{
	public:
		vector <AST*> blocks;
		string definition;

		array_data_ast(vector <AST*> blocks);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Строковая лексема
class string_lexeme_ast : public AST 
{
	public:
		string definition;

		string_lexeme_ast(Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

class symbol_lexeme_ast : public AST 
{
	public:
		string definition;

		symbol_lexeme_ast(Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Числовая лексема(константа)
class number_ast : public AST 
{
	public:
		string definition;

		number_ast(Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Переменная
class variable_ast : public AST 
{
	public:
		string definition;

		variable_ast(Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Ключевое слово break
class break_ast : public AST 
{
	public:
		string definition;

		break_ast(Token token);
		void print(int level) override;
		void symtab(void) override;
		void sema(void) override;
		void codgen(void) override;
};

//Вывод AST-дерева на экран
void print_ast(AST *tree);

