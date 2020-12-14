#include "parser.hpp"

//Текущая область видимости
string declr_level;
//Предыдущая область видимости
string prev_declr_level;

//Символы, используемые при определении названия области видимости
string alphabet[] = {"", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
			"n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
//Индекс, по которому обращаемся к одному из элементов "alphabet[]"
int _index = 0;

//Буферный вектор, в котором хранятся переменные текущей (обрабатываемой)
//области видимости, которые затем попадают в таблицу символов
vector <string> identificators;
//Непосредственно таблица символов
map <string, vector <string>> sym_table;
//Используется для вычисления индекса "_index"
multiset <string> levels;

SymbolTable *symbol_table = new SymbolTable();
SymbolTable::SymbolTable() {}

void php_beginning_ast::symtab()
{
	prev_declr_level = "0";
	declr_level = "0";

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->symtab();
	}

	//Вставка последней области видимости в таблицу символов
	identificators = symbol_table->check_multi_declaration(identificators);
	sym_table.emplace(declr_level, identificators);
}

//Обработка данных массива
void array_data_ast::symtab()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->symtab();
	}
}

//Обработка имени массива
void array_name_ast::symtab()
{	
	identificators.push_back(definition);

	if(identificator != NULL) {
		identificator->symtab();
	}
}

//Обработка цикла "for"
void for_ast::symtab()
{
	cond->symtab();
	prev_declr_level = declr_level;

	body->symtab();
	prev_declr_level = to_string(stoi(prev_declr_level) - 1);
}

//Обработка условия цикла "for"
void for_cond_ast::symtab()
{
	identificators = symbol_table->check_multi_declaration(identificators);

	sym_table.emplace(declr_level, identificators);
	++_index;

	if(symbol_table->compare_levels(declr_level, to_string(stoi(prev_declr_level) + 1))) {
		_index = levels.count(to_string(stoi(prev_declr_level) + 1)) + 1;
	}

	declr_level = to_string(stoi(prev_declr_level) + 1) + alphabet[_index];
	levels.insert(to_string(stoi(prev_declr_level) + 1));
	identificators.clear();

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->symtab();
	}
}


//Обработка тела цикла "for"
void for_body_ast::symtab()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->symtab();
	}
}

//Обработка условного ветвления
void if_ast::symtab()
{
	cond->symtab();
	prev_declr_level = declr_level;

	body->symtab();
	prev_declr_level = to_string(stoi(prev_declr_level) - 1);
}

//Обработка тела условного ветвления
void if_body_ast::symtab()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->symtab();
	}
}

//Обработка условия условного ветвления
void if_cond_ast::symtab()
{
	identificators = symbol_table->check_multi_declaration(identificators);

	sym_table.emplace(declr_level, identificators);
	++_index;

	if(symbol_table->compare_levels(declr_level, to_string(stoi(prev_declr_level) + 1))) {
		_index = levels.count(to_string(stoi(prev_declr_level) + 1)) + 1;
	}

	declr_level = to_string(stoi(prev_declr_level) + 1) + alphabet[_index];

	levels.insert(to_string(stoi(prev_declr_level) + 1));
	identificators.clear();

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->symtab();
	}
}

//Обработка ветвления "else"
void else_ast::symtab()
{
	identificators = symbol_table->check_multi_declaration(identificators);

	sym_table.emplace(declr_level, identificators);
	++_index;

	if(symbol_table->compare_levels(declr_level, to_string(stoi(prev_declr_level) + 1))) {
		_index = levels.count(to_string(stoi(prev_declr_level) + 1)) + 1;
	}

	declr_level = to_string(stoi(prev_declr_level) + 1) + alphabet[_index];
	levels.insert(to_string(stoi(prev_declr_level) + 1));
	identificators.clear();

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->symtab();
	}
}

//Обработка присваивания
void assignment_ast::symtab()
{	
	l_operand->symtab();
	r_operand->symtab();
}

//Обработка параметров логического оператора
void logic_oper_ast::symtab()
{
	l_operand->symtab();
	r_operand->symtab();
}

//Обработка параметров тернарного оператора
void ternar_oper_ast::symtab()
{
	l_operand->symtab();
	r_operand->symtab();
}

//Обработка параметров бинарного оператора
void binary_oper_ast::symtab()
{
	l_operand->symtab();
	r_operand->symtab();
}

//Обработка параметров унарного оператора
void unary_oper_ast::symtab()
{
	operand->symtab();
}

//Обработка параметров функции "printf"
void printf_ast::symtab()
{
	for(int i = 0; i < params.size(); ++i) {
		params[i]->symtab();
	}
}

//Обработка строковых констань
void string_lexeme_ast::symtab()
{
}

//Обработка числовых констант
void number_ast::symtab()
{
}

//Обработка переменных
void variable_ast::symtab()
{
	identificators.push_back(definition);
}

//Обработка ключевого слова "break"
void break_ast::symtab()
{
}

//Вывод таблицы символов на экран
void SymbolTable::print_symtab()
{
	cout << "\nIdentificators | Declaration level\n";
	cout << "+------------------------------------+";

	for(auto it = sym_table.begin(); it != sym_table.end(); ++it) {
		for(int i = 0; i <(it->second).size(); ++i) {
			if(i % 2 == 0) {
				cout << endl;
			}

			//Если переменная - последняя в списек
			if(i ==(it->second).size() - 1) {
				cout << (it->second)[i];
			//Иначе
			} else {
				cout << (it->second)[i] << ", ";
			}
		}

		cout << "\t\t\t" << it->first;
		cout << "\n+------------------------------------+\n";
	}
}

//Создание таблицы символов
void SymbolTable::set_symtab(AST *tree)
{
	php_beginning_ast *php_beginning = static_cast <php_beginning_ast*> (tree);
	
	php_beginning->symtab();
}
