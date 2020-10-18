#include <gtest/gtest.h>
#include "parser.hpp"

TEST(parser_test, parse_array_data)
{
	Lexer *lexer = new Lexer();
	AST *result = new AST();
	Parser *parser = new Parser();

	lexer->file_string = "array(110, 172, 510, 183, 66, 410, 77, 20, 34, 81)";
	lexer->next_file_string = "array(110, 172, 510, 183, 66, 410, 77, 20, 34, 81)";

	lexer->input_spaces();
	result = parser->parse_array_data(lexer);
	array_data_ast *arr_data = static_cast <array_data_ast*> (result); 
	EXPECT_EQ("array_data", arr_data->definition);
}

TEST(parser_test, parse_array_name)
{
	Lexer *lexer = new Lexer();
	AST *result = new AST();
	Parser *parser = new Parser();

	lexer->file_string = "$arr[$k]";
	lexer->next_file_string = "$arr[$k]";

	lexer->input_spaces();
	Token token = parser->get_token(lexer);
	result = parser->parse_array_name(token, lexer);
	array_name_ast *arr_name = static_cast <array_name_ast*> (result); 
	EXPECT_EQ("$arr", arr_name->definition);
}
