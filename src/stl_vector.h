#ifndef __STL_VECTOR_H__
#define __STL_VECTOR_H__

#include "stl_alloc.h"
#include "stl_uninitialized.h"

#include <new>
#include <string>
#include <cstddef>
#include <stdexcept>

namespace stl {

typedef _default_alloc_template<true, 0> alloc;

// vector 
template<typename T, typename Alloc = alloc>
class vector {
public:
  // stl container definition
  typedef T value_type;
  typedef T* pointer;
  typedef T* iterator;
  typedef T& reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

protected:
  typedef simple_alloc<T, alloc> data_allocator;

public:
  /**
   * @brief get begin iterator
   * */
  iterator begin() { return start_; }
  
  /**
   * @brief get end iterator
   * */
  iterator end() { return finish_; }
  
  // element access
public:
  /**
   * @brief get index element
   * @param[in] index element index
   * */
  reference at(size_type index) {
    // get total count
    std::ptrdiff_t count = finish_ - start_;
    // check if out of range
    if (index >= count)
      throw std::out_of_range("vector index");
    return *(start_ + index);
  }
  
  /**
   * @brief get index element
   * @param[in] index element index
   * */
  reference operator[] (size_type index) {
    return *(start_ + index);
  }
  
  /**
   * @brief get front element
   * */ 
  reference front() {
    return *start_;
  }
  
  /**
   * @brief get back element
   * */
  reference back() {
    return *(finish_ - 1);
  }

  T* data() {
    return start_;
  }

  // capacity 
public:
  
  /**
   * @brief check if vec is empty
   * */
  bool empty() {
    return start_ == finish_;
  }
  
  /**
   * @brief element count
   * */
  size_type size() {
    return size_type(finish_ - start_);
  }
  
  /**
   * @brief get total max size 
   * */
  size_type max_size() {
    return size_type(end_of_storage_ - start_);
  }

  void reverse(size_type size) {

  }

  /**
   * @brief get current avaible size
   * */ 
  size_type capacity() {
    return end_of_storage_ - finish_;
  }

  void shrink_to_fit() {

  }
  
  // modifier
public:
  void clear() {

  }

  iterator insert(iterator pos, const T& value) {

  }

  iterator insert(iterator pos, size_type count, const T& value) {

  }

  void push_back(const T& value) {

  }
  
  template<typename... Args>
  void emplace_back(Args&&... args) {
    
  }

  void resize(size_type size) {

  }
  
  void erase(iterator pos) {

  }

  void erase(iterator begin, iterator end) {

  }

  void pop_back() {

  }

private:
  void fill_initialize(size_type count, const T& value) {

  }
  
  void allocate_and_fill(size_type count, const T& value) {

  }

  void insert_aux(iterator pos, const T& value) {
    // check if current storage not enough
    if (finish_ != end_of_storage_) {
      uninitialized_copy(pos, finish_, pos + 1);
      *pos = value;
      ++finish_;
      return;
    }
    // none capacity is left, should realloc memory
    size_type old_size = size();
    size_type new_size = old_size == 0 ? 1 : old_size * 2;
    iterator new_start = data_allocator::allocate();
      

  }

private:
  /// start iterator
  iterator start_ { nullptr };
  /// finish iterator
  iterator finish_ { nullptr };
  /// end of storage, real cap
  iterator end_of_storage_ { nullptr };
};



}


#endif // !__STL_VECTOR_H__
