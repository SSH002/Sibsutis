#include "parser.hpp"

//Буферные переменные
Token Parser::preview;
Token Parser::current;

//Вектор "blocks" объявлен здесь, в остальных
//методах он вызывается отсюда
vector <AST*> blocks;

Parser::Parser() {}

//Считываем токен
Token Parser::get_token(Lexer *lexer)
{
	return lexer->get_token();
}

//Обработчик токенов
AST *Parser::handler(Token token, Lexer *lexer)
{
	AST *node;

	//Если при обработке "array_name" была пропущена лексема "="
	if(assigment_omitted) {
		return parse_assignment(buffer, lexer);
	}

	//Обработка условия условного ветвления
	if(token.get_lexeme() == "(" && Parser::preview.get_lexeme() == "if") {
		return parse_if_cond(lexer);
	}

	//Обработка тела условного ветвления
	if(token.get_lexeme() == "{" && Parser::preview.get_lexeme() == "if") {
		return parse_if_body(lexer);
	}

	//Обработка условия цикла for
	if(token.get_lexeme() == "(" && Parser::preview.get_lexeme() == "for") {
		return parse_for_cond(lexer);
	}

	//Обработка тела цикла for
	if(token.get_lexeme() == "{" && Parser::preview.get_lexeme() == "for") {
		return parse_for_body(lexer);
	}

	//Обработка массива
	if(token.get_descr() == "array_name") {
		return parse_array_name(token, lexer);
	}

	//Обработка данных, записываемых в массив
	if(current.get_lexeme() == "array(") {
		return parse_array_data(lexer);
	}

	//Функция printf
	if(token.get_lexeme() == "printf") {
		return parse_printf(token, lexer);
	}

	//Условное ветвление
	if(token.get_lexeme() == "if") {
		return parse_if(token, lexer);
	}

	//Ключевое слово else
	if(token.get_lexeme() == "else") {
		return parse_else(token, lexer);
	}

	//Цикл for
	if(token.get_lexeme() == "for") {
		return parse_for(token, lexer);
	}

	//Строковая лексема
	if(token.get_descr() == "string_literal") {
		return parse_string_lexeme(token);
	}

	//Число(числовая константа)
	if(check_parse_number(token)) {
		return parse_number(token);
	}

	//Переменная
	if(token.get_descr() == "variable") {
		return parse_variable(token);
	}

	//Ключевое слово break
	if(token.get_lexeme() == "break") {
		return parse_break(token, lexer);
	}

	//Операция присваивания
	if(token.get_lexeme() == "=") {
		return parse_assignment(token, lexer);
	}

	//Логический оператор
	if(token.get_lexeme() == "&&" || token.get_lexeme() == "||") {
		return parse_logic_oper(token, lexer);
	}

	//Бинарный оператор
	if(check_parse_binary_oper(token)) {
		return parse_binary_oper(token, lexer);
	}

	//Унарный оператор
	if(token.get_lexeme() == "++" || token.get_lexeme() == "--") {
		return parse_unary_oper(token, lexer);
	}

	//Тернарный оператор
	if(token.get_lexeme() == "?") {
		return parse_ternar_oper(token, lexer);
	}

	return node;
}

//Построение AST-дерева
AST *Parser::build_ast(Lexer *lexer)
{
	//Обрабатываем тело программы
	return parse_php_beginning(lexer);
}

