#pragma once

#include <iostream>

struct Row {
public:
    explicit Row(std::size_t size = 1, int val = {});
    Row(const Row& other);
    Row& operator=(const Row& other);
    Row(Row&& other);
    Row& operator=(Row&& other);
    ~Row();

    std::size_t GetSize() const { return size; }

    int& operator[](std::size_t idx);
    const int& operator[](std::size_t idx) const;

    bool operator==(const Row& other) const;
    bool operator!=(const Row& other) const;

    Row& operator+=(const Row& other);
    Row& operator*=(int val);

private:
    Row() = default;

    std::size_t size = 0;
    int* data = nullptr;
};

std::ostream& operator<<(std::ostream& os, const Row& row);
Row operator+(const Row& lhs, const Row& rhs);
Row operator*(const Row& lhs, int rhs);
Row operator*(int lhs, const Row& rhs);