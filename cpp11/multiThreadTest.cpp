#include <thread>
#include <atomic>
#include <iostream>

#include <unistd.h> //posix unix

using namespace std;

namespace atomTest{
	std::atomic_flag lock = ATOMIC_FLAG_INIT;

	void f(int n)
	{
		while(lock.test_and_set(std::memory_order_acquire)){
			cout<<"waiting from thread "<< n <<endl;
		}

		cout<<"thread "<< n <<"starts working!"<<endl;
	}

	void g(int n)
	{
		cout<<"thread "<< n << "is going to start"<<endl;
		lock.clear();
		cout<<"thread "<< n << "starts working"<<endl;
	}

	void test()
	{
		lock.test_and_set();
		thread t1(f, 1);
		thread t2(g, 2);

		t1.join();
		usleep(100); //posix unix
		t2.join();
	}
}

int main()
{
	atomTest::test();

	return 0;
}
