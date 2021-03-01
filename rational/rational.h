#ifndef RATIONAL_RATIONAL_H
#define RATIONAL_RATIONAL_H

//#include <util/constants.h>
#include <cstdlib>
#include <istream>
#include <ostream>

class RationalDivisionByZero : public std::runtime_error {
public:
    RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
    }
};

int gcd(int a, int b);

class Rational {
private:
    int p;
    int q;

public:
    Rational() : p(0), q(1){};
    Rational(const int x) : p(x), q(1){};  // NOLINT
    Rational(const int x, const int y);
    void Reduce();
    int GetNumerator() const;
    int GetDenominator() const;
    void SetNumerator(const int x);
    void SetDenominator(const int y);
    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational& operator/=(const Rational& other);
    Rational operator+() const;
    Rational operator-() const;
    Rational& operator++();
    Rational& operator--();
    Rational operator++(int);
    Rational operator--(int);
    friend std::istream& operator>>(std::istream& is, Rational& value);
    friend std::ostream& operator<<(std::ostream& os, const Rational& value);
};

Rational operator+(const Rational& x, const Rational& y);
Rational operator-(const Rational& x, const Rational& y);
Rational operator*(const Rational& x, const Rational& y);
Rational operator/(const Rational& x, const Rational& y);
bool operator<(const Rational& x, const Rational& y);
bool operator>(const Rational& x, const Rational& y);
bool operator<=(const Rational& x, const Rational& y);
bool operator>=(const Rational& x, const Rational& y);
bool operator==(const Rational& x, const Rational& y);
bool operator!=(const Rational& x, const Rational& y);

#endif  // RATIONAL_RATIONAL_H