#ifndef __STL_ALLOC_H__
#define __STL_ALLOC_H__

#include <new>
#include <cstdlib>
#include <cstddef>

#include "stl_helper.h"

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
  static void deallocate(void* ptr, std::size_t /* n*/) {
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

// redefine malloc alloc
typedef _malloc_alloc_template<0> malloc_alloc;

template<bool thead, int insl> 
class _default_alloc_template {
public:
  /**
   * @brief allocate memory from alloc 
   * @param[in] size alloc size
   * */
  static void* allocate(std::size_t size) {
    // check if size reach the max size
    // if is, use malloc directly
    if (size > max_block_size_) {
      return malloc_alloc::allocate(size);
    }
    // try to get block index
    int index = get_block_index(size);
    obj* link_head = *(free_list_[index]);
    // check if not link exist
    if (link_head == nullptr) {
      return refill(bound_up(size));
    }
    // get next link, in order to save unused link to free list
    obj* next_link = link_head->free_list_link;
    free_list_[index] = next_link;
    return (void*)*link_head;
  }
  
  /**
   * @brief deallocate memory to alloc
   * @param[in] ptr memory address
   * @param[in] size memory size
   * */
  static void deallocate(void* ptr, std::size_t size) {
    // check if size is larger than max block size,
    // if is, deallocate directly
    if (size > max_block_size_)
      return malloc_alloc::deallocate(ptr, size);
    // get block index
    int index = get_block_index(size);
    // get free list
    obj* link_head = *(free_list_[index]);
    ((obj*)ptr)->free_list_link = link_head;
    free_list_[index] = (obj*)ptr;
  }

  static void* realloc(void* ptr, std::size_t /*n*/, std::size_t new_size) {

  }

private:
  /**
   * @brief realloc heap 
   * @param[in] size alloc size
   * */
  static void* refill(std::size_t size) {
    // bound up size
    size = bound_up(size);
    // stl default use 20 block
    std::size_t count = 20;
    // try to chunk alloc
    char* result = chunk_alloc(size, count);
    // is is ok return nullptr here
    // we use malloc_alloc to malloc memory
    // if user want to handle failure situation
    // handler can be set, if not, nullptr should be return
    if (result == nullptr) 
      return nullptr;
    // check if only one block is creared
    if (count == 1)
      return result;
    // find next
    obj* origin, tail;
    tail = origin = (obj*)(result + size);
    for (int index = 2; index < count; index++) {
      obj* next = (obj*)(index * size);
      tail->free_list_link = next;
      tail = next;
    }
    // get link head
    int index = get_block_index(size);
    obj* link_head = *(free_list_[index]);
    // link origin head to tail
    tail->free_list_link = link_head;
    // store origin to free list
    free_list_[index] = origin;
    return result;
  }
  
  /**
   * @brief chunk alloc memory
   * */
  static char* chunk_alloc(std::size_t size, std::size_t& count) {
    // get total bytes
    std::size_t total_bytes = size * count;
    // get capibility 
    std::ptrdiff_t capibility = end_free_ - start_free_;
    /// result 
    char* result = nullptr;
    // check if left capibility is enough
    if (capibility >= total_bytes) {
      result = start_free_;
      start_free_ += total_bytes;
      return result; 
    }
    // support at least block
    if (capibility >= size) {
      count = capibility / size;
      // calculate available bytes
      std::size_t available_bytes = size * count;
      result = start_free_;
      start_free_ += available_bytes;
      return result;
    } 
    // put left to free list
    if (capibility > 0) {
      // it is ok here, because, alloc and use block is N times of 8
      // so left capibility is also n times of 8
      // there will no memory leak in this memory pool
      int index = get_block_index(capibility);
      obj* link_head = *(free_list_[index]);
      // store left capibility to free list
      ((obj*) start_free_)->free_list_link = *link_head;
      free_list_[index] = (obj*) start_free_;
      // reset start and end free
      start_free_ = nullptr;
      end_free_ = nullptr;
    }
    // new alloc size, 2 total bytes and origin heap size / 16
    std::size_t alloc_size = 2 * total_bytes + (heap_size_ >> 4);
    // malloc from address
    char* alloc_ptr = (char*)malloc_alloc::allocate(alloc_size);
    // check if malloc successfully
    // if malloc successfully, add new memory to free list
    if (alloc_ptr != nullptr) { 
      heap_size_ += alloc_size;
      start_free_ = alloc_ptr;
      end_free_ = start_free_ + alloc_size;
      return chunk_alloc(size, count);
    }
    // if not, may system has no enough memory
    // try to get memory from exist alloc
    // try to find larger unused block from free list
    // TODO: i dont think it is the best solution, 
    // just think about this, there doesnt exist larger memory block
    // but exist continous smaller memory block, 
    // obviously, traditional stl cant deal with this situation
    int index = get_block_index(size);
    for (; index < get_block_count(); index++) {
      obj* link_head = *(free_list_[index]);
      // check if exist at least one free block
      if (link_head == nullptr)
        continue;
      // get start and end free
      start_free_ = *link_head;
      end_free_ = start_free_ + (index + 1) * align_size_;
      return chunk_alloc(size, count);
    }
    return nullptr;
  }

private:
  // obj to store heap block
  union  obj {
    obj* free_list_link;
    char* client_data;
  };
  ///
  static constexpr int get_block_count() {
    return max_block_size_ / align_size_;
  }
  
  /**
   * @brief calculate located block index
   * @param[in] size block size
   * */  
  static int get_block_index(std::size_t size) {
    // TODO: may not correct here
    return (size - 1) / align_size_;
  }

  /**
   * @brief bound up
   * @param[in] size bound size
   * */
  static int bound_up(std::size_t size) {
    return stl::bound_up(size, align_size_);
  }

private:
  /// align block size 
  static const std::size_t align_size_ = 8;
  /// max block size
  static const std::size_t max_block_size_ = 128;
  /// free list to store first block of obj 
  /// TODO: should use violate here
  static obj* free_list_[get_block_count()];
  /// free memory start address
  static char* start_free_;
  /// free memory end address
  static char* end_free_;
  /// heap size
  static std::size_t heap_size_;
};

/// init start free static address
template<bool thread, int insl>
char* _default_alloc_template<thread, insl>::start_free_ = nullptr;
/// init end free static address
template<bool thread, int insl>
char* _default_alloc_template<thread, insl>::end_free_ = nullptr;
/// init head size
template<bool thread, int insl>
std::size_t _default_alloc_template<thread, insl>::heap_size_ = 0;
/// init free list 
template<bool thread, int insl>
typename _default_alloc_template<thread, insl>::obj* _default_alloc_template<thread, insl>::free_list_[get_block_count()] = {};
}

#endif //!__STL_ALLOC_H__
