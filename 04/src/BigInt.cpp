#include <iomanip>

#include "BigInt.h"

BigInt operator*(const BigInt &left, const BigInt &right) {
    BigInt result;
    result.negative_ = left.negative_ or right.negative_;
    std::vector<unsigned long long> resultDigits;
    Fourier::multiply(left.digits_, right.digits_, resultDigits);
    unsigned long long perenos = 0;
    result.digits_.resize(resultDigits.size());
    for (auto &resultDigit: resultDigits) {
        resultDigit += perenos;
        perenos = resultDigit / BigInt::BASE;
        resultDigit %= BigInt::BASE;
    }
    while (perenos) {
        resultDigits.emplace_back(perenos % BigInt::BASE);
        perenos /= BigInt::BASE;
    }
    result.digits_.resize(resultDigits.size());
    for (size_t i = 0; i < resultDigits.size(); ++i) {
        result.digits_[i] = (int) resultDigits[i];
    }
    result.deleteLeadingZeroes();
    return result;
}

bool BigInt::Odd() const {
    if (digits_.empty()) {
        return false;
    }
    return digits_[0] & 1;
}

bool BigInt::Even() const {
    return !Odd();
}

BigInt &BigInt::operator*=(const BigInt &val) {
    return (*this) = (*this * val);
}

BigInt operator/(const BigInt &left, const BigInt &right) {
    if (right == 0) {
        throw std::runtime_error("Divide by zero");
    }
    BigInt b = right;
    b.negative_ = false;
    BigInt result, current;
    result.digits_.resize(left.digits_.size());
    for (long long i = static_cast<long long>(left.digits_.size()) - 1; i >= 0; --i) {
        current.shiftRight();
        current.digits_[0] = left.digits_[i];
        current.deleteLeadingZeroes();
        int x = 0, l = 0, r = BigInt::BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInt t = b * m;
            if (t <= current) {
                x = m;
                l = m + 1;
            } else r = m - 1;
        }

        result.digits_[i] = x;
        current = current - b * x;
    }

    result.negative_ = left.negative_ ^ right.negative_;
    result.deleteLeadingZeroes();
    return result;
}

BigInt &BigInt::operator/=(const BigInt &val) {
    return (*this) = (*this / val);
}

BigInt operator^(BigInt a, BigInt n) {
    BigInt result(1);
    while (n != 0) {
        if (n.Odd()) {
            result *= a;
        }
        a *= a;
        n /= 2;
    }
    return result;
}

BigInt operator^(BigInt a, unsigned long long int n) {
    BigInt result(1);
    while (n != 0) {
        if (n & 1) {
            result *= a;
        }
        a *= a;
        n >>= 2;
    }
    return result;
}

BigInt operator/(const BigInt &left, long long int right) {
    BigInt res(left);
    bool negative = right < 0;
    res.negative_ = left.negative_ ^ (negative);
    right = negative ? right * -1 : right;
    unsigned long long reserve = 0;
    for (ssize_t i = res.digits_.size() - 1; i < res.digits_.size(); --i) {
        unsigned long long cur = res.digits_[i] + reserve * BigInt::BASE;
        res.digits_[i] = static_cast<int>(cur / right);
        reserve = cur % right;
    }
    res.deleteLeadingZeroes();
    return res;
}

long long BigInt::toLL() const {
    if (digits_.size() < 2) {
        return digits_[0] * (negative_ ? -1 : 1);
    }
    return (digits_[0] + BASE * digits_[1]) * (negative_ ? -1 : 1);
}

long double BigInt::toDouble() const {
    long double res = 0;
    for (ssize_t i = digits_.size() - 1; i < digits_.size(); --i) {
        res = res * BASE + digits_[i];
    }
    return res;
}

void BigInt::shiftRight() {
    if (this->digits_.empty()) {
        this->digits_.emplace_back(0);
        return;
    }
    this->digits_.emplace_back(this->digits_[this->digits_.size() - 1]);
    // здесь размер массива равен как минимум двум и перебор идет до предпоследнего разряда,
    // поэтому i имеет "верный" тип size_t
    for (size_t i = this->digits_.size() - 2; i > 0; --i) {
        this->digits_[i] = this->digits_[i - 1];
    }
    this->digits_[0] = 0;
}

int BigInt::operator[](size_t index) const {
    return (index < static_cast<size_t>(digits_.size())) ? digits_[index] : 0;
}
