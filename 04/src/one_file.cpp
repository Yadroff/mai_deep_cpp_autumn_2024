#include <iostream>
#include <vector>
#include <exception>

#include <complex>

#define all(v) v.begin(), v.end()

typedef std::complex<double> complex;

class Fourier {
public:
    static void fft(std::vector<complex> &, bool invert);

    static void multiply(const std::vector<int> &a, const std::vector<int> &b,
                         std::vector<unsigned long long> &res);
};

void Fourier::fft(std::vector<complex> &a, bool invert) {
    int n = (int) a.size();
    if (n == 1) return;

    std::vector<complex> a0(n / 2), a1(n / 2);
    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }
    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * M_PI / n * (invert ? -1 : 1);
    complex w(1), wn(std::cos(ang), std::sin(ang));
    for (int i = 0; i < n / 2; ++i) {
        a[i] = a0[i] + w * a1[i];
        a[i + n / 2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

void Fourier::multiply(const std::vector<int> &a, const std::vector<int> &b,
                       std::vector<unsigned long long> &res) {
    std::vector<complex> fa(all(a)), fb(all(b));
    size_t n = 1;
    while (n < std::max(fa.size(), fb.size())) {
        n <<= 1;
    }
    n <<= 1;
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    for (size_t i = 0; i < n; ++i) {
        fa[i] *= fb[i];
    }
    fft(fa, true);
    res.resize(n);
    for (size_t i = 0; i < n; ++i) {
        res[i] = (unsigned long long) (fa[i].real() + 0.5);
    }
}


class BigInt {
public:
    BigInt();

    BigInt(const std::string &str);

    BigInt(signed char);

    BigInt(unsigned char);

    BigInt(signed short);

    BigInt(unsigned short);

    BigInt(signed int);

    BigInt(unsigned int);

    BigInt(signed long);

    BigInt(unsigned long);

    BigInt(signed long long);

    BigInt(unsigned long long);

    friend std::ostream &operator<<(std::ostream &os, const BigInt &anInt);

    explicit operator std::string() const;

    friend bool operator==(const BigInt &left, const BigInt &right);

    friend bool operator<(const BigInt &left, const BigInt &right);

    friend bool operator!=(const BigInt &left, const BigInt &right);

    friend bool operator>(const BigInt &left, const BigInt &right);

    friend bool operator>=(const BigInt &left, const BigInt &right);

    friend bool operator<=(const BigInt &left, const BigInt &right);

    BigInt operator+() const;

    BigInt operator-() const;

    friend BigInt operator+(const BigInt &left, const BigInt &right);

    friend BigInt operator-(const BigInt &left, const BigInt &right);

    BigInt &operator+=(const BigInt &val);

    BigInt &operator-=(const BigInt &val);

    BigInt &operator++();

    BigInt operator++(int);

    BigInt &operator--();

    BigInt operator--(int);

    BigInt &operator*=(const BigInt &val);

    friend BigInt operator*(const BigInt &left, const BigInt &right);

    friend BigInt operator/(const BigInt &left, const BigInt &right);

    friend BigInt operator/(const BigInt &left, long long right);

    [[nodiscard]] long long toLL() const;

    [[nodiscard]] long double toDouble() const;

    BigInt &operator/=(const BigInt &val);

    friend BigInt operator^(BigInt a, BigInt n);

    friend BigInt operator^(BigInt a, unsigned long long n);


    bool odd(); // нечетное число

    bool even(); // четное число

    void shiftRight(); // смещение вправо на один разряд

private:
    void deleteLeadingZeroes();

    bool negative_;
    std::vector<int> digits_;
    static const int BASE = 1000000;
    static const int LENGTH = 6;
};



#include <iomanip>

BigInt::BigInt() : negative_(false), digits_(1, 0) {}

BigInt::BigInt(signed char c) {
    negative_ = (c < 0);
    digits_.emplace_back(std::abs(c));
}

BigInt::BigInt(unsigned char c) {
    negative_ = false;
    digits_.emplace_back(c);
}

BigInt::BigInt(short sh) {
    negative_ = (sh < 0);
    digits_.emplace_back(std::abs(sh));
}

BigInt::BigInt(unsigned short c) {
    negative_ = false;
    digits_.emplace_back(c);
}

BigInt::BigInt(int l) {
    negative_ = (l < 0);
    l = std::abs(l);
    do {
        this->digits_.emplace_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(unsigned int l) {
    negative_ = false;
    do {
        this->digits_.emplace_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(long l) {
    negative_ = (l < 0);
    l = std::abs(l);
    do {
        this->digits_.emplace_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(unsigned long l) {
    negative_ = false;
    do {
        this->digits_.emplace_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(long long int l) {
    negative_ = (l < 0);
    l = std::abs(l);
    do {
        this->digits_.emplace_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(unsigned long long int l) {
    negative_ = false;
    do {
        this->digits_.emplace_back(l % BigInt::BASE);
        l /= BigInt::BASE;
    } while (l != 0);
}

BigInt::BigInt(const std::string &str) {
    negative_ = false;
    if (str.empty()) {
        digits_.resize(1, 0); // для однозначного представления нуля
    } else {
        std::string digits = str;
        if (str[0] == '-' or str[0] == '+') {
            digits = str.substr(1);
            negative_ = (str[0] == '-');
        }
        for (long long i = digits.size(); i > 0; i -= LENGTH) {
            if (i < LENGTH) {
                digits_.emplace_back(std::atoi(digits.substr(0, i).c_str()));
            } else {
                digits_.emplace_back(std::atoi(digits.substr(i - BigInt::LENGTH, BigInt::LENGTH).c_str()));
            }
        }
        deleteLeadingZeroes();
    }
}

std::ostream &operator<<(std::ostream &os, const BigInt &anInt) {
    if (anInt.digits_.empty()) {
        os << 0;
        return os;
    }
    if (anInt.negative_) {
        os << '-';
    }
    os << anInt.digits_.back();
    /*if (anInt.digits_.size() > 1) {
        os << "\'";
    }*/
    for (long long i = anInt.digits_.size() - 2; i >= 0; --i) {
        os << std::setw(BigInt::LENGTH) << std::setfill('0') << anInt.digits_[i];
/*        if (i != 0) {
            os << "\'";
        }*/
    }
    return os;
}

BigInt::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

void BigInt::deleteLeadingZeroes() {
    while (digits_.size() > 1 and digits_.back() == 0) {
        digits_.pop_back();
    }
    if (digits_.size() == 1 and digits_.back() == 0) {
        negative_ = false;
    }
}

bool operator==(const BigInt &left, const BigInt &right) {
    if (left.negative_ != right.negative_ or left.digits_.size() != right.digits_.size()) {
        // если разного знака или разного размера, то они не равны (имея однозначное представление 0, можно не опасаться ошибок)
        return false;
    }
    for (size_t i = 0; i < left.digits_.size(); ++i) {
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
    for (size_t i = left.digits_.size(); true; --i) {
        if (left.digits_[i] != right.digits_[i]) {
            return left.digits_[i] < right.digits_[i];
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

BigInt BigInt::operator+() const {
    return {*this};
}

BigInt BigInt::operator-() const {
    BigInt copy(*this);
    copy.negative_ = !this->negative_;
    return {copy};
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
    res.digits_.resize(std::max(left.digits_.size(), right.digits_.size()));
    for (size_t i = 0; i < res.digits_.size(); ++i) {
        res.digits_[i] += perenos + right.digits_[i];
        perenos = res.digits_[i] >= BigInt::BASE;
        res.digits_[i] -= ((perenos) ? BigInt::BASE : 0);
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
    for (size_t i = 0; i < right.digits_.size(); ++i) {
        res.digits_[i] -= perenos + right.digits_[i];
        perenos = res.digits_[i] < 0;
        if (perenos) {
            res.digits_[i] += BigInt::BASE;
            res.digits_[i + 1] -= 1;
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
    result.digits_.resize(resultDigits.size());
    for (size_t i = 0; i < resultDigits.size(); ++i) {
        result.digits_[i] = (int) resultDigits[i];
    }
    result.deleteLeadingZeroes();
    return result;
}

bool BigInt::odd() {
    if (digits_.empty()) {
        return false;
    }
    return digits_[0] & 1;
}

bool BigInt::even() {
    return !odd();
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
        if (n.odd()) {
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
    for (size_t i = res.digits_.size() - 1; i < res.digits_.size(); --i) {
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
    for (size_t i = digits_.size() - 1; i < digits_.size(); --i) {
        res = res * BASE + digits_[i];
    }
    return res;
}

void BigInt::shiftRight() {
    if (this->digits_.empty()) {
        this->digits_.push_back(0);
        return;
    }
    this->digits_.push_back(this->digits_[this->digits_.size() - 1]);
    // здесь размер массива равен как минимум двум и перебор идет до предпоследнего разряда,
    // поэтому i имеет "верный" тип size_t
    for (size_t i = this->digits_.size() - 2; i > 0; --i) {
        this->digits_[i] = this->digits_[i - 1];
    }
    this->digits_[0] = 0;
}


int main() {
    std::string a, b, sign;
    while (std::cin >> a >> b >> sign) {
        BigInt first(a), second(b);
        if (sign == "+") {
            std::cout << (first + second) << std::endl;
        } else if (sign == "-") {
            if (first < second) {
                std::cout << "Error" << std::endl;
            } else {
                std::cout << (first - second) << std::endl;
            }
        } else if (sign == "*") {
            std::cout << (first * second) << std::endl;
        } else if (sign == "^") {
            if (first == 0 and second == 0) {
                std::cout << "Error" << std::endl;
            } else {
                std::cout << (first ^ second) << std::endl;
            }
        } else if (sign == "/") {
            if (second == 0) {
                std::cout << "Error" << std::endl;
            } else {
                std::cout << (first / second) << std::endl;
            }
        } else if (sign == ">") {
            std::string ans = (first > second) ? "true" : "false";
            std::cout << ans << std::endl;
        } else if (sign == "<"){
            std::string ans = (first < second) ? "true" : "false";
            std::cout << ans << std::endl;
        } else if (sign == "="){
            std::string ans = (first == second) ? "true" : "false";
            std::cout << ans << std::endl;
        }
    }
    return 0;
}
