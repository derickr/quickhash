CFLAGS=-Wall -O0 -Winline -Wall -ggdb3 -DDEBUG=1
LIBFILES=quickhash.o iterator.o hash-algorithms.o

TESTFILES=test-hash-add test-hash-delete test-set-delete-extensive test-hash-get \
	test-hash-save-to-file2 test-hash-save-to-file test-hash-set test-hash-update \
	test-normalize-size test-set-add test-set-load-from-file test-set-save-to-file \
	test-hash-string-add test-hash-string-update test-hash-string-delete \
	test-hash-string-set test-hash-string-save-to-file \
	test-hash-load-from-file test-hash-load-from-file2 test-hash-load-from-file3 \
	test-hash-string-save-to-file2 test-string-hash-add test-string-hash-delete \
	test-hash-string-load-from-file test-hash-string-load-from-file2 test-hash-string-load-from-file3 \
	test-string-hash-set test-string-hash-update test-string-hash-save-to-file \
	test-string-hash-save-to-file2

all: $(TESTFILES)

clean:
	rm $(TESTFILES)

quickhash.a: $(LIBFILES)
	ar r quickhash.a $(LIBFILES)

test-hash-add: test-hash-add.c quickhash.a
	gcc $(CFLAGS) -o test-hash-add test-hash-add.c quickhash.a 

test-hash-delete: test-hash-delete.c quickhash.a
	gcc $(CFLAGS) -o test-hash-delete test-hash-delete.c quickhash.a 

test-set-delete-extensive: test-set-delete-extensive.c quickhash.a
	gcc $(CFLAGS) -o test-set-delete-extensive test-set-delete-extensive.c quickhash.a 

test-hash-get: test-hash-get.c quickhash.a
	gcc $(CFLAGS) -o test-hash-get test-hash-get.c quickhash.a 

test-hash-save-to-file2: test-hash-save-to-file2.c quickhash.a
	gcc $(CFLAGS) -o test-hash-save-to-file2 test-hash-save-to-file2.c quickhash.a 

test-hash-save-to-file: test-hash-save-to-file.c quickhash.a
	gcc $(CFLAGS) -o test-hash-save-to-file test-hash-save-to-file.c quickhash.a 

test-hash-set: test-hash-set.c quickhash.a
	gcc $(CFLAGS) -o test-hash-set test-hash-set.c quickhash.a 

test-hash-update: test-hash-update.c quickhash.a
	gcc $(CFLAGS) -o test-hash-update test-hash-update.c quickhash.a 

test-hash-load-from-file: test-hash-load-from-file.c quickhash.a
	gcc $(CFLAGS) -o test-hash-load-from-file test-hash-load-from-file.c quickhash.a 

test-hash-load-from-file2: test-hash-load-from-file2.c quickhash.a
	gcc $(CFLAGS) -o test-hash-load-from-file2 test-hash-load-from-file2.c quickhash.a 

test-hash-load-from-file3: test-hash-load-from-file3.c quickhash.a
	gcc $(CFLAGS) -o test-hash-load-from-file3 test-hash-load-from-file3.c quickhash.a 

test-normalize-size: test-normalize-size.c quickhash.a
	gcc $(CFLAGS) -o test-normalize-size test-normalize-size.c quickhash.a 

test-set-add: test-set-add.c quickhash.a
	gcc $(CFLAGS) -o test-set-add test-set-add.c quickhash.a 

test-set-load-from-file: test-set-load-from-file.c quickhash.a
	gcc $(CFLAGS) -o test-set-load-from-file test-set-load-from-file.c quickhash.a 

test-set-save-to-file: test-set-save-to-file.c quickhash.a
	gcc $(CFLAGS) -o test-set-save-to-file test-set-save-to-file.c quickhash.a 

test-hash-string-add: test-hash-string-add.c quickhash.a
	gcc $(CFLAGS) -o test-hash-string-add test-hash-string-add.c quickhash.a 

test-hash-string-update: test-hash-string-update.c quickhash.a
	gcc $(CFLAGS) -o test-hash-string-update test-hash-string-update.c quickhash.a 

test-hash-string-delete: test-hash-string-delete.c quickhash.a
	gcc $(CFLAGS) -o test-hash-string-delete test-hash-string-delete.c quickhash.a 

test-hash-string-set: test-hash-string-set.c quickhash.a
	gcc $(CFLAGS) -o test-hash-string-set test-hash-string-set.c quickhash.a 

test-hash-string-save-to-file: test-hash-string-save-to-file.c quickhash.a
	gcc $(CFLAGS) -o test-hash-string-save-to-file test-hash-string-save-to-file.c quickhash.a 

test-hash-string-save-to-file2: test-hash-string-save-to-file2.c quickhash.a
	gcc $(CFLAGS) -o test-hash-string-save-to-file2 test-hash-string-save-to-file2.c quickhash.a 

test-hash-string-load-from-file: test-hash-string-load-from-file.c quickhash.a
	gcc $(CFLAGS) -o test-hash-string-load-from-file test-hash-string-load-from-file.c quickhash.a 

test-hash-string-load-from-file2: test-hash-string-load-from-file2.c quickhash.a
	gcc $(CFLAGS) -o test-hash-string-load-from-file2 test-hash-string-load-from-file2.c quickhash.a 

test-hash-string-load-from-file3: test-hash-string-load-from-file3.c quickhash.a
	gcc $(CFLAGS) -o test-hash-string-load-from-file3 test-hash-string-load-from-file3.c quickhash.a 

test-string-hash-add: test-string-hash-add.c quickhash.a
	gcc $(CFLAGS) -o test-string-hash-add test-string-hash-add.c quickhash.a 

test-string-hash-delete: test-string-hash-delete.c quickhash.a
	gcc $(CFLAGS) -o test-string-hash-delete test-string-hash-delete.c quickhash.a 

test-string-hash-set: test-string-hash-set.c quickhash.a
	gcc $(CFLAGS) -o test-string-hash-set test-string-hash-set.c quickhash.a 

test-string-hash-update: test-string-hash-update.c quickhash.a
	gcc $(CFLAGS) -o test-string-hash-update test-string-hash-update.c quickhash.a 

test-string-hash-save-to-file: test-string-hash-save-to-file.c quickhash.a
	gcc $(CFLAGS) -o test-string-hash-save-to-file test-string-hash-save-to-file.c quickhash.a 

test-string-hash-save-to-file2: test-string-hash-save-to-file2.c quickhash.a
	gcc $(CFLAGS) -o test-string-hash-save-to-file2 test-string-hash-save-to-file2.c quickhash.a 
