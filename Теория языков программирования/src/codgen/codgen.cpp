#include "parser.hpp"

//Таблица идентификаторов
extern map <string, string> iden_table;
//Словарь, в котором указано расположение переменных в памяти
map <string, string> mem_pos;
//Вектор, в котором хранятся метки на строковые константы
vector <string> SC_marks;
//Вектор, в котором хранятся указатели на области видимости
vector <int> area_marks;

//Идентификатор переменной
string var;
//Значение переменной
string value;
//Левый операнд
string operand1;
//Правый операнд
string operand2;
//Команда или операция
string command;
//Операция сравнения
string comp_command;
//Тип данных массива
string d_type_arrs;

//Файл с исходным кодом
string src_file_name;
//Файл с ассемблерным кодом
string asm_file_name;
//Исполняемый файл
string exc_file_name;

//Имя массива
string array_name;
//Элемент массива
string array_member;
//Название переменной
string def;

//Используется ли регистр "eax"
bool use_reg_eax = true;
//Используется ли регистр "edx"
bool use_reg_edx = true;

//Впервые ли встречается условное ветвление
bool first_if = true;

//Обрабатываем лои условное ветвление
bool is_if = false;
//Обрабатываем ли цикл "for"
bool is_for = false;
//Обрабатываем ли тернарный оператор
bool is_ternar = false;
//Обрабатываем ли 
bool is_break = false;
//Обрабатываем ли инициализацию массива
bool is_array_pos = false;
//Обрабатываем ли операцию деление (или остаток от деления)
bool is_div_op = false;
 	
//Счётчик обработанных условных ветвлений
int if_counter = 0;
//Смещение (отображает, сколько памяти уже выделено для программы)
int offset = 0;
//Номер метки области видимости
int area_mark_num = 0;
//Номер метки строковой константы
int SC_mark_num = 0;
//Метка для случая, когда в теле цикла есть ключевое слово "break"
int L_mark_storage;
//Индекс, отвечает за обрабатываемую часть условия цикла "for"
int for_cond_elem;

CodGen *cod_gen = new CodGen();

CodGen::CodGen()
{
	
}

void php_beginning_ast::codgen()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->codgen();
	}
}

void array_data_ast::codgen()
{
	int arr_offset;

	if(d_type_arrs == "int") {
		arr_offset = offset + blocks.size() * sizeof(int);
		offset = arr_offset;
	} else if(d_type_arrs == "string") {
		arr_offset = offset + blocks.size() * sizeof(char);
		offset = arr_offset;
	}

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->codgen();
		array_member = array_name + "[" + to_string(i) + "]";

		if(mem_pos.find(array_member) == mem_pos.end()) {
			if(d_type_arrs == "int") {
				mem_pos.emplace(array_member, "DWORD PTR [rbp-" + to_string(arr_offset));
				arr_offset -= 4;
			} else if(d_type_arrs == "string") {
				mem_pos.emplace(array_member, "BYTE PTR [rbp-" + to_string(arr_offset));
				arr_offset -= 1;
			}
		}

		operand1 = array_member;
		cod_gen->handle_asm_mov();
	}

	 operand1.clear();
}

void array_name_ast::codgen()
{	
	array_name = definition;
	auto it = iden_table.find(definition);
	d_type_arrs = it->second;

	if(identificator != NULL) {
		identificator->codgen();
		//Если идёт обращение к одному из элементов массива
		if(!value.empty()) {
			array_member = array_name + "[" + value + "]";
			var = array_member;
		//При инициализации массива
		} else {
			string buff = command;
			command = "array_index";
			is_array_pos = true;

			cod_gen->handle_asm_mov();
			array_member = array_name + "[0]";
			var = array_member;
		
			command = buff;
		}
	}

	value.clear();
}

void for_ast::codgen()
{
	is_for = true;
	for_cond_elem = 0;

	ofstream file("build/asm/" + asm_file_name, ios::app);	

	cond->codgen();
	cod_gen->handle_asm_jmp();

	L_mark_storage = area_mark_num;
	body->codgen();

	if(is_if) {
		file << ".Area" + to_string(area_mark_num) + ":" << endl;
		is_if = false;
	}

	for_cond_elem = 2;
	cond->codgen();
	file  <<  ".Area" + to_string(L_mark_storage) + ":" << endl;
	for_cond_elem = 1;

	cond->codgen();
	cod_gen->handle_asm_cond_pass_loop();

	if(is_break) {
		file <<  ".Area" + to_string(L_mark_storage + if_counter + 2) + ":" << endl;
	}

	is_break = false;
	is_for = false;

	file.close();
}

void for_body_ast::codgen()
{
	string node_type;

	ofstream file("build/asm/" + asm_file_name, ios::app);

	for(int i = 0; i < blocks.size(); ++i) {
		node_type = typeid(*(blocks[i])).name();

		if(node_type.find("if_ast") != string::npos) {
			++if_counter;
		}
	}

	file << ".Area" + to_string(area_mark_num + if_counter + 1) + ":" << endl;

	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->codgen();
	}

	file.close();
}

void for_cond_ast::codgen()
{	
	blocks[for_cond_elem]->codgen();
}

void if_ast::codgen()
{
	is_if = true;

	if(is_for) {
		ofstream file("build/asm/" + asm_file_name, ios::app);
		if(!first_if) {
			file << ".Area" + to_string(area_mark_num) + ":" << endl;
		}
		first_if = false;

		file.close();
	}

	cond->codgen();
	cod_gen->handle_asm_cond_pass_if();

	body->codgen();
}

void if_body_ast::codgen()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->codgen();
	}

	if(!is_for) {
		ofstream file("build/asm/" + asm_file_name, ios::app);
		file << ".Area" + to_string(area_mark_num) + ":" << endl;
		file.close();
	}
}

void if_cond_ast::codgen()
{	
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->codgen();
	}
}

void else_ast::codgen()
{
	for(int i = 0; i < blocks.size(); ++i) {
		blocks[i]->codgen();
	}

	if(!is_if) {
		ofstream file("build/asm/" + asm_file_name, ios::app);
		file  <<  ".Area" + to_string(area_mark_num) + ":" << endl;
		file.close();
	}
}

void assignment_ast::codgen()
{	
	command = definition;
	l_operand->codgen();
	operand1 = var;
	r_operand->codgen();
	operand2 = var;

	cod_gen->handle_asm_mov();
	is_array_pos = false;
	is_ternar = false;

	value.clear();
	var.clear();
	operand1.clear();
	operand2.clear();
	command.clear();
	array_name.clear();
	array_member.clear();
}

void logic_oper_ast::codgen()
{
	l_operand->codgen();

	ofstream file("build/asm/" + asm_file_name, ios::app);
	int numb = 1;

	if(!is_for) {
		++numb;
	}

	if(definition == "&&") {
		file  <<  "\tjne\t\t.Area" + to_string(area_mark_num + numb) << endl;
	} else if(definition == "||") {
		file  <<  "\tje\t\t.Area" + to_string(area_mark_num + numb) << endl;
	}

	r_operand->codgen();

	file.close();
}

void ternar_oper_ast::codgen()
{
	is_ternar = true;
	l_operand->codgen();
	cod_gen->handle_asm_cmovl();
	r_operand->codgen();
}

void binary_oper_ast::codgen()
{
	string buff = command;
	command = definition;

	if(command == "+") {
		l_operand->codgen();
		cod_gen->handle_asm_mov();
		r_operand->codgen();
		cod_gen->handle_asm_mov();
		cod_gen->handle_asm_add();
	} else if(command == "-") {
		l_operand->codgen();
		cod_gen->handle_asm_mov();
		r_operand->codgen();
		cod_gen->handle_asm_mov();
		cod_gen->handle_asm_sub();
	} else if(command == "*") {
		l_operand->codgen();
		cod_gen->handle_asm_mov();
		r_operand->codgen();
		cod_gen->handle_asm_mov();

		cod_gen->handle_asm_mul();
	} else if(command == "/" || command == "%") {
		l_operand->codgen();
		cod_gen->handle_asm_mov();
		r_operand->codgen();
		cod_gen->handle_asm_mov();
		cod_gen->handle_asm_div();

		is_div_op = true;
	} else if(command == "==" || command == "!=" || command == ">" || command == "<" 
		|| command == ">=" || command == "<=") {

		l_operand->codgen();
		cod_gen->handle_asm_mov();
		r_operand->codgen();
		cod_gen->handle_asm_mov();

		cod_gen->handle_asm_cmp();
		comp_command = command;
	}

	command = buff;
	use_reg_edx = true;
	use_reg_eax = true;
	is_array_pos = false;
}

void unary_oper_ast::codgen()
{
	operand->codgen();

	if(definition == "++") {
		cod_gen->handle_asm_inc();
	} else if(definition == "--") {
		cod_gen->handle_asm_dec();
	}
}

void printf_ast::codgen()
{
	string buff = command;
	command = definition;

	for(int i = 0; i < params.size(); ++i) {
		params[i]->codgen();
	}

	if(params.size() == 1) {
		command = "puts";
	}

	cod_gen->handle_asm_mov();
	cod_gen->handle_asm_call();
	command = buff;
}

void string_lexeme_ast::codgen()
{
	if(command == "printf") {
		SC_marks.push_back(".SC" + to_string(SC_mark_num) + ":\n\t\t.string\t" + definition);
	} else {
		char symbol = definition.c_str()[1];
		int askii_cod =(int)symbol;

		value = to_string(askii_cod);
	}
}

void number_ast::codgen()
{
	value = definition;

}

void variable_ast::codgen()
{
	string d_type_vars;
	def = definition;

	auto it = iden_table.find(definition);

	d_type_vars = it->second; 

	if(mem_pos.find(definition) == mem_pos.end()) {
		if(d_type_vars == "int") {
			mem_pos.emplace(definition, "DWORD PTR [rbp-" + to_string(offset + 4));
			offset += 4;
		} else if(d_type_vars == "string") {
			mem_pos.emplace(definition, "BYTE PTR [rbp-" + to_string(offset + 1));
			offset += 1;
		}
	}

	var = definition;
	value.clear();
}

void break_ast::codgen() 
{
	is_break = true;
	ofstream file("build/asm/" + asm_file_name, ios::app);
	file  <<  "\tjmp\t\t.Area" + to_string(L_mark_storage + if_counter + 2) << endl;

	file.close();
}

void CodGen::set_fileame(string file_path)
{
	int start_pos = file_path.rfind("/");
	int end_pos = file_path.rfind(".");	

	asm_file_name = file_path.substr(start_pos + 1, end_pos - start_pos - 1);
	
	src_file_name = asm_file_name + ".php";
	exc_file_name = asm_file_name;
	asm_file_name += ".s";
}

void CodGen::start_codgen(AST *tree, string file_path)
{
	set_fileame(file_path);
	
	handle_begin();
	php_beginning_ast *php_beginning = static_cast <php_beginning_ast*> (tree);
	php_beginning->codgen();
	handle_end();
	handle_SC_marks();
}

