memfun : driver.o memcpy.o
#	gcc -std=c++11 memcpy.o driver.o -static-libgcc -static-libstdc++ -o memcpy 
	gcc -std=c++11 memcpy.o driver.o -o memfun.out 

driver.o : driver.cpp
	gcc -g -c -std=c++11 driver.cpp

memcpy.o : memcpy.S
	yasm -f elf64 -g dwarf2 memcpy.S -o memcpy.o

.PHONY : clean
clean:
	rm *.o
