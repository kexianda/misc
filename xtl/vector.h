
#ifndef _XTL_VECTOR_H
#define _XTL_VECTOR_H 1

#include "xtl_memory.h"
#include "xtl_allocate.h"

namespace xtl {

	template <typename T, typename Alloc = xtl::allocator<T> >
	class vector {
	public:
		typedef	T			value_type;
		typedef T&			reference;
		typedef const T&	const_reference;
		
		typedef T*			pointer;
		typedef const T*	const_pointer; 

		typedef T*			iterator;

		typedef size_t		size_type;

		typedef Alloc		allocator_type;
		
		//todo:  reverse_iterator
		//reverse_iterator
		//const_reverse_iterator

		vector() : m_start(nullptr), m_finish(nullptr), m_end_of_storage(nullptr) {}
		
		
		explicit vector(size_type n, const value_type& val = value_type()) {
			m_start = allocator_type::allocate (n);
				
			//todo, to be improved
			for (size_type i=0; i < n; ++i) {
				xtl::copy_construct(m_start+i, val);
			}
		}

		/*
		vector(Iterator from_itr, Iterator to_itr) {
			typename	iterator_traits<Iterator>::difference_type n = 0;	
			n = distance (from_itr, to_itr);
			m_start = allocator_type::allocate (n);
			Iterator curr = m_start;
			while (from_itr != to_itr) {
				copy_construct (curr++, *from_itr++);	
			}
		}

		vector (const vector& v) {
			//todo
		}
		*/

#if __cplusplus >= 201103L
		//todo: move constructor. c++11
#endif //C++ 11
		
		
		~vector() {
			//todo
			xtl::destory(m_start, m_finish);
			allocator_type::deallocate(m_start);	
		}
		
		iterator begin () {
			return m_start;	
		}
		iterator end () {
			//todo
		}

	protected:
		iterator m_start;
		iterator m_finish;
		iterator m_end_of_storage;
	};
}

#endif /*_XTL_VECOTR_H*/
