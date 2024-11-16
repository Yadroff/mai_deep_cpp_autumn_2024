#include "matrix.h"

Matrix::Matrix(std::size_t rows, std::size_t columns, int default_val)
    : rows_num(rows)
{
    if (rows_num == 0 || columns == 0) {
        throw std::invalid_argument("Can not create matrix with 0 dimension size");
    }
    { // т.к. нельзя выделить с помощью new[] с передачей аргументов, то выделяем сырую память и вызываем placement-new
        void* raw_memory = operator new[](rows_num * sizeof(Row));
        this->rows = reinterpret_cast<Row*>(raw_memory);
        for (std::size_t i = 0; i < rows_num; ++i) {
            new (&this->rows[i]) Row(columns, default_val);
        }
    }
}

Matrix::Matrix(const Matrix& other)
    : rows_num(other.rows_num)
{
    void* raw_memory = operator new[](rows_num * sizeof(Row));
    rows = reinterpret_cast<Row*>(raw_memory);
    for (std::size_t i = 0; i < rows_num; ++i) {
        new (&rows[i]) Row(other.GetColumns());
    }
    std::copy(other.rows, other.rows + rows_num, rows);
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if (this == &other) {
        return *this;
    }
    // free used memory
    {
        delete[] rows;
        rows = nullptr;
    }
    rows_num = other.rows_num;
    void* raw_memory = operator new[](rows_num * sizeof(Row));
    rows = reinterpret_cast<Row*>(raw_memory);
    std::copy(other.rows, other.rows + rows_num, rows);
    return *this;
}

Matrix::Matrix(Matrix&& other) noexcept
    : rows_num(other.rows_num)
    , rows(other.rows)
{
    other.rows_num = 0;
    other.rows = nullptr;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept
{
    std::swap(rows_num, other.rows_num);
    std::swap(rows, other.rows);
    return *this;
}

Matrix::~Matrix()
{
    delete rows; // not delete[] because of placement new (asan fail)
}

Row& Matrix::operator[](std::size_t row)
{
    if (row >= rows_num) {
        throw std::out_of_range("Idx >= rows number");
    }
    return rows[row];
}

const Row& Matrix::operator[](std::size_t row) const
{
    if (row >= rows_num) {
        throw std::out_of_range("Idx >= rows number");
    }
    return rows[row];
}

bool Matrix::operator==(const Matrix& other) const
{
    if (this == &other) {
        return true;
    }

    if (GetRows() != other.GetRows() || GetColumns() != other.GetColumns()) {
        return false;
    }

    for (std::size_t i = 0; i < rows_num; ++i) {
        if (rows[i] != other.rows[i]) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const
{
    return !(*this == other);
}

Matrix& Matrix::operator*=(int val)
{
    for (std::size_t i = 0; i < rows_num; ++i) {
        rows[i] *= val;
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
    if (GetRows() != other.GetRows() || GetColumns() != other.GetColumns()) {
        throw std::invalid_argument("Dimensions size are different");
    }

    for (std::size_t i = 0; i < rows_num; ++i) {
        rows[i] += other.rows[i];
    }

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
    os << "{\n";
    for (std::size_t i = 0; i < matrix.GetRows(); ++i) {
        os << '\t' << matrix[i] << '\n';
    }
    os << "}";
    return os;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
    Matrix result(lhs);
    result += rhs;
    return result;
}

Matrix operator*(const Matrix& lhs, int rhs)
{
    Matrix result(lhs);
    result *= rhs;
    return result;
}

Matrix operator*(int lhs, const Matrix& rhs)
{
    return rhs * lhs;
}
