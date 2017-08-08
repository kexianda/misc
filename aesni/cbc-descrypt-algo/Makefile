test : test.o cbc.o cbc_noparallel.o cbc_old.o
	gcc -std=gnu99 -o test test.o cbc.o  cbc_old.o cbc_noparallel.o -lrt

cbc.o : cbc.s
	yasm -f elf64 cbc.s -o cbc.o

cbc_old.o : cbc_old.s
	yasm -f elf64 cbc_old.s -o cbc_old.o

cbc_noparallel.o : cbc_noparallel.s
	yasm -f elf64 cbc_noparallel.s -o cbc_noparallel.o

test.o : test.c
	gcc -c -g test.c

clean:
	rm test *.o
