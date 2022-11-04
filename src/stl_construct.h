#ifndef __STL_CONSTRUCT_H__
#define __STL_CONSTRUCT_H__

#include <new>
#include <type_traits>

#include "stl_define.h"

namespace stl {

template<typename T, typename U>
/**
 * @brief obj construct with arguement
 * @param[in] ptr obj address
 * @param[in] arg construct arguement
 * */
inline void construct(T* ptr, const U& arg) {
  new (ptr) T(arg);
}

template<typename T> 
/**
 * @brief destroy obj
 * @param[in] ptr obj address
 * */
inline void destroy(T* ptr) {
  ptr->~T();
}

template<typename ForwardIterator> 
/**
 * @brief trivial type will be destroyed automatic
 * @param[in] begin iterator header 
 * @param[in] end iterator tail
 * */
inline void _destroy_aux(ForwardIterator begin, ForwardIterator end, __true_type) {}


template<typename ForwardIterator>
/**
 * @brief non-trivial type should call destroy method
 * @param[in] begin iterator header
 * @param[in] end iterator tail
 * */
inline void _destroy_aux(ForwardIterator begin, ForwardIterator end, __false_type) {
  for (; begin != end; begin++)
    destroy(&*begin);
}

template<typename ForwardIterator, typename T> 
/**
 * @brief call diff destroy method base on trivial type
 * @param[in] begin iterator header
 * @param[in] end iterator tail
 * @param[in] T type obj
 * */
inline void _destroy(ForwardIterator begin, ForwardIterator end, const T*) {
  typedef std::is_trivial<T> is_trivial;
  _destroy_aux(begin, end, is_trivial());
}

template<typename ForwardIterator>
/**
 * @brief destroy all obj sorted by iterator
 * @param[in] begin iterator header
 * @param[in] end iterator tail
 * */
inline void destroy(ForwardIterator begin, ForwardIterator end) {
  typedef typename ForwardIterator::value_type value_type;
  _destroy(begin, end, &value_type{});  
}


}

#endif // !__STL_CONSTRUCT_H__
