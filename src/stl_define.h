#ifndef __STL_DEFINE_H__
#define __STL_DEFINE_H__

namespace stl {

template<typename T, T value>
/**
 * @brief create compile period obj 
 * */
struct interal_const {
  /// compile period value
  static const T v = value;
  /// value type
  typedef T value_type;
  /// interal type
  typedef interal_const<T, value> type;
  /// convert operate
  operator value_type () {
    return v;
  }
};

// compile period true
typedef interal_const<bool, true> __true_type;
// compile period false
typedef interal_const<bool, false> __false_type;




}



#endif // !__STL_DEFINE_H__
