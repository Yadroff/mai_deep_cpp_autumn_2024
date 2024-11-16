#pragma once
#include <iostream>

namespace ds {

struct deleter{
  void operator()(int* ptr){
#ifdef DEBUG
	std::cout << std::hex << "Delete " << ptr << std::dec <<  std::endl;
#endif
	delete[] ptr;
  }
};

class unique_ptr : public deleter {
 public:
  using SELF = unique_ptr;
  using POINTER = int *;
  using REF = int&;
  using CONST_REF = const int&;

  unique_ptr(POINTER ptr);
  unique_ptr(SELF&& rval) noexcept;
  SELF& operator=(SELF&& rval) noexcept;
  unique_ptr(const SELF&) = delete;
  SELF& operator=(const SELF& rval) = delete;
  ~unique_ptr();

  POINTER release(POINTER new_val = nullptr);

  POINTER operator->() const { return ptr_; }
  REF operator*() { return *ptr_; }
  CONST_REF operator*() const { return *ptr_; }
 private:
  POINTER ptr_;
};
} // ds