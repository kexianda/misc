#include<iostream>
using namespace std;
const char*  funName()
{
	return __func__;
}
int main()
{
	cout<<funName()<<endl;
}
