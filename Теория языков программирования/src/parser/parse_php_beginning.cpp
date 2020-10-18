#include "parser.hpp"

extern vector <AST*> blocks;

AST *Parser::parse_php_beginning(Lexer *lexer)
{
	Token tok, prev;
	php_beginning_ast *php_beginning;
	
	//Сюда ещё попадает array(
	/*int d = 0;
	while(tok.get_lexeme() != "?>") {	
		tok = get_token(lexer);
		
		cout << d << "} " << tok.get_lexeme() << " | " << tok.get_token_class() << endl;
		++d;
	} exit(1);*/

	while(tok.get_lexeme() != "?>") {	
		prev = tok;
		tok = get_token(lexer);
	
		if(check_body_tok(tok)) {
			blocks.push_back(handler(tok, lexer));
		}
	}

	if(!blocks.empty()) {
		php_beginning = new php_beginning_ast(blocks);
	}

	blocks.clear();

	return php_beginning;
}


