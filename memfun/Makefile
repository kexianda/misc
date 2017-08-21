memcpy : driver.o memcpy.o
	gcc -std=c++11 -o memcpy memcpy.o driver.o -lrt

driver.o : driver.cpp
	gcc -g -c -std=c++11 driver.cpp

memcpy.o : memcpy.S
	yasm -f elf64 -g dwarf2 memcpy.S -o memcpy.o


#play : asm_playground.o playground.o
#	gcc -std=gnu99 -o play asm_playground.o playground.o -lrt

#asm_playground.o : asm_playground.S
#	yasm -f elf64 asm_playground.S -o asm_playground.o

#playground.o : playground.c
#	gcc -g -c -std=gnu99 playground.c

.PHONY : clean
clean:
	rm *.o
