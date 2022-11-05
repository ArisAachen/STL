#ifndef __STL_HELPER_H__
#define __STL_HELPER_H__

#include <cstddef>

namespace stl {

/**
 * @brief adjust size to align block 
 * @param[in] size source size
 * @param[in] block adjust block
 * */
int bound_up(std::size_t size, std::size_t block) {
  return (size + block - 1) / block * block;
}




}

#endif //!__STL_HELPER_H__
