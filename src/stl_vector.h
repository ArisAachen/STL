#ifndef __STL_VECTOR_H__
#define __STL_VECTOR_H__

#include "stl_alloc.h"

#include <string>
#include <cstddef>

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
  //
  iterator begin() { return start_; }
  
  //
  iterator end() { return finish_; }
  
  // element access
public:
  /**
   * @brief get index element
   * @param[in] index 
   * */
  reference at(size_type index) {
    return *(start_ + index);
  }
  
  //
  reference operator[] (size_type index) {
    return *(start_ + index);
  }
  
  // 
  reference front() {

  }
  
  reference back() {

  }

  T* data() {

  }




private:
  



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
