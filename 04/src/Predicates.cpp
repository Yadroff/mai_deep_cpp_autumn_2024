#include "BigInt.h"

bool operator==(const BigInt &left, const BigInt &right) {
    if (left.negative_ != right.negative_ or left.digits_.size() != right.digits_.size()) {
        // если разного знака или разного размера, то они не равны (имея однозначное представление 0, можно не опасаться ошибок)
        return false;
    }
    for (ssize_t i = 0; i < left.digits_.size(); ++i) {
        if (left.digits_[i] != right.digits_[i]) {
            return false;
        }
    }
    return true;
}

bool operator<(const BigInt &left, const BigInt &right) {
    // случаи разных знаков
    if (left.negative_ and !right.negative_) {
        return true; // отрицательное число меньше положительного
    } else if (!left.negative_ and right.negative_) {
        // положительное число не меньше отрицательного
        return false;
    }
    // случай одинаковых знаков
    if (left.digits_.size() != right.digits_.size()) {
        return (left.negative_) ? left.digits_.size() > right.digits_.size()
                                : left.digits_.size() < right.digits_.size();
    }
    for (long long i = left.digits_.size() - 1; i >= 0; --i) {
        if (left.digits_[i] != right.digits_[i]) {
            return left.negative_ ? left.digits_[i] > right.digits_[i] :left.digits_[i] < right.digits_[i];
        }
        if (i == 0) {
            break;
        }
    }
    // если оба числа равны
    return false;
}

bool operator!=(const BigInt &left, const BigInt &right) {
    return !(left == right);
}

bool operator>(const BigInt &left, const BigInt &right) {
    return (right < left);
}

bool operator>=(const BigInt &left, const BigInt &right) {
    return !(left < right);
}

bool operator<=(const BigInt &left, const BigInt &right) {
    return !(left > right);
}