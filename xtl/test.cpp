
#if defined(_WIN32) || defined(_WIN64) 
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif //Windows CRT GBD

#ifdef __linux__
#include <cstdlib>
#include <mcheck.h>
#endif //__linux__

#include <cstring>
#include <cstdio>
#include <iostream>
#include <vector>
#include "xtl_memory.h"

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

	#if (defined(_WIN32) || defined(_WIN64)) && defined(_DEBUG)
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);  
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT); //_CRT_WARN
	_CrtDumpMemoryLeaks();
	#endif //Windows CRT GBD	return 0;
}
