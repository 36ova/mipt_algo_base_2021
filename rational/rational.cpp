#include "rational.h"

Rational::Rational(const int x, const int y) {
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

void Rational::SetNumerator(const int x) {
    p = x;
    Reduce();
}

void Rational::SetDenominator(const int y) {
    q = y;
    Reduce();
}

Rational& Rational::operator+=(const Rational& other) {
    if (other == *this) {
        p *= 2;
    } else {
        int gcd = get_gcd(q, other.q);
        p = p * (other.q / gcd) + other.p * (q / gcd);
        q = q * other.q / gcd;
    }
    Reduce();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    if (other == *this) {
        p = 0;
        q = 1;
    } else {
        *this += (-other);
        Reduce();
    }
    return *this;
}

Rational& Rational::operator*=(const Rational& other) {
    p *= other.p;
    q *= other.q;
    Reduce();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    if (other == *this) {
        p = q = 1;
    } else {
        p *= other.q;
        q *= other.p;
        Reduce();
    }
    return *this;
}

Rational Rational::operator+() const {
    return Rational(p, q);
}

Rational Rational::operator-() const {
    return Rational(-p, q);
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

bool operator<(const Rational& x, const Rational& y) {
    int fold = x.GetNumerator() * y.GetDenominator();
    return fold < y.GetNumerator() * x.GetDenominator();
}

bool operator>(const Rational& x, const Rational& y) {
    return y < x;
}

bool operator<=(const Rational& x, const Rational& y) {
    return !(x > y);
}

bool operator>=(const Rational& x, const Rational& y) {
    return !(x < y);
}

bool operator==(const Rational& x, const Rational& y) {
    int fold = x.GetNumerator() * y.GetDenominator();
    return fold == y.GetNumerator() * x.GetDenominator();
}

bool operator!=(const Rational& x, const Rational& y) {
    return !(x == y);
}
