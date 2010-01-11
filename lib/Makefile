all: test1 test2

test1: test1.c quickhash.c
	gcc -ggdb3 -o test1-64 test1.c quickhash.c
	gcc -m32 -ggdb3 -o test1-32 test1.c quickhash.c

test2: test2.c quickhash.c
	gcc -ggdb3 -o test2-64 test2.c quickhash.c
	gcc -m32 -ggdb3 -o test2-32 test2.c quickhash.c
