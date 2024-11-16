#pragma once

#include <iostream>
#include <exception>

#include "Fourier.h"
#include "vector.h"

class BigInt {
 public:
  BigInt() = default;

  BigInt(std::string_view);

  BigInt(signed char);

  BigInt(unsigned char);

  BigInt(signed short);

  BigInt(unsigned short);

  BigInt(signed int);

  BigInt(unsigned int);

  BigInt(signed long);

  BigInt(unsigned long);

  BigInt(signed long long);

  BigInt(unsigned long long);

  friend std::ostream& operator<<(std::ostream& os, const BigInt& anInt);

  operator std::string() const;

  friend bool operator==(const BigInt& left, const BigInt& right);

  friend bool operator<(const BigInt& left, const BigInt& right);

  friend bool operator!=(const BigInt& left, const BigInt& right);

  friend bool operator>(const BigInt& left, const BigInt& right);

  friend bool operator>=(const BigInt& left, const BigInt& right);

  friend bool operator<=(const BigInt& left, const BigInt& right);

  BigInt operator+() const;

  BigInt operator-() const;

  friend BigInt operator+(const BigInt& left, const BigInt& right);

  friend BigInt operator-(const BigInt& left, const BigInt& right);

  BigInt& operator+=(const BigInt& val);

  BigInt& operator-=(const BigInt& val);

  BigInt& operator++();

  BigInt operator++(int);

  BigInt& operator--();

  BigInt operator--(int);

  BigInt& operator*=(const BigInt& val);

  friend BigInt operator*(const BigInt& left, const BigInt& right);

  friend BigInt operator/(const BigInt& left, const BigInt& right);

  friend BigInt operator/(const BigInt& left, long long right);

  [[nodiscard]] long long toLL() const;

  [[nodiscard]] long double toDouble() const;

  BigInt& operator/=(const BigInt& val);

  friend BigInt operator^(BigInt a, BigInt n);

  friend BigInt operator^(BigInt a, unsigned long long n);

  int operator[](size_t index) const;

  [[nodiscard]] bool Odd() const; // нечетное число

  [[nodiscard]] bool Even() const; // четное число

  void shiftRight(); // смещение вправо на один разряд

  [[nodiscard]] bool IsNegative() const { return negative_; }

  static constexpr int BASE = 10000;
  static constexpr const char *BASE_STR = "10000";
 private:
  void deleteLeadingZeroes();

  static int from_string_view(std::string_view view);

  bool negative_ = false;
  ds::vector digits_{0};
  static constexpr int LENGTH = 4;
};
