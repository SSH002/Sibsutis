#include "lexer.hpp"

//Папка, в которой лежат файлы, в которых описаны стандартные лексемы языка
string path_to_file = "res/";
//Имена файлов в папке "res"(path_to_file)
string file_name[] = {"key_words", "std_funcs", "parens", "spec_symbols", 
			"comp_ops", "logic_ops", "arith_ops"};
//Список допустимых символов
string terminals = "$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_1234567890";
//Список недопустимых символов
string unaccept_symbols = "@^\\№АаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя";
string path;

//Считывание лексем из файлов
Lexer::Lexer()
{ 
	string str;

	for(int i = 0; i < files_count; ++i) {
		ifstream file(path_to_file + file_name[i]);
		
		int j = 0;
		while(getline(file, str)) {
			std_lexemes[i].push_back(str);
			++j;
		}
		
		file.close();
		str.clear();
	}
}

//Разделяем пробелами с обеих сторон вокруг лексем, указанных в "shrinked_lexemes"
void Lexer::input_spaces()
{
	//Флаг, есть ли строковая лексема в обрабатываемой строке входного файла
	bool is_str = false;
	next_file_string = ' ' + next_file_string;

	for(int i  = 0; i < next_file_string.length(); i++) {
		//В строковых лексемах пробелы не расставляем
		if(next_file_string[i] == '\"') {
			is_str =((is_str) ? false : true);
		}

		//Не отделять, если идёт комментарий
		if((next_file_string[i] == '/') && (next_file_string[i + 1] == '/')) {
			break;
		}
		//Не отделять, если идёт комментарий
		if((next_file_string[i] == '#')) {
			break;
		}

		for(int j  = 0; j < shrinked_lexemes.size(); j++) {
			//Вставляем пробелы, если:
			//1) Лексема соответствует одной из лексем в векторе "shrinked_lexemes"
			//2) Лексема не является строкой
			//3) Перед лексемой отсутствует пробел
			if((next_file_string.substr(i, shrinked_lexemes[j].length()) == shrinked_lexemes[j]) && !is_str && next_file_string[i - 1] != ' ') {
				next_file_string.replace(i, shrinked_lexemes[j].length(), ' ' + shrinked_lexemes[j] + ' ');
				i += shrinked_lexemes[j].length();
				break;
			}
			if((next_file_string.substr(i, shrinked_lexemes[j].length()) == shrinked_lexemes[j]) && !is_str && next_file_string[i - 1] == ' ') {
				next_file_string.replace(i, shrinked_lexemes[j].length(), shrinked_lexemes[j] + ' ');
				i += shrinked_lexemes[j].length();
				break;
			}
		}
	}
}

//Удаление однострочных комментариев
void Lexer::del_comments() 
{ 
	string lexeme;
	//Ищем начало комментария
	int start_pos = next_file_string.find("//"); 

	//Если обнаружен комментарий
	if(start_pos != string::npos) { 
		lexeme = next_file_string.substr(start_pos);

		//Строку с комментарием заменяем пробелами
		file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
		next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
	} else {
		//Ищем начало комментария
		start_pos = next_file_string.find("#"); 

		//Если обнаружен комментарий
		if(start_pos != string::npos) { 
			lexeme = next_file_string.substr(start_pos);

			//Строку с комментарием заменяем пробелами
			file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
		}
	}
}

//Вырезаем лексему из строки (определяем по пробелам)
string Lexer::shrink_lex_from_string(int *start_pos) 
{
	//Ищем по пробелу конец лексемы
	int end_pos = next_file_string.find(" ", *start_pos + 1); 
	//Считываем лексему
	string lexeme = next_file_string.substr(*start_pos + 1,(end_pos - *start_pos - 1)); 
	//Ищем по пробелу начало следующей лексемы
	*start_pos = next_file_string.find(" ", end_pos); 	

	return lexeme;
}

//Поиск строковых лексем
//Через указатели возвращаем лексему и её позиццию,
//а сам метод возвращает описание токена
string Lexer::search_string_lex(string *lexeme, int *lexeme_pos, char symbol) 
{ 
	int start_pos = next_file_string.find(symbol);
	int end_pos; 

	//Проверка, была ли найдена лексема
	if(start_pos != string::npos) { 
		end_pos = next_file_string.find(symbol, start_pos + 1);
		//Если отсутствует вторая кавычка
		if(end_pos == string::npos) {	
			*lexeme = next_file_string.substr(start_pos);
			*lexeme_pos = file_string.find(*lexeme) + 1;

			file_string.replace(file_string.find(*lexeme),(*lexeme).length(),(*lexeme).length(), ' ');
			next_file_string.replace(next_file_string.find(*lexeme),(*lexeme).length(),(*lexeme).length(), ' ');
			return "unknown";
		//Если всё в порядке
		} else { 
			*lexeme = next_file_string.substr(start_pos,(end_pos - start_pos + 1));
			*lexeme_pos = file_string.find(*lexeme) + 1;

			file_string.replace(file_string.find(*lexeme),(*lexeme).length(),(*lexeme).length(), ' ');
			next_file_string.replace(next_file_string.find(*lexeme),(*lexeme).length(),(*lexeme).length(), ' ');
			return "string_literal";
		}
	}

	return NOLEXEME;
}

//Поиск стандартных лексем языка, перечисленных в файлах
string Lexer::search_std_lex(string lexeme, int *lexeme_pos) 
{ 
	for(int i = 0; i < files_count; ++i) { 
		for(auto it = std_lexemes[i].begin(); it != std_lexemes[i].end(); ++it) { 
			int pos = lexeme.find(*it);

			//Если лексема найдена
			if(pos != string::npos) { 
				*lexeme_pos = file_string.find(*it) + 1;	
				string token_descr = check_std_lex(lexeme);

				file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
				next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
				return token_descr;
			}
		}
	}
	return NOLEXEME;
}

//Поиск числовых лексем(числовые константы)
string Lexer::search_digit_lex(string lexeme, int *lexeme_pos) 
{ 
	int start_pos = lexeme.find_first_of(terminals); 

	if(start_pos != string::npos) { 
		if(regex_search(lexeme.begin(), lexeme.end(), regex_hex)) { 	
			*lexeme_pos = file_string.find(lexeme) + 1;				
			file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			return "digit_literal_hex";
		} else if(regex_search(lexeme.begin(), lexeme.end(), regex_bin)) { 
			*lexeme_pos = file_string.find(lexeme) + 1;				
			file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			return "digit_literal_bin";
		} else if(regex_search(lexeme.begin(), lexeme.end(), regex_oct)) { 
			*lexeme_pos = file_string.find(lexeme) + 1;
			file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			return "digit_literal_oct";
		} else if(regex_search(lexeme.begin(), lexeme.end(), regex_dec) || lexeme == "0") { 
			*lexeme_pos = file_string.find(lexeme) + 1;
			file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			return "digit_literal_dec";
		}
	}

	return NOLEXEME;
}

//Поиск переменных
string Lexer::search_variables(string lexeme, int *lexeme_pos, int start_pos_next_lexeme) 
{ 
	string next_lexeme, next_next_lexeme;
	int start_pos = lexeme.find_first_of(terminals); 

	if(start_pos != string::npos) { 
		//Считываем две следующие лексемы (необходимо для определения массива)
		while(next_lexeme == "") {
			 next_lexeme = shrink_lex_from_string(&start_pos_next_lexeme);
		}
		while(next_next_lexeme == "") {
			next_next_lexeme = shrink_lex_from_string(&start_pos_next_lexeme);
		}

		//Если первый символ лексемы - цифра
		if(isdigit(lexeme[0])) { 
			*lexeme_pos = file_string.find(lexeme) + 1;
			file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			return "unknown";
		//Если лексема - массив

		} else if(next_lexeme == "[" || next_next_lexeme == "array(") {
			*lexeme_pos = file_string.find(lexeme) + 1;
			file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			return "array_name";
		//Если лексема - переменная
		} else if(regex_search(lexeme.begin(), lexeme.end(), regex_var)) {
			*lexeme_pos = file_string.find(lexeme) + 1;
			file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
			return "variable";
		}
	}	

	return NOLEXEME;
}

//Поиск недопустимых символов в строке
string Lexer::search_unaccept_symbols(string lexeme, int *lexeme_pos) 
{
	//Ищем вхождение любого из недопустимых символов
	int start_pos = lexeme.find_first_of(unaccept_symbols); 

	if(start_pos != string::npos) {
		*lexeme_pos = file_string.find(lexeme) + 1;
		file_string.replace(file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
		next_file_string.replace(next_file_string.find(lexeme), lexeme.length(), lexeme.length(), ' ');
		return "unacceptable_symbol";
	}

	return NOLEXEME;
}

//Получаем токен
Token Lexer::get_token() 
{ 
	int lexeme_pos = 0;
	string lexeme;
	string description;
	Token token;

	//Определяем, является ли файл пустым
	if(next_file_string.find_first_not_of(" 	") == string::npos) { 
		ifstream file(path);
		file_pos += file_string.length() + 1;

		file.seekg(file_pos);
		getline(file, file_string);
		next_file_string = file_string;
		
		input_spaces(); 
		start_pos = 0;

		//Определяем, достигли ли конца файла
		if(file.eof() && next_file_string.find_first_not_of(" ") == string::npos) {
			end_of_file = true;
		}

		//Определяем номер строки
		++line_count;	
		file.close();
	}

	lexeme = shrink_lex_from_string(&start_pos); 

	if(lexeme[0] == '/' && lexeme[1] == '/' || lexeme[0] == '#') { 
		del_comments();
		return token;
	}

	if(lexeme[0] == '\"' || lexeme[0] == '\'') {
		description = search_string_lex(&lexeme, &lexeme_pos, lexeme[0]);

		if(description != NOLEXEME) { 
			Token token(description, lexeme, line_count, lexeme_pos);
			return token;
		}
	}

	description = search_std_lex(lexeme, &lexeme_pos);
	if(description != NOLEXEME) {
		Token token(description, lexeme, line_count, lexeme_pos);
		return token;
	}

	description = search_digit_lex(lexeme, &lexeme_pos);
	if(description!= NOLEXEME) {
		Token token(description, lexeme, line_count, lexeme_pos);
		return token;
	}

	description = search_variables(lexeme, &lexeme_pos, start_pos);
	if(description != NOLEXEME) {
		Token token(description, lexeme, line_count, lexeme_pos);
		return token;
	}

	description = search_unaccept_symbols(lexeme, &lexeme_pos);
	if(description != NOLEXEME) {
		Token token(description, lexeme, line_count, lexeme_pos);
		return token;
	}

	if(end_of_file) { 
		token.set_lexeme(" ");
		return token;
	}

   	return token;
}

//Считывание файла с исходным кодом компилируемой программы
void Lexer::set_start_string(string file_path) 
{
	path = file_path;

	ifstream file(path);
	getline(file, file_string);
	next_file_string = file_string;
	input_spaces();

	file.close();
}

//Вывод токена на экран
void Lexer::print(Token token)
{
	cout << "Loc=<" << token.get_line() << ":" << token.get_pos() << ">	";
	cout << token.get_descr() << "\t\t" << token.get_lexeme() << endl;

}

//Вывод описания токенов
string Lexer::check_std_lex(string lexeme)
{
	if(lexeme == "++") {
		return "arithmetic_operator_++";
	}

	if(lexeme == "--") {
		return "arithmetic_operator_--";
	}

	if(lexeme == "=") {
		return "arithmetic_operator_=";
	}

	if(lexeme == "-") {
		return "arithmetic_operator_-";
	}

	if(lexeme == "+") {
		return "arithmetic_operator_+";
	}

	if(lexeme == "*") {
		return "arithmetic_operator_*";
	}

	if(lexeme == "/") {
		return "arithmetic_operator_/";
	}

	if(lexeme == "%") {
		return "arithmetic_operator_%";
	}

	if(lexeme == "==") {
		return "compare_operator_==";
	}

	if(lexeme == "!=") {
		return "compare_operator_!=";
	}

	if(lexeme == ">=") {
		return "compare_operator_>=";
	}

	if(lexeme == "<=") {
		return "compare_operator_<=";
	}

	if(lexeme == ">") {
		return "compare_operator_>";
	}

	if(lexeme == "<") {
		return "compare_operator_<";
	}

	if(lexeme == "break") {
		return "key_word_break";
	}

	if(lexeme == "continue") {
		return "key_word_continue";
	}

	if(lexeme == "if") {
		return "key_word_if";
	}

	if(lexeme == "else") {
		return "key_word_else";
	}

	if(lexeme == "for") {
		return "key_word_for";
	}

	if(lexeme == "true") {
		return "bool_value_true";
	}

	if(lexeme == "false") {
		return "bool_value_false";
	}

	if(lexeme == "!") {
		return "logic_operator_!";
	}

	if(lexeme == "&&") {
		return "logic_operator_&&";
	}

	if(lexeme == "||") {
		return "logic_operator_||";
	}

	if(lexeme == "<?php") {
		return "left_paren_<?php";
	}

	if(lexeme == "<?") {
		return "left_paren_<?";
	}

	if(lexeme == "?>") {
		return "right_paren_?>";
	}

	if(lexeme == "array(") {
		return "left_paren_array(";
	}

	if(lexeme == "(") {
		return "left_paren_(";
	}

	if(lexeme == ")") {
		return "right_paren_)";
	}

	if(lexeme == "[") {
		return "left_paren_[";
	}

	if(lexeme == "]") {
		return "right_paren_]";
	}

	if(lexeme == "{") {
		return "left_paren_{";
	}

	if(lexeme == "}") {
		return "right_paren_}";
	}

	if(lexeme == ";") {
		return "spec_symbols_;";	
	}

	if(lexeme == ":") {
		return "spec_symbols_:";
	}

	if(lexeme == "?") {
		return "spec_symbols_?";
	}

	if(lexeme == ",") {
		return "spec_symbols_,";
	}

	if(lexeme == ".") {
		return "spec_symbols_.";
	}

	if(lexeme == "&") {
		return "spec_symbols_&";
	}

	if(lexeme == "printf") {
		return "std_function_printf";
	}
	
	if(lexeme == "strlen") {
		return "std_function_strlen";
	}

	return NOLEXEME;
}

