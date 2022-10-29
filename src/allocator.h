#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <new>
#include <cstddef>
#include <iostream>

namespace aris {

template<typename T> 
/**
 * @brief allocate ptr from type
 * @param[in] size obj count 
 * @param[in] T obj class
 * */
inline T* _allocate(ptrdiff_t size, T*) {
  // set handler as default
  std::set_new_handler(0);
  // try to allocate mempry
  T* tmp = (T*)::operator new((size_t)size * sizeof(T));
  // deal with empty ptr
  if (tmp == nullptr) {
    std::cerr << "allocate memory failed" << std::endl;
    exit(1);
  }
  // allocate success
  return tmp;
}

template<typename T>
/**
 * @brief deallocate 
 * @param[in] buffer buf address 
 * */
inline void _deallocate(T* buffer) {
  // delete buffer
  ::operator delete(buffer);
}

template<typename T, typename U>
/**
 * @brief construct 
 * @param[in] ptr address
 * @param[in] value args
 * */
inline void _construct(T* ptr, const U& value) {
  // construct value
  new (ptr) T(value);
}

template<typename T>
/**
 * @brief destroy
 * @param[in] ptr address
 * */
inline void _destroy(T* ptr) {
  // destroy
  ptr->~T();
}

template<typename T>
class allocator {
public:
  /// interface define
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

public:
  /**
   * @brief default constructor
   * @param[in] size obj count
   * */
  allocator(size_type size, const void* hint = 0) {
    return _allocate((difference_type)size, (pointer)0);
  } 
  
  void deallocate(pointer )
  
  

};

 
}

#endif // !__ALLOCATOR_H__
