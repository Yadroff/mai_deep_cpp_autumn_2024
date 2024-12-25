#include <gtest/gtest.h>

#include "formatter.h"

TEST(Parse, OneArg)
{
    std::string format = "{0} Format with one arg";
    auto index = fmt::impl::parse(format, 1);
    ASSERT_EQ(index.idx.size(), 1);
    ASSERT_EQ(index.map.size(), 1);
    ASSERT_EQ(index.map[0].size(), 1);
    ASSERT_EQ(index.map[0].front(), 0);
    ASSERT_EQ(index.idx.front().start, 0);
    ASSERT_EQ(index.idx.front().end, 2);
}

TEST(Format, Base)
{
    std::string str = fmt::format("Format without args");
    ASSERT_STREQ(str.c_str(), "Format without args");
}

TEST(Format, OneArg)
{
    std::string str = fmt::format("{0} arg", "One");
    ASSERT_STREQ(str.c_str(), "One arg");
}

TEST(Format, ManyArgs)
{
    std::string str = fmt::format("{0} {1} {2}", "One", "Two", "Three");
    ASSERT_STREQ(str.c_str(), "One Two Three");

    str = fmt::format("{0} {2} {1}", 1, 2, 3);
    ASSERT_STREQ(str.c_str(), "1 3 2");
}