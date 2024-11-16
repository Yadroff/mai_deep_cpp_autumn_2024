#pragma once

#include <cstddef> // for size_t
#include <vector>
#include "unique.h"

namespace ds {

struct vector_allocation_policy {
  int *allocate(size_t elems_size);
  size_t get_capacity(size_t size) const;
  static constexpr inline int DEFAULT_SIZE = 0;
  static constexpr inline int DEFAULT_CAPACITY = 10;
};

class vector : public vector_allocation_policy, deleter {
 public:
  using SELF = vector;
  using POLICY = vector_allocation_policy;
  using POINTER = int *;
  using VAL = int;

  explicit vector(size_t size = POLICY::DEFAULT_SIZE,
				  VAL val = {},
				  size_t capacity = POLICY::DEFAULT_CAPACITY);

  vector(const SELF& other);
  SELF& operator=(const SELF& other);

  [[nodiscard]] inline ssize_t size() const { return size_; }
  const VAL& operator[](size_t idx) const;
  VAL& operator[](size_t idx);
  void resize(size_t size, VAL val = 0);
  void emplace_back(VAL val);
  [[nodiscard]] inline bool empty() const { return size_ == 0; }
  const VAL& back() const;
  VAL& back();
  void pop_back();

  operator std::vector<int>() const {
	return {data_.operator->(), data_.operator->() + size_};
  }

 private:
  POINTER get_new_elem_place();
  POINTER relocate(size_t new_size);
 private:
  unique_ptr data_;
  size_t size_ = 0;
  size_t capacity_ = 0;
};
} // namespace ds
