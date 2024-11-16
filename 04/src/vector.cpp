#include "vector.h"

#include <cstring>
#include <stdexcept>

namespace ds {
int *vector_allocation_policy::allocate(size_t elems_size) {
#ifdef DEBUG
  std::cout << "Allocating " << elems_size << " for vector" << std::endl;
#endif
  int *ptr = new int[elems_size];
  return ptr;
}

size_t vector_allocation_policy::get_capacity(size_t size) const {
  return size * 3 / 2;
}

vector::vector(size_t size, VAL val, size_t capacity)
	: data_(POLICY::allocate(capacity))
	  , size_(size), capacity_(capacity) {
  for (size_t i = 0; i < size_; ++i) {
	data_.operator->()[i] = val;
  }
}

const vector::VAL& vector::operator[](size_t idx) const {
  if (idx >= size_) {
	throw std::out_of_range("Idx >= size");
  }

  return data_.operator->()[idx];
}

vector::VAL& vector::operator[](size_t idx) {
  if (idx >= size_) {
	throw std::out_of_range("Idx >= size");
  }

  return data_.operator->()[idx];
}

void vector::resize(size_t size, vector::VAL val) {
  if (size <= size_) {
	size_ = size;
	return;
  }

  POINTER new_place = relocate(size);
  for (size_t i = size_; i < size; ++i) {
	new_place[i] = val;
  }

  size_ = size;
}

void vector::emplace_back(vector::VAL val) {
  POINTER place = get_new_elem_place();
  *place = val;
  ++size_;
}

vector::POINTER vector::get_new_elem_place() {
  if (size_ <= capacity_) {
	return data_.operator->() + size_;
  }
  relocate(size_ + 1);
  return data_.operator->() + size_;
}

vector::POINTER vector::relocate(size_t new_size) {
  // allocate new memory
  size_t new_capacity = POLICY::get_capacity(new_size);
  POINTER new_place = POLICY::allocate(new_capacity);
  // copy old memory to new place
  // WARNING: if exists pointer to object itself, it would be invalid
  POINTER old_data = data_.release();
  std::memcpy(new_place, old_data, size_);
  // release old memory
  deleter::operator()(old_data);

  capacity_ = new_capacity;
  data_.release(new_place);
  return new_place;
}

void vector::pop_back() {
  if (empty()) {
	throw std::out_of_range("Try to pop from empty vector");
  }
  --size_;
}

vector::VAL& vector::back() {
  if (empty()) {
	throw std::out_of_range("Try to get back elem from empty vector");
  }
  return data_.operator->()[size_ - 1];
}

const vector::VAL& vector::back() const {
  if (empty()) {
	throw std::out_of_range("Try to get back elem from empty vector");
  }
  return data_.operator->()[size_ - 1];
}

vector::vector(const SELF& other)
	: data_(POLICY::allocate(other.capacity_))
	  , size_(other.size_)
	  , capacity_(other.capacity_) {
  for (size_t i = 0; i < size_; ++i) {
	data_.operator->()[i] = other[i];
  }
}

vector::SELF& vector::operator=(const SELF& other) {
  data_.release();
  size_ = other.size_;
  capacity_ = other.capacity_;
  data_ = POLICY::allocate(other.capacity_);
  for (size_t i = 0; i < size_; ++i) {
	data_.operator->()[i] = other[i];
  }
  return *this;
}

} // namespace ds