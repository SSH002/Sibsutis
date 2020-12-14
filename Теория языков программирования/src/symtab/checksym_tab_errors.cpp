#include "parser.hpp"

//Переменная объявлена в "sym_tab.cpp"
extern map <string, vector <string>> sym_table;

//Сравнение иденификаторов областей видимости
//Возвращает "true", если значение "prev_level" меньше "new_level"
bool SymbolTable::compare_levels(string preview_level, string current_level)
{
	return (stoi(preview_level) < stoi(current_level));
}

//Проверяем, не были ли занесены какие-то переменные в таблицу символов несколько раз
vector <string> SymbolTable::check_multi_declaration(vector <string> identificators)
{
	//Проверяем, ны была ли занесена одна и та же переменная повторно
	for(int i = 0; i < identificators.size(); ++i) {
		for(auto j = 0; j < identificators.size(); ++j) {
			if(i != j && !identificators[i].empty()) {
				if(identificators[i] == identificators[j]) {
					identificators[j].clear();
				}
			}
		}
	}

	//Проверяем, не была ли занесена переменная в другую область видимости
	for(auto it = sym_table.begin(); it != sym_table.end(); ++it) {
		for(int i = 0; i < (it->second).size(); ++i) {
			for(int j = 0; j < identificators.size(); ++j) {
				if((it->second)[i] == identificators[j]) {
					identificators[j].clear();
				}
			}
		}
	}

	//Поскольку из вектора "" были удаелны повторяющиеся идентификаторы, в нём остались пустые ячейки
	//Создаём новый вектор, в котором не будет пустых ячеек
	vector <string> new_vec;

	for(int i = 0; i < identificators.size(); ++i) {
		if(!identificators[i].empty()) {
			new_vec.push_back(identificators[i]);
		}
	}

	return new_vec;
}
