#include <gtest/gtest.h>

#include "BigInt.h"

TEST(ArithmeticOperations, Addition) {
  BigInt a("12345678901234567890");
  BigInt b("98765432109876543210");
  BigInt expected_sum("111111111011111111100");
  ASSERT_EQ(a + b, expected_sum);
}

TEST(ArithmeticOperations, Subtraction) {
  BigInt a("98765432109876543210");
  BigInt b("12345678901234567890");
  BigInt expected_difference("86419753208641975320");
  ASSERT_EQ(a - b, expected_difference);
}

TEST(ArithmeticOperations, Multiplication) {
  BigInt a("123456789");
  BigInt b("987654321");
  BigInt expected_product("121932631112635269");
  ASSERT_EQ(a * b, expected_product);
}

TEST(ArithmeticOperations, Division) {
  BigInt a("121932631112635269");
  BigInt b("123456789");
  BigInt expected_quotient("987654321");
  ASSERT_EQ(a / b, expected_quotient);
}

TEST(ArithmeticOperations, MultiplyZero) {
  BigInt a("123456789");
  BigInt b("0");
  EXPECT_EQ(a * b, b);
  EXPECT_EQ(b * a, b);
}
