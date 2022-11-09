
template<typename Iter, typename Type> 
Type func_impl(Iter value, const Type&) {

}


template<typename Iter>
void func(Iter value) {
  
}

template<typename T>
struct MyIter {
  typedef T value_type;
  T* ptr;
  value_type& operator*() {
    return *ptr;
  }
};

template<typename Iter> 
typename Iter::value_type 
get_value(Iter iter) {
  return *iter;
}

// template<>
// int get_value(int* value) {
//   return *value;
// }

// function partional spec not allowed
// template <typename T>
// T get_value<T*>(T iter) {
//   return *iter;
// }
//

template<typename Iter>
struct iterator_traits {
  typedef typename Iter::value_type value_type;
};

template<typename Iter>
typename iterator_traits<Iter>::value_type 
get_value(Iter iter) {
  return *iter;
}

template<typename T>
struct iterator_traits<T*> {
  typedef T value_type;
};

#include <string>

class TestOp {

public:
  bool operator[](const std::string& index) {
    return false;
  }

};


#include <vector>

void test() {
  std::vector<int> abc;
  abc.at(1);
    
}
