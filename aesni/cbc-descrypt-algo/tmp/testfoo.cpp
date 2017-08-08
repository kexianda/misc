#include <iostream>
extern "C" int foo(void);
using namespace std;
int main() {
	std::cout<<foo()<<endl;
	return 0;
}
