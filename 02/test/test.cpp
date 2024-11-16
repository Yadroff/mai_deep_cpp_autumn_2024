#include <gtest/gtest.h>

#include "tokenizer.h"

namespace {
struct Functor {
    void operator()(int newVal)
    {
        val = newVal;
    }

    void operator()(std::string_view arg)
    {
        str = std::string(arg.data(), arg.size());
    }

    static int val;
    static std::string str;
};
int Functor::val = 0;
std::string Functor::str = "";

int intVal = 0;
std::string strVal = "";

void CallbackFuncInt(int val)
{
    intVal = val;
}

void CallbackFuncStr(std::string_view arg)
{
    strVal = std::string(arg.data(), arg.size());
}
} // namespace

TEST(TokenizerTests, Base)
{
    Tokenizer tokenizer;
    ASSERT_NO_THROW(tokenizer.Parse(""));
    ASSERT_NO_THROW(tokenizer.Parse("1 2 3 4 5 6"));
}

TEST(TokenizerTests, onStart)
{
    Tokenizer tokenizer;
    int out = 0;
    tokenizer.SetStartCallback([&out]() { out = 42; });
    tokenizer.Parse("");
    ASSERT_EQ(out, 42);
}

TEST(TokenizerTests, onEnd)
{
    Tokenizer tokenizer;
    int out = 0;
    tokenizer.SetEndCallback([&out]() { out = 42; });
    tokenizer.Parse("");
    ASSERT_EQ(out, 42);
}

TEST(TokenizerTests, onDigit)
{
    Tokenizer tokenizer;
    static constexpr int expected = 42;
    int out = 0;
    tokenizer.SetOnDigit([&out](int val) { out = val; });
    tokenizer.Parse("42");
    ASSERT_EQ(out, expected);
}

TEST(TokenizerTests, onString)
{
    Tokenizer tokenizer;
    static std::vector<const char*> expected = { "Hello,", "world!" };
    std::vector<std::string> strings;
    tokenizer.SetOnString([&strings](std::string_view val) { strings.emplace_back(val.data(), val.size()); });
    tokenizer.Parse("Hello, world!");
    ASSERT_EQ(expected.size(), strings.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_STREQ(expected[i], strings[i].c_str());
    }
}

TEST(TokenizerTests, DigitWithStrings)
{
    Tokenizer tokenizer;
    static std::vector<int> expectedDigits = { 1, 2, 3, 4 };
    static std::vector<std::string> expectedStrings = { "Hello,", "World!" };
    std::vector<int> digits;
    std::vector<std::string> strings;
    tokenizer.SetOnDigit([&digits](int val) { digits.emplace_back(val); });
    tokenizer.SetOnString([&strings](std::string_view val) { strings.emplace_back(val.data(), val.size()); });
    tokenizer.Parse("1 2 Hello, 3 4 World!");
    ASSERT_EQ(expectedDigits.size(), digits.size());
    for (size_t i = 0; i < expectedDigits.size(); ++i) {
        ASSERT_EQ(expectedDigits[i], digits[i]);
    }
    ASSERT_EQ(expectedStrings.size(), strings.size());
    for (size_t i = 0; i < expectedStrings.size(); ++i) {
        ASSERT_STREQ(expectedStrings[i].c_str(), strings[i].c_str());
    }
}

TEST(CallbackTypes, FunctorCallback)
{
    Functor functor;
    Tokenizer tokenizer;
    tokenizer.SetOnDigit(functor);
    tokenizer.SetOnString(functor);
    tokenizer.Parse("42 C++");
    ASSERT_EQ(Functor::val, 42);
    ASSERT_STREQ(Functor::str.c_str(), "C++");
}

TEST(CallbackTypes, FunctionCallback)
{
    Tokenizer tokenizer;
    tokenizer.SetOnDigit(&CallbackFuncInt);
    tokenizer.SetOnString(&CallbackFuncStr);
    tokenizer.Parse("42 C++");
    ASSERT_EQ(intVal, 42);
    ASSERT_STREQ(strVal.c_str(), "C++");
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}