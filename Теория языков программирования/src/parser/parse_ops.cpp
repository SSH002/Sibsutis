#include "parser.hpp"

extern vector <AST*> blocks;

AST *Parser::parse_assignment(Token token, Lexer *lexer)
{
	Token tok, buffer, prev;
	assignment_ast *assignment;
	AST *l_operand, *r_operand;

	buffer = Parser::preview;
	Parser::preview = token;

	//Заполняем левый операнд операции присваивания
	l_operand = blocks[blocks.size() - 1];

	//Считываем токены до конца выражения
	while(tok.get_lexeme() != ";" && tok.get_lexeme() != "," && Parser::current.get_lexeme() != ";") {	
		prev = tok;
		//Если ранее, при обработке массивов, было пропущено присваивание
		if(assigment_omitted) {
			assigment_omitted = false;
			tok.set_lexeme("array(");
			tok.set_descr("left_paren_array(");
		} else {
			tok = get_token(lexer);
		}
		//Проверяем, не была ли пропущена запятая или точка с запятой
		check_semicolon_and_comma_error(tok, prev);

		//Если токены соответствуют требованиям,
		//то идентифицируем их и записываем в blocks
		if(check_assignment_tok(tok)) {
			blocks.push_back(handler(tok, lexer));	
		}
	}

	//Заполняем правый операнд операции присваивания
	r_operand = blocks[blocks.size() - 1];

	//Создаём узел AST-дерева
	assignment = new assignment_ast(l_operand, r_operand, token);

	//Удаляем из вектора левый и правый операнды
	blocks.pop_back();
	blocks.pop_back();

	Parser::preview = buffer;
	Parser::current.set_lexeme("");

	return assignment;
}

AST *Parser::parse_unary_oper(Token token, Lexer *lexer)
{
	Token tok;
	unary_oper_ast *unary_op;
	AST *operand;

	//Проверяем, является ли токен унарным оператором
	if(check_unary_oper_tok(Parser::preview)) {
		operand = blocks[blocks.size() - 1];
		blocks.pop_back();
		//Создаём узел AST-дерева
		unary_op = new unary_oper_ast(operand, token);
	} else {
		//Получаем токены, пока не найдём требуемые
		while(!check_unary_oper_tok(tok)) {
			tok = get_token(lexer);
		}

		operand = handler(tok, lexer);
		//Создаём узел AST-дерева
		unary_op = new unary_oper_ast(operand, token);
	}

	return unary_op;
}

AST *Parser::parse_binary_oper(Token token, Lexer *lexer)
{ 
	Token tok;
	binary_oper_ast *binary_op;
	AST *l_operand, *r_operand;

	//Получаем токены, пока не найдём требуемые
	while(!check_binary_oper_tok(tok)) {
		tok = get_token(lexer);
	}
	//Заполняем левый операнд
	l_operand = blocks[blocks.size() - 1];
	//Заполняем правый операнд
	r_operand = handler(tok, lexer);

	blocks.pop_back();
	//Создаём узел AST-дерева
	binary_op = new binary_oper_ast(l_operand, r_operand, token);
	
	return binary_op;
}

AST *Parser::parse_ternar_oper(Token token, Lexer *lexer)
{
	Token tok;
	ternar_oper_ast *ternar_op;
	AST *l_operand, *r_operand;

	//Получаем токены, пока не найдём требуемые
	while(!check_ternar_oper_tok(tok)) {
		tok = get_token(lexer);
	}
	//Заполняем левый операнд
	l_operand = blocks[blocks.size() - 1];
	
	while(check_ternar_oper_tok(tok)) {	
		blocks.push_back(handler(tok, lexer));
		tok = get_token(lexer);
		Parser::current = tok;
	}
	//Заполняем правый операнд
	r_operand = blocks[blocks.size() - 1];
	//Создаём узел AST-дерева
	ternar_op = new ternar_oper_ast(l_operand, r_operand, token);
	blocks.pop_back();
	blocks.pop_back();

	return ternar_op;
}

AST *Parser::parse_logic_oper(Token token, Lexer *lexer)
{
	Token tok;
	logic_oper_ast *logic_op;
	AST *l_operand, *r_operand;

	//Получаем токены, пока не найдём требуемые
	while(!check_logic_oper_tok(tok)) {
		tok = get_token(lexer);
	}
	//Заполняем левый операнд
	l_operand = blocks[blocks.size() - 1];
	
	while(check_logic_oper_tok(tok)) {	
		blocks.push_back(handler(tok, lexer));
		tok = get_token(lexer);
		Parser::current = tok;
	}
	//Заполняем правый операнд
	r_operand = blocks[blocks.size() - 1];
	//Создаём узел AST-дерева
	logic_op = new logic_oper_ast(l_operand, r_operand, token);
	blocks.pop_back();
	blocks.pop_back();

	return logic_op;
}
