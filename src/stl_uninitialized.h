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
  // TODO: should self-rels copy algorithm
  std::copy(begin, end, result);
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
                                           ForwardIterator result, const T&) {
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

/**
 * @brief fill pod data to target place
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[in] value obj param
 * @param[in] true_type pod type
 * */
template<typename ForwardIterator, typename T>
inline void _uninitialized_fill_aux(ForwardIterator begin, ForwardIterator end,
                                const T& value, std::true_type) {
  std::fill(begin, end, value);
}

/**
 * @brief fill pod data to target place
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[in] value obj param
 * @param[in] false_type non-pod type
 * */
template<typename ForwardIterator, typename T>
inline void _uninitialized_fill_aux(ForwardIterator begin, ForwardIterator end, 
                                    const T& value, std::false_type) {
  for (; begin != end; begin++) {
    construct(&(*begin), value);
  }
}

/**
 * @brief fill pod data to target place
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[in] value obj param
 * */
template<typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator begin, ForwardIterator end, const T& value) {
  typedef typename std::is_pod<T>::type pod_type;
  return _uninitialized_fill_aux(begin, end, value, pod_type());
}

/**
 * @brief fill pod data to target place 
 * @param[in] begin iterator begin
 * @param[in] count fill count
 * @param[in] value obj param
 * @param[in] true_type pod type
 * */
template<typename ForwardIterator, typename T>
inline void _uninitialized_fill_n_aux(ForwardIterator begin, int count, const T& value,
                                      std::true_type) {
  return std::fill_n(begin, count, value);
}

/**
 * @brief fill pod data to target place 
 * @param[in] begin iterator begin
 * @param[in] count fill count
 * @param[in] value obj param
 * @param[in] false_type pod type
 * */
template<typename ForwardIterator, typename T>
inline void _uninitialized_fill_n_aux(ForwardIterator begin, int count, const T& value,
                                 std::false_type) {
  // use default construct
  for (; count > 0; count--, begin++) {
    construct(&(*begin), value);
  }
}

/**
 * @brief fill pod data to target place 
 * @param[in] begin iterator begin
 * @param[in] count fill count
 * @param[in] value obj param
 * */
template<typename ForwardIterator, typename T>
inline void uninitialized_fill_n(ForwardIterator begin, int count, const T& value) {
  typedef typename std::is_pod<T> pod_type;
  _uninitialized_fill_n_aux(begin, count, value, pod_type());
}

}

#endif // !__STL_UNINITIALIZED_H__
