test : gcm.c
	gcc -std=gnu99 -O2 gcm.c -o test.out -lrt -lssl -lcrypto
.PHONY : clean
clean : 
	-rm *.o test.out
