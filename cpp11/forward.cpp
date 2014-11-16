
/**
 *	focus on following concepts.
 *	left value, right value, template deduction, move, perfect forward
 *
 * */
#include <iostream>
#include <stdio.h>
using namespace std;

/*
struct Foo {
	long val = 0;
};

Foo& lRef () { static Foo f; return f;}
const Foo& clRef () { static Foo f; return f;}
Foo rRef () { return Foo (); }
const Foo crRef () { return Foo (); }



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


	// (rule 1): if P is NOT a reference type: (CPP ISO standard N3690, 14.8.2.1/2)
	// 	1.1 A is array, array-to-pointer conversion
	// 	1.2 A is function type, function-to-pointer conversion
	// 	1.3 if A is cv-qualified type, top level cv-qulifiers of A's type are ignored
	//
	//  N3690, 14.8.2.1/3
	// (rule 2): if P is cv-qualified type, the top level cv-qualifiers of P's type are ignored for tyep deduction
	// (rule 3): if P is a reference type, the type referred by P is used for type deduction
	// (rule 4): if P is rvalue refernce to a cv-unqualified template parameter and the argument is an lvalue,
	// 		the type "lvalue reference to A" is used in place of A for type deduction (special case)
	//

	//(rule 4): In general, the deduction process attempts to find template argument values that will make the deduced A
	//   identical to A (after the type A is transformed as described above). However, there are three cases that allow
	//   a difference:
	//  	(4.1) — If the original P is a reference type, the deduced A (i.e., the type referred to by the reference) can be
	// 			more cv-qualified than the transformed A.
	// 		(4.2) — The transformed A can be another pointer or pointer to member type that can be converted to the
	// 			deduced A via a qualification conversion (4.4).
	// 		(4.3) — If P is a class and P has the form simple-template-id, then the transformed A can be a derived class of
	// 			the deduced A. Likewise, if P is a pointer to a class of the form simple-template-id, the transformed A
	// 			can be a pointer to a derived class pointed to by the deduced A.
	





template <typename T> void fun_non_ref_param (T val) {
	cout<< val << endl;
}

template <typename T> void fun_const_param (const T val) {
	cout<< val << endl;
}

template <typename T> void fun_ref_param (T& val) {
	cout << val << endl;
};

template <typename T> void fun_const_ref_param (const T& val) {
	cout<< val << endl;
}

template <typename T> void fun_rvalue_ref_param (T&& val) {
	cout << val << endl;
};

template <typename T> void fun_const_rvalue_ref_param (const T&& val) {
	cout << val << endl;
};

template <typename T> void g (const T&&) {};


int rvalue_Ref () { return 1; }
const int const_rvalue_Ref () { return 2; }

/**
 * if P is NOT a reference type
 *
 */
void test_non_ref_param () {

	const int const_int_arg = 1;
	int int_lvalue_arg = 2;

	fun_non_ref_param (int_lvalue_arg);
	//P, no adjustment
	//A, no adjustment
	//T -> int.  calls: fun_non_ref_param <int> (int)

	fun_non_ref_param (const_int_arg);
	//P is T, no adjustment, deducted P is T.
	//A is 'const int', transformed A is 'int' (rule 1.3)
	//call: fun_non_ref_param <int> (int)

	fun_non_ref_param ( (1) );  //rvalue
	//P is T, no adjustment, deducted P is T.
	//A is int, rvalue, no adjustment, transformed A is int
	//calls: fun_non_ref_param <int> (int)
}

/**
 * if P is cv-qualified type, the top level cv-qualifiers of P's type are ignored for tyep deduction
 *  P is 'const T' type. deducted P is T
 */
void test_const_param () {

	const int const_int_arg = 1;
	int int_lvalue_arg = 2;

	fun_const_param (int_lvalue_arg);
	//P is 'const T' type. deducted P is T. 
	//no adjustment for A. transformed A is int.
	//so, T is int. call: fun_const_param <int> (const int)

	fun_const_param (const_int_arg);
	//P is 'const T' type, deducted P is T (rule 2)
	//A is 'const int', transformed A is int (rule 1.3)
	//so, T is int. calls: fun_const_param <int> (const int)

	fun_const_param (1);
	//P is 'const T' type, deducted P is T (rule 2)
	//A is 'int' type. (rvalue). no adjustment.
	//so, T is int. calls: fun_const_param <int> (const int)
}


/**
 * if P is a reference type, the type referred by P is used for type deduction
 *  P is 'T&' type. deducted P is T
 *
 */
void test_ref_param () {

	const int const_int_arg = 1;
	int int_lvalue_arg = 2;

	fun_ref_param (int_lvalue_arg);
	//P is 'T&' type, deducted P is T. 
	//A is 'int' type, no adjustmetn, transformed A is 'int'
	//so, T is int. calls: fun_ref_param<int>(int&)

	fun_ref_param (const_int_arg);
	//P is 'T&' type, deducted P is T. 
	//A is 'int' type, no adjustmetn, transformed A is 'const int'. (cv-qulifiers of A's type are NOT ignored, because P is reference type.)
	//so, T is 'const T'. calls: fun_ref_param <const int>(const int&)


	//fun_ref_param (1);  //error
	//P is 'T&' type, deducted P is T. 
	//A is 'int' type, no adjustmetn, transformed A is 'int'
	//deduces: T is int.  but lvalue can't refer to a rvalue
	//error: expects an l-value

	fun_ref_param (const_rvalue_Ref());
}


 /**
 * (3) if P is a reference type, the type referred by P is used for type deduction
 *  P is 'const T&' type. deducted P is 'const T'
 *
 * (4.1) If the original P is a reference type, the deduced A (i.e., the type referred to by the reference) can be
 * more cv-qualified than the transformed A.
 */
void test_const_ref_param () {

	const int const_int_arg = 1;
	int int_lvalue_arg = 2;

	fun_const_ref_param (int_lvalue_arg);
	//P is 'const T&' type, deducted P is 'const T'. 
	//A is 'int' type, no adjustmetn, transformed A is 'int'
	//rule 4.1 deduces: T is int
	//calls: fun_const_ref_param<int>(const int&)

	fun_const_ref_param (const_int_arg);
	//P is 'const T&' type, deducted P is 'const T'. 
	//A is 'const int' type, no adjustmetn, transformed A is 'const int'. 
	// (because P is reference type. cv-qulifiers of A's type can NOT be ignored, )
	//so, T is 'const T'. calls: fun_const_ref_param <const int>(const int&)


	fun_const_ref_param (1);  
	//P is 'const T&' type, deducted P is 'const T'. 
	//A is 'int' type, rvalue, no adjustmetn, transformed A is 'int'
	//deduces: T is int.  
	//calls: fun_const_ref_param <int>(const int&)
}



/**
 * (3) if P is a reference type, the type referred by P is used for type deduction
 *  
 * (rule 4): if P is rvalue refernce to a cv-unqualified template parameter and the argument is an lvalue,
 * the type "lvalue reference to A" is used in place of A for type deduction (special case)
 * 
 */
 void test_rvalue_ref_param () {

 	const int const_int_arg = 1;
	int int_lvalue_arg = 2;

	fun_rvalue_ref_param (int_lvalue_arg);
	//P is 'T&&' type, deduced P is T.
	//A is 'int' type, lvalue, transformed A is 'int&' (rule 4, special case)
	//calls: fun_rvalue_ref_param <int&> (int&).  


	fun_rvalue_ref_param (const_int_arg);
	//P is 'T&&' type, deduced P is T.
	//A is 'const int' type, lvalue, (special case), transformed A is 'const int &' 
	//calls: fun_rvalue_ref_param <const int &> (const int &)


	fun_rvalue_ref_param (rvalue_Ref());
	//P is 'T&&' type, deduced P is T.
	//A is 'int', rvalue, no adjustment. transformed A is int
	//deduces: T is int
	//calls: fun_rvalue_ref_param<int>(int &&)


	fun_rvalue_ref_param ((const int)const_rvalue_Ref());
	//P is 'T&&' type, deduced P is T. (rule 3)
	//A is 'const int', rvalue, no adjustment. transformed A is 'const int'
	//deduces: T is 'const int'
	//calls: fun_rvalue_ref_param<const int>(const int &&)
 }


 void test_const_rvalue_ref_param () {

 	const int const_int_arg = 1;
	int int_lvalue_arg = 2;

	const int const_int_argument = 1;
	int int_lvalue_argument = 2;


	//------ const T&& ----------------------------------------------
	//fun_const_rvalue_ref_param (lvalue_arg);  //error: cannot bind lvalue T to const T&&


	//fun_const_rvalue_ref_param (const_lvalue_arg); //error: cannot bind const lvalue T to const T&&


	fun_const_rvalue_ref_param (rvalue_Ref());
	//calls: fun_const_rvalue_ref_param<int>(int const &&)


	fun_const_rvalue_ref_param (const_rvalue_Ref());
	//calls: fun_const_rvalue_ref_param<Foo const>(Foo const&&)
 }



int main ()
{
	test_non_ref_param ();
	test_const_param ();
	test_ref_param ();
	test_const_ref_param ();
	test_rvalue_ref_param ();
	test_const_rvalue_ref_param ();

	return 0;
}

