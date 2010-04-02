CFLAGS=-Wall -O0 -Winline -Wall -ggdb3 -DDEBUG=1
LIBFILES=quickhash.c iterator.c hash-algorithms.c

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

test-hash-add: test-hash-add.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-add test-hash-add.c $(LIBFILES) 

test-hash-delete: test-hash-delete.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-delete test-hash-delete.c $(LIBFILES) 

test-set-delete-extensive: test-set-delete-extensive.c $(LIBFILES)
	gcc $(CFLAGS) -o test-set-delete-extensive test-set-delete-extensive.c $(LIBFILES) 

test-hash-get: test-hash-get.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-get test-hash-get.c $(LIBFILES) 

test-hash-save-to-file2: test-hash-save-to-file2.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-save-to-file2 test-hash-save-to-file2.c $(LIBFILES) 

test-hash-save-to-file: test-hash-save-to-file.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-save-to-file test-hash-save-to-file.c $(LIBFILES) 

test-hash-set: test-hash-set.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-set test-hash-set.c $(LIBFILES) 

test-hash-update: test-hash-update.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-update test-hash-update.c $(LIBFILES) 

test-hash-load-from-file: test-hash-load-from-file.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-load-from-file test-hash-load-from-file.c $(LIBFILES) 

test-hash-load-from-file2: test-hash-load-from-file2.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-load-from-file2 test-hash-load-from-file2.c $(LIBFILES) 

test-hash-load-from-file3: test-hash-load-from-file3.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-load-from-file3 test-hash-load-from-file3.c $(LIBFILES) 

test-normalize-size: test-normalize-size.c $(LIBFILES)
	gcc $(CFLAGS) -o test-normalize-size test-normalize-size.c $(LIBFILES) 

test-set-add: test-set-add.c $(LIBFILES)
	gcc $(CFLAGS) -o test-set-add test-set-add.c $(LIBFILES) 

test-set-load-from-file: test-set-load-from-file.c $(LIBFILES)
	gcc $(CFLAGS) -o test-set-load-from-file test-set-load-from-file.c $(LIBFILES) 

test-set-save-to-file: test-set-save-to-file.c $(LIBFILES)
	gcc $(CFLAGS) -o test-set-save-to-file test-set-save-to-file.c $(LIBFILES) 

test-hash-string-add: test-hash-string-add.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-string-add test-hash-string-add.c $(LIBFILES) 

test-hash-string-update: test-hash-string-update.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-string-update test-hash-string-update.c $(LIBFILES) 

test-hash-string-delete: test-hash-string-delete.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-string-delete test-hash-string-delete.c $(LIBFILES) 

test-hash-string-set: test-hash-string-set.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-string-set test-hash-string-set.c $(LIBFILES) 

test-hash-string-save-to-file: test-hash-string-save-to-file.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-string-save-to-file test-hash-string-save-to-file.c $(LIBFILES) 

test-hash-string-save-to-file2: test-hash-string-save-to-file2.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-string-save-to-file2 test-hash-string-save-to-file2.c $(LIBFILES) 

test-hash-string-load-from-file: test-hash-string-load-from-file.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-string-load-from-file test-hash-string-load-from-file.c $(LIBFILES) 

test-hash-string-load-from-file2: test-hash-string-load-from-file2.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-string-load-from-file2 test-hash-string-load-from-file2.c $(LIBFILES) 

test-hash-string-load-from-file3: test-hash-string-load-from-file3.c $(LIBFILES)
	gcc $(CFLAGS) -o test-hash-string-load-from-file3 test-hash-string-load-from-file3.c $(LIBFILES) 

test-string-hash-add: test-string-hash-add.c $(LIBFILES)
	gcc $(CFLAGS) -o test-string-hash-add test-string-hash-add.c $(LIBFILES) 

test-string-hash-delete: test-string-hash-delete.c $(LIBFILES)
	gcc $(CFLAGS) -o test-string-hash-delete test-string-hash-delete.c $(LIBFILES) 

test-string-hash-set: test-string-hash-set.c $(LIBFILES)
	gcc $(CFLAGS) -o test-string-hash-set test-string-hash-set.c $(LIBFILES) 

test-string-hash-update: test-string-hash-update.c $(LIBFILES)
	gcc $(CFLAGS) -o test-string-hash-update test-string-hash-update.c $(LIBFILES) 

test-string-hash-save-to-file: test-string-hash-save-to-file.c $(LIBFILES)
	gcc $(CFLAGS) -o test-string-hash-save-to-file test-string-hash-save-to-file.c $(LIBFILES) 

test-string-hash-save-to-file2: test-string-hash-save-to-file2.c $(LIBFILES)
	gcc $(CFLAGS) -o test-string-hash-save-to-file2 test-string-hash-save-to-file2.c $(LIBFILES) 
