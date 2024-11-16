#pragma once

#include "matrix_row.h"

class Matrix {
public:
    Matrix(std::size_t rows = 1, std::size_t columns = 1, int default_val = {});
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;
    ~Matrix();

    Row& operator[](std::size_t row);
    const Row& operator[](std::size_t row) const;

    std::size_t GetRows() const { return rows_num; };
    std::size_t GetColumns() const { return rows[0].GetSize(); };

    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    Matrix& operator*=(int val);
    Matrix& operator+=(const Matrix& other);

private:
    Matrix() = default;

    std::size_t rows_num = 0;
    Row* rows = nullptr;
};

std::ostream& operator<<(std::ostream& os, const Matrix& row);
Matrix operator+(const Matrix& lhs, const Matrix& rhs);
Matrix operator*(const Matrix& lhs, int rhs);
Matrix operator*(int lhs, const Matrix& rhs);