#pragma once
#include <iostream>
#include <string>

using namespace std;

class Token
{
	private:
		string description;
		string lexeme;
		int line;
		int pos;
	
	public:
		Token();
		Token(string description, string lexeme, int line, int pos);	
		string get_descr();
		string get_lexeme();
		int get_line();
		int get_pos();
		void set_descr(string description);
		void set_lexeme(string lexeme);
};
