#include <gtest/gtest.h>
#include "parser.hpp"

TEST(parser_test, parse_unary_oper)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "++$i";
	lexer->next_file_string = "++$i";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_unary_oper(token, lexer);
	unary_oper_ast *uanry_op = static_cast <unary_oper_ast*> (result); 
	EXPECT_EQ("++", uanry_op->definition);
}

TEST(parser_test, parse_binary_oper)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "% $num2;";
	lexer->next_file_string = "% $num2;";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_binary_oper(token, lexer);
	binary_oper_ast *bin_op = static_cast <binary_oper_ast*> (result); 
	EXPECT_EQ("%", bin_op->definition);
}

TEST(parser_test, parse_ternar_oper)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "? $expected : $reality;";
	lexer->next_file_string = "? $expected : $reality;";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_ternar_oper(token, lexer);
	ternar_oper_ast *ternar_op = static_cast <ternar_oper_ast*> (result); 
	EXPECT_EQ("?", ternar_op->definition);
}

TEST(parser_test, parse_logic_oper)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "&& $num2 != 0";
	lexer->next_file_string = "&& $num2 != 0";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_logic_oper(token, lexer);
	logic_oper_ast *logic_op = static_cast <logic_oper_ast*> (result); 
	EXPECT_EQ("&&", logic_op->definition);
}

TEST(parser_test, parse_assignment)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "= $num1 + $num2;";
	lexer->next_file_string = "= $num1 + $num2;";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_assignment(token, lexer);
	assignment_ast *assignment = static_cast <assignment_ast*> (result); 
	EXPECT_EQ("=", assignment->definition);
}
