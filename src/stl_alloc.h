#ifndef __STL_ALLOC_H__
#define __STL_ALLOC_H__

#include <cstdlib>
#include <cstddef>
#include <new>


namespace stl {

template<int insl> 
class _malloc_alloc_template {
public:
  /// define malloc oom handler
  typedef void(*_malloc_alloc_handler)();

public:
  /**
   * @brief allocate memory
   * @param[in] size memory size
   * */
  static void* allocate(std::size_t size) {
    // try to malloc memory
    void* ptr = std::malloc(size);
    // call oom to remalloc memory
    if (ptr == nullptr)
      ptr = oom_malloc(size);
    return ptr;
  }

  /**
   * @brief deallocate memory
   * @param[in] ptr memory address
   * @param[in] size memory size
   * */
  static void* deallocate(void* ptr, std::size_t /* n*/) {
    std::free(ptr);
  }

  /**
   * @brief realloc memory
   * @param[in] ptr memory address
   * @param[in] n old size
   * @param[in] size realloc size
   * */
  static void* reallocate(void* ptr, std::size_t /* n*/, std::size_t size) {
    void* new_ptr = std::realloc(ptr, size);
    if (new_ptr == nullptr)
      new_ptr = oom_realloc(ptr, size);
    return ptr;
  }
  
  /**
   * @brief malloc oom handler 
   * @param[in] handler oom handler
   * */
  static void set_malloc_handler(_malloc_alloc_handler handler) {
    oom_handler_ = handler;
  }

private:
  /**
   * @brief malloc when out of memory
   * @param[in] size buffer size
   * */
  static void* oom_malloc(std::size_t size) {
    // check if handler is set
    // if not, should throw exception
    if (oom_handler_ == nullptr) 
      throw std::bad_alloc{};
    void* ptr = nullptr;
    do {
      // try to handler
      oom_handler_();
      // remalloc
      ptr = std::malloc(size);
    } while (ptr == nullptr);
    return ptr;
  }

  /**
   * @brief realloc when out of memory
   * @param[in] ptr realloc address
   * @param[in] size realloc size
   * */
  static void* oom_realloc(void* ptr, std::size_t size) {
    // check if handler is set
    // if not, should throw exception
    if (oom_handler_ == nullptr) 
      throw std::bad_alloc{};
    void* new_ptr = nullptr;
    do {
      // try to handler
      oom_handler_();
      // remalloc
      new_ptr = std::realloc(ptr, size);
    } while (new_ptr == nullptr);
    return new_ptr;
  }
  
private:
  /// oom handler func
  static _malloc_alloc_handler oom_handler_;
};






}

#endif //!__STL_ALLOC_H__
