comp := comp
lexer := bin/lexer_test
parser := bin/parser_test
symtab := bin/symtab_test
sema := bin/sema_test
G++ := g++ -std=c++11  -c

.PHONY: all clean dir gtest

all: $(comp) $(lexer) $(parser) $(symtab) $(sema)

$(comp): build/compiler/main.o build/compiler/lexer.o build/compiler/token.o build/compiler/handler.o build/compiler/parse_array.o
$(comp): build/compiler/parse_for.o build/compiler/parse_if.o build/compiler/parse_ops.o build/compiler/parse_simple_nodes.o
$(comp): build/compiler/parse_other_nodes.o build/compiler/parse_php_beginning.o build/compiler/check_error.o
$(comp): build/compiler/ast.o build/compiler/print_ast.o build/compiler/sym_tab.o build/compiler/checksym_tab_errors.o
$(comp): build/compiler/sema.o build/compiler/check_sema_errors.o build/compiler/codgen.o build/compiler/asm_handler.o
	
	g++ -std=c++11 build/compiler/main.o build/compiler/lexer.o build/compiler/token.o build/compiler/handler.o build/compiler/parse_array.o build/compiler/parse_for.o build/compiler/parse_if.o build/compiler/parse_ops.o build/compiler/parse_simple_nodes.o build/compiler/parse_other_nodes.o build/compiler/parse_php_beginning.o build/compiler/check_error.o build/compiler/ast.o build/compiler/print_ast.o build/compiler/sym_tab.o build/compiler/checksym_tab_errors.o build/compiler/sema.o build/compiler/check_sema_errors.o build/compiler/codgen.o build/compiler/asm_handler.o -o $(comp)

build/compiler/main.o: src/main.cpp
	$(G++) src/main.cpp -o build/compiler/main.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen

build/compiler/lexer.o: src/lexer/lexer.cpp
	$(G++) src/lexer/lexer.cpp -o build/compiler/lexer.o
build/compiler/token.o: src/lexer/token.cpp
	$(G++) src/lexer/token.cpp -o build/compiler/token.o

build/compiler/handler.o: src/parser/handler.cpp
	$(G++) src/parser/handler.cpp -o build/compiler/handler.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/parse_array.o: src/parser/parse_array.cpp
	$(G++) src/parser/parse_array.cpp -o build/compiler/parse_array.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/parse_for.o: src/parser/parse_for.cpp
	$(G++) src/parser/parse_for.cpp -o build/compiler/parse_for.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/parse_if.o: src/parser/parse_if.cpp
	$(G++) src/parser/parse_if.cpp -o build/compiler/parse_if.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/parse_ops.o: src/parser/parse_ops.cpp
	$(G++) src/parser/parse_ops.cpp -o build/compiler/parse_ops.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/parse_simple_nodes.o: src/parser/parse_simple_nodes.cpp
	$(G++) src/parser/parse_simple_nodes.cpp -o build/compiler/parse_simple_nodes.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/parse_other_nodes.o: src/parser/parse_other_nodes.cpp
	$(G++) src/parser/parse_other_nodes.cpp -o build/compiler/parse_other_nodes.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/parse_php_beginning.o: src/parser/parse_php_beginning.cpp
	$(G++) src/parser/parse_php_beginning.cpp -o build/compiler/parse_php_beginning.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/check_error.o: src/parser/check_error.cpp
	$(G++) src/parser/check_error.cpp -o build/compiler/check_error.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/ast.o: src/parser/ast.cpp
	$(G++) src/parser/ast.cpp -o build/compiler/ast.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/print_ast.o: src/parser/print_ast.cpp
	$(G++) src/parser/print_ast.cpp -o build/compiler/print_ast.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/codgen
build/compiler/sym_tab.o: src/symtab/sym_tab.cpp
	$(G++) src/symtab/sym_tab.cpp -o build/compiler/sym_tab.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/parser -Isrc/codgen
build/compiler/checksym_tab_errors.o: src/symtab/checksym_tab_errors.cpp
	$(G++) src/symtab/checksym_tab_errors.cpp -o build/compiler/checksym_tab_errors.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/parser -Isrc/codgen
build/compiler/sema.o: src/sema/sema.cpp
	$(G++) src/sema/sema.cpp -o build/compiler/sema.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/parser -Isrc/codgen
build/compiler/check_sema_errors.o: src/sema/check_sema_errors.cpp
	$(G++) src/sema/check_sema_errors.cpp -o build/compiler/check_sema_errors.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/parser -Isrc/codgen
build/compiler/codgen.o: src/codgen/codgen.cpp
	$(G++) src/codgen/codgen.cpp -o build/compiler/codgen.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/parser -Isrc/codgen
build/compiler/asm_handler.o: src/codgen/asm_handler.cpp
	$(G++) src/codgen/asm_handler.cpp -o build/compiler/asm_handler.o -Isrc/lexer -Isrc/symtab -Isrc/sema -Isrc/parser -Isrc/codgen

$(lexer): build/test/main.o build/test/lexer.o build/compiler/lexer.o build/compiler/token.o 
	g++ -std=c++11 build/test/main.o build/test/lexer.o build/compiler/lexer.o build/compiler/token.o -lgtest -o $(lexer)

build/test/main.o: test/main.cpp
	$(G++) test/main.cpp -o build/test/main.o 
build/test/lexer.o: test/lexer.cpp
	$(G++) test/lexer.cpp -o build/test/lexer.o -Isrc/lexer

$(parser): build/test/main.o build/compiler/lexer.o build/compiler/token.o build/test/parse_for.o 
$(parser): build/test/parse_if.o build/test/parse_array.o build/test/parse_operations.o 
$(parser): build/test/parse_other_nodes.o build/test/parse_simple_nodes.o build/test/parse_php_beginning.o  
$(parser): build/compiler/handler.o build/compiler/parse_array.o build/compiler/parse_for.o build/compiler/parse_if.o 
$(parser): build/compiler/parse_ops.o build/compiler/parse_simple_nodes.o build/compiler/parse_other_nodes.o 
$(parser): build/compiler/parse_php_beginning.o build/compiler/check_error.o
$(parser): build/compiler/ast.o build/compiler/print_ast.o build/compiler/sym_tab.o build/compiler/checksym_tab_errors.o build/compiler/sema.o build/compiler/check_sema_errors.o 
$(parser): build/compiler/codgen.o build/compiler/asm_handler.o
	g++ -std=c++11 build/test/main.o build/test/parse_php_beginning.o build/test/parse_for.o build/test/parse_if.o build/test/parse_array.o build/test/parse_operations.o build/test/parse_other_nodes.o build/test/parse_simple_nodes.o build/compiler/lexer.o build/compiler/token.o build/compiler/handler.o build/compiler/parse_array.o build/compiler/parse_for.o build/compiler/parse_if.o build/compiler/parse_ops.o build/compiler/parse_simple_nodes.o build/compiler/parse_other_nodes.o build/compiler/parse_php_beginning.o build/compiler/check_error.o build/compiler/ast.o build/compiler/print_ast.o build/compiler/sym_tab.o build/compiler/checksym_tab_errors.o build/compiler/sema.o build/compiler/check_sema_errors.o build/compiler/codgen.o build/compiler/asm_handler.o -lgtest -o $(parser)

build/test/parse_func.o: test/parse_func.cpp
	$(G++) test/parse_func.cpp -o build/test/parse_func.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen
build/test/parse_for.o: test/parse_for.cpp
	$(G++) test/parse_for.cpp -o build/test/parse_for.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen
build/test/parse_if.o: test/parse_if.cpp
	$(G++) test/parse_if.cpp -o build/test/parse_if.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen
build/test/parse_array.o: test/parse_array.cpp
	$(G++) test/parse_array.cpp -o build/test/parse_array.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen
build/test/parse_operations.o: test/parse_operations.cpp 
	$(G++) test/parse_operations.cpp -o build/test/parse_operations.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen
build/test/parse_other_nodes.o: test/parse_other_nodes.cpp
	$(G++) test/parse_other_nodes.cpp -o build/test/parse_other_nodes.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen
build/test/parse_simple_nodes.o: test/parse_simple_nodes.cpp
	$(G++) test/parse_simple_nodes.cpp -o build/test/parse_simple_nodes.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen
build/test/parse_php_beginning.o: test/parse_php_beginning.cpp
	$(G++) test/parse_php_beginning.cpp -o build/test/parse_php_beginning.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen

$(symtab): build/test/main.o build/test/symtab.o build/compiler/checksym_tab_errors.o build/compiler/sym_tab.o
	g++ -std=c++11 build/test/main.o build/test/symtab.o build/compiler/checksym_tab_errors.o build/compiler/sym_tab.o -lgtest -o $(symtab)

build/test/symtab.o: test/symtab.cpp
	$(G++) test/symtab.cpp -o build/test/symtab.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen

$(sema): build/test/main.o build/test/sema.o build/compiler/check_sema_errors.o build/compiler/sema.o
	g++ -std=c++11 build/test/main.o build/test/sema.o build/compiler/check_sema_errors.o build/compiler/sema.o -lgtest -o $(sema)

build/test/sema.o: test/sema.cpp 
	$(G++) test/sema.cpp -o build/test/sema.o -Isrc/lexer -Isrc/parser -Isrc/symtab -Isrc/sema -Isrc/codgen

compiler: bin/comp
	bin/comp

lexer: bin/lexer_tests
	bin/lexer_tests

parser: bin/parser_tests
	bin/parser_tests

symtab: bin/symtab_tests
	bin/symtab_tests

sema: bin/sema_tests
	bin/sema_tests

clean:
	@rm -rf build/test/*.o
	@rm -rf build/compiler/*.o
	@rm -rf build/asm/*.o	
	@rm -rf bin/*

dir:
	@mkdir build
	@mkdir build/test
	@mkdir build/compiler
	@mkdir build/asm
	@mkdir bin

gtest:
	@apt-get install cmake
	@sudo apt isntall libgtest-dev
	@cd /usr/src/gtest/
	@cmake -DBUILD_SHARED_LIBS=ON
	@make
	@cp *.so /usr/lib
