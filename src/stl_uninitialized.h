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
  for (; begin != end; begin++, cur++) {
    construct(&(*cur), *begin);
  }
  return cur;
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
  return std::fill(begin, end, value);
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
  ForwardIterator cur = begin;
  for (; cur != end; cur++) {
    construct(&(*cur), value);
  }
}

/**
 * @brief fill pod data to target place
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[in] value obj param
 * @param[in] type iterator type
 * */
template<typename ForwardIterator, typename T, typename Type>
inline void _uninitialized_fill(ForwardIterator begin, ForwardIterator end, const T& value, 
                                const Type&) {
  typedef typename std::is_pod<Type>::type pod_type;
  return _uninitialized_fill_aux(begin, end, value, pod_type());
}

/**
 * @brief fill pod data to target place
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[in] value obj param
 * */
template<typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator begin, ForwardIterator end, const T& value) {
  typedef typename ForwardIterator::type value_type;
  return _uninitialized_fill(begin, end, value, value_type());
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
  ForwardIterator cur = begin;
  // use default construct
  for (; count > 0; count--, cur++) {
    construct(&(*cur), value);
  }
}

/**
 * @brief fill pod data to target place 
 * @param[in] begin iterator begin
 * @param[in] count fill count
 * @param[in] value obj param
 * @param[in] type obj type
 * */
template<typename ForwardIterator, typename T, typename Type>
inline void _uninitialized_fill_n(ForwardIterator begin, int count, const T& value,
                                 const Type&) {
  typedef typename std::is_pod<Type>::type pod_type;
  return _uninitialized_fill_n_aux(begin, count, value, pod_type());
}

/**
 * @brief fill pod data to target place 
 * @param[in] begin iterator begin
 * @param[in] count fill count
 * @param[in] value obj param
 * */
template<typename ForwardIterator, typename T>
inline void uninitialized_fill_n(ForwardIterator begin, int count, const T& value) {
  typedef typename ForwardIterator::type value_type;
  return _uninitialized_fill_n(begin, count, value, value_type());
}

/**
 * @brief copy specialization
 * @param[in] begin iterator begin
 * @param[in] end iterator end
 * @param[in] result iterator result
 * */
template<>
inline char* uninitialized_copy(const char* begin, const char* end, char* result) {
  memmove(result, begin, (end - begin));
  return result + (end - begin);
}

}

#endif // !__STL_UNINITIALIZED_H__
