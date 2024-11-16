#include <gtest/gtest.h>
#include "BigInt.h"

/* Тесты конструкторов
 *
 * По сути тесты скорее на компиляцию.
 * Кроме этого проверяется отрицательность, а также перевод в строку
*/

using signed_types = ::testing::Types<int8_t, int16_t, int32_t, int64_t>;
using all_types = ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t>;
using string_types = ::testing::Types<std::string, std::string_view, const char *>;
using big_num_types = ::testing::Types<int32_t, uint32_t, int64_t, uint64_t>;

template<typename T>
class UnsignedConstructor : public ::testing::Test {};

template<typename T>
class SignedConstructor : public UnsignedConstructor<T> {};

template<typename T>
class StringConstructor : public ::testing::Test {};

template<typename T>
class BigNumConstructor : public UnsignedConstructor<T> {};

template<typename T>
using StringConstructor_DeathTest = StringConstructor<T>;

TYPED_TEST_SUITE(UnsignedConstructor, all_types);
TYPED_TEST_SUITE(SignedConstructor, signed_types);
TYPED_TEST_SUITE(StringConstructor, string_types);
TYPED_TEST_SUITE(StringConstructor_DeathTest, string_types);
TYPED_TEST_SUITE(BigNumConstructor, big_num_types);

TYPED_TEST(UnsignedConstructor, PositiveValue) {
  TypeParam value = 1;
  BigInt big_int(value);
  ASSERT_FALSE(big_int.IsNegative());
  ASSERT_STREQ(std::string(big_int).c_str(), "1");
}

TYPED_TEST(UnsignedConstructor, ZeroValue) {
  TypeParam value = 0;
  BigInt big_int(value);
  ASSERT_FALSE(big_int.IsNegative());
  ASSERT_STREQ(std::string(big_int).c_str(), "0");
}

TYPED_TEST(SignedConstructor, NegativeValue) {
  TypeParam value = -1;
  BigInt big_int(value);
  ASSERT_TRUE(big_int.IsNegative());
  ASSERT_STREQ(std::string(big_int).c_str(), "-1");
}

TYPED_TEST(BigNumConstructor, BaseTest) {
  static constexpr TypeParam value = BigInt::BASE;
  BigInt big_int(value);
  ASSERT_STREQ(std::string(big_int).c_str(), BigInt::BASE_STR);
}

TYPED_TEST(StringConstructor, PositiveValue) {
  const char *val = "1";
  TypeParam value = val;
  BigInt big_int(value);
  ASSERT_FALSE(big_int.IsNegative());
  ASSERT_STREQ(std::string(big_int).c_str(), val);
}

TYPED_TEST(StringConstructor, ZeroValue) {
  const char *val = "0";
  TypeParam value = val;
  BigInt big_int(value);
  ASSERT_FALSE(big_int.IsNegative());
  ASSERT_STREQ(std::string(big_int).c_str(), val);
}

TYPED_TEST(StringConstructor, NegativeValue) {
  const char *val = "-1";
  TypeParam value = val;
  BigInt big_int(value);
  ASSERT_TRUE(big_int.IsNegative());
  ASSERT_STREQ(std::string(big_int).c_str(), val);
}

TYPED_TEST(StringConstructor, BigNumber) {
  const char *val = "12345678900987654321";
  TypeParam value = val;
  BigInt big_int(value);
  ASSERT_FALSE(big_int.IsNegative());
  ASSERT_STREQ(std::string(big_int).c_str(), val);
}

TYPED_TEST(StringConstructor_DeathTest, EmptyString) {
  const char *val = "";
  TypeParam value = val;
  ASSERT_ANY_THROW(BigInt big_int(value));
}

TYPED_TEST(StringConstructor_DeathTest, InvalidString) {
  const char *val = "1234 abcddfkjlhgg";
  TypeParam value = val;
  ASSERT_ANY_THROW(BigInt big_int(value));
}

TEST(CopyConstructor, Base){
  BigInt big_int = 1;
  BigInt copy = big_int;
  ASSERT_STREQ(std::string(copy).c_str(), "1");
}

TEST(MoveConstructor, Base){
  BigInt big_int = 1;
  BigInt copy = std::move(big_int);
  ASSERT_STREQ(std::string(copy).c_str(), "1");
}