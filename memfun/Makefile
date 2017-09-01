
BENCH_FLAG = -D_RUN_BENCHMARK_
TEST_FLAG = -D_RUN_BENCHMARK_

memfun : driver.o memcpy.o
	gcc -O2 -std=c++11 memcpy.o driver.o -lstdc++ -o memcpy 
#	gcc -g -Wall -std=c++11 memcpy.o driver.o -lstdc++ -o memcpy 

driver.o : driver.cpp
#	gcc -g -c -std=c++11 driver.cpp
	gcc -O2 -c -std=c++11 $(TEST_FLAG) $(BENCH_FLAG) driver.cpp

memcpy.o : memcpy.S
#	yasm -f elf64 -g dwarf2 memcpy.S -o memcpy.o
	yasm -f elf64 memcpy.S -o memcpy.o

.PHONY : clean
clean:
	rm *.o


# gcc -std=c++11 memcpy.o driver.o -static-libgcc -static-libstdc++ -o memcpy 