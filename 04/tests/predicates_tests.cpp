#include <gtest/gtest.h>

#include "BigInt.h"

/*
 * Тесты на проверку предикатов сравнения, четности
*/

TEST(Equals, FromSameTypes) {
  int val = 123;
  ASSERT_EQ(BigInt(val), BigInt(val));
}

TEST(Equals, FromDifferentTypes) {
  char ch = 123;
  int val = static_cast<int>(ch);
  ASSERT_EQ(BigInt(val), BigInt(ch));
}

TEST(Equals, FromIntegralAndFromString) {
  static constexpr std::string_view str = "1234";
  int val = 1234;
  ASSERT_EQ(BigInt(val), BigInt(str));
}

TEST(Equals, ThisEqThis) {
  BigInt big_int = 123;
  ASSERT_EQ(big_int, big_int);
}

TEST(NotEquals, FromSameTypes) {
  int val = 123;
  ASSERT_NE(BigInt(val), BigInt(val + 1));
}

TEST(NotEquals, FromDifferentTypes) {
  char ch = 123;
  int val = 1;
  ASSERT_NE(BigInt(val), BigInt(ch));
}

TEST(NotEquals, FromIntegralAndFromString) {
  static constexpr std::string_view str = "12345";
  int val = 1234;
  ASSERT_NE(BigInt(val), BigInt(str));
}

TEST(IsEven, Even) {
  BigInt val = 2;
  ASSERT_TRUE(val.Even());
}

TEST(IsEven, Odd) {
  BigInt val = 1;
  ASSERT_FALSE(val.Even());
}

TEST(IsOdd, Even) {
  BigInt val = 2;
  ASSERT_FALSE(val.Odd());
}

TEST(IsOdd, Odd) {
  BigInt val = 1;
  ASSERT_TRUE(val.Odd());
}

TEST(Less, Base) {
  BigInt less = 1;
  BigInt more = 2;
  ASSERT_TRUE(less < more);
  ASSERT_FALSE(more < less);
}

TEST(Less, Negatives) {
  BigInt less = -2;
  BigInt more = -1;
  ASSERT_TRUE(less < more);
  ASSERT_FALSE(more < less);
}

TEST(Less, NegativeWithPositive) {
  BigInt less = -1;
  BigInt more = 1;
  ASSERT_TRUE(less < more);
  ASSERT_FALSE(more < less);
}

TEST(Less, Equals) {
  BigInt val = 1;
  BigInt other = val;
  ASSERT_FALSE(val < other);
  ASSERT_FALSE(other < val);
}