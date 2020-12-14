#include <gtest/gtest.h>
#include "parser.hpp"

TEST(parser_test, parse_php_beginning)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "<?php $answer2 = 2147483647; ?>";
	lexer->next_file_string = "<?php $answer2 = 2147483647; ?>";

	lexer->input_spaces();
	result = parser->parse_php_beginning(lexer);
	php_beginning_ast *php_begin = static_cast <php_beginning_ast*> (result); 
	EXPECT_EQ("BEGIN", php_begin->definition);
}

TEST(parser_test, build_ast)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "<?php $answer = 1000; ?>";
	lexer->next_file_string = "<?php $answer = 1000; ?>";

	lexer->input_spaces();
	result = parser->build_ast(lexer);
	php_beginning_ast *php_begin = static_cast <php_beginning_ast*> (result); 
	EXPECT_EQ("BEGIN", php_begin->definition);
}
