#include <sstream>

#include <gtest/gtest.h>

#include "serializer.h"
#include "deserializer.h"

struct TestData {
  bool first;
  uint64_t second;

  template<typename Serializer>
  Serializer::Error serialize(Serializer& serializer)
  {
      return serializer(first, second);
  }
};

static_assert(is_serializable<TestData, void(Serializer&)>::value, "TestData should be serializable");

class Serializer_Test : public ::testing::Test {
public:
    template<typename T>
    static void print(Serializer& ser, T&& arg)
    {
        ser.print(std::forward<T&&>(arg));
    }
};

class Deserializer_Test : public ::testing::Test {
public:
    template<typename T>
    static void read(Deserializer& des, T& arg)
    {
        des.read(arg);
    }
};

TEST_F(Serializer_Test, Print)
{
    {
        std::stringstream stream;
        Serializer ser(stream);
        print(ser, true);
        ASSERT_STREQ(stream.str().c_str(), "true");
    }
    {
        std::stringstream stream;
        Serializer ser(stream);
        print(ser, false);
        ASSERT_STREQ(stream.str().c_str(), "false");
    }
    {
        std::stringstream stream;
        Serializer ser(stream);
        print(ser, 1234ULL);
        ASSERT_STREQ(stream.str().c_str(), "1234");
    }
    {
        std::stringstream stream;
        Serializer ser(stream);
        std::string_view view = std::string_view("Hello, world!").substr(0, 5); // Hello
        print(ser, std::forward<std::string_view>(view));
        ASSERT_STREQ(stream.str().c_str(), std::string(std::to_string(view.size())+" Hello").c_str());
    }
    {
        std::stringstream stream;
        Serializer ser(stream);
        std::string str = "Hello, world!";
        print(ser, std::forward<const char*>(str.c_str()));
        ASSERT_STREQ(stream.str().c_str(), std::string(std::to_string(str.size())+" Hello, world!").c_str());
    }
    {
        std::stringstream stream;
        Serializer ser(stream);
        std::string str = "Hello, world!";
        print(ser, std::forward<std::string>(str));
        ASSERT_STREQ(stream.str().c_str(), std::string(std::to_string(str.size())+" Hello, world!").c_str());
    }
}

TEST_F(Serializer_Test, Process)
{
    std::stringstream stream;
    Serializer ser(stream);
    Serializer::Error err = ser(true, false, 1234ULL);
    ASSERT_EQ(err, Serializer::NoError);
    ASSERT_STREQ(stream.str().c_str(), "true false 1234");
}

TEST_F(Serializer_Test, ProcessData)
{
    {
        std::stringstream stream;
        Serializer ser(stream);
        Serializer::Error err = ser.save(TestData{true, 1234});
        ASSERT_EQ(err, Serializer::NoError);
        ASSERT_STREQ(stream.str().c_str(), "true 1234");
    }
    {
        std::stringstream stream;
        Serializer ser(stream);
        Serializer::Error err = ser(TestData{true, 1234});
        ASSERT_EQ(err, Serializer::NoError);
        ASSERT_STREQ(stream.str().c_str(), "true 1234");
    }
    {
        std::stringstream stream;
        Serializer ser(stream);
        TestData data{true, 1234};
        Serializer::Error err = data.serialize(ser);
        ASSERT_EQ(err, Serializer::NoError);
        ASSERT_STREQ(stream.str().c_str(), "true 1234");
    }
}

TEST_F(Serializer_Test, ManySaveCalls)
{
    std::stringstream stream;
    Serializer ser(stream);
    Serializer::Error err = ser.save(TestData{true, 1234});
    ASSERT_EQ(err, Serializer::NoError);
    err = ser(true, 1234ULL);
    ASSERT_EQ(err, Serializer::NoError);
    ASSERT_STREQ(stream.str().c_str(), "true 1234 true 1234");
}

TEST_F(Serializer_Test, CorruptedData)
{
    std::stringstream stream;
    Serializer ser(stream);
    Serializer::Error err = ser(Serializer::CorruptedData());
    ASSERT_EQ(err, Serializer::CorruptedArchive);
    err = ser(true, false, 1234, Serializer::CorruptedData(), 12345);
    ASSERT_EQ(err, Serializer::CorruptedArchive);
}

TEST_F(Deserializer_Test, Read)
{
    {
        std::stringstream stream;
        stream << "true";
        Deserializer ser(stream);
        bool b;
        read(ser, b);
        ASSERT_TRUE(b);
    }
    {
        std::stringstream stream;
        stream << "1234";
        Deserializer ser(stream);
        int val;
        read(ser, val);
        ASSERT_EQ(val, 1234);
    }
    {
        std::stringstream stream;
        Deserializer ser(stream);
        std::string_view view = std::string_view("Hello, world!").substr(0, 5); // Hello
        stream << view.size() << " " << view;
        std::string out;
        read(ser, out);
        ASSERT_STREQ(out.c_str(), "Hello");
    }
    {
        std::stringstream stream;
        Deserializer ser(stream);
        std::string str = "Hello, world!";
        stream << str.size() << " " << str;
        str = "";
        read(ser, str);
        ASSERT_STREQ(str.c_str(), "Hello, world!");
    }
}

TEST_F(Deserializer_Test, Process)
{
    std::stringstream stream;
    stream << "true false 1234";
    bool first;
    bool second;
    uint64_t third;
    Deserializer ser(stream);
    Deserializer::Error err = ser(first, second, third);
    ASSERT_EQ(err, Deserializer::NoError);
    ASSERT_EQ(first, true);
    ASSERT_EQ(second, false);
    ASSERT_EQ(third, 1234);
}

TEST_F(Deserializer_Test, ProcessData)
{
    std::stringstream stream;
    Deserializer ser(stream);
    stream << "true 1234";
    TestData data{};
    Deserializer::Error err = ser.load(data);
    ASSERT_EQ(err, Deserializer::NoError);
    ASSERT_EQ(data.first, true);
    ASSERT_EQ(data.second, 1234);
}

TEST_F(Deserializer_Test, ManySaveCalls)
{
    std::stringstream stream;
    stream << "true 1234 true 1234";
    Deserializer ser(stream);
    TestData first{};
    bool second;
    int third;
    Deserializer::Error err = ser.load(first);
    ASSERT_EQ(err, Deserializer::NoError);
    err = ser(second, third);
    ASSERT_EQ(err, Deserializer::NoError);
}

TEST_F(Serializer_Test, SerializeViaOperator)
{
    std::stringstream stream;
    Serializer ser(stream);
    ser << true << 1234 << "Hello, world!";
    ASSERT_STREQ(stream.str().c_str(), "true 1234 13 Hello, world!");
}

TEST_F(Deserializer_Test, CorruptedData)
{
    std::stringstream stream;
    Deserializer ser(stream);
    Deserializer::CorruptedData corrupted_data;
    Deserializer::Error err = ser(corrupted_data);
    ASSERT_EQ(err, Serializer::CorruptedArchive);
    stream << "true 1234 corrupted_data 12345";
    TestData data{};
    int val;
    err = ser(data, corrupted_data, val);
    ASSERT_EQ(err, Serializer::CorruptedArchive);
}

TEST_F(Deserializer_Test, Integration)
{
    std::stringstream stream;
    Serializer serializer(stream);
    Deserializer deserializer(stream);

    serializer(true, 1234, 1234ll, TestData{false, 123}, "Hello, world!");
    bool bool_val;
    int int_val;
    long long ll_val;
    TestData test_data{};
    std::string str_val;

    deserializer(bool_val, int_val, ll_val, test_data, str_val);

    ASSERT_EQ(bool_val, true);
    ASSERT_EQ(int_val, 1234);
    ASSERT_EQ(ll_val, 1234);
    ASSERT_EQ(test_data.first, false);
    ASSERT_EQ(test_data.second, 123);
    ASSERT_STREQ(str_val.c_str(), "Hello, world!");
}

TEST_F(Deserializer_Test, DeserializeViaOperator)
{
    std::stringstream stream;
    stream << "true 1234 13 Hello, world!";
    Deserializer ser(stream);
    bool bool_val;
    int int_val;
    std::string str_val;
    ser >> bool_val >> int_val >> str_val;
    ASSERT_EQ(bool_val, true);
    ASSERT_EQ(int_val, 1234);
    ASSERT_STREQ(str_val.c_str(), "Hello, world!");
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}