.default: all

all: assignment1

clean:
	rm -rf assignment1 *.o *.dSYM

assignment1:
	g++ -Wall -Werror -std=c++14 -O -o $@ $^ Node.cpp NodeList.cpp PathSolver.cpp main.cpp
