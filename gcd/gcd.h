#ifndef GCD_GCD_H
#define GCD_GCD_H

template <class T>
T Gcd(T a, T b) {
    while (a != 0) {
        if (b > a) {
            auto temp = a;
            a = b;
            b = temp;
        }
        a %= b;
    }
    return b;
}

#endif  // GCD_GCD_H