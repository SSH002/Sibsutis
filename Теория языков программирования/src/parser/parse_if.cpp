#include "parser.hpp"

extern vector <AST*> blocks;

AST *Parser::parse_if(Token token, Lexer *lexer) 
{
	if_ast *key_word_if; 
	AST *cond, *body;
	Token tok;
	
	Parser::preview = token;

	//Считываем токены, пока не найдём "("
	while(tok.get_lexeme() != "(") {
		tok = get_token(lexer);
		check_right_paren_error(tok, token);
	}
	//Идентифицируем токен "tok"
	cond = handler(tok, lexer);

	//Считываем токены, пока не найдём "{"
	while(tok.get_lexeme() != "{") {
		tok = get_token(lexer);
		check_right_brace_error(tok);
	}
	//Идентифицируем токен "tok"
	body = handler(tok, lexer);

	//Если условие и тело идентифицированы
	if(cond && body) {
		//Создаём узел AST-дерева
		key_word_if = new if_ast(cond, body, token);
	}
	Parser::preview.set_lexeme("");

  	return key_word_if;
}

AST *Parser::parse_if_cond(Lexer *lexer)
{
	Token tok;
	if_cond_ast *cond;
	vector <AST*> if_blocks;
	int old_size = blocks.size();

	//Считываем токены до конца выражения
	while(tok.get_lexeme() != ")" && Parser::current.get_lexeme() != ")") {
		tok = get_token(lexer);
		//Проверка, была ли пропущена правая круглая скобка
		check_left_paren_error(tok);

		//Если токен соответствует требованиям,
		//то идентифицируем его и записываем в blocks
		if(check_if_cond_tok(tok)) {
			blocks.push_back(handler(tok, lexer));
		}
	}

	for(int i = old_size; i < blocks.size(); ++i) {
		if_blocks.push_back(blocks[i]);
	}

	//Отчищаем вектор blocks от ненужных токенов
	blocks.erase(blocks.begin() + old_size, blocks.begin() + blocks.size());

	//Если условие не пустое
	if(!if_blocks.empty()) {
		//Создаём узел AST-дерева
		cond = new if_cond_ast(if_blocks);
	}

	Parser::current.set_lexeme("");

	return cond;
}

AST *Parser::parse_if_body(Lexer *lexer)
{
	Token tok, prev, buff;
	if_body_ast *body;
	vector <AST*> if_blocks;
	int old_size = blocks.size();

	buff = Parser::preview;

	//Считываем токены, пока не дойдём до конца тела
	while(tok.get_lexeme() != "}") {
		prev = tok;
		tok = get_token(lexer);
		check_semicolon_and_comma_error(tok, prev);

		//Если токен соответствует требованиям,
		//то идентифицируем его и записываем в blocks
		if(check_body_tok(tok)) {
			blocks.push_back(handler(tok, lexer));
			Parser::preview = tok;
		}
		Parser::current = tok;
	}

	for(int i = old_size; i < blocks.size(); ++i) {
		if_blocks.push_back(blocks[i]);
	}

	//Отчищаем вектор blocks от ненужных токенов
	blocks.erase(blocks.begin() + old_size, blocks.begin() + blocks.size());

	//Если тело не пустое
	if(!if_blocks.empty()) {
		//Создаём узел AST-дерева
		body = new if_body_ast(if_blocks);
	}

	Parser::preview = buff;
	
	return body;
}

AST *Parser::parse_else(Token token, Lexer *lexer)
{
	Token tok, prev, buff;
	else_ast *key_words_else;
	vector <AST*> else_blocks;
	int old_size = blocks.size();

	buff = Parser::preview;

	Parser::current.set_lexeme("");

	//Считываем токены, пока не дойдём до конца тела
	while(tok.get_lexeme() != "}" && Parser::current.get_lexeme() != "}") {	
		prev = tok;
		tok = get_token(lexer);
		//Проверка, не была ли пропущена запятая или точка с запятой
		check_semicolon_and_comma_error(tok, prev);

		//Если токен соответствует требованиям,
		//то идентифицируем его и записываем в blocks
		if(check_body_tok(tok)) {
			blocks.push_back(handler(tok, lexer));
			Parser::preview = tok;
		}
	}

	for(int i = old_size; i < blocks.size(); ++i) {
		else_blocks.push_back(blocks[i]);
	}

	//Отчищаем вектор blocks от ненужных токенов
	blocks.erase(blocks.begin() + old_size, blocks.begin() + blocks.size());

	//Если тело не пустое
	if(!else_blocks.empty()) {
		//Создаём узел AST-дерева
		key_words_else = new else_ast(else_blocks, token);
	}

	Parser::preview = buff;
	Parser::current.set_lexeme("");
	
	return key_words_else;
}
