#ifndef __STL_VECTOR_H__
#define __STL_VECTOR_H__

#include "stl_alloc.h"

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

};


}


#endif // !__STL_VECTOR_H__
