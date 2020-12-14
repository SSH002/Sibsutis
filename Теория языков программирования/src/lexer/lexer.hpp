#pragma once
#include <regex>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "token.hpp"

using namespace std;

#define files_count 7
#define NOLEXEME "Error"

/*Регулярные выражения*/

//Используется при нахождении переменных
const regex regex_var = regex("^[$][A-Za-z_][A-Za-z0-9_]*$");
//Используются при нахождении чисел
const regex regex_bin = regex("-?^0[bB][01]*$");
const regex regex_oct = regex("^-?0[0-7]*$");
const regex regex_dec = regex("^-?[1-9][0-9]*$");
const regex regex_hex = regex("^-?0[xX][A-Fa-f0-9]*$");

/*Регулярные выражения*/

class Lexer
{
	private:
		int line_count = 1;
		int file_pos = 0;

		int start_pos = 0;
		bool end_of_file = false;

		vector <string> shrinked_lexemes =  {", ", ";", "array(", "(", ")", "{", "}", "[", "]", "!=",
							"++", "+", "--", "-", "*", "/", "%", "<?php", "<?", "?>",
							"==", ">=", "<=", ">", "<", "=", "&&", "&", "||" "!"};
		vector <string> std_funcs;
		vector <string> parens;
		vector <string> key_words;
		vector <string> spec_symbols;
		vector <string> std_headers;
		vector <string> arith_ops;
		vector <string> logic_ops;
		vector <string> comp_ops;
		vector <vector<string>> std_lexemes = {key_words, std_funcs, parens, spec_symbols, comp_ops, logic_ops, arith_ops};

	public:
		string file_string;
		string next_file_string;
		Lexer();
		void input_spaces();
		void set_start_string(string file_path);
		void del_comments();
		string check_std_lex(string lexeme);
		string shrink_lex_from_string(int *start_pos);
		string search_string_lex(string *lexeme, int *lexeme_pos, char symbol);
		string search_std_lex(string lexeme, int *lexeme_pos);
		string search_digit_lex(string lexeme, int *lexeme_pos);
		string search_variables(string lexeme, int *lexeme_pos, int start_pos);
		string search_unaccept_symbols(string lexeme, int *lexeme_pos);
		Token get_token();
		void print(Token token);
};

