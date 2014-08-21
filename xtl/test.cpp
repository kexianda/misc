#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "xmemory.h"

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
	
	void testMem (){
		vector <Foo> vi;
		for (int i=0; i<3; ++i){
			vi.push_back (Foo());
		}
		auto itr = std::begin (vi);	
		xtl::destory (itr, itr+2);

		Foo arr[2];
		Foo * p = arr;
		xtl::construct (p);
		xtl::construct (p+1);
		xtl::destory (p, p+2);//[, )
	}
}

int main () {
	
	xtl_memeory_test::testMem ();

	return 0;
}
