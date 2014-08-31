#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "xtl_memory.h"

#ifdef __linux__
#include <mcheck.h>
#endif //__linux__

#ifdef _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif //Windows CRT GBD


using namespace std;

struct Foo{
	Foo () {
	   val = 5;
   	   std::cout<<"Foo()" << std::endl; 
	}
	
	int val;
	~Foo () {val = 0; std::cout<<"~Foo()"<<endl;}	
};

namespace xtl_memeory_test {
	
	
	void test_xtl_Memory (){
		
		xtl::allocator<Foo> fooAlloc;
			
		Foo* buff = fooAlloc.allocate (10); 
		for (Foo* itr = buff; itr < buff+5; ++itr) {
			xtl::construct (itr);
		}

		Foo f;
		f.val = 888;
		for (Foo* itr = buff+5; itr < buff+10; ++itr) {
			xtl::copy_construct (itr, f); 
		}		

		xtl::destory(buff, buff+10); //[,)

		fooAlloc.deallocate (buff);
	}
}

int main () {
	
	#ifdef __linux__
	//$./test.out
	//$mtrace test.out memcheck.o > report.o
	setenv ("MALLOC_TRACE", "memcheck.o", 1);
	mtrace();
	#endif //__linux__

	
	xtl_memeory_test::test_xtl_Memory();


	#ifdef __linux__
	//muntrace();
	#endif //__linux__

	#ifdef _CRTDBG_MAP_ALLOC
	_CrtDumpMemoryLeaks();
	#endif //Windows CRT GBD	return 0;
}
