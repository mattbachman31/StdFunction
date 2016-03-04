#Makefile

function:	function.o
	g++ -std=c++11 -g -Wall -Wextra -pedantic test2.cpp -o function

function.o:
	g++ -std=c++11 -c Function.hpp -o function.o

clean:
	rm -f *.o *~ function
