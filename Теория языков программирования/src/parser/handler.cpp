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
	if(check_parse_if_cond(token, Parser::preview)) {
		return parse_if_cond(lexer);
	}

	//Обработка тела условного ветвления
	if(check_parse_if_body(token, Parser::preview)) {
		return parse_if_body(lexer);
	}

	//Обработка условия цикла for
	if(check_parse_for_cond(token, Parser::preview)) {
		return parse_for_cond(lexer);
	}

	//Обработка тела цикла for
	if(check_parse_for_body(token, Parser::preview)) {
		return parse_for_body(lexer);
	}

	//Обработка массива
	if(check_parse_array_name(token)) {
		return parse_array_name(token, lexer);
	}

	//Обработка данных, записываемых в массив
	if(check_parse_array_data(token, Parser::preview)) {
		return parse_array_data(lexer);
	}

	//Функция printf
	if(check_parse_printf(token)) {
		return parse_printf(token, lexer);
	}

	//Условное ветвление
	if(check_parse_if(token)) {
		return parse_if(token, lexer);
	}

	//Ключевое слово else
	if(check_parse_else(token)) {
		return parse_else(token, lexer);
	}

	//Цикл for
	if(check_parse_for(token)) {
		return parse_for(token, lexer);
	}

	//Строковая лексема
	if(check_parse_string_lexeme(token)) {
		return parse_string_lexeme(token);
	}

	//Число(числовая константа)
	if(check_parse_number(token)) {
		return parse_number(token);
	}

	//Переменная
	if(check_parse_variable(token)) {
		return parse_variable(token);
	}

	//Ключевое слово break
	if(check_parse_break(token)) {
		return parse_break(token, lexer);
	}

	//Операция присваивания
	if(check_parse_assignment(token)) {
		return parse_assignment(token, lexer);
	}

	//Логический оператор
	if(check_parse_logic_oper(token)) {
		return parse_logic_oper(token, lexer);
	}

	//Бинарный оператор
	if(check_parse_binary_oper(token)) {
		return parse_binary_oper(token, lexer);
	}

	//Унарный оператор
	if(check_parse_unary_oper(token)) {
		return parse_unary_oper(token, lexer);
	}

	//Тернарный оператор
	if(check_parse_ternar_oper(token)) {
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

