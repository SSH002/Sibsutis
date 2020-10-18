#include "parser.hpp"

extern vector <AST*> blocks;

AST *Parser::parse_for(Token token, Lexer *lexer) 
{
	for_ast *key_word_for; 
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
		key_word_for = new for_ast(cond, body, token);
	}
	Parser::preview.set_lexeme("");

  	return key_word_for;
}

AST *Parser::parse_for_cond(Lexer *lexer)
{
	Token tok, buff, prev;
	for_cond_ast *for_cond;
	vector <AST*> for_blocks;
	int old_size = blocks.size();
	
	buff = Parser::preview;

	//Считываем токены до конца выражения
	while(tok.get_lexeme() != ")") {
		prev = tok;
		tok = get_token(lexer);
		//Проверка, не была ли пропущена запятая или точка с запятой
		check_semicolon_and_comma_error(tok, prev);
		//Проверка, была ли пропущена правая круглая скобка
		check_left_paren_error(tok);

		//Если токены соответствуют требованиям,
		//то идентифицируем их и записываем в blocks
		if(check_for_cond_tok(tok)) {
			blocks.push_back(handler(tok, lexer));
			Parser::preview = tok;
		}
	}

	for(int i = old_size; i < blocks.size(); ++i) {
		for_blocks.push_back(blocks[i]);
	}

	//Отчищаем вектор blocks от ненужных токенов
	blocks.erase(blocks.begin() + old_size, blocks.begin() + blocks.size());

	//Если условие не пустое
	if(!for_blocks.empty()) {
		//Создаём узел AST-дерева
		for_cond = new for_cond_ast(for_blocks);
	}

	Parser::preview = buff;
	Parser::current.set_lexeme("");

	return for_cond;
}

AST *Parser::parse_for_body(Lexer *lexer)
{
	Token tok, buff, prev;
	for_body_ast *for_body;
	vector <AST*> for_blocks;
	int old_size = blocks.size();

	buff = Parser::preview;

	//Считываем токены, пока не дойдём до конца тела
	while(tok.get_lexeme() != "}") {
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
		for_blocks.push_back(blocks[i]);
	}
	//Отчищаем вектор blocks от ненужных токенов
	blocks.erase(blocks.begin() + old_size, blocks.begin() + blocks.size());

	//Если тело не пустое
	if(!for_blocks.empty()) {
		//Создаём узел AST-дерева
		for_body = new for_body_ast(for_blocks);
	}
	
	Parser::preview = buff;

	return for_body;
}

