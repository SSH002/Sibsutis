#include "parser.hpp"

//Проверка корректности параметров
//********************************
bool check_body_tok(Token token)
{
	return (token.get_descr() == "variable" || token.get_descr() == "array_name"
		|| token.get_lexeme() == "printf" || token.get_lexeme() == "break" || token.get_lexeme() == "="
		|| token.get_lexeme() == "int" || token.get_lexeme() == "char" || token.get_lexeme() == "float"
		|| token.get_lexeme() == "if" || token.get_lexeme() == "else" || token.get_lexeme() == "for"
		|| token.get_lexeme() == "++" || token.get_lexeme() == "--" || token.get_lexeme() == "+"
		|| token.get_lexeme() == "-" || token.get_lexeme() == "*" || token.get_lexeme() == "/"
		|| token.get_lexeme() == "%" || token.get_lexeme() == "array(");
}

bool check_printf_tok(Token token)
{
	return (token.get_descr() == "variable" || token.get_descr() == "string_literal" 
		|| token.get_descr() == "array_name");
}

bool check_assignment_tok(Token token)
{
	return (token.get_descr() == "variable" || token.get_descr() == "array_name" || token.get_descr() == "string_literal"
		|| token.get_descr() == "digit_literal_bin" || token.get_descr() == "digit_literal_oct"
		|| token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_hex"
		|| token.get_lexeme() == "array(" || token.get_lexeme() == "true" || token.get_lexeme() == "false"
		|| token.get_lexeme() == "+" || token.get_lexeme() == "-" || token.get_lexeme() == "*" 
		|| token.get_lexeme() == "/" || token.get_lexeme() == "%" || token.get_lexeme() == "?"
		|| token.get_lexeme() == "==" || token.get_lexeme() == "!=" || token.get_lexeme() == ">="
		|| token.get_lexeme() == "<=" || token.get_lexeme() == ">" || token.get_lexeme() == "<" );
}

bool check_logic_oper_tok(Token token)
{
	return (token.get_descr() == "variable" || token.get_descr() == "array_name"
		|| token.get_descr() == "digit_literal_bin" || token.get_descr() == "digit_literal_oct"
		|| token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_hex"
		|| token.get_lexeme() == "+" || token.get_lexeme() == "-" || token.get_lexeme() == "*" 
		|| token.get_lexeme() == "/" || token.get_lexeme() == "==" || token.get_lexeme() == "!="
		|| token.get_lexeme() == ">=" || token.get_lexeme() == "<=" || token.get_lexeme() == ">" 
		|| token.get_lexeme() == "<" || token.get_lexeme() == "%");
}

bool check_unary_oper_tok(Token token)
{
	return (token.get_descr() == "variable" || token.get_descr() == "array_name"
		|| token.get_descr() == "digit_literal_bin" || token.get_descr() == "digit_literal_oct"
		|| token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_hex");
}

bool check_binary_oper_tok(Token token)
{
	return (token.get_descr() == "variable" || token.get_descr() == "array_name"
		|| token.get_descr() == "digit_literal_bin" || token.get_descr() == "digit_literal_oct"
		|| token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_hex"
		|| token.get_descr() == "string_literal" || token.get_lexeme() == "+"
		|| token.get_lexeme() == "-" || token.get_lexeme() == "*"
		|| token.get_lexeme() == "/" || token.get_lexeme() == "%");
}

bool check_ternar_oper_tok(Token token)
{
	return (token.get_descr() == "variable" || token.get_descr() == "array_name"
		|| token.get_descr() == "digit_literal_bin" || token.get_descr() == "digit_literal_oct"
		|| token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_hex"
		|| token.get_lexeme() == ":");
}

bool check_if_cond_tok(Token token)
{
	return (token.get_descr() == "variable" || token.get_descr() == "array_name"
		|| token.get_descr() == "digit_literal_bin" || token.get_descr() == "digit_literal_oct"
		|| token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_hex"
		|| token.get_lexeme() == "==" || token.get_lexeme() == "!=" || token.get_lexeme() == ">="
		|| token.get_lexeme() == "<=" || token.get_lexeme() == ">" || token.get_lexeme() == "<"
		|| token.get_lexeme() == "&&" || token.get_lexeme() == "||" || token.get_lexeme() == "+"
		|| token.get_lexeme() == "-" || token.get_lexeme() == "*" || token.get_lexeme() == "/"
		|| token.get_lexeme() == "%");
}

bool check_for_cond_tok(Token token)
{
	return (token.get_descr() == "variable" || token.get_descr() == "array_name"
		|| token.get_descr() == "digit_literal_bin" || token.get_descr() == "digit_literal_oct"
		|| token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_hex"
		|| token.get_lexeme() == "==" || token.get_lexeme() == "!=" || token.get_lexeme() == ">="
		|| token.get_lexeme() == "<=" || token.get_lexeme() == ">" || token.get_lexeme() == "<" 
		|| token.get_lexeme() == "++" || token.get_lexeme() == "--" || token.get_lexeme() == "int"
		|| token.get_lexeme() == "char" || token.get_lexeme() == "float" || token.get_lexeme() == "=" 
		|| token.get_lexeme() == "&&" || token.get_lexeme() == "||" || token.get_lexeme() == "+"
		|| token.get_lexeme() == "-" || token.get_lexeme() == "*" || token.get_lexeme() == "/"
		|| token.get_lexeme() == "%");
}

bool check_array_data_tok(Token token)
{
	return (token.get_descr() == "digit_literal_bin" || token.get_descr() == "digit_literal_oct"
		|| token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_hex" 
		|| token.get_descr() == "string_literal");
}

bool check_array_name_tok(Token token)
{
	return (token.get_descr() == "digit_literal_bin" || token.get_descr() == "digit_literal_oct"
		|| token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_hex"
		|| token.get_descr() == "variable" || token.get_lexeme() == "]");
}

//Проверка токена (чем он является)
//************************************************************************

bool check_parse_number(Token token)
{
	return (token.get_descr() == "digit_literal_dec" || token.get_descr() == "digit_literal_bin"
		|| token.get_descr() == "digit_literal_hex" || token.get_descr() == "digit_literal_oct");
}

bool check_parse_binary_oper(Token token)
{
	return (token.get_lexeme() == "+" || token.get_lexeme() == "-" || token.get_lexeme() == "*"
		|| token.get_lexeme() == "/" || token.get_lexeme() == "%" || token.get_lexeme() == "=="
		|| token.get_lexeme() == "!=" || token.get_lexeme() == ">=" || token.get_lexeme() == "<="
		|| token.get_lexeme() == ">" || token.get_lexeme() == "<" || token.get_lexeme() == ":" );
}

//Проверка на различные синтаксические ошибки
//*******************************************

//Проверка, была ли пропущена левая круглая скобка
void check_right_paren_error(Token next, Token current)
{
	if(next.get_lexeme() != "(" && next.get_lexeme() != "") {
		cout << "check_left_paren_error   " << next.get_lexeme() << "\n";
		cout << "Loc=<" << next.get_line() << ":" << next.get_pos() << "> ";
		cout << "Expected '(' after '" << current.get_lexeme() << "'\n";
		
		exit(1);
	}
}

//Проверка, была ли пропущена правая круглая скобка
void check_left_paren_error(Token token)
{
	if(token.get_lexeme() == "{") {
		cout << "check_right_paren_error   " << token.get_lexeme() << "\n";
		cout << "Loc=<" << token.get_line() << ":" << token.get_pos() << "> ";
		cout << "Expected ')' becouse '(' previously used\n";
		
		exit(1);
	}
}

//Проверка, была ли пропущена левая фигурная скобка
void check_right_brace_error(Token token)
{
	if(token.get_lexeme() != "{" && token.get_lexeme() != "") {
		cout << "check_right_brace_error   " << token.get_lexeme() << "\n";
		cout << "Loc=<" << token.get_line() << ":" << token.get_pos() << "> ";
		cout << "Expected '{' after  ')'\n";
		
		exit(1); 
	}
}

//Проверка, не была ли пропущена запятая или точка с запятой в выражении
void check_semicolon_and_comma_error(Token current, Token preview)
{
	if(preview.get_descr() != "variable" && preview.get_descr() != "digit_literal_oct"
		&& preview.get_descr() != "digit_literal_hex" && preview.get_descr() != "digit_literal_bin"
		&& preview.get_descr() != "digit_literal_dec" && preview.get_lexeme() != "int" 
		&& preview.get_lexeme() != "char" && preview.get_lexeme() != "float" && preview.get_lexeme() != "return"
		&& preview.get_lexeme() != "++" && preview.get_lexeme() != "+" && preview.get_lexeme() != ",") {
		return;
	}

	if(current.get_lexeme() != "," && current.get_lexeme() != ";" && current.get_lexeme() != "(" 
		&& current.get_lexeme() != ")" && current.get_lexeme() != "=" && current.get_lexeme() != "=="
		&& current.get_lexeme() != "!=" && current.get_lexeme() != "<" && current.get_lexeme() != ">"
		&& current.get_lexeme() != ">=" && current.get_lexeme() != "<=" && current.get_lexeme() != "++"
		&& current.get_lexeme() != "--" && current.get_lexeme() != "+" && current.get_lexeme() != "-"
		&& current.get_lexeme() != "*" && current.get_lexeme() != "/" && current.get_lexeme() != "%"
		&& current.get_lexeme() != "") {
		cout << "check_semicolon_and_comma_error   " << current.get_lexeme() << " | " << preview.get_descr() <<"\n";
		cout << "Loc=<" << preview.get_line() << ":" << preview.get_pos() << "> ";
		cout << "expected ',' or ';' after identidicator\n";
		
		exit(1);
	}

}

//Проверка, поставлена ли в конце операции точка с запятой
void check_end_of_oper_error(Token current, Token next)
{
	if(next.get_lexeme() != ";") {
		cout << "check_end_of_oper_error   " << next.get_lexeme() << "\n";
		cout << "Loc=<" << current.get_line() << ":" << current.get_pos() << "> ";
		cout << "expected ';' after '" << current.get_lexeme() << "'\n";
		
		exit(1);
	}
}



