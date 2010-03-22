CFLAGS=-Wall -O0 -ggdb3 -DDEBUG=1
LIBFILES=quickhash.c iterator.c

all: test-hash-add test-hash-delete test-set-delete-extensive test-hash-get \
	test-hash-save-to-file2 test-hash-save-to-file test-hash-set test-hash-update \
	test-normalize-size test-set-add test-set-load-from-file test-set-save-to-file \
	test-hash-string-add test-hash-string-update test-hash-string-delete \
	test-hash-string-set test-hash-string-save-to-file \
	test-hash-string-save-to-file2

test-hash-add:
	gcc $(CFLAGS) -o test-hash-add-64 test-hash-add.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-add-32 test-hash-add.c $(LIBFILES) 

test-hash-delete:
	gcc $(CFLAGS) -o test-hash-delete-64 test-hash-delete.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-delete-32 test-hash-delete.c $(LIBFILES) 

test-set-delete-extensive:
	gcc $(CFLAGS) -o test-set-delete-extensive-64 test-set-delete-extensive.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-set-delete-extensive-32 test-set-delete-extensive.c $(LIBFILES) 

test-hash-get:
	gcc $(CFLAGS) -o test-hash-get-64 test-hash-get.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-get-32 test-hash-get.c $(LIBFILES) 

test-hash-save-to-file2:
	gcc $(CFLAGS) -o test-hash-save-to-file2-64 test-hash-save-to-file2.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-save-to-file2-32 test-hash-save-to-file2.c $(LIBFILES) 

test-hash-save-to-file:
	gcc $(CFLAGS) -o test-hash-save-to-file-64 test-hash-save-to-file.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-save-to-file-32 test-hash-save-to-file.c $(LIBFILES) 

test-hash-set:
	gcc $(CFLAGS) -o test-hash-set-64 test-hash-set.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-set-32 test-hash-set.c $(LIBFILES) 

test-hash-update:
	gcc $(CFLAGS) -o test-hash-update-64 test-hash-update.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-update-32 test-hash-update.c $(LIBFILES) 

test-normalize-size:
	gcc $(CFLAGS) -o test-normalize-size-64 test-normalize-size.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-normalize-size-32 test-normalize-size.c $(LIBFILES) 

test-set-add:
	gcc $(CFLAGS) -o test-set-add-64 test-set-add.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-set-add-32 test-set-add.c $(LIBFILES) 

test-set-load-from-file:
	gcc $(CFLAGS) -o test-set-load-from-file-64 test-set-load-from-file.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-set-load-from-file-32 test-set-load-from-file.c $(LIBFILES) 

test-set-save-to-file:
	gcc $(CFLAGS) -o test-set-save-to-file-64 test-set-save-to-file.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-set-save-to-file-32 test-set-save-to-file.c $(LIBFILES) 

test-hash-string-add:
	gcc $(CFLAGS) -o test-hash-string-add-64 test-hash-string-add.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-string-add-32 test-hash-string-add.c $(LIBFILES) 

test-hash-string-update:
	gcc $(CFLAGS) -o test-hash-string-update-64 test-hash-string-update.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-string-update-32 test-hash-string-update.c $(LIBFILES) 

test-hash-string-delete:
	gcc $(CFLAGS) -o test-hash-string-delete-64 test-hash-string-delete.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-string-delete-32 test-hash-string-delete.c $(LIBFILES) 

test-hash-string-set:
	gcc $(CFLAGS) -o test-hash-string-set-64 test-hash-string-set.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-string-set-32 test-hash-string-set.c $(LIBFILES) 

test-hash-string-save-to-file:
	gcc $(CFLAGS) -o test-hash-string-save-to-file-64 test-hash-string-save-to-file.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-string-save-to-file-32 test-hash-string-save-to-file.c $(LIBFILES) 

test-hash-string-save-to-file2:
	gcc $(CFLAGS) -o test-hash-string-save-to-file2-64 test-hash-string-save-to-file2.c $(LIBFILES) 
	gcc -m32 $(CFLAGS) -o test-hash-string-save-to-file2-32 test-hash-string-save-to-file2.c $(LIBFILES) 
