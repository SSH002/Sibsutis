#pragma once

using namespace std;

class CodGen
{
	public:
		CodGen();
		void print_asm();
		void handle_begin();
		void handle_end();
		void handle_SC_marks();
		void compile_asm_file();
		void set_fileame(string file_path);
		void start_codgen(AST *tree, string file_path);
		void handle_asm_mov();
		void handle_asm_add();
		void handle_asm_sub();
		void handle_asm_mul();
		void handle_asm_div();
		void handle_asm_inc();
		void handle_asm_dec();
		void handle_asm_cmp();
		void handle_asm_cond_pass_if();
		void handle_asm_cond_pass_loop();
		void handle_asm_jmp();
		void handle_asm_call();
		void handle_asm_cmovl();
};
