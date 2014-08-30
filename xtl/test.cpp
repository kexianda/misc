#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "xtl_memory.h"

using namespace std;

struct Foo{
	Foo () { std::cout<<"Foo()" << std::endl; }
	int val;
	~Foo () {std::cout<<"~Foo()"<<endl;}	
};

namespace xtl_memeory_test {
	
	void testStl () {
	
		std::vector<Foo> vf;
		Foo f1;
		vf.push_back(f1);
	}
	
	void test_xtl_Memory (){
		vector <Foo> vf;
		vf.reserve (5);
		for (int i=0; i<3; ++i){
			Foo f;
			vf.push_back (f);
		}
		
		auto itr = std::begin (vf);	
		xtl::destory (itr, itr+2);

		Foo arr[2];
		Foo * p = arr;
		xtl::construct (p);
		xtl::construct (p+1);
		xtl::destory (p, p+2);
		
		//nontrival
		int intarr[5];
		for(int i=0; i<5; ++i){
			xtl::construct (intarr+i);
		}
		xtl::copy_construct (intarr, 100);
		xtl::destory(intarr, intarr+4);
	}
}

int main () {
	
	xtl_memeory_test::test_xtl_Memory();

	return 0;
}
