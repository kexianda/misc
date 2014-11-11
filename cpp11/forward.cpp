
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
	// 	1.3 if A is cv-qualified type, top level cv-qulifiers of A's type are ignored
	//
	// rule 2: N3690, 14.8.2.1/3
	// 	2.1 if P is cv-qualified type, the top level cv-qualifiers of P's type are ignored for tyep deduction
	// 	2.2 if P is a reference type, the type referred by P is used for type deduction
	// 	2.3 if P is rvalue refernce to a cv-unqualified template parameter and the argument is an lvalue,
	// 		the type "lvalue reference to A" is used in place of A for type deduction (special case)
	//

	/**
	If the original P is a reference type, the deduced A (i.e., the type referred to by the reference) can be
        more cv-qualified than the transformed A.
            — The transformed A can be another pointer or pointer to member type that can be converted to the
                deduced A via a qualification conversion (4.4).
            — If P is a class and P has the form simple-template-id, then the transformed A can be a derived class of
                the deduced A. Likewise, if P is a pointer to a class of the form simple-template-id, the transformed A
                can be a pointer to a derived class pointed to by the deduced A.
    */

    int&& getrvalue() { return 5;}


	template <typename T> void fun_non_ref_param (T val) {
		cout<< ++val << endl;
	}

	template <typename T> void fun_const_param (const T val) {
		cout<< val << endl;
	}

    template <typename T> void fun_ref_param (T& val) {
		cout << val << endl;
	};

    template <typename T> void fun_const_ref_param (const T val) {
		cout<< val << endl;
	}

	template <typename T> void fun_rvalue_ref_param (T&& val) {
		cout << val << endl;
	};

	template <typename T> void fun_const_rvalue_ref_param (const T&& val) {
		cout << val << endl;
	};

	template <typename T> void g (const T&&) {};



	void test() {

        const int const_int_argument = 1;
        int int_lvalue_argument = 2;





		fun_non_ref_param (int_lvalue_argument);
		//no adjuestment, T ->  int

        fun_non_ref_param (const_int_argument);
		//P is T, (no adjustment for non-ref-parameter P),
		//A is 'const int', transformed A is 'int' (rule 1.3)
		// deducted A = transformed A, so T is 'int', call: fun_non_ref_param <int> (int)

		fun_non_ref_param ( (1) );  //rvalue
		//no adjuestment, T ->  int

		fun_const_param (int_lvalue_argument);
		fun_const_param (const_int_argument);
		fun_const_param ( (1));

        //2.2 if P is a reference type, the type referred by P is used for type deduction
        fun_ref_param (int_lvalue_argument); //call fun_ref_param<int>(int)
        fun_ref_param (const_int_argument); //call fun_ref_param <const int>(const int)
        //fun_ref_param ((1));    //error expects an l-value


        //If the original P is a reference type, the deduced A (i.e., the type referred to by the reference) can be
        //more cv-qualified than the transformed A.
        fun_const_ref_param (int_lvalue_argument); //call fun_ref_param<int>(int)
        fun_const_ref_param (const_int_argument); //call fun_ref_param <int>(const int)
        fun_const_ref_param ((1));    //call func_ref_param <int> (int)


		/*

        fun_const_param  (int_lvalue);
        fun_const_param  (int_lvalue_ref);
        fun_const_param  (const_int_argument);

        // 2.2 P is reference type,
        fun_ref_param  (int_lvalue); //transformed A is 'int',  so T is 'int', call fun_ref_param<T> (int&)
        fun_ref_param  (int_lvalue_ref);
        fun_ref_param  (const_int_argument);// T is 'const int' call fun_ref_param<int const> (const int)

        //fun_ref_param ( (2*3)); //error! expected lvalue
        int&& rvalref = 5;
        fun_ref_param (rvalref); // name rvalref is lvalue

		// 2.2
		fun_rvalue_ref_param (int_lvalue);
		//P is T&&, deducted A is T
		//A is lvalue, transformed A is int&
		//so, T is int&.  call fun_rvalue_ref_param<int&) (int&)
        */

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

