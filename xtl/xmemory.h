
#include <new>

struct _true_type {}
struct _false_type {};

//template<typename T>


template<typename T>
inline void _construct(T*){
	new (p) T();
}

template<typename T>
inline void _copy_construct (T* p, const T& value) {
	new (p) T (value);
} 

template<typename T>
inline void _destory (T* p) {
	//_destory_aux (p, 
}
