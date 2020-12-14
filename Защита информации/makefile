G++ := g++ -O3 -std=c++17 -Ithirdparty -c

.PHONY: default clean dir trash

default: build/main.o build/lab1.o build/lab2.o build/lab3.o build/f-in-out.o build/sha256.o build/lab4.o build/lab5.o
	g++ -O3 -std=c++17 build/main.o build/lab1.o build/lab2.o build/f-in-out.o build/lab3.o build/sha256.o build/lab4.o build/lab5.o -o main

build/main.o: main.cpp cryptography.hpp
	$(G++) main.cpp -o build/main.o

build/lab1.o: lab1.cpp
	$(G++) lab1.cpp -o build/lab1.o

build/lab2.o: lab2.cpp
	$(G++) lab2.cpp -o build/lab2.o

build/f-in-out.o: f-in-out.cpp
	$(G++) f-in-out.cpp -o build/f-in-out.o

build/lab3.o: lab3.cpp
	$(G++) lab3.cpp -o build/lab3.o

build/sha256.o: thirdparty/sha256.cpp thirdparty/sha256.hpp 
	$(G++) thirdparty/sha256.cpp -o build/sha256.o

build/lab4.o: lab4.cpp
	$(G++) lab4.cpp -o build/lab4.o

build/lab5.o: lab5.cpp
	$(G++) lab5.cpp -o build/lab5.o

clean:
	@rm -rf *.o
	@rm -rf main
	trash:

dir:
	@mkdir build keys

trash:
	@rm res/*.enc
	@rm res/*.dec
	@rm keys/*
