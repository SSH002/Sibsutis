#include "parser.hpp"

extern vector <AST*> blocks;

AST *Parser::parse_string_lexeme(Token token)
{
	string_lexeme_ast *text = new string_lexeme_ast(token);

	return text;
}

AST *Parser::parse_number(Token token)
{		
	number_ast *val = new number_ast(token);

	return val;
}

AST *Parser::parse_variable(Token token)
{		
	variable_ast *var = new variable_ast(token);

	return var;
}

AST *Parser::parse_break(Token token, Lexer *lexer)
{		
	break_ast *key_word_break = new break_ast(token);
	Token next = get_token(lexer);
	check_end_of_oper_error(token, next);

	return key_word_break;
}

