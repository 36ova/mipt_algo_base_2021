#ifndef ARRAY_ARRAY_H
#define ARRAY_ARRAY_H

#include <stdexcept>

class ArrayOutOfRange : public std::out_of_range {
public:
    ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
    }
};

template <class T, size_t N>
class Array {
public:
    T array[N];

    const T &operator[](size_t idx) const;
    T &operator[](size_t idx);
    T At(size_t idx) const;
    T &At(size_t idx);
    T Front() const;
    T &Front();
    T Back() const;
    T &Back();
    const T *Data() const;
    T *Data();
    size_t Size() const;
    bool Empty() const;
    void Fill(const T &value);
    void Swap(Array<T, N> &other);
    bool operator<(const Array<T, N> &other) const;
    bool operator>(const Array<T, N> &other) const;
    bool operator==(const Array<T, N> &other) const;
    bool operator<=(const Array<T, N> &other) const;
    bool operator>=(const Array<T, N> &other) const;
    bool operator!=(const Array<T, N> &other) const;
};

template <class T, size_t N>
const T &Array<T, N>::operator[](size_t idx) const {
    return array[idx];
}

template <class T, size_t N>
T &Array<T, N>::operator[](size_t idx) {
    return array[idx];
}

template <class T, size_t N>
T Array<T, N>::At(size_t idx) const {
    if (idx >= N) {
        throw ArrayOutOfRange{};
    }
    return array[idx];
}

template <class T, size_t N>
T &Array<T, N>::At(size_t idx) {
    if (idx >= N) {
        throw ArrayOutOfRange{};
    }
    return array[idx];
}

template <class T, size_t N>
T Array<T, N>::Front() const {
    return array[0];
}

template <class T, size_t N>
T &Array<T, N>::Front() {
    return array[0];
}

template <class T, size_t N>
T Array<T, N>::Back() const {
    return array[N - 1];
}

template <class T, size_t N>
T &Array<T, N>::Back() {
    return array[N - 1];
}

template <class T, size_t N>
const T *Array<T, N>::Data() const {
    return &(array[0]);
}

template <class T, size_t N>
T *Array<T, N>::Data() {
    return &(array[0]);
}

template <class T, size_t N>
size_t Array<T, N>::Size() const {
    return N;
}

template <class T, size_t N>
bool Array<T, N>::Empty() const {
    return N == 0;
}

template <class T, size_t N>
void Array<T, N>::Fill(const T &value) {
    for (size_t i = 0; i < N; ++i) {
        array[i] = value;
    }
}

template <class T, size_t N>
void Array<T, N>::Swap(Array<T, N> &other) {
    for (size_t i = 0; i < N; ++i) {
        T temp = other[i];
        other[i] = array[i];
        array[i] = temp;
    }
}

template <class T, size_t N>
bool Array<T, N>::operator<(const Array<T, N> &other) const {
    for (size_t i = 0; i < N; ++i) {
        if (array[i] < other[i]) {
            return true;
        }
        if (array[i] > other[i]) {
            return false;
        }
    }
    return false;
}

template <class T, size_t N>
bool Array<T, N>::operator>(const Array<T, N> &other) const {
    for (size_t i = 0; i < N; ++i) {
        if (array[i] > other[i]) {
            return true;
        }
        if (array[i] < other[i]) {
            return false;
        }
    }
    return false;
}

template <class T, size_t N>
bool Array<T, N>::operator==(const Array<T, N> &other) const {
    for (size_t i = 0; i < N; ++i) {
        if (array[i] != other[i]) {
            return false;
        }
    }
    return true;
}

template <class T, size_t N>
bool Array<T, N>::operator<=(const Array<T, N> &other) const {
    return !(*this > other);
}

template <class T, size_t N>
bool Array<T, N>::operator>=(const Array<T, N> &other) const {
    return !(*this < other);
}

template <class T, size_t N>
bool Array<T, N>::operator!=(const Array<T, N> &other) const {
    return !(*this == other);
}

namespace std {  // NOLINT
template <class T, size_t N>
void swap(Array<T, N> &first, Array<T, N> &second) {
    first.Swap(second);
}
}  // namespace std

#endif  // ARRAY_ARRAY_H
