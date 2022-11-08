#ifndef __STL_DEFINE_H__
#define __STL_DEFINE_H__

namespace stl {

template<typename T>
T value_type(T* t) {
  typedef T value_type;
}

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

// basic input iterator, read-only
struct input_iterator_tag {};
// basic output iterator, write-only
struct output_iterator_tag {};
// forward type, rw support
struct forward_iterator_tag : input_iterator_tag, output_iterator_tag {};
// bidirectional, can reverse
struct bidirectional_iterator_tag : forward_iterator_tag{};
// random access, any postion
struct random_access_iterator_tag : bidirectional_iterator_tag{};


}



#endif // !__STL_DEFINE_H__
