#ifndef __STL_TRAIT_H__
#define __STL_TRAIT_H__

#include "stl_define.h"

#include <cstddef>

namespace stl {

// general iterator trait
template<typename T>
struct iterator_trait {
  // iterator trait order definition
  typedef typename T::value_type value_type;
  typedef typename T::reference_type reference_type;
  typedef typename T::difference_type difference_type;
  typedef typename T::pointer_type pointer_type;
  typedef typename T::iterator_category iterator_category;
};

// basic type template partional spec
template<typename T>
struct iterator_trait<T*> {
  // iterator trait order definition
  typedef T value_type;
  typedef T& reference_type;
  typedef std::ptrdiff_t difference_type;
  typedef T* pointer_type;
  typedef random_access_iterator_tag iterator_category;
};

// TODO: const type value type iterator trait

}

#endif // !__STL_TRAIT_H__
