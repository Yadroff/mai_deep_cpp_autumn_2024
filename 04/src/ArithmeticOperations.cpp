#include "BigInt.h"

BigInt BigInt::operator+() const {
    return *this;
}

BigInt BigInt::operator-() const {
    BigInt copy(*this);
    copy.negative_ = !this->negative_;
    return copy;
}

BigInt operator+(const BigInt &left, const BigInt &right) {
    if (left.negative_) {
        if (right.negative_) {
            // оба отрицательных
            return -(-left + (-right));
        } else {
            // левое число отрицательное
            return right - (-left);
        }
    } else if (right.negative_) {
        // право число отрицательное
        return left - (-right);
    }
    // оба числа положительные
    int perenos = 0; // не нашел адекватного названия на англе
    BigInt res(left);
    size_t size = std::max(left.digits_.size(), right.digits_.size());
    res.digits_.resize(std::max(left.digits_.size(), right.digits_.size()));
    for (size_t i = 0; i < size; ++i) {
        res.digits_[i] += perenos + right[i];
        perenos = res.digits_[i] / BigInt::BASE;
        res.digits_[i] %= BigInt::BASE;
    }
    if (perenos) {
        res.digits_.emplace_back(perenos);
    }
    return res;
}

BigInt operator-(const BigInt &left, const BigInt &right) {
    if (right.negative_) {
        // - на - дает +
        return left + (-right);
    }
    if (left.negative_) {
        // вычитание = сложение двух отрицательных => выносим -
        return -(-left + right);
    }
    if (left < right) {
        // меняем местами, чтобы меньшее число вычиталось из большего
        return -(right - left);
    }
    BigInt res(left);
    int perenos = 0;
    for (ssize_t i = 0; i < res.digits_.size(); ++i) {
        res.digits_[i] -= perenos + right[i];
        perenos = res.digits_[i] < 0;
        if (perenos) {
            res.digits_[i] += BigInt::BASE;
        }
    }
    res.negative_ = false;
    res.deleteLeadingZeroes();
    return res;
}

BigInt &BigInt::operator+=(const BigInt &val) {
    return *this = (*this + val);
}

BigInt &BigInt::operator-=(const BigInt &val) {
    return *this = (*this - val);
}

BigInt &BigInt::operator++() {
    (*this) += 1;
    return *this;

}

BigInt BigInt::operator++(int) {
    BigInt old(*this);
    (*this) += 1;
    return old;
}

BigInt &BigInt::operator--() {
    (*this) -= 1;
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt old(*this);
    (*this) -= 1;
    return old;
}