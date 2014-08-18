#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

struct Foo{
	Foo () { std::cout<<"Foo()" << std::endl; }
	int val;
	
};
void testStl () {

	std::vector<Foo> vf;
	Foo f1;
	vf.push_back(f1);
}

int main () {
	testStl ();
	return 0;
}
