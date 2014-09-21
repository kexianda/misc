
/**
 *	focus on following concepts.
 *	left value, right value, template deduction, move, perfect forward
 *
 * */
#include <iostream>  
#include <stdio.h>
using namespace std;


template<typename T> struct TypeName { 	static const char* get () {	return "T "; } };

template<typename T> struct TypeName <const T>{ static const char* get () {return "const T ";} };

template<typename T> struct TypeName <T&> {	static const char* get () {	return "T& "; } };

template<typename T> struct TypeName <const T&> { 	static const char* get () {return "const T& ";} };

template<typename T> struct TypeName <T&&> { 	static const char* get () {return "T&& ";} };

template<typename T> struct TypeName <const T&&> {	static const char* get () {return "const T&& ";} };


template<typename T>
void printValType (T&& val, const char* expectedStr) {
	cout<< expectedStr << TypeName<T&&>::get() << endl;
}

struct Foo{
	int val = 0;
};

class A{}; 

A& lRefA() { static A a; return a;} 
const A& clRefA() { static A a; return a;}
A rRefA() { return A(); } 
const A crRefA() { return A(); } 


/**
 * http://en.cppreference.com/w/cpp/language/template_argument_deduction
 *
 * Note: function template parameter type (call it P)
 * argument of the call. (call it A)
 *
 */
namespace TemplateArgumentDeduction {

	// rule 1: if P is NOT a reference type: (CPP ISO standard N3690, 14.8.2.1/2)
	// 	1.1 A is array, array-to-pointer conversion
	// 	1.2 A is function type, function-to-pointer conversion
	// 	1.3 if A is cn-qualified type, top level cv-qulifiers of A's type are ignored
	//
	// rule 2: N3690, 14.8.2.1/3
	// 	2.1 if P is cv-qualified type, the top level cv-qualifiers of P's type are ignored for tyep deduction
	// 	2.2 if P is a reference type, the type referred by P is used for type deduction
	// 	2.3 if P is rvalue refernce to a cv-unqualified template parameter and the argument is an lvalue, 
	// 		the type "lvalue reference to A" is used in place of A for type deduction (special case)
	//
	
	
	template <typename T> void fun_non_ref_param (T val) {
		cout<< val << endl;
	}

	
	template <typename T> int fun_rvalue_ref_param (T&& val) {
		cout << val << endl;
		printValType(val, " ");
	};

	template <typename T> int g (const T&&) {};


	
	void test() {

		// 1.3 
		const int const_int_argument = 1;
		fun_non_ref_param (const_int_argument);
		//P is T,  deducted A is T,  (no adjustment for non-ref-parameter P)
		//A is 'const int', transformed A is 'int' (rule 1.3)
		// deducted A = transformed A, so T is 'int', call: fun_non_ref_param <int> (int)


		
		// 2.2
		int lvalue_int = 1;
		fun_rvalue_ref_param (lvalue_int);
		//P is T&&, deducted A is T&,
		//A is
	}
}


int main() 
{
	TemplateArgumentDeduction::test ();

	printValType (lRefA(), "Expected: T&, Actual: ");
	printValType (clRefA(), "Expected: const T&, Actual: ");
	printValType (rRefA(),  "Expected: T&&, Actual: ");
	printValType (crRefA(), "Expected: const T&&, Actual: ");
	return 0;
}

