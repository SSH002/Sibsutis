#include <gtest/gtest.h>
#include "parser.hpp"

TEST(parser_test, parse_string_lexeme)
{
	Lexer *lexer = new Lexer();
	AST *result = new AST();
	Parser *parser = new Parser();

	lexer->file_string = "\"Sum summus mus\"";
	lexer->next_file_string = "\"Sum summus mus\"";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_string_lexeme(token);
	string_lexeme_ast *text = static_cast <string_lexeme_ast*> (result); 
	EXPECT_EQ("\"Sum summus mus\"", text->definition);
}

TEST(parser_test, parse_number)
{
	Lexer *lexer = new Lexer();
	AST *result = new AST();
	Parser *parser = new Parser();

	lexer->file_string = "0x3f";
	lexer->next_file_string = "0x3f";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_number(token);
	number_ast *val = static_cast <number_ast*> ( result); 
	EXPECT_EQ("0x3f", val->definition);
}

TEST(parser_test, parse_variable)
{
	Lexer *lexer = new Lexer();
	AST *result = new AST();
	Parser *parser = new Parser();

	lexer->file_string = "$variable";
	lexer->next_file_string = "$variable";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_variable(token);
	variable_ast *var = static_cast <variable_ast*> (result); 
	EXPECT_EQ("$variable", var->definition);
}

TEST(parser_test, parse_break)
{
	Lexer *lexer = new Lexer();
	AST *result = new AST();
	Parser *parser = new Parser();

	lexer->file_string = "break;";
	lexer->next_file_string = "break;";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_break(token, lexer);
	break_ast *key_words_break = static_cast <break_ast*> (result); 
	EXPECT_EQ("break", key_words_break->definition);
}
