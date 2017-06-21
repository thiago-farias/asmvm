all: asmvm_out

CPPFLAGS=-std=gnu++11

asmvm_out: asmvm.o op.o lexer.o parser.o main.o parser_aid.o
	g++ $(CPPFLAGS) *.o -o asmvm_out

main.o: parser_aid.h parser.cpp main.cpp asmvm.h
	g++ $(CPPFLAGS) -c main.cpp

parser_aid.o: parser_aid.h asmvm.h
	g++ $(CPPFLAGS) -c parser_aid.cpp

lexer.o: lexer.cpp asmvm.h
	g++ $(CPPFLAGS) -c lexer.cpp
	
parser.o: parser.cpp parser_aid.h asmvm.h
	g++ $(CPPFLAGS) -c parser.cpp
	
op.o: op.cpp params.h op.h asmvm.h
	g++ $(CPPFLAGS) -c op.cpp

asmvm.o: asmvm.cpp asmvm.h
	g++ $(CPPFLAGS) -c asmvm.cpp

lexer.cpp: asmvm.l parser.cpp
	flex -olexer.cpp asmvm.l

parser.cpp: asmvm.y
	bison -v -d -o parser.cpp asmvm.y
	

clean: 
	rm -f *.o
	rm -f lexer.cpp
	rm -f parser.*
	rm -f asmvm_out

install: asmvm_out
	cp asmvm_out /usr/local/bin/asmvm
	

