SHELL := /bin/bash
CPP = clang++
FLAGS = -std=c++11 -O2 -g
#LLVMFLAGS = `llvm-config-3.8 --cxxflags --ldflags --libs  --system-libs`
LLVMFLAGS = `llvm-config --cxxflags --ldflags --libs core --system-libs`
simple: lexer.o parser.o ast.o symbol.o general.o error.o sem.o llvm.o
	$(CPP) -$(FLAGS) -o $@ $^ -lfl  $(LLVMFLAGS)

lexer: lexer.cpp
	$(CPP) $< -lfl -o lexer

lexer.cpp: danalexer.l ast.h ast.cpp
	flex -s -o lexer.cpp danalexer.l

parser.hpp parser.cpp: parser.y ast.h ast.cpp
	bison -dv -o parser.cpp parser.y

lexer.o: lexer.cpp parser.hpp

llvm.o: llvm.cpp llvm.h
	$(CPP) llvm.cpp -c -$(FLAGS)  `llvm-config --cxxflags` 


parser.o: parser.cpp parser.hpp symbol.c symbol.h
	$(CPP) $(FLAGS) -c $<
%.o: %.cpp
	$(CPP) $(FLAGS) -c $<

clean :
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *~

distclean: clean
	$(RM) simple
