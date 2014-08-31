/*
 *	@Author: Ke, Xianda
 *	@Date: 2014-08-31
 *	@Note: Just for fun
 *
 *	HP-Stlye allocators. Simple allocator, because  ms-crt / linux glibc will do the work.
 *
 *
 * */
#include <cstdlib> //malloc, free
#include <cstddef> //ptrdiff_t, size_t

namespace xtl {
	
	//exception
	//stuct bad_alloc {};

	template <typename T>
	inline T* _allocate (ptrdiff_t size, T* ) {
		T* tmp = (T*) malloc ( (size_t)(size * sizeof(T)) );
		if (nullptr == tmp) {
			throw std::bad_alloc; //use std::bad_alloc 
		}
		return tmp;
	}

	template <typename T>
	inline void _deallocate (T* buf) {
		free (buf);
	}



	template<typename T>
	class allocator {
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef const T&	reference;
		typedef T&			const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		pointer allocate (size_t n) {
			return _allocate (n, (pointer)(0));
		}

		void deallocate (pointer p) {
			_deallocate (p);
		}

		pointer address (reference r) {
			return (pointer)&r;
		}

		const_pointer address (reference r) {
			return (const_pointer)&r;
		}

		size_type init_page_size () {
			size_type a = size_type(1);
			size_type b = size_type(4096/sizeof(value_type)) ;
			return a > b ? a : b;
		}
		
	};

}//end of xtl
