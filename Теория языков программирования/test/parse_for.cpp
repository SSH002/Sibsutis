#include <gtest/gtest.h>
#include "parser.hpp"

TEST(parser_test, parse_for)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "for($k = 0; $k < 10; ++$k) { $answer2 = $k; }";
	lexer->next_file_string = "for($k = 0; $k < 10; ++$k) { $answer2 = $k; }";

	lexer->input_spaces();

	Token token = parser->get_token(lexer);
	result = parser->parse_for(token, lexer);
	for_ast *key_words_for = static_cast <for_ast*> (result); 
	EXPECT_EQ("for", key_words_for->definition);
}

TEST(parser_test, parse_for_cond)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "($k = 0; $k < 10; ++$k)";
	lexer->next_file_string = "($k = 0; $k < 10; ++$k)";

	lexer->input_spaces();

	result = parser->parse_for_cond(lexer);
	for_cond_ast *for_cond = static_cast <for_cond_ast*> (result); 
	EXPECT_EQ("CONDITION", for_cond->definition);
}

TEST(parser_test, parse_for_body)
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *result = new AST();

	lexer->file_string = "{ if($arr[$k] < $answer2) {$answer2 = $arr[$k];} }";
	lexer->next_file_string = "{ if($arr[$k] < $answer2) {$answer2 = $arr[$k];} }";

	lexer->input_spaces();

	result = parser->parse_for_body(lexer);
	for_body_ast *for_body = static_cast <for_body_ast*>( result); 
	EXPECT_EQ("BODY", for_body->definition);
}
