#include <gtest/gtest.h>
#include "parser.hpp"

TEST(parser_test, parse_printf)
{
	Lexer *lexer = new Lexer();
	AST *result = new AST();
	Parser *parser = new Parser();

	lexer->file_string = "printf(\"Min_Div = %d\", $answer1);";
	lexer->next_file_string = "printf(\"Min_Div = %d\", $answer1);";

	lexer->input_spaces();
	Token token = parser->get_token(lexer);
	result = parser->parse_printf(token, lexer);
	printf_ast *func_printf = static_cast <printf_ast*> (result); 
	EXPECT_EQ("printf", func_printf->definition);
}
