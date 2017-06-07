all: asmvm

asmvm: asmvm.o op.o asmvm_lex.o asmvm_parser.o
	g++ -std=gnu++11 -g *.o -o asmvm

asmvm_lex.o: asmvm_lex.cpp
	g++ -std=gnu++11 -g -c asmvm_lex.cpp
	
asmvm_parser.o: asmvm_parser.cpp
	g++ -std=gnu++11 -g -c asmvm_parser.cpp
	
op.o: op.cpp params.h op.h
	g++ -std=gnu++11 -g -c op.cpp

asmvm.o: asmvm.cpp asmvm.h
	g++ -std=gnu++11 -g -c asmvm.cpp

asmvm_lex.cpp: asmvm.l asmvm_parser.cpp
	flex -oasmvm_lex.cpp gen.l

asmvm_parser.cpp: asmvm.y
	bison -v -d -o asmvm_parser.cpp asmvm.y
	

clean: 
	rm -f *.o
	rm -f asmvm_lex.cpp
	rm -f asmvm_parser.*
	rm -f asmvm

install: asmvm
	cp asmvm /usr/local/bin
	

