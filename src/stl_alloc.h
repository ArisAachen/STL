#ifndef __STL_ALLOC_H__
#define __STL_ALLOC_H__

#include <new>
#include <cstdlib>
#include <cstddef>

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


template<bool thead, int insl> 
class _default_alloc_template {
public:

  static void* allocate(std::size_t size) {

  }

  static void* deallocate(void* ptr, std::size_t /*n*/) {

  }
 
private:
  /**
   * @brief realloc heap 
   * @param[in] size alloc size
   * */
  static void* refill(std::size_t size) {
    // bound up size
    size = bound_up(size);

  }

  static void* chunk_allock() {

  }

private:
  // obj to store heap block
  union  obj {
    obj* free_list_link;
    char* client_data;
  };
  /// block count
  static constexpr int get_block_count() {
    return max_block_size_ / align_size_;
  }
  
  /**
   * @brief calculate located block index
   * @param[in] size block size
   * */  
  static int get_lock_index(std::size_t size) {
    return size / align_size_;
  }

  /**
   * @brief bound up
   * @param[in]
   * */
  static int bound_up(std::size_t size) {
    return (size + align_size_ - 1) / align_size_ * align_size_;
  }

private:
  /// align block size 
  static const int align_size_ = 8;
  /// max block size
  static const int max_block_size_ = 128;
  /// free list to store first block of obj 
  static obj* volatile free_list_[get_block_count()];
  /// free memory start address
  static void* start_free_;
  /// free memory end address
  static void* end_free_;
  /// heap size
  static std::size_t heap_size_;
};

/// init start free static address
template<bool thread, int insl>
void* _default_alloc_template<thread, insl>::start_free_ = nullptr;
/// init end free static address
template<bool thread, int insl>
void* _default_alloc_template<thread, insl>::end_free_ = nullptr;
/// init head size
template<bool thread, int insl>
std::size_t _default_alloc_template<thread, insl>::heap_size_ = 0;

}

#endif //!__STL_ALLOC_H__
