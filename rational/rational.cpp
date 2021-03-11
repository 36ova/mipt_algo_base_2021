#include "rational.h"

Rational::Rational(const int first, const int second) {
    num_ = first;
    denom_ = second;
    Reduce();
}

int GetGcd(int first, int second) {
    first = abs(first);
    second = abs(second);
    while (first != 0) {
        if (second > first) {
            std::swap(first, second);
        }
        first %= second;
    }
    return second;
}

void Rational::Reduce() {
    if (denom_ == 0) {
        throw RationalDivisionByZero{};
    }
    int gcd = GetGcd(num_, denom_);
    if (denom_ < 0) {
        num_ *= -1;
        denom_ *= -1;
    }
    num_ /= gcd;
    denom_ /= gcd;
}

int Rational::GetNumerator() const {
    return num_;
}

int Rational::GetDenominator() const {
    return denom_;
}

void Rational::SetNumerator(const int number) {
    num_ = number;
    Reduce();
}

void Rational::SetDenominator(const int number) {
    denom_ = number;
    Reduce();
}

Rational& Rational::operator+=(const Rational& other) {
    if (other == *this) {
        num_ *= 2;
    } else {
        int gcd = GetGcd(denom_, other.denom_);
        num_ = num_ * (other.denom_ / gcd) + other.num_ * (denom_ / gcd);
        denom_ = denom_ * other.denom_ / gcd;
    }
    Reduce();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    if (other == *this) {
        num_ = 0;
        denom_ = 1;
    } else {
        *this += (-other);
        Reduce();
    }
    return *this;
}

Rational& Rational::operator*=(const Rational& other) {
    num_ *= other.num_;
    denom_ *= other.denom_;
    Reduce();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    if (other == *this) {
        num_ = denom_ = 1;
    } else {
        num_ *= other.denom_;
        denom_ *= other.num_;
        Reduce();
    }
    return *this;
}

Rational Rational::operator+() const {
    return Rational(num_, denom_);
}

Rational Rational::operator-() const {
    return Rational(-num_, denom_);
}

Rational& Rational::operator++() {
    num_ += denom_;
    Reduce();
    return *this;
}

Rational& Rational::operator--() {
    num_ -= denom_;
    Reduce();
    return *this;
}

Rational Rational::operator++(int) {
    auto old_value = *this;
    num_ += denom_;
    Reduce();
    return old_value;
}

Rational Rational::operator--(int) {
    auto old_value = *this;
    num_ -= denom_;
    Reduce();
    return old_value;
}

std::istream& operator>>(std::istream& is, Rational& value) {
    const int MaxStringSize = 32;
    char str[MaxStringSize];
    is >> str;
    char* end_ptr;
    value.num_ = strtoll(str, &end_ptr, 10);
    if (*end_ptr != 0) {
        ++end_ptr;
        value.denom_ = strtoll(end_ptr, &end_ptr, 10);
    } else {
        value.denom_ = 1;
    }
    value.Reduce();
    return is;
}

std::ostream& operator<<(std::ostream& os, const Rational& value) {
    if (value.denom_ == 1) {
        os << value.num_;
    } else {
        os << value.num_ << '/' << value.denom_;
    }
    return os;
}

Rational operator+(const Rational& first, const Rational& second) {
    Rational result;
    int first_denom = first.GetDenominator();
    int second_denom = second.GetDenominator();
    int gcd = GetGcd(first_denom, second_denom);
    int num = first.GetNumerator() * (second_denom / gcd) + second.GetNumerator() * (first_denom / gcd);
    result.SetNumerator(num);
    result.SetDenominator(first_denom * second_denom / gcd);
    result.Reduce();
    return result;
}

Rational operator-(const Rational& first, const Rational& second) {
    return (first + (-second));
}

Rational operator*(const Rational& first, const Rational& second) {
    Rational result;
    result.SetNumerator(first.GetNumerator() * second.GetNumerator());
    result.SetDenominator(first.GetDenominator() * second.GetDenominator());
    result.Reduce();
    return result;
}

Rational operator/(const Rational& first, const Rational& second) {
    Rational result;
    result.SetNumerator(first.GetNumerator() * second.GetDenominator());
    result.SetDenominator(first.GetDenominator() * second.GetNumerator());
    result.Reduce();
    return result;
}

bool operator<(const Rational& first, const Rational& second) {
    int fold = first.GetNumerator() * second.GetDenominator();
    return fold < second.GetNumerator() * first.GetDenominator();
}

bool operator>(const Rational& first, const Rational& second) {
    return second < first;
}

bool operator<=(const Rational& first, const Rational& second) {
    return !(first > second);
}

bool operator>=(const Rational& first, const Rational& second) {
    return !(first < second);
}

bool operator==(const Rational& first, const Rational& second) {
    int fold = first.GetNumerator() * second.GetDenominator();
    return fold == second.GetNumerator() * first.GetDenominator();
}

bool operator!=(const Rational& first, const Rational& second) {
    return !(first == second);
}
