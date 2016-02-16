test : gcm.c
	gcc -std=gnu99 -O2 -lssl -lcrypto gcm.c -o test -lrt
.PHONY : clean
clean : 
	-rm *.o test
