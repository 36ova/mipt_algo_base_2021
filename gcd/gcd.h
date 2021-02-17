#ifndef GCD_GCD_H
#define GCD_GCD_H

template <class T>
T Gcd(T a, T b) {
    while (a != b) {
        if (a < b) {
            T temp = a;
            a = b;
            b = temp;
        }
        a -= b;
    }
    return a;
}

#endif
