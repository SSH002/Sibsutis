#include "parser.hpp"

extern vector <AST*> blocks;

AST *Parser::parse_array_data(Lexer *lexer)
{
	Token tok;
	array_data_ast *arr_data;
	vector <AST*> arr_blocks;
	int old_size = blocks.size();

	//Получаем токены, пока не найдём требуемые
	while(!check_array_data_tok(tok)) {
		tok = get_token(lexer);
	}
	
	//Если токен соответствует требованиям,
	//то идентифицируем его и записываем в blocks
	while(check_array_data_tok(tok)) {
		blocks.push_back(handler(tok, lexer));
		tok = get_token(lexer);

		if(tok.get_lexeme() == ",") {
			//Получаем токены, пока не найдём требуемые
			while(!check_array_data_tok(tok)) {
				tok = get_token(lexer);
			}
		}
	}

	//Записываем найденные значения (числа, символы и т.д.)
	for(int i = old_size; i < blocks.size(); ++i) {
		arr_blocks.push_back(blocks[i]);
	}

	//Отчищаем вектор blocks
	blocks.erase(blocks.begin() + old_size, blocks.begin() + blocks.size());
	//Создаём узел AST-дерева
	arr_data = new array_data_ast(arr_blocks);

	return arr_data;
}

AST *Parser::parse_array_name(Token token, Lexer *lexer) 
{
	array_name_ast *arr_name; 
	AST *identificator;
	Token tok;

	//Получаем токены, пока не найдём требуемые
	while(!check_array_name_tok(tok)) {
		tok = get_token(lexer);
		if(tok.get_lexeme() == "=") {
			break;
		}
	}

	//Проверяем, не был ли пропущен знак присваивания
	//Необходимо для корректной обработки array_data
	if(tok.get_lexeme() == "=") {
		buffer = tok;
		assigment_omitted = true;
		//Массив был объявлен
		arr_name = new array_name_ast(NULL, token);
		return arr_name;
	}

	//Обращаемся к элементам массива
	if(check_array_name_tok(tok)) {
		//Идентифицируем токен "tok"
		identificator = handler(tok, lexer);

		//Если токен "tok" удалось идентифицировать
		if(identificator) {
			arr_name = new array_name_ast(identificator, token);
		}
	}

	return arr_name;
}
