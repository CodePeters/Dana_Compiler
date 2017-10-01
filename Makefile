SHELL := /bin/bash
CPP = clang++
FLAGS = -std=c++11 -g -O2 -Qunused-arguments -Wno-unknown-warning-option -Wall
LLVMFLAGS = `/usr/local/Cellar/llvm@3.8/3.8.1/bin/llvm-config-3.8 --cxxflags --ldflags --libs all --system-libs`

simple: lexer.o parser.o ast.o symbol.o general.o error.o sem.o llvm.o opt.o
	$(CPP) -$(FLAGS) -o $@ $^ -lfl  $(LLVMFLAGS)

lexer: lexer.cpp
	$(CPP) $< -lfl -o lexer

lexer.cpp: danalexer.l ast.h ast.cpp
	flex -s -o lexer.cpp danalexer.l

parser.hpp parser.cpp: parser.y ast.h ast.cpp
	bison -dv -o parser.cpp parser.y

lexer.o: lexer.cpp parser.hpp

llvm.o: llvm.cpp llvm.h
	$(CPP) llvm.cpp -c -$(FLAGS)  `/usr/local/Cellar/llvm@3.8/3.8.1/bin/llvm-config-3.8  --cxxflags `

opt.o: opt.cpp opt.h
	$(CPP) opt.cpp -c -$(FLAGS)  $(LLVMFLAGS)


parser.o: parser.cpp parser.hpp symbol.c symbol.h
	$(CPP) $(FLAGS) -c $<
%.o: %.cpp
	$(CPP) $(FLAGS) -c $<

clean :
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *~

distclean: clean
	$(RM) simple
