#include <gtest/gtest.h>
#include "lexer.hpp"

int lex_pos;

TEST(lexer_test, search_string_lex)
{
	Lexer lexer;

	string str;
	string expected = "string_literal";
	lexer.file_string = "\"Sum summus mus\"";
	lexer.next_file_string = "\"Sum summus mus\"";

	string result = lexer.search_string_lex(&str, &lex_pos, '"');

	EXPECT_EQ(expected, result);
}

TEST(lexer_test, search_std_lex)
{
	Lexer lexer;
	
	string expected = "key_word_return";
	lexer.file_string = " return ";
	lexer.next_file_string = " return ";

	string result = lexer.search_std_lex("return", &lex_pos);

	EXPECT_EQ(expected, result);
}

TEST(lexer_test, search_digit_lex)
{
	Lexer lexer;
	
	string expected = "digit_literal_dec";
	lexer.file_string = "2147483647";
	lexer.next_file_string = "2147483647";

	string result = lexer.search_digit_lex("2147483647", &lex_pos);

	EXPECT_EQ(expected, result);
}

TEST(lexer_test, search_variables)
{
	Lexer lexer;
	
	string expected = "variable";
	lexer.file_string = "$answer1";
	lexer.next_file_string = "$answer1";

	string result = lexer.search_variables("$answer1", &lex_pos, 0);

	EXPECT_EQ(expected, result);
}

TEST(lexer_test, search_unaccept_symbols)
{
	Lexer lexer;
	
	string expected = "unacceptable_symbol";
	lexer.file_string = "@№";
	lexer.next_file_string = "@№";

	string result = lexer.search_unaccept_symbols("@№", &lex_pos);

	EXPECT_EQ(expected, result);
}

TEST(lexer_test, check_std_lexs)
{
	Lexer lexer;
	string expected = "compare_operator_==";

	string result = lexer.check_std_lex("==");

	EXPECT_EQ(expected, result);
}

TEST(lexer_test, shrink_lex_from_string)
{
	Lexer lexer;
	
	int start_pos = 0;
	string expected = "<?php";
	lexer.next_file_string = " <?php $answer = 1000; ?> ";

	string result = lexer.shrink_lex_from_string(&start_pos);

	EXPECT_EQ(expected, result);
}

TEST(lexer_test, get_token)
{
	Lexer lexer;
	
	string expected = "5";
	lexer.file_string = " 5 + 2 * 3 ";
	lexer.next_file_string = " 5 + 2 * 3 ";

	Token token = lexer.get_token();
	
	EXPECT_EQ(expected, token.get_lexeme());
}

