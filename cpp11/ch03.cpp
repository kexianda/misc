#include<iostream>
#include<cstring>
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

namespace rightValue{
	class BigData{
		public:
			BigData(){
				mem = new char[1000];			
				cout<<"BigData()"<<endl;}
			
			//copy constructor
			BigData(const BigData& rh){
				cout<<"Copy BigData()"<<endl;
				if(this == &rh){
					return;// *this;
				}
				mem = new char[1000];
				for(int i = 0; i < 1000; ++i){
					*(mem + i) = *(rh.mem + i);
				}
			}

			//operator= 
			BigData& operator=(const BigData& rh){
				cout<<"BigData operato="<<endl;
				if(this == &rh){
					return *this;
				}
				cout<<"Copy BigData()"<<endl;  			
				mem = new char[1000];
				for(int i = 0; i < 1000; ++i){
					*(mem + i) = *(rh.mem + i);
				}
				return *this;
			}	

			//move constructor
			BigData(BigData && rh) : mem(rh.mem){
				rh.mem = nullptr;
				cout<<"move constructor BigData(BigData&& r)"<<endl;
			}

			~BigData(){
				delete[] mem;
				cout<<"~BigData()"<<endl;
			}
		private:
			char * mem;

	};

	class ManyData{
		public:
			ManyData() {
				cout<<"ManyData()"<<endl;
				name = new char[100];
			}
			ManyData(const ManyData& r) : bd(r.bd){
				cout<<"Copy ManyData()"<<endl;
				if(this== &r){
					return;
				}
				name = new char[100];
				strcpy(r.name, name);
				//bd = r.bd;// if move ? 
			}
			ManyData(ManyData && r): name(r.name), bd(move(r.bd)){
				cout<<"move ManyData()"<<endl;
				r.name = nullptr;
			}
			~ManyData() {
				cout<<"~ManyData()"<<endl;
				delete[] name;
			}
		private:
			BigData bd;
			char* name;
	};

	BigData getBigData()
	{
		BigData a;
		return a; //-fno-elide-constructors
	}		
	ManyData getManyData()
	{
		ManyData m;
		return m;
	}

	void test()
	{
		//BigData a;
		//BigData b(a);
		BigData d = getBigData();
		ManyData m = getManyData();
	}
};

int main()
{
	delegateCstor::test();
	rightValue::test();
	return 0;
}
