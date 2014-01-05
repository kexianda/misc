/*
 * Author: Ke, Xianda
 * Date: 2014-01-05
 * */
#include<iostream>
#include<stdio.h>

#define LOG(...){\
	fprintf(stderr, "%s: Line %d\t", __FILE__, __LINE__); \
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr,"\n");\
}

using namespace std;
const char*  funName()
{
	int x = 3;
	LOG("x = %d", x);

	return __func__;
}
int main()
{
	#if __cplusplus < 201103L
		#error "should use c++11 implementation. please use -std=c++11 option"
   	#endif
	cout<<funName()<<endl;
}
