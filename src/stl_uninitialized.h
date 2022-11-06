#ifndef __STL_UNINITIALIZED_H__
#define __STL_UNINITIALIZED_H__

#include <cstring>
#include <cstddef>
#include <algorithm>
#include <type_traits>

#include "stl_construct.h"

namespace stl {

/**
 * @brief copy pod data to target place
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[out] result iterator result
 * @param[in] true_type pod true
 * */ 
template<typename InputIterator, typename ForwardIterator> 
inline ForwardIterator _uninitialized_copy_aux(InputIterator begin, InputIterator end,
                                               ForwardIterator result, std::true_type) {
  // use memcpy directly
  // TODO: not sure if this works when input iterator is non pod
  memcpy(&(*result), &(*begin), std::ptrdiff_t(end - begin));
}

/**
 * @brief copy non pod data to target place
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[out] result iterator result
 * @param[in] false_type pod false
 * */ 
template<typename InputIterator, typename ForwardIterator> 
inline ForwardIterator _uninitialized_copy_aux(InputIterator begin, InputIterator end,
                                               ForwardIterator result, std::false_type) {
  ForwardIterator cur = result;
  // construct all obj
  for (; begin != end; begin++, result++) {
    construct(&(*result), *begin);
  }
  return result;
}

/**
 * @brief copy non pod data to target place
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[out] result iterator result
 * @param[in] T value type
 * */
template<typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator _uninitialized_copy(InputIterator begin, InputIterator end,
                                           ForwardIterator result, const T*) {
  typedef typename std::is_pod<T>::type pod_type;
  return _uninitialized_copy_aux(begin, end, result, pod_type());
}

/**
 * @brief copy non pod data to target place
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[out] result iterator result
 * */
template<typename InputIterator, typename ForwardIterator> 
inline ForwardIterator uninitialized_copy(InputIterator begin, InputIterator end, 
                                  ForwardIterator result) {
  typedef typename ForwardIterator::value_type value_type;
  return _uninitialized_copy(begin, end, result, value_type());
}


}

#endif // !__STL_UNINITIALIZED_H__
