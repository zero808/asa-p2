all:
	g++ -O0 -ggdb -ansi -Wall main.cpp
#	g++ -O3 -ansi -Wall main.cpp -lm
#
clean:
	rm -f a.out
