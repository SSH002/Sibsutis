#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include "token.hpp"
#include "lexer.hpp"
#include "ast.hpp"
#include "sym_tab.hpp"
#include "sema.hpp"
#include "codgen.hpp"
using namespace std;

class Parser
{
	public:
		static Token preview;
		static Token current;
		Token buffer;

		//Типы данных, используемые в языке
		vector <string> Data_types =  {
			"int",						//Для целочисленных переменных
			"float",					//Для вещественных переменных
			"string",					//Для строковых переменных
			"bool",						//Для логических переменных
			"NULL"						//Для неинициализированных переменных
		};

		//Необходимо для проверки, был ли пропущен знак "=" при обработке имени массива(array_name)
		bool assigment_omitted = false;

		Parser();
		AST *build_ast(Lexer *lexer);
		AST *parse_php_beginning(Lexer *lexer);
		AST *handler(Token token, Lexer *lexer);
		AST *parse_data_types(Token token, Lexer *lexer); 
		AST *parse_printf(Token token, Lexer *lexer);
		AST *parse_assignment(Token token, Lexer *lexer);
		AST *parse_unary_oper(Token token, Lexer *lexer);
		AST *parse_binary_oper(Token token, Lexer *lexer);
		AST *parse_ternar_oper(Token token, Lexer *lexer);
		AST *parse_logic_oper(Token token, Lexer *lexer);
		AST *parse_if(Token token, Lexer *lexer);
		AST *parse_if_cond(Lexer *lexer);
		AST *parse_if_body(Lexer *lexer); 
		AST *parse_else(Token token, Lexer *lexer);
		AST *parse_for(Token token, Lexer *lexer);
		AST *parse_for_cond(Lexer *lexer);						
		AST *parse_for_body(Lexer *lexer);						
		AST *parse_array_name(Token token, Lexer *lexer); 		
		AST *parse_array_data(Lexer *lexer);					
		AST *parse_string_lexeme(Token token);
		AST *parse_number(Token token);
		AST *parse_variable(Token token);
		AST *parse_break(Token token, Lexer *lexer);
		Token get_token(Lexer *lexer);
};

bool check_body_tok(Token token);
bool check_assignment_tok(Token token);
bool check_ternar_oper_tok(Token tok);
bool check_binary_oper_tok(Token token);
bool check_unary_oper_tok(Token token);
bool check_logic_oper_tok(Token token);
bool check_if_cond_tok(Token token);
bool check_for_cond_tok(Token token);
bool check_array_data_tok(Token token);
bool check_array_name_tok(Token token);
bool check_printf_tok(Token token);

bool check_parse_if(Token token);
bool check_parse_if_cond(Token current, Token preview);
bool check_parse_if_body(Token current, Token preview);
bool check_parse_else(Token token);
bool check_parse_for(Token token);
bool check_parse_for_cond(Token current, Token preview);
bool check_parse_for_body(Token current, Token preview);
bool check_parse_array_data(Token current, Token preview);
bool check_parse_array_name(Token token);
bool check_parse_printf(Token token);
bool check_parse_return(Token token);
bool check_parse_string_lexeme(Token token);
bool check_parse_number(Token token);
bool check_parse_variable(Token token);
bool check_parse_break(Token token);
bool check_parse_assignment(Token token);
bool check_parse_binary_oper(Token token);
bool check_parse_unary_oper(Token token);
bool check_parse_logic_oper(Token token);
bool check_parse_ternar_oper(Token token);

void check_right_paren_error(Token next, Token current);
void check_left_paren_error(Token token);
void check_right_brace_error(Token token);
void check_left_brace_error(Token token);
void check_data_types_next_tok_error(Token next, Token current);
void check_semicolon_and_comma_error(Token current, Token preview);
void check_end_of_oper_error(Token current, Token next);


