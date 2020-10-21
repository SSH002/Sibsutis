G++ := g++ -std=c++14 -c 
Bin := main

.PHONY: default clean dir trash

default: build/main.o build/lab1.o build/lab2.o build/f-in-out.o
	g++ -O3 -std=c++14 build/main.o build/lab1.o build/lab2.o build/f-in-out.o -o $(Bin) && ./$(Bin)

build/main.o: main.cpp
	g++ -O3 -std=c++14 -c main.cpp -o build/main.o

build/lab1.o: lab1.cpp
	g++ -O3 -std=c++14 -c lab1.cpp -o build/lab1.o

build/lab2.o: lab2.cpp
	g++ -O3 -std=c++14 -c lab2.cpp -o build/lab2.o

build/f-in-out.o: f-in-out.cpp
	g++ -O3 -std=c++14 -c f-in-out.cpp -o build/f-in-out.o

clean:
	@rm -rf build/*.o
	@rm -rf $(Bin)

dir:
	@mkdir build

trash:
	@rm res/*.enc
	@rm res/*.dec
