all: gen

gen: gen_framework.o gen_lex.o gen_parser.o
	g++ -std=gnu++11 -g *.o -o gen

gen_lex.o: gen_lex.cpp
	g++ -std=gnu++11 -g -c gen_lex.cpp
	
gen_parser.o: gen_parser.cpp
	g++ -std=gnu++11 -g -c gen_parser.cpp
	
gen_framework.o: gen_framework.cpp gen_framework.h
	g++ -std=gnu++11 -g -c gen_framework.cpp

gen_lex.cpp: gen.l gen_parser.cpp
	flex -ogen_lex.cpp gen.l

gen_parser.cpp: gen.y
	bison -d -o gen_parser.cpp gen.y
	

clean: 
	rm -f *.o
	rm -f gen_lex.cpp
	rm -f gen_parser.*
	rm -f gen

install: gen
	cp gen /usr/local/bin
	

