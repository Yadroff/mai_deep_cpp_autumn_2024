#include "unique.h"

namespace ds {
unique_ptr::unique_ptr(POINTER ptr)
	: ptr_(ptr) {}

unique_ptr::unique_ptr(SELF&& rval) noexcept
	: ptr_(rval.release()) {}

unique_ptr::SELF& unique_ptr::operator=(SELF&& rval) noexcept {
  if (&rval == this) {
	return *this;
  }
  deleter::operator()(ptr_);
  ptr_ = rval.release();
  return *this;
}

unique_ptr::~unique_ptr() {
  deleter::operator()(ptr_);
}

unique_ptr::POINTER unique_ptr::release(POINTER new_val) {
  POINTER ret = ptr_;
  ptr_ = new_val;
  return ret;
}

} // ds