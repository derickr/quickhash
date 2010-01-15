CFLAGS=-Wall -O3 -ggdb3 -DDEBUG=1

all: test1 test2 test3 test4

test1: test1.c quickhash.c
	gcc $(CFLAGS) -o test1-64 test1.c quickhash.c
	gcc -m32 $(CFLAGS) -o test1-32 test1.c quickhash.c

test2: test2.c quickhash.c
	gcc $(CFLAGS) -o test2-64 test2.c quickhash.c
	gcc -m32 $(CFLAGS) -o test2-32 test2.c quickhash.c

test3: test3.c quickhash.c
	gcc $(CFLAGS) -o test3-64 test3.c quickhash.c
	gcc -m32 $(CFLAGS) -o test3-32 test3.c quickhash.c

test4: test4.c quickhash.c
	gcc $(CFLAGS) -o test4-64 test4.c quickhash.c
	gcc -m32 $(CFLAGS) -o test4-32 test4.c quickhash.c
