#include <gtest/gtest.h>

#include "matrix.h"
#include "matrix_row.h"

TEST(Row_tests, GetSize)
{
    {
        Row row(1);
        ASSERT_EQ(row.GetSize(), 1);
    }
    {
        Row row(2);
        ASSERT_EQ(row.GetSize(), 2);
    }
}

TEST(Row_tests, get_val)
{
    {
        Row row(1, 1);
        ASSERT_EQ(row[0], 1);
    }

    {
        Row row(3);
        ASSERT_EQ(row[2], 0);
        row[0] = 1;
        row[1] += 2;
        row[2] *= 3;
        ASSERT_EQ(row[0], 1);
        ASSERT_EQ(row[1], 2);
        ASSERT_EQ(row[2], 0);
    }
}

TEST(Row_tests, equal)
{
    Row row(1);
    ASSERT_EQ(row, row);
    Row another(1, 2);
    Row different_size(2, 2);
    ASSERT_NE(row, another);
    ASSERT_NE(row, different_size);
}

TEST(Row_arithmetic_tests, multiplication_to_number)
{
    Row row(1, 1);
    row *= 1;
    ASSERT_EQ(row[0], 1);
    row *= 2;
    ASSERT_EQ(row[0], 2);
    row *= 0;
    ASSERT_EQ(row[0], 0);
}

TEST(Row_arithmetic_tests, addition)
{
    Row row(1, 1);
    Row another(1, 2);
    Row res = row + another;
    Row expected = Row(1, 3);
    ASSERT_EQ(res, expected);

    row += another;
    ASSERT_EQ(row, expected);
}

TEST(Row_Death_tests, WrongSize)
{
    ASSERT_THROW(Row(0), std::invalid_argument);
}

TEST(Row_Death_tests, WrongIdx)
{
    ASSERT_THROW(Row(1)[static_cast<std::size_t>(- 1)], std::out_of_range);
    ASSERT_THROW(Row(1)[static_cast<std::size_t>(2)], std::out_of_range);
}

TEST(Row_arithmetic_Death_tests, DifferentSizes)
{
    Row row(1);
    Row another(2);
    ASSERT_ANY_THROW(row + another);
}

TEST(Row_tests, print)
{
    std::stringstream stream;
    std::string expected = "[ 0 ]";
    Row row(1);
    stream << row;
    ASSERT_STREQ(expected.c_str(), stream.str().c_str());
}

TEST(Matrix_tests, GetSize)
{
    {
        Matrix mat(1, 1, 1);
        ASSERT_EQ(mat.GetRows(), 1);
        ASSERT_EQ(mat.GetColumns(), 1);
    }
    {
        Matrix mat(52, 52, 52);
        ASSERT_EQ(mat.GetRows(), 52);
        ASSERT_EQ(mat.GetColumns(), 52);
    }
}

TEST(Matrix_tests, GetVal)
{
    Matrix mat(1, 1, 1);
    ASSERT_EQ(mat[0], Row(1, 1));
    ASSERT_EQ(mat[0][0], 1);
}

TEST(Matrix_tests, print)
{
    Matrix mat(2, 2, 2);
    std::stringstream stream;
    std::string expected = "{\n"
                           "\t[ 2, 2 ]\n"
                           "\t[ 2, 2 ]\n"
                           "}";
    stream << mat;
    ASSERT_STREQ(expected.c_str(), stream.str().c_str());
}

TEST(Matrix_arithmetic_tests, equal)
{
    Matrix mat(1, 1, 1);
    Matrix equal(1, 1, 1);
    Matrix not_equal(1, 1, 2);
    Matrix different_size(52, 52, 52);

    ASSERT_EQ(mat, equal);
    ASSERT_NE(mat, not_equal);
    ASSERT_NE(mat, different_size);
}

TEST(Matrix_arithmetic_tests, multiplication_to_number)
{
    Matrix mat(2, 2, 1);
    Matrix left_res = mat * 2;
    Matrix right_res = 2 * mat;
    Matrix expected(2, 2, 2);
    ASSERT_EQ(left_res, right_res);
    ASSERT_EQ(left_res, expected);
    mat *= 2;
    ASSERT_EQ(mat, expected);
}

TEST(Matrix_arithmetic_tests, addition)
{
    Matrix mat(2, 2, 1);
    Matrix add(2, 2, 1);
    Matrix expected(2, 2, 2);
    Matrix res = mat + add;
    ASSERT_EQ(res, expected);
    mat += add;
    ASSERT_EQ(res, expected);
    res += mat;
    ASSERT_NE(res, expected);
}

TEST(Matrix_Death_tests, WrongSize)
{
    ASSERT_THROW(Matrix(0, 1, 1), std::invalid_argument);
    ASSERT_THROW(Matrix(1, 0, 1), std::invalid_argument);
}

TEST(Matrix_Death_tests, WrongIdx)
{
    ASSERT_THROW(Matrix(1, 2)[static_cast<std::size_t>(-1)], std::out_of_range);
    ASSERT_THROW(Matrix(1, 1)[2ull], std::out_of_range);
}

TEST(Matrix_arithmetic_Death_tests, DifferentSizes)
{
    Matrix mat(1, 1, 1);
    Matrix another_rows(2, 1, 2);
    Matrix another_cols(1, 2, 1);
    ASSERT_ANY_THROW(mat + another_rows);
    ASSERT_ANY_THROW(mat + another_cols);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}