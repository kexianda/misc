#include<iostream>
using namespace std;


namespace delegateCstor{
	class base{
		public:
			base(int age):base(age, 80.00, nullptr){}
			base(int age, double grade): base(age, grade, "myName"){}
		private:
			//targeting constructor
			base(int age, double grade, const char*name): m_age(age), m_grade(grade),m_name(name){}		

			int m_age {0};
			double m_grade {100.00};
			const char * m_name {nullptr};
			
	};

	void test()
	{
		base a(10);
		base b(11,70);		
	}

	//struct derive: base
};

int main()
{
	delegateCstor::test();
	return 0;
}
