#include "parser.hpp"

php_beginning_ast::php_beginning_ast(vector <AST*> blocks) 
{
	this->definition = "BEGIN";
	this->blocks = blocks;
}

number_ast::number_ast(Token token)
{
	this->definition = token.get_lexeme();
}

variable_ast::variable_ast(Token token)
{
	this->definition = token.get_lexeme();
}

string_lexeme_ast::string_lexeme_ast(Token token)
{
	this->definition = token.get_lexeme();
}

assignment_ast::assignment_ast(AST *l_operand, AST *r_operand, Token token)
{
	this->definition = token.get_lexeme();
	this->l_operand = l_operand;
	this->r_operand = r_operand;
}

unary_oper_ast::unary_oper_ast(AST *operand, Token token)
{
	this->definition = token.get_lexeme();
	this->operand = operand;
}

binary_oper_ast::binary_oper_ast(AST *l_operand, AST *r_operand, Token token)
{
	this->definition = token.get_lexeme();
	this->l_operand = l_operand;
	this->r_operand = r_operand;
}

ternar_oper_ast::ternar_oper_ast(AST *l_operand, AST *r_operand, Token token)
{
	this->definition = token.get_lexeme();
	this->l_operand = l_operand;
	this->r_operand = r_operand;
}

logic_oper_ast::logic_oper_ast(AST *l_operand, AST *r_operand, Token token)
{
	this->definition = token.get_lexeme();
	this->l_operand = l_operand;
	this->r_operand = r_operand;
}


array_data_ast::array_data_ast(vector <AST*> blocks) 
{
	this->definition = "array_data";
	this->blocks = blocks;
}

array_name_ast::array_name_ast(AST *identificator, Token token)
{
	this->definition = token.get_lexeme();
	this->identificator = identificator;
}

printf_ast::printf_ast(vector <AST*> params, Token token)
{
	this->definition = token.get_lexeme();
	this->params = params;
}

if_ast::if_ast(AST *cond, AST *body, Token token)
{
	this->definition = token.get_lexeme();
	this->cond = cond;
	this->body = body;
}

if_body_ast::if_body_ast(vector <AST*> blocks) 
{
	this->definition = "BODY";
	this->blocks = blocks;
}

if_cond_ast::if_cond_ast(vector <AST*> blocks) 
{
	this->definition = "CONDITION";
	this->blocks = blocks;
}

else_ast::else_ast(vector <AST*> blocks, Token token) 
{
	this->definition = token.get_lexeme();
	this->blocks = blocks;
}

break_ast::break_ast(Token token)
{
	this->definition = token.get_lexeme();
}

for_ast::for_ast(AST *cond, AST *body, Token token)
{
	this->definition = token.get_lexeme();
	this->cond = cond;
	this->body = body;
}

for_body_ast::for_body_ast(vector <AST*> blocks) 
{
	this->definition = "BODY";
	this->blocks = blocks;
}

for_cond_ast::for_cond_ast(vector <AST*> blocks) 
{
	this->definition = "CONDITION";
	this->blocks = blocks;
}
