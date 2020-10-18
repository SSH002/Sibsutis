#include "parser.hpp"

extern map <string, string> iden_table;
extern map <string, string> mem_pos;
extern vector <string> SC_marks;
extern vector <int> area_marks;

extern string var;
extern string value;
extern string operand1;
extern string operand2;
extern string command;
extern string comp_command;
extern string d_type_arrs;

extern string src_file_name;
extern string asm_file_name;
extern string exc_file_name;

extern string def;

extern int offset;
extern int area_mark_num;
extern int SC_mark_num;

extern bool use_reg_eax;
extern bool use_reg_edx;

extern bool first_if;
extern bool is_for;
extern bool is_ternar;
extern bool is_array_pos;
extern bool is_div_op;

//Компиляция ассемблерного файла
void CodGen::compile_asm_file()
{
	string comp = "gcc build/asm/" + asm_file_name + " -o " + exc_file_name;

	if(!system(comp.c_str())) {
		cout << "Done: " << src_file_name << " -> " <<  asm_file_name << " -> " << exc_file_name << endl;
	}
}

//Вывод ассемблерного файла на экран
void CodGen::print_asm()
{
	string asm_info;
	ifstream file("build/asm/" + asm_file_name);

	getline(file, asm_info, '\0');
	cout << asm_info << endl;

	file.close();
}

//Вставляется в начало ассемблерного файла
void CodGen::handle_begin()
{
	ofstream file("build/asm/" + asm_file_name);
	file << ".intel_syntax noprefix\n.global main\n\nmain:\n\tpush\t\trbp\n\tmov\t\trbp, rsp\n\tsub\t\trsp, 64" << endl;

	file.close();
}

//Вставляется в конец ассемблерного файла
void CodGen::handle_end()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);
	file << "\tleave\n\tret\n";

	file.close();
}

//Строковые константы
void CodGen::handle_SC_marks()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);

	for(auto &it : SC_marks) {
		file << it << endl;
	}

	file.close();
}

//Присваивание (при различных обстоятельствах)
void CodGen::handle_asm_mov()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);

	//Инициализация массива
	if(command == "array_index") {
		auto it = mem_pos.find(var);
		//Значение хранится в регистре "EAX"
		if(use_reg_eax) {
			file << "\tmov\t\teax, " + it->second + "]" << endl;
		//Значение хранится в регистре "EDX"
		} else if(use_reg_edx) { 	
			file << "\tmov\t\tedx, " + it->second + "]" << endl;
		}
		file << "\tcdqe" << endl;
	//Операция присваивания
	} else if(command == "=") {
		//Ищем в памяти расположение операндов
		auto it1 = mem_pos.find(operand1);
		auto it2 = mem_pos.find(operand2);
		//Тернарный оператор
		if(is_ternar) {
			file << "\tmov\t\t" + it1->second + "], eax" << endl;
		} else if(it1 != mem_pos.end() && !value.empty()) {
			if(is_array_pos) {
				if(d_type_arrs == "int") {
					file << "\tmov\t\t" + it1->second + "+rax*" + to_string(sizeof(int)) + "], " + value << endl;
				} else if(d_type_arrs == "string") {
					file << "\tmov\t\t" + it1->second + "+rax*" + to_string(sizeof(char)) + "], " + value << endl;
				}
			} else {
				if(iden_table.find(def)->second == "int" || iden_table.find(def)->second == "string") {
					file << "\tmov\t\t" + it1->second + "], " + value << endl;
				}
			}
		} else if(it1 != mem_pos.end() && it2 != mem_pos.end() && value.empty()) {
			//Не инициализированный массив
			if(is_array_pos) {
				//Целочисленный тип
				if(d_type_arrs == "int") {
					if(operand1.find("[0]") != string::npos && operand2.find("[0]") != string::npos) {
						file << "\tmov\t\tedx, " + it2->second + "+rdx*" + to_string(sizeof(int)) + "]" << endl << "\tmov\t\t" + it1->second + "+rax*" + to_string(sizeof(int)) + "], edx" << endl;
					} else if(operand1.find("[0]") != string::npos) {
						file << "\tmov\t\tedx, " + it2->second + "]" << endl << "\tmov\t\t" + it1->second + "+rax*" + to_string(sizeof(int)) + "], edx" << endl;
					} else if(operand2.find("[0]") != string::npos) {
						file << "\tmov\t\tedx, " + it2->second + "+rax*" + to_string(sizeof(int)) + "]" << endl << "\tmov\t\t" + it1->second + "], edx" << endl;
						}
				}
			//Строковый тип
			} else if(d_type_arrs == "string") {
				if(operand1.find("[0]") != string::npos && operand2.find("[0]") != string::npos) {
						file << "\tmov\t\tbl, " + it2->second + "+rdx*" + to_string(sizeof(char)) + "]" << endl << "\tmov\t\t" + it1->second + "+rax*" + to_string(sizeof(char)) + "], bl" << endl;
				} else if(operand1.find("[0]") != string::npos) {
						file << "\tmov\t\tbl, " + it2->second + "]" << endl << "\tmov\t\t" + it1->second + "+rax*" + to_string(sizeof(char)) + "], bl" << endl;
				} else if(operand2.find("[0]") != string::npos) {
						file << "\tmov\t\tbl, " + it2->second + "+rdx*" + to_string(sizeof(char)) + "]" << endl << "\tmov\t\t" + it1->second + "], bl" << endl;
				}
			} else {
				if(iden_table.find(def)->second == "int") {
					file << "\tmov\t\teax, " + it2->second + "]" << endl << "\tmov\t\t" + it1->second + "], eax" << endl;
				} else if(iden_table.find(def)->second == "string") {
					file << "\tmov\t\tal, " + it2->second + "]" << endl << "\tmov\t\t" + it1->second + "], al" << endl;
				}
			}
		}
	//Обрабатываем арифметические операции и операции сранения
	} else if(command == "+" || command == "-" || command == "*" || command == "/" 
		|| command == "%" || command == "==" || command == "!=" || command == "<="
		|| command == ">=" || command == "<" || command == ">") {
		if(is_div_op) {
			file << "\tmov\t\teax, edx" << endl;
			is_div_op = false;
			use_reg_eax = false;
		//Инициализация левого операнда
		} else if(use_reg_eax) {
			if(!value.empty()) {
				file << "\tmov\t\teax, " + value << endl;
			} else {
				auto it = mem_pos.find(var);
				//Если операнд - массив
				if(is_array_pos) {
					if(d_type_arrs == "int") {
						file << "\tmov\t\teax, " + it->second + "+rax*" + to_string(sizeof(int)) + "]" << endl;
						is_array_pos = false;
					} else if(d_type_arrs == "string") {
						file << "\tmov\t\tal, " + it->second + "+rax*" + to_string(sizeof(char)) + "]" << endl;
						is_array_pos = false;
					}
				//Если операнд - переменная
				} else {
					if(iden_table.find(def)->second == "int") {
						file << "\tmov\t\teax, " + it->second + "]" << endl;
					} else if(iden_table.find(def)->second == "string") {
						file << "\tmov\t\tal, " + it->second + "]" << endl;
					}
				}
			}

			use_reg_eax = false;
		//Деление или деление с остатком
		} else if(command == "/" || command == "%") {
			file << "\tcdq" << endl;
		//Инициализация правого операнда
		} else if(use_reg_edx) {
			if(!value.empty()) {
				file << "\tmov\t\tedx, " + value << endl;
			} else {
				auto it = mem_pos.find(var);
				//Если операнд - массив
				if(is_array_pos) {
					if(d_type_arrs == "int") {
						file << "\tmov\t\tedx, " + it->second + "+rdx*" + to_string(sizeof(int)) + "]" << endl;
						is_array_pos = false;
					} else if(d_type_arrs == "string") {
						file << "\tmov\t\tdl, " + it->second + "+rdx*" + to_string(sizeof(char)) + "]" << endl;
						is_array_pos = false;
					}
				//Если операнд - переменная
				} else {
					if(iden_table.find(def)->second == "int") {
						file << "\tmov\t\tedx, " + it->second + "]" << endl;
					} else if(iden_table.find(def)->second == "string") {
						file << "\tmov\t\tdl, " + it->second + "]" << endl;
					}
				}
			}

			use_reg_edx = false;
		}
	} else if(command == "printf") {
		auto it = mem_pos.find(var);
		//Операнд - массив
		if(is_array_pos) {
			if(d_type_arrs == "int") {
				file << "\tmov\t\teax, " + it->second + "+rax*" + to_string(sizeof(int)) + "]" << endl;
				is_array_pos = false;
			} else if(d_type_arrs == "string") {
				file << "\tmov\t\tal, " + it->second + "+rax*" + to_string(sizeof(char)) + "]" << endl;
				is_array_pos = false;
			}
		//Операнд - переменная
		} else {
			if(iden_table.find(def)->second == "int") {
				file << "\tmov\t\teax, " + it->second + "]" << endl;
			} else if(iden_table.find(def)->second == "string") {
				file << "\tmov\t\tal, " + it->second + "]" << endl;
			}
		}

		file  <<  "\tmov\t\tesi, eax" << endl;
		file  <<  "\tmov\t\tedi, OFFSET FLAT:.SC"+ to_string(SC_mark_num) << endl;
		file  <<  "\tmov\t\teax, 0" << endl;

		++SC_mark_num;
	} else if(command == "puts") {
			file << "\tmov\t\tedi, OFFSET FLAT:.SC"+ to_string(SC_mark_num) << endl;
			++SC_mark_num;
	}

	file.close();
}

//Суммирование
void CodGen::handle_asm_add()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);

	file << "\tadd\t\teax, edx" << endl;
	value = "eax";

	file.close();
}

//Разность
void CodGen::handle_asm_sub()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);

	file << "\tsub\t\teax, edx" << endl;
	value = "eax";

	file.close();
}

//Умножение
void CodGen::handle_asm_mul()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);

	file << "\timul\teax, edx" << endl;
	value = "eax";

	file.close();
}

//Целочисленное деление и деление с остатком
void CodGen::handle_asm_div()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);

	auto it = mem_pos.find(var);

	if(is_array_pos) {
		file << "\tidiv\t" + it->second + "+rdx*" + to_string(sizeof(int)) + "]" << endl;
	} else {
		file << "\tidiv\t" + it->second + "]" << endl;
	}

	value = (command != "%") ? "eax" : "edx";

	file.close();
}

//Инкрементирование
void CodGen::handle_asm_inc()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);
	auto it = mem_pos.find(var);
	file << "\tinc\t\t" + it->second + "]" << endl;

	file.close();
}

//Декрементирование
void CodGen::handle_asm_dec()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);
	auto it = mem_pos.find(var);
	file << "\tdec\t\t" + it->second + "]" << endl;

	file.close();
}

//Сравнение
void CodGen::handle_asm_cmp()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);

	if(is_ternar) {
		file << "\tcmp\t\tedx, eax" << endl;
	} else {
		file << "\tcmp\t\teax, edx" << endl;
	}

	file.close();
}

//Условие ветвления (if)
void CodGen::handle_asm_cond_pass_if()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);
	if(!is_for) {
		++area_mark_num;
	}

	if(comp_command == "==") {
		++area_mark_num;
		area_marks.push_back(area_mark_num);
		file << "\tjne\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == "!=") {
		++area_mark_num;
		area_marks.push_back(area_mark_num);

		file << "\tje\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == ">") {
		++area_mark_num;
		area_marks.push_back(area_mark_num);

		file << "\tjle\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == "<") {
		++area_mark_num;
		area_marks.push_back(area_mark_num);

		file << "\tjge\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == ">=") {
		++area_mark_num;
		area_marks.push_back(area_mark_num);

		file << "\tjl\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == "<=") {
		++area_mark_num;
		area_marks.push_back(area_mark_num);

		file << "\tjg\t\t.Area" + to_string(area_mark_num) << endl;
	}

	file.close();
}

//Условие выполнения цикла (for)
void CodGen::handle_asm_cond_pass_loop()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);

	if(comp_command == "==") {
		++area_mark_num;
		file << "\tje\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == "!=") {
		++area_mark_num;
		file << "\tjne\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == ">") {
		++area_mark_num;
		file << "\tjg\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == "<") {
		++area_mark_num;
		file << "\tjl\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == ">=") {
		++area_mark_num;
		file << "\tjge\t\t.Area" + to_string(area_mark_num) << endl;
	} else if(comp_command == "<=") {
		++area_mark_num;
		file << "\tjle\t\t.Area" + to_string(area_mark_num) << endl;
	}

	file.close();
}

//Переход к некоторой метке ('.Area' + area_mark_num)
void CodGen::handle_asm_jmp()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);
	++area_mark_num;
	file << "\tjmp\t\t.Area" + to_string(area_mark_num) << endl;

	file.close();
}

//Вызов функции (вывод на экран)
void CodGen::handle_asm_call()
{
	ofstream file("build/asm/" + asm_file_name, ios::app);

	if(command == "printf" || command == "puts") {
		file << "\tcall\t\tprintf" << endl;
	}

	file.close();
}

//Условное присваивание
//Присваивание происходит, если флаги "SF" != "OF"
void CodGen::handle_asm_cmovl()
{
	auto it = mem_pos.find(var);
	ofstream file("build/asm/" + asm_file_name, ios::app);
	file << "\tcmovl\teax, " + it->second + "]" << endl;

	file.close();
}

