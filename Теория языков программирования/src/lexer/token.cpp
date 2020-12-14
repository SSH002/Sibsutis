#include "token.hpp"

Token::Token() 	{}

Token::Token(string description, string lexeme, int line, int pos)
{
	this->description = description;
	this->lexeme = lexeme;
	this->line = line;
	this->pos = pos;
}

string Token::get_descr()
{
	return this->description;
}

string Token::get_lexeme()
{
	return this->lexeme;
}

int Token::get_line()
{
	return this->line;
}

int Token::get_pos()
{
	return this->pos;
}

void Token::set_descr(string description)
{
	this->description = description;
}

void Token::set_lexeme(string lexeme)
{
	this->lexeme = lexeme;
}