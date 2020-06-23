compile=g++
options=-Wall -std=c++11
build=./build

test: main
	cat test.txt | $(build)/main

main: arithmetic.o build
	$(compile) $(options) main.cpp $(build)/arithmetic.o -o $(build)/$@

arithmetic.o: build
	$(compile) $(options) arithmetic.cpp -c -o $(build)/$@

build:
	mkdir -p $(build)

clean:
	rm -rf $(build)

.PHONY: all build clean
