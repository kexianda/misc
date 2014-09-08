
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
#include "vector.h"
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
		
		typedef xtl::allocator<Foo> FooAlloc;
			
		Foo* buff = FooAlloc::allocate (10); 
		for (Foo* itr = buff; itr < buff+5; ++itr) {
			xtl::construct (itr);
		}

		Foo f;
		f.val = 888;
		for (Foo* itr = buff+5; itr < buff+10; ++itr) {
			xtl::copy_construct (itr, f); 
		}		

		xtl::destory(buff, buff+10); //[,)

		FooAlloc::deallocate (buff);
	}
}
namespace xtl_vector_test {
	struct Foo {
		Foo(int v) : val(v) {}
		int val;
	};
	void test() {

		typedef xtl::vector<Foo>	FooVector;
		
		
		// constructors used in the same order as described above:
		// empty vector of ints
		FooVector first;                                
		
		// four ints with value 100
		FooVector second (4, Foo(8));                       
		
		// iterating through second
		//FooVector third (second.begin(), second.end());  
		
		//copy constructor
		//FooVector fourth (third);                       
  		
		for(int i= 0; i<5; ++i){
			//first.push_back(i);
		}
		
		//test iterator, operator*,  operator->
		for (FooVector::iterator itr = first.begin(); itr < first.end(); ++itr ) {
			int tmp1 = (*itr).val;
			int tmp2 = itr->val;
			cout<<(tmp1 + tmp2)<<endl;
		}

		// the iterator constructor can also be used to construct from arrays:
        //int myints[] = {16,2,77,29};
		//FooVector fifth (myints, myints + sizeof(myints) / sizeof(int) );
		//std::cout << "The contents of fifth are:";
 		//for (FooVector::iterator it = fifth.begin(); it != fifth.end(); ++it){
		
		//}
 		
		//move constructor
		
		//initializer list constructor

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

	xtl_vector_test::test();


	#ifdef __linux__
	//muntrace();
	#endif //__linux__

	#if (defined(_WIN32) || defined(_WIN64)) && defined(_DEBUG)
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);  
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT); //_CRT_WARN
	_CrtDumpMemoryLeaks();
	#endif //Windows CRT GBD	return 0;
}
