#include <gtest/gtest.h>
#include "parser.hpp"

TEST(parser_test, parse_if)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "if($arr[$k] < $answer2) {$answer2 = $arr[$k];}";
	lexer->next_file_string = "if($arr[$k] < $answer2) {$answer2 = $arr[$k];}";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_if(token, lexer);
	if_ast *key_words_if = static_cast <if_ast*> (result); 
	EXPECT_EQ("if", key_words_if->definition);
}

TEST(parser_test, parse_if_cond)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "($arr[$k] < $answer2)";
	lexer->next_file_string = "($arr[$k] < $answer2)";

	lexer->input_spaces();
	result = parser->parse_if_cond(lexer);
	if_cond_ast *if_cond = static_cast <if_cond_ast*> (result); 
	EXPECT_EQ("CONDITION", if_cond->definition);
}

TEST(parser_test, parse_if_body)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "{$answer2 = $arr[$k];}";
	lexer->next_file_string = "{$answer2 = $arr[$k];}";

	lexer->input_spaces();
	result = parser->parse_if_body(lexer);
	if_body_ast *if_body = static_cast <if_body_ast*> (result); 
	EXPECT_EQ("BODY", if_body->definition);
}

TEST(parser_test, parse_else)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "else if($arr[$k] < $answer2) {$answer2 = $arr[$k];}";
	lexer->next_file_string = "else if($arr[$k] < $answer2) {$answer2 = $arr[$k];}";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_else(token, lexer);
	else_ast *key_words_else = static_cast <else_ast*> (result); 
	EXPECT_EQ("else", key_words_else->definition);
}
