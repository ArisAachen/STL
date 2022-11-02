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
inline void _construct(T* ptr, const U& arg) {
  new (ptr) T(arg);
}

template<typename T> 
/**
 * @brief destroy obj
 * @param[in] ptr obj address
 * */
inline void _destroy(T* ptr) {
  ptr->~T();
}

template<typename ForwardIterator> 
/**
 * @brief trivial type will be destroyed automatic
 * @param[in] begin iterator header 
 * @param[in] end iterator tail
 * */
inline void _destroy(ForwardIterator begin, ForwardIterator end, __true_type) {}

template<typename ForwardIterator>

inline void _destroy(ForwardIterator begin, ForwardIterator end, __false_type) {

}



template<typename ForwardIterator, typename T> 
inline void _destroy(ForwardIterator begin, ForwardIterator end, const T*) {
  
}

template<typename ForwardIterator>
/**
 * @brief destroy all obj sorted by iterator
 * @param[in] begin iterator header
 * @param[in] end iterator tail
 * */
inline void _destroy(ForwardIterator begin, ForwardIterator end) {
  
}













}



#endif // !__STL_CONSTRUCT_H__
