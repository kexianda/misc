
/**
 *	focus on following concepts.
 *	left value, right value, template deduction, move, perfect forward
 *
 * */
#include <iostream>
#include <stdio.h>
using namespace std;


struct Foo {
	long val = 0;
};

Foo& lRef () { static Foo f; return f;}
const Foo& clRef () { static Foo f; return f;}
Foo rRef () { return Foo (); }
const Foo crRef () { return Foo (); }


/*
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



printValType (lRefA(), "Expected: T&, Actual: ");
printValType (clRefA(), "Expected: const T&, Actual: ");
printValType (rRefA(),  "Expected: T&&, Actual: ");
printValType (crRefA(), "Expected: const T&&, Actual: ");
*/

/**
 * http://en.cppreference.com/w/cpp/language/template_argument_deduction
 *
 * Note: function template parameter type (call it P)
 * argument of the call. (call it A)
 *
 */


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

	/** In general, the deduction process attempts to find template argument values that will make the deduced A
	 *  identical to A (after the type A is transformed as described above). However, there are three cases that allow
	 *  a difference:
	 * 		—(rule 3) If the original P is a reference type, the deduced A (i.e., the type referred to by the reference) can be
	 *			more cv-qualified than the transformed A.
	 *		— The transformed A can be another pointer or pointer to member type that can be converted to the
	 *			deduced A via a qualification conversion (4.4).
	 *		— If P is a class and P has the form simple-template-id, then the transformed A can be a derived class of
	 *			the deduced A. Likewise, if P is a pointer to a class of the form simple-template-id, the transformed A
	 *			can be a pointer to a derived class pointed to by the deduced A.
	*/


	//int getrvalue() { int val = 5; return val;}


template <typename T> void fun_non_ref_param (T val) {
	//cout<< ++val << endl;
}

template <typename T> void fun_const_param (const T val) {
	//cout<< val << endl;
}

template <typename T> void fun_ref_param (T& val) {
	//cout << val << endl;
};

template <typename T> void fun_const_ref_param (const T& val) {
	//cout<< val << endl;
}

template <typename T> void fun_rvalue_ref_param (T&& val) {
	//cout << val << endl;
};

template <typename T> void fun_const_rvalue_ref_param (const T&& val) {
	//cout << val << endl;
};

template <typename T> void g (const T&&) {};



void test() {

	const int const_int_argument = 1;
	int int_lvalue_argument = 2;

	Foo lvalue_arg;
	const Foo const_lvalue_arg;

	//---- T -------------------------------------------
	fun_non_ref_param (int_lvalue_argument);
	//P is not a reference tpye, cv-unqualified. no adjustment, deducted P is T
	//no adjustment for A, transformed A is int
	//so, T is int.  calls: fun_non_ref_param <int> (int)

	fun_non_ref_param (const_int_argument);
	//P is T, no adjustment, deducted P is T.
	//A is 'const int', transformed A is 'int' (rule 1.3)
	//so, T is int. call: fun_non_ref_param <int> (int)

	fun_non_ref_param ( (1) );  //rvalue
	//no adjustment for P, deducted P is T.
	//no adjustment for A, transformed A is int
	//so, T is int. calls: fun_non_ref_param <int> (int)

	//----const T -------------------------------------------
	fun_const_param (int_lvalue_argument);
	//P is 'const T' type. deducted P is T. (rule 2.1: top level cv-qualifiers of P's type are ignored)
	//no adjustment for A. transformed A is int.
	//so, T is int. call: fun_const_param <int> (const int)

	fun_const_param (const_int_argument);
	//P is 'const T' type, deducted P is T (rule 2.1)
	//A is 'const int', transformed A is int (rule 1.3)
	//so, T is int. calls: fun_const_param <int> (const int)

	fun_const_param ( (1));
	//P is 'const T' type, deducted P is T (rule 2.1)
	//A is 'int' type. (rvalue). transformed A is int (rule 1.3)
	//so, T is int. calls: fun_const_param <int> (const int)


	//---- T& -------------------------------------------
	fun_ref_param (int_lvalue_argument);
	//P is 'T&' type, deducted P is T. (rule 2.2:if P is a reference type, the type referred by P is used for type deduction)
	//A is 'int' type, no adjustmetn, transformed A is 'int'
	//so, T is int. calls: fun_ref_param<int>(int&)

	fun_ref_param (const_int_argument);
	//P is 'T&' type, deducted P is T. (rule 2.2:if P is a reference type, the type referred by P is used for type deduction)
	//A is 'int' type, no adjustmetn, transformed A is 'const int'. (cv-qulifiers of A's type are NOT ignored, because P is reference type.)
	//so, T is 'const T'. calls: fun_ref_param <const int>(const int&)


	//fun_ref_param ((1));
	//error expects an l-value

	//-----const T& -----------------------------------------------
	fun_const_ref_param (int_lvalue_argument);
	//P is 'const T&' type, deducted P is 'const T' (2.2 the type referred by P is used for type deduction)
	//A is 'int' type, transformed A is 'int'
	//(rule 3) T is int. calls: fun_ref_param<int>(const int&)

	fun_const_ref_param (const_int_argument);
	//P is 'const T&' type, deducted P is 'const T'
	//A is 'cosnt int' type, transformed A is 'cosnt int'
	//(rule 3), T is int.  calls: fun_ref_param <int>(const int&)

	fun_const_ref_param ((1));
	//P is 'const T&' type, deducted P is 'const T'
	//A is 'int' type, (rlvaue).  transformed A is 'int'
	//(rule 3), T is int. calls: fun_ref_param<int>(const int&)


	//------ T&& ----------------------------------------------
	fun_rvalue_ref_param (int_lvalue_argument);
	//P is T&& type, deducted P is T& . Because P is rvalue refernce to a cv-unqualified template parameter and the argument is an lvalue.( 2.3 special case)
	//calls: fun_rvalue_ref_param <int&> (int &)


	fun_rvalue_ref_param (const_int_argument);
	//calls: fun_rvalue_ref_param <int const &> (int const&)


	fun_rvalue_ref_param (rRef());
	//calls: fun_rvalue_ref_param<Foo>(Foo&&)


	fun_rvalue_ref_param (crRef());
	//calls: fun_rvalue_ref_param<Foo const>(Foo const &&)


	//------ const T&& ----------------------------------------------
	//fun_const_rvalue_ref_param (lvalue_arg);  //error: cannot bind lvalue T to const T&&


	//fun_const_rvalue_ref_param (const_lvalue_arg); //error: cannot bind const lvalue T to const T&&


	fun_const_rvalue_ref_param (rRef());
	//calls: fun_const_rvalue_ref_param<Foo>(Foo const&&)


	fun_const_rvalue_ref_param (crRef());
	//calls: fun_const_rvalue_ref_param<Foo const>(Foo const&&)

	return;
}



int main ()
{

	test ();

	return 0;
}

