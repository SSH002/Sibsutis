#include "parser.hpp"

void AST::print_level(int level) 
{
	for(int i = 0; i < level; ++i) {
		cout << "-----";
	}
	
	cout << "{";
}

void php_beginning_ast::print(int level)
{	
	print_level(level);
	
	cout << definition << endl;

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->print(level + 1);
	}
}

void assignment_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	l_operand->print(level + 1);
	r_operand->print(level + 1);
}

void logic_oper_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	l_operand->print(level + 1);
	r_operand->print(level + 1);
}

void ternar_oper_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	l_operand->print(level + 1);
	r_operand->print(level + 1);
}

void binary_oper_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	l_operand->print(level + 1);
	r_operand->print(level + 1);
}

void unary_oper_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	operand->print(level + 1);
}

void array_data_ast::print(int level)
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->print(level);
	}
}

void array_name_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;
		
	if(identificator != NULL) {
		identificator->print(level + 1);
	}
}

void for_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	cond->print(level + 1);
	body->print(level + 1);
}

void for_body_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->print(level + 1);
	}
}

void for_cond_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->print(level + 1);
	}
}

void if_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	cond->print(level + 1);
	body->print(level + 1);
}

void if_body_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;
	
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->print(level + 1);
	}
}

void if_cond_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;
	
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->print(level + 1);
	}
}

void else_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->print(level + 1);

	}
}

void printf_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;

	for(int i = 0; i < params.size(); ++i) {
		params[i]->print(level + 1);
	}
}

void string_lexeme_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;
}

void number_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;
}

void variable_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;
}

void break_ast::print(int level)
{
	print_level(level);
	
	cout << definition << endl;
}

void print_ast(AST *tree)
{
	php_beginning_ast *php_beginning = static_cast <php_beginning_ast*> (tree);
	
	php_beginning->print(1);
	cout << endl;
}
