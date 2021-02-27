#include "rational.h"
#include <iostream>
#include <util/constants.h>

Rational::Rational(int x, int y) {
    p = x;
    q = y;
    Reduce();
}

int get_gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (a != 0) {
        if (b > a) {
            std::swap(a, b);
        }
        a %= b;
    }
    return b;
}

void Rational::Reduce() {
    if (q == 0) {
        throw RationalDivisionByZero{};
    }
    int gcd = get_gcd(p, q);
    if (q < 0) {
        p *= -1;
        q *= -1;
    }
    p /= gcd;
    q /= gcd;
}

int Rational::GetNumerator() const {
    return p;
}

int Rational::GetDenominator() const {
    return q;
}

void Rational::SetNumerator(int x) {
    p = x;
    Reduce();
}

void Rational::SetDenominator(int y) {
    q = y;
    Reduce();
}

Rational& Rational::operator+=(const Rational& other) {
    int a = GetDenominator();
    int b = other.GetDenominator();
    int gcd = get_gcd(a, b);
    int num = GetNumerator() * (b / gcd) + other.GetNumerator() * (a / gcd);
    SetNumerator(num);
    SetDenominator(a * b / gcd);
    Reduce();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    *this += (-other);
    Reduce();
    return *this;
}

Rational& Rational::operator*=(const Rational& other) {
    p *= other.p;
    q *= other.q;
    Reduce();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    p *= other.q;
    q *= other.p;
    Reduce();
    return *this;
}

Rational Rational::operator+() const {
    return Rational(p, q);
}

Rational Rational::operator-() const {
    return Rational(-p, q);
}

bool Rational::operator<(Rational& other) const {
    return p * other.q < other.p * q;
}

bool Rational::operator>(Rational& other) const {
    return p * other.q > other.p * q;
}

bool Rational::operator<=(Rational& other) const {
    return !(*this > other);
}

bool Rational::operator>=(Rational& other) const {
    return !(*this < other);
}

bool Rational::operator==(Rational& other) const {
    return p * other.q == other.p * q;
}

bool Rational::operator!=(Rational& other) const {
    return !(*this == other);
}

Rational& Rational::operator++() {
    p += q;
    Reduce();
    return *this;
}

Rational& Rational::operator--() {
    p -= q;
    Reduce();
    return *this;
}

Rational Rational::operator++(int) {
    auto old_value = *this;
    p += q;
    Reduce();
    return old_value;
}

Rational Rational::operator--(int) {
    auto old_value = *this;
    p -= q;
    Reduce();
    return old_value;
}

std::istream& operator>>(std::istream& is, Rational& value) {
    const int MaxStringSize = 32;
    char str[MaxStringSize];
    is >> str;
    char* end_ptr;
    value.p = strtoll(str, &end_ptr, 10);
    if (*end_ptr != 0) {
        ++end_ptr;
        value.q = strtoll(end_ptr, &end_ptr, 10);
    } else {
        value.q = 1;
    }
    value.Reduce();
    return is;
}

std::ostream& operator<<(std::ostream& os, const Rational& value) {
    if (value.q == 1) {
        os << value.p;
    } else {
        os << value.p << '/' << value.q;
    }
    return os;
}

Rational operator+(const Rational& x, const Rational& y) {
    Rational res;
    int a = x.GetDenominator();
    int b = y.GetDenominator();
    int gcd = get_gcd(a, b);
    int num = x.GetNumerator() * (b / gcd) + y.GetNumerator() * (a / gcd);
    res.SetNumerator(num);
    res.SetDenominator(a * b / gcd);
    res.Reduce();
    return res;
}

Rational operator-(const Rational& x, const Rational& y) {
    return (x + (-y));
}

Rational operator*(const Rational& x, const Rational& y) {
    Rational res;
    res.SetNumerator(x.GetNumerator() * y.GetNumerator());
    res.SetDenominator(x.GetDenominator() * y.GetDenominator());
    res.Reduce();
    return res;
}

Rational operator/(const Rational& x, const Rational& y) {
    Rational res;
    res.SetNumerator(x.GetNumerator() * y.GetDenominator());
    res.SetDenominator(x.GetDenominator() * y.GetNumerator());
    res.Reduce();
    return res;
}
