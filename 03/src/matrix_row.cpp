#include "matrix_row.h"

Row::Row(std::size_t size, int val)
    : size(size)
{
    if (size == 0) {
        throw std::invalid_argument("Can not create row with size 0");
    }
    data = new int[size];
    for (std::size_t i = 0; i < size; ++i) {
        data[i] = val;
    }
}

Row::Row(const Row& other)
{
    size = other.size;
    data = new int[size];
    std::copy(other.data, other.data + size, data);
}

Row& Row::operator=(const Row& other)
{
    if (this == &other) {
        return *this;
    }
    // free used memory
    {
        delete[] data;
        data = nullptr;
    }
    // copy memory
    size = other.size;
    data = new int[size];
    std::copy(other.data, other.data + size, data);
    return *this;
}

Row::Row(Row&& other)
    : size(other.size)
    , data(other.data)
{
    other.size = 0;
    other.data = nullptr;
}

Row& Row::operator=(Row&& other)
{
    std::swap(size, other.size);
    std::swap(data, other.data);
    return *this;
}

Row::~Row()
{
    delete[] data;
}

int& Row::operator[](std::size_t idx)
{
    if (idx >= size) {
        throw std::out_of_range("Idx >= size of row");
    }
    return data[idx];
}

const int& Row::operator[](std::size_t idx) const
{
    if (idx >= size) {
        throw std::out_of_range("Idx >= size of row");
    }
    return data[idx];
}

bool Row::operator==(const Row& other) const
{
    if (this == &other) {
        return true;
    }

    if (size != other.size) {
        return false;
    }

    for (std::size_t i = 0; i < size; ++i) {
        if (data[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}

bool Row::operator!=(const Row& other) const
{
    return !(*this == other);
}

Row& Row::operator+=(const Row& other)
{
    if (size != other.size) {
        throw std::invalid_argument("Sizes of rows are different");
    }

    for (std::size_t i = 0; i < size; ++i) {
        data[i] += other.data[i];
    }
    return *this;
}

Row& Row::operator*=(int val)
{
    for (std::size_t i = 0; i < size; ++i) {
        data[i] *= val;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Row& row)
{
    os << "[ ";
    for (std::size_t i = 0; i < row.GetSize(); ++i) {
        os << row[i];
        if (i != row.GetSize() - 1) {
            os << ", ";
        }
    }
    os << " ]";
    return os;
}

Row operator+(const Row& lhs, const Row& rhs)
{
    Row res = lhs;
    res += rhs;
    return res;
}

Row operator*(const Row& lhs, int rhs)
{
    Row res = lhs;
    res *= rhs;
    return res;
}

Row operator*(int lhs, const Row& rhs)
{
    return rhs * lhs;
}
