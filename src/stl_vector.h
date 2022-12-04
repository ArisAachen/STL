#ifndef __STL_VECTOR_H__
#define __STL_VECTOR_H__

#include "stl_alloc.h"
#include "stl_construct.h"
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

  
  /**
   * @brief insert value to pos
   * @param[in] pos insert pos
   * @param[in] value insert value
   * */
  iterator insert(iterator pos, const T& value) {
    insert_aux(pos, value);
    return pos;
  }

  /**
   * @brief push value to back
   * @param[in] value back value
   * */
  void push_back(const T& value) {
    // check if already at last pos
    if (finish_ != end_of_storage_) {
      construct(finish_, value);
      ++finish_;
      return;
    }
    // realloc and place
    insert_aux(end() + 1, value);
  }
  
  template<typename... Args>
  void emplace_back(Args&&... args) {
    
  }

  void resize(size_type size) {

  }
  
  /**
   * @brief erase iterator pos 
   * @param[in] pos iterator pos
   * */
  iterator erase(iterator pos) {
    // check if current pos valid
    if (pos >= finish_)
      return;
    // copy next pos to finish
    stl::uninitialized_copy(pos + 1, finish_, pos);
    --finish_;
    destroy(finish_);
    return pos;
  }

  /**
   * @brief erase begin to end
   * @param[in] begin iterator begin
   * @param[in] end iterator end
   * */
  void erase(iterator begin, iterator end) {
    
  }

  void pop_back() {

  }

private:
  /**
   * @brief use count value to initial memory
   * @param[in] count elem count
   * @param[in] value elem value
   * */
  void fill_initialize(size_type count, const T& value) {
    // allocate and fill
    start_ = allocate_and_fill(count, value);
    finish_ = start_ + count;
    end_of_storage_ = finish_;
  }
 
  /**
   * @brief allocate and fill memory
   * @param[in] count elem count
   * @param[in] value elem value
   * */
  void allocate_and_fill(size_type count, const T& value) {
    // allocate memory
    iterator result = data_allocator::allocate(count);
    stl::uninitialized_fill_n(result, count, value);
    return result;
  }

  /**
   * @brief insert value to position
   * @param[in] pos insert pos
   * @param[in] value elem value
   * */
  void insert_aux(iterator pos, const T& value) {
    // check if current storage not enough
    if (finish_ != end_of_storage_) {
      stl::uninitialized_copy(pos, finish_, pos + 1);
      *pos = value;
      ++finish_;
      return;
    }
    // none capacity is left, should realloc memory
    size_type old_size = size();
    size_type new_size = old_size == 0 ? 1 : old_size * 2;
    iterator new_start = data_allocator::allocate(new_size);
    // copy start to pos value to new start
    iterator new_finish = stl::uninitialized_copy(start_, pos, new_start);    
    construct(pos, value);
    // add finish
    ++new_finish;
    // copy left part
    new_finish = stl::uninitialized_copy(pos, finish_, new_finish);
    // destroy old memory
    stl::destroy(start_, finish_); 
    start_ = new_start;
    finish_ = new_finish;
    end_of_storage_ = start_ + new_size;
  }

  void insert_aux(iterator begin, iterator end, const T& value) {

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
