compile=g++
options=-Wall -std=c++14

all:
	$(compile) $(options) -o main main.cpp
	./main

bytecode:
	$(compile) $(options) -o main -masm=intel --save-temps main.cpp
