#ifndef BIG_INTEGER_BIG_INTEGER_H
#define BIG_INTEGER_BIG_INTEGER_H

#include <cstring>
#include <stdexcept>
#include <iostream>

class BigIntegerOverflow : public std::runtime_error {
public:
    BigIntegerOverflow() : std::runtime_error("BigIntegerOverflow") {
    }
};

class BigIntegerDivisionByZero : public std::runtime_error {
public:
    BigIntegerDivisionByZero() : std::runtime_error("BigIntegerDivisionByZero") {
    }
};

template <size_t NDigits = 40000>
class BigInteger {
private:
    const size_t blocks_ = NDigits / 4 + 1;
    bool negative_ = false;
    int number_[NDigits / 4 + 1];

    void Add(const BigInteger<NDigits>& other);
    void Subtract(const BigInteger<NDigits>& other);
    void TenTimes();
    void TenTimes(const size_t repeat);
    size_t Digits() const;
    void Multiply(const int digit);
    void MultiplyByBlock(size_t block);

public:
    BigInteger();
    BigInteger(int num);      //  NOLINT
    BigInteger(int64_t num);  //  NOLINT
    explicit BigInteger(const char* str);
    BigInteger(const BigInteger<NDigits>& other);
    bool IsNegative() const;
    BigInteger<NDigits>& operator=(const BigInteger<NDigits>& other);
    BigInteger<NDigits> operator+() const;
    BigInteger<NDigits> operator-() const;
    BigInteger<NDigits>& operator+=(const BigInteger<NDigits>& other);
    BigInteger<NDigits>& operator-=(const BigInteger<NDigits>& other);
    BigInteger<NDigits>& operator*=(const BigInteger<NDigits>& other);
    BigInteger<NDigits>& operator++();
    BigInteger<NDigits>& operator--();
    BigInteger<NDigits> operator++(int);
    BigInteger<NDigits> operator--(int);
    explicit operator bool() const;
    friend std::istream& operator>>(std::istream& is, BigInteger<NDigits>& big_num) {
        for (size_t i = 0; i < big_num.blocks_; ++i) {
            big_num.number_[i] = 0;
        }
        const int max_string_size = 400;
        char str[max_string_size];
        is >> str;
        if (str[0] == '-') {
            big_num.negative_ = true;
        }
        size_t length = ((str[0] == '-' || str[0] == '+') ? strlen(str) - 1 : strlen(str));
        if (length > NDigits) {
            throw BigIntegerOverflow{};
        }
        length = ((str[0] == '-' || str[0] == '+') ? length + 1 : length);
        const char* ptr = &str[length];
        const char* end = ((str[0] == '-' || str[0] == '+') ? &str[1] : &str[0]);
        size_t counter = 0;
        while (ptr > end) {
            size_t block = 0;
            --ptr;
            block += int(*ptr - '0') * 1;
            if (ptr > end) {
                --ptr;
                block += int(*ptr - '0') * 10;
            }
            if (ptr > end) {
                --ptr;
                block += int(*ptr - '0') * 100;
            }
            if (ptr > end) {
                --ptr;
                block += int(*ptr - '0') * 1000;
            }
            big_num.number_[counter] = block;
            ++counter;
        }
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const BigInteger<NDigits>& big_num) {
        if (big_num.IsNegative()) {
            os << "-";
        }
        int ind = big_num.blocks_ - 1;
        while (big_num.number_[ind] == 0 && ind > 0) {
            --ind;
        }
        os << big_num.number_[ind];
        --ind;
        if (ind >= 0) {
            for (; ind >= 0; --ind) {
                if (big_num.number_[ind] / 10 == 0) {
                    os << "000";
                } else if (big_num.number_[ind] / 100 == 0) {
                    os << "00";
                } else if (big_num.number_[ind] / 1000 == 0) {
                    os << "0";
                }
                os << big_num.number_[ind];
            }
        }
        return os;
    }
    friend bool AbsLess(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
        for (int i = first.blocks_ - 1; i >= 0; --i) {
            if (first.number_[i] > second.number_[i]) {
                return false;
            }
            if (first.number_[i] < second.number_[i]) {
                return true;
            }
        }
        return false;
    }
    friend bool AbsEqual(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
        for (int i = first.blocks_ - 1; i >= 0; --i) {
            if (first.number_[i] != second.number_[i]) {
                return false;
            }
        }
        return true;
    }
    friend bool operator<(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
        if (first.IsNegative() && !second.IsNegative()) {
            return true;
        }
        if (!first.IsNegative() && second.IsNegative()) {
            return false;
        }
        if (!first.IsNegative() && !second.IsNegative()) {
            return AbsLess(first, second);
        }
        return AbsLess(second, first);
    }
    friend bool operator==(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
        if (first.IsNegative() != second.IsNegative()) {
            return false;
        }
        return AbsEqual(first, second);
    }
};

template <size_t NDigits>
BigInteger<NDigits>::BigInteger() : negative_(false) {
    for (size_t i = 0; i < blocks_; ++i) {
        this->number_[i] = 0;
    }
}

template <size_t NDigits>
BigInteger<NDigits>::BigInteger(int num) : negative_(false) {
    for (size_t i = 0; i < blocks_; ++i) {
        this->number_[i] = 0;
    }
    if (num < 0) {
        this->negative_ = true;
        num *= -1;
    }
    size_t counter = 0;
    while (num != 0) {
        int block = num % 10000;
        if (counter >= blocks_) {
            throw BigIntegerOverflow{};
        }
        this->number_[counter] = block;
        num /= 10000;
        ++counter;
    }
}

template <size_t NDigits>
BigInteger<NDigits>::BigInteger(int64_t num) : negative_(false) {
    for (size_t i = 0; i < blocks_; ++i) {
        this->number_[i] = 0;
    }
    if (num < 0) {
        this->negative_ = true;
        num *= -1;
    }
    size_t counter = 0;
    while (num != 0) {
        int block = static_cast<int>(num % 10000);
        if (counter >= blocks_) {
            throw BigIntegerOverflow{};
        }
        this->number_[counter] = block;
        num /= 10000;
        ++counter;
    }
}

template <size_t NDigits>
BigInteger<NDigits>::BigInteger(const char* str) : negative_(false) {
    for (size_t i = 0; i < blocks_; ++i) {
        this->number_[i] = 0;
    }
    if (str[0] == '-') {
        this->negative_ = true;
    }
    size_t length = ((str[0] == '-' || str[0] == '+') ? strlen(str) - 1 : strlen(str));
    if (length > NDigits) {
        throw BigIntegerOverflow{};
    }
    length = ((str[0] == '-' || str[0] == '+') ? length + 1 : length);
    const char* ptr = &str[length];
    const char* end = ((str[0] == '-' || str[0] == '+') ? &str[1] : &str[0]);
    size_t counter = 0;
    while (ptr > end) {
        size_t block = 0;
        --ptr;
        block += int(*ptr - '0') * 1;
        if (ptr > end) {
            --ptr;
            block += int(*ptr - '0') * 10;
        }
        if (ptr > end) {
            --ptr;
            block += int(*ptr - '0') * 100;
        }
        if (ptr > end) {
            --ptr;
            block += int(*ptr - '0') * 1000;
        }
        this->number_[counter] = block;
        ++counter;
    }
}

template <size_t NDigits>
BigInteger<NDigits>::BigInteger(const BigInteger<NDigits>& other) {
    for (size_t i = 0; i < blocks_; ++i) {
        this->number_[i] = other.number_[i];
    }
    this->negative_ = other.negative_;
}

template <size_t NDigits>
bool BigInteger<NDigits>::IsNegative() const {
    return this->negative_;
}

template <size_t NDigits>
BigInteger<NDigits>& BigInteger<NDigits>::operator=(const BigInteger<NDigits>& other) {
    if (*this != other) {
        this->negative_ = other.negative_;
        for (size_t i = 0; i < blocks_; ++i) {
            this->number_[i] = other.number_[i];
        }
    }
    return *this;
}

template <size_t NDigits>
BigInteger<NDigits> BigInteger<NDigits>::operator+() const {
    return *this;
}

template <size_t NDigits>
BigInteger<NDigits> BigInteger<NDigits>::operator-() const {
    BigInteger<NDigits> result(*this);
    result.negative_ = !(this->negative_);
    return result;
}

template <size_t NDigits>
void BigInteger<NDigits>::Add(const BigInteger<NDigits>& other) {
    size_t inc = 0;
    for (size_t i = 0; i < blocks_; ++i) {
        int sum = number_[i] + other.number_[i];
        number_[i] = (sum + inc) % 10000;
        inc = (sum + inc) / 10000;
    }
    if (inc != 0) {
        throw BigIntegerOverflow{};
    }
}

template <size_t NDigits>
void BigInteger<NDigits>::Subtract(const BigInteger<NDigits>& other) {
    if (AbsEqual(*this, other)) {
        *this = BigInteger<NDigits>();
    } else if (!AbsLess(*this, other)) {
        for (size_t i = 0; i < blocks_; ++i) {
            if (number_[i] < other.number_[i]) {
                number_[i] += 10000;
                number_[i + 1] -= 1;
            }
            number_[i] -= other.number_[i];
        }
    } else {
        for (size_t i = 0; i < blocks_; ++i) {
            if (other.number_[i] < number_[i]) {
                number_[i] -= 10000;
                number_[i + 1] += 1;
            }
            number_[i] = other.number_[i] - number_[i];
        }
        negative_ = !(negative_);
    }
}

template <size_t NDigits>
BigInteger<NDigits>& BigInteger<NDigits>::operator+=(const BigInteger<NDigits>& other) {
    if (this == &other) {
        this->Multiply(2);
    } else {
        if (negative_ == other.negative_) {
            this->Add(other);
        } else {
            this->Subtract(other);
        }
    }
    return *this;
}

template <size_t NDigits>
BigInteger<NDigits>& BigInteger<NDigits>::operator-=(const BigInteger<NDigits>& other) {
    if (this == &other) {
        this->negative_ = false;
        for (size_t i = 0; i < blocks_; ++i) {
            this->number_[i] = 0;
        }
    } else {
        if (negative_ == other.negative_) {
            this->Subtract(other);
        } else {
            this->Add(other);
        }
    }
    return *this;
}

template <size_t NDigits>
BigInteger<NDigits>& BigInteger<NDigits>::operator*=(const BigInteger<NDigits>& other) {
    BigInteger<NDigits> result;
    for (size_t i = 0; i < blocks_; ++i) {
        BigInteger<NDigits> summand(*this);
        summand.MultiplyByBlock(other.number_[i]);
        summand.TenTimes(i * 4);
        if (summand.IsNegative()) {
            summand.negative_ = false;
        }
        result += summand;
    }
    if (this->negative_ != other.negative_ && result.number_[0] != 0) {
        result.negative_ = true;
    }
    *this = result;
    return *this;
}

template <size_t NDigits>
void BigInteger<NDigits>::TenTimes() {
    size_t ind = 0;
    while (ind < blocks_ && number_[ind] == 0) {
        ++ind;
    }
    if (ind == blocks_) {
        return;
    }
    size_t inc = number_[ind] / 1000;
    number_[ind] = (number_[ind] * 10) % 10000;
    ++ind;
    while (ind < blocks_) {
        number_[ind] = number_[ind] * 10 + inc;
        if (number_[ind] > 9999) {
            inc = number_[ind] / 10000;
            number_[ind] %= 10000;
            ++ind;
        } else {
            inc = 0;
            ++ind;
        }
    }
    if (inc != 0) {
        throw BigIntegerOverflow{};
    }
}

template <size_t NDigits>
void BigInteger<NDigits>::TenTimes(const size_t repeat) {
    if (repeat != 0) {
        for (size_t i = 0; i < repeat; ++i) {
            this->TenTimes();
        }
    }
}

template <size_t NDigits>
size_t BigInteger<NDigits>::Digits() const {
    int ind = blocks_ - 1;
    while (number_[ind] == 0) {
        --ind;
    }
    if (ind == -1) {
        return 0;
    }
    size_t result = 4 * ind;
    if (number_[ind] / 10 == 0) {
        result += 1;
    } else if (number_[ind] / 100 == 0) {
        result += 2;
    } else if (number_[ind] / 1000 == 0) {
        result += 3;
    } else {
        result += 4;
    }
    return result;
}

template <size_t NDigits>
void BigInteger<NDigits>::Multiply(const int digit) {
    size_t inc = 0;
    for (size_t i = 0; i < blocks_; ++i) {
        int product = number_[i] * digit;
        number_[i] = (product + inc) % 10000;
        inc = (product + inc) / 10000;
    }
    if (inc != 0) {
        throw BigIntegerOverflow{};
    }
}

template <size_t NDigits>
void BigInteger<NDigits>::MultiplyByBlock(size_t block) {
    size_t digits[4];
    for (size_t i = 0; i < 4; ++i) {
        digits[i] = block % 10;
        block /= 10;
    }
    BigInteger<NDigits> result;
    for (size_t i = 0; i < 4; ++i) {
        BigInteger<NDigits> summand(*this);
        summand.Multiply(digits[i]);
        summand.TenTimes(i);
        result += summand;
    }
    *this = result;
}

template <size_t NDigits>
BigInteger<NDigits>& BigInteger<NDigits>::operator++() {
    *this += 1;
    return *this;
}

template <size_t NDigits>
BigInteger<NDigits>& BigInteger<NDigits>::operator--() {
    *this -= 1;
    return *this;
}

template <size_t NDigits>
BigInteger<NDigits> BigInteger<NDigits>::operator++(int) {
    auto old_value = *this;
    *this += 1;
    return old_value;
}

template <size_t NDigits>
BigInteger<NDigits> BigInteger<NDigits>::operator--(int) {
    auto old_value = *this;
    *this -= 1;
    return old_value;
}

template <size_t NDigits>
BigInteger<NDigits>::operator bool() const {
    BigInteger<NDigits> null;
    return !(*this == null);
}

template <size_t NDigits>
BigInteger<NDigits> operator+(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
    BigInteger<NDigits> result(first);
    result += second;
    return result;
}

template <size_t NDigits>
BigInteger<NDigits> operator-(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
    BigInteger<NDigits> result(first);
    result -= second;
    return result;
}

template <size_t NDigits>
BigInteger<NDigits> operator*(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
    BigInteger<NDigits> result(first);
    result *= second;
    return result;
}

template <size_t NDigits>
bool operator>(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
    return (second < first);
}

template <size_t NDigits>
bool operator<=(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
    return !(second < first);
}

template <size_t NDigits>
bool operator>=(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
    return !(first < second);
}

template <size_t NDigits>
bool operator!=(const BigInteger<NDigits>& first, const BigInteger<NDigits>& second) {
    return !(first == second);
}

#endif
