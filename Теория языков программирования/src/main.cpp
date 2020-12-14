#include <dirent.h>
#include "parser.hpp"

//Проверка, существует ли указанный файл
int check_file(const char *filename)
{
	DIR *dir;
	//Если указанный файл - каталог
	if((dir = opendir(filename))) {
		cout << "Error! File \"" << filename << "\" is directory.\n";
		closedir(dir);
		return 3;
	} else {
		FILE *file = fopen(filename, "r");
		//Если указанный файл не существует
		if(!file) {
			cout << "Error! File \"" << filename << "\" not exist.\n";
			return 2;
		}
	}

	//Если всё в порядке
	return 0;
}

int main(int argc, char const *argv[])
{
	Lexer *lexer = new Lexer();
	Parser *parser = new Parser();
	AST *tree = new AST();
	SymbolTable *symbol_table = new SymbolTable();
	Sema *sema = new Sema();
	CodGen *cod_gen = new CodGen();

	Token token;
	string file_path;
	string command;
	const char *filename;

	//Если указана опция при запуске
	if(argc == 3) {
		command = argv[1];
		file_path = argv[2];
		filename = argv[2];
	//Если нет
	} else if(argc == 2) {
		file_path = argv[1];
		filename = argv[1];
	}

	//Проверка, существует ли указанный файл
	if(!check_file(filename)) {
		lexer->set_start_string(file_path);
		//Посмотреть работу лексера
		if(command == "--dump-tokens") {
			while(1) {
				token = lexer->get_token();
				if(token.get_lexeme() == " ") {
					break;
				}
				if(!token.get_lexeme().empty()) {
					lexer->print(token);
				}
			}
		//Посмотреть работу парсера
		} else if(command == "--dump-ast") {
			tree = parser->build_ast(lexer);
			print_ast(tree);
		//Посмотреть работу таблицы символов
		} else if(command == "--dump-smt") {
			tree = parser->build_ast(lexer);
			symbol_table->set_symtab(tree);
			symbol_table->print_symtab();
		//Посмотреть семантический анализ
		} else if(command == "--dump-idt") {
			tree = parser->build_ast(lexer);
			symbol_table->set_symtab(tree);
			sema->check_semantic(tree);
			sema->print_semantic();
		//Посмотреть ассемблер
		} else if(command == "--dump-asm") {
			tree = parser->build_ast(lexer);
			symbol_table->set_symtab(tree);
			sema->check_semantic(tree);
			cod_gen->start_codgen(tree, file_path);
			cod_gen->print_asm();
		//Если не задана опция при запуске
		} else if(command.empty()) {
			tree = parser->build_ast(lexer);
			symbol_table->set_symtab(tree);
			sema->check_semantic(tree);
			cod_gen->start_codgen(tree, file_path);
			cod_gen->compile_asm_file();
		//Если при запуске указаны неверные параметры
		} else {
			cout << "Error! Input incorrect parameters. Examples:\n./comp --dump-tokens *.php\n./comp --dump-ast *.php\n./comp --dump-smt *.php\n./comp --dump-idt *.php\n./comp --dump-asm *.php\n./comp *.php\n" << endl;
		}
	}

	free(lexer);
	free(parser);
	free(tree);
	free(symbol_table);
	free(sema);
	free(cod_gen);

	return 0;
}
