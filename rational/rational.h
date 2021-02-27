#ifndef RATIONAL_RATIONAL_H
#define RATIONAL_RATIONAL_H

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
    Rational(int x) : p(x), q(1){};  // NOLINT
    Rational(int x, int y);
    void Reduce();
    int GetNumerator() const;
    int GetDenominator() const;
    void SetNumerator(int x);
    void SetDenominator(int y);
    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational& operator/=(const Rational& other);
    Rational operator+() const;
    Rational operator-() const;
    bool operator<(Rational& other) const;
    bool operator>(Rational& other) const;
    bool operator<=(Rational& other) const;
    bool operator>=(Rational& other) const;
    bool operator==(Rational& other) const;
    bool operator!=(Rational& other) const;
    Rational& operator++();
    Rational& operator--();
    Rational operator++(int);
    Rational operator--(int);
    friend std::istream& operator>>(std::istream& is, Rational& value);
    friend std::ostream& operator<<(std::ostream& os, const Rational& rt);
};

Rational operator+(const Rational& x, const Rational& y);
Rational operator-(const Rational& x, const Rational& y);
Rational operator*(const Rational& x, const Rational& y);
Rational operator/(const Rational& x, const Rational& y);

#endif