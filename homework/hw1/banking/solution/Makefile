shell:=/bin/bash
compile=g++
options=-Wall -std=c++11
build=./build

commands=$(patsubst %.cpp, %.o, $(wildcard commands/*.cpp))

main: $(build)/banking.o $(build)/models/account.o $(build)/models/transaction.o $(commands)
	$(compile) $(options) -o $@ $^ main.cpp

clean:
	rm -rf build
	rm -f main

$(build)/banking.o: banking.cpp skeleton
	$(compile) $(options) -c -o $@ $<

$(build)/models/account.o: models/account.cpp skeleton
	$(compile) $(options) -c -o $@ $<

$(build)/models/transaction.o: models/transaction.cpp skeleton
	$(compile) $(options) -c -o $@ $<

$(build)/commands/%.o: commands/%.cpp skeleton
	$(compile) $(options) -c -o $@ $<

skeleton: force
	mkdir -p build
	mkdir -p build/models
	mkdir -p build/commands

force: ;

.PHONY: clean skeleton;
