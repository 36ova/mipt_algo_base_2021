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

int GetGcd(int first, int second);

class Rational {
private:
    int num_;
    int denom_;

public:
    Rational() : num_(0), denom_(1){};
    Rational(const int number) : num_(number), denom_(1){};  // NOLINT
    Rational(const int first, const int second);
    void Reduce();
    int GetNumerator() const;
    int GetDenominator() const;
    void SetNumerator(const int number);
    void SetDenominator(const int number);
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

Rational operator+(const Rational& first, const Rational& second);
Rational operator-(const Rational& first, const Rational& second);
Rational operator*(const Rational& first, const Rational& second);
Rational operator/(const Rational& first, const Rational& second);
bool operator<(const Rational& first, const Rational& second);
bool operator>(const Rational& first, const Rational& second);
bool operator<=(const Rational& first, const Rational& second);
bool operator>=(const Rational& first, const Rational& second);
bool operator==(const Rational& first, const Rational& second);
bool operator!=(const Rational& first, const Rational& second);

#endif  // RATIONAL_RATIONAL_H