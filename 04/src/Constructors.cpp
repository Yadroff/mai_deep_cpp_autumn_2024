#include <iomanip>
#include <charconv>

#include "BigInt.h"

BigInt::BigInt(signed char c)
{
  negative_ = (c < 0);
  digits_.emplace_back(std::abs(c));
}

BigInt::BigInt(unsigned char c) {
  negative_ = false;
  digits_.emplace_back(c);
}

BigInt::BigInt(short sh) {
  negative_ = (sh < 0);
  digits_.emplace_back(std::abs(sh));
}

BigInt::BigInt(unsigned short c) {
  negative_ = false;
  digits_.emplace_back(c);
}

BigInt::BigInt(int l) {
  negative_ = (l < 0);
  l = std::abs(l);
  do {
	this->digits_.emplace_back(l % BigInt::BASE);
	l /= BigInt::BASE;
  } while (l != 0);
}

BigInt::BigInt(unsigned int l) {
  negative_ = false;
  do {
	this->digits_.emplace_back(l % BigInt::BASE);
	l /= BigInt::BASE;
  } while (l != 0);
}

BigInt::BigInt(long l) {
  negative_ = (l < 0);
  l = std::abs(l);
  do {
	this->digits_.emplace_back(l % BigInt::BASE);
	l /= BigInt::BASE;
  } while (l != 0);
}

BigInt::BigInt(unsigned long l) {
  negative_ = false;
  do {
	this->digits_.emplace_back(l % BigInt::BASE);
	l /= BigInt::BASE;
  } while (l != 0);
}

BigInt::BigInt(long long int l) {
  negative_ = (l < 0);
  l = std::abs(l);
  do {
	this->digits_.emplace_back(l % BigInt::BASE);
	l /= BigInt::BASE;
  } while (l != 0);
}

BigInt::BigInt(unsigned long long int l) {
  negative_ = false;
  do {
	this->digits_.emplace_back(l % BigInt::BASE);
	l /= BigInt::BASE;
  } while (l != 0);
}

BigInt::BigInt(std::string_view str) {
  negative_ = false;
  if (str.empty()) {
	throw std::invalid_argument("Empty string");
  } else {
	std::string_view digits = str;
	if (str[0] == '-' or str[0] == '+') {
	  digits = str.substr(1);
	  negative_ = (str[0] == '-');
	}
	for (auto i = static_cast<ssize_t>(digits.size()); i > 0; i -= LENGTH) {
	  if (i < LENGTH) {
		digits_.emplace_back(from_string_view(digits.substr(0, i)));
	  } else {
		digits_.emplace_back(from_string_view(digits.substr(i - BigInt::LENGTH, BigInt::LENGTH)));
	  }
	}
	deleteLeadingZeroes();
  }
}

std::ostream& operator<<(std::ostream& os, const BigInt& big_int) {
  if (big_int.digits_.empty()) {
	os << 0;
	return os;
  }
  if (big_int.negative_) {
	os << '-';
  }
  os << big_int.digits_.back();
  for (ssize_t i = big_int.digits_.size() - 2; i >= 0; --i) {
	os << std::setw(BigInt::LENGTH) << std::setfill('0') << big_int.digits_[i];
  }
  return os;
}

BigInt::operator std::string() const {
  std::stringstream ss;
  ss << *this;
  return ss.str();
}

void BigInt::deleteLeadingZeroes() {
  while (digits_.size() > 1 and digits_.back() == 0) {
	digits_.pop_back();
  }
  if (digits_.size() == 1 and digits_.back() == 0) {
	negative_ = false;
  }
}

int BigInt::from_string_view(std::string_view view) {
  int result;
  auto [ptr, ec] = std::from_chars(view.data(), view.data() + view.size(), result);
  if (ec == std::errc()) {
	return result;
  } else if (ec == std::errc::invalid_argument) {
	throw std::invalid_argument({view.data(), view.size()});
  }
  return 0;
}