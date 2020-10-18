#include "parser.hpp"

extern vector <AST*> blocks;

AST *Parser::parse_printf(Token token, Lexer *lexer)
{
	Token tok, next_tok;
	printf_ast *func_printf;
	vector <AST*> printf_blocks;
	int old_size = blocks.size();

	//Считываем токены, пока не найдём "("
	while(tok.get_lexeme() != "(") {
		tok = get_token(lexer);
	}

	//Считываем токены, пока не найдём ")"
	while(tok.get_lexeme() != ")") {
		tok = get_token(lexer);
		//Проверяем, корректен ли параметр
		if(check_printf_tok(tok)) {
			//Идентифицируем токен "tok"
			blocks.push_back(handler(tok, lexer));
		}			
	}

	next_tok = get_token(lexer);
	check_end_of_oper_error(tok, next_tok);

	//Записываем найденные значения (строку, параметры)
	for(int i = old_size; i < blocks.size(); ++i) {
		printf_blocks.push_back(blocks[i]);
	}

	//Отчищаем вектор
	blocks.erase(blocks.begin() + old_size, blocks.begin() + blocks.size());

	//Если printf содержит параметры
	if(!printf_blocks.empty()) {
		//Создаём узел AST-дерева
		func_printf = new printf_ast(printf_blocks, token);
	}

	return func_printf;
}
