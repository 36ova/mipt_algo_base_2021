#ifndef ARRAY_ARRAY_H
#define ARRAY_ARRAY_H

#include <iterator>
#include <stdexcept>

class ArrayOutOfRange : public std::out_of_range {
public:
  ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {}
};

template <class T, size_t N> struct Array {
public:
  T array[N];
  T operator[](size_t i) const;
  T &operator[](size_t i);
  T &At(size_t i);
  T Front() const;
  T &Front();
  T Back() const;
  T &Back();
  const T *Data() const;
  size_t Size() const;
  bool Empty() const;
  void Fill(const T &value);
  void Swap(Array<T, N> &other);
  bool operator<(Array<T, N> &other) const;
  bool operator>(Array<T, N> &other) const;
  bool operator==(Array<T, N> &other) const;
  bool operator<=(Array<T, N> &other) const;
  bool operator>=(Array<T, N> &other) const;
  bool operator!=(Array<T, N> &other) const;
};

template <class T, size_t N> T Array<T, N>::operator[](size_t i) const {
  return array[i];
}

template <class T, size_t N> T &Array<T, N>::operator[](size_t i) {
  return array[i];
}

template <class T, size_t N> T &Array<T, N>::At(size_t i) {
  if (i >= N) {
    throw ArrayOutOfRange{};
  }
  return array[i];
}

template <class T, size_t N> T Array<T, N>::Front() const { return array[0]; }

template <class T, size_t N> T &Array<T, N>::Front() { return array[0]; }

template <class T, size_t N> T Array<T, N>::Back() const {
  return array[N - 1];
}

template <class T, size_t N> T &Array<T, N>::Back() { return array[N - 1]; }

template <class T, size_t N> const T *Array<T, N>::Data() const {
  return &(array[0]);
}

template <class T, size_t N> size_t Array<T, N>::Size() const { return N; }

template <class T, size_t N> bool Array<T, N>::Empty() const { return !(N); }

template <class T, size_t N> void Array<T, N>::Fill(const T &value) {
  for (size_t i = 0; i < N; ++i) {
    array[i] = value;
  }
}

template <class T, size_t N> void Array<T, N>::Swap(Array<T, N> &other) {
  for (size_t i = 0; i < N; ++i) {
    std::swap(array[i], other[i]);
  }
}

template <class T, size_t N>
bool Array<T, N>::operator<(Array<T, N> &other) const {
  for (size_t i = 0; i < N; ++i) {
    if (array[i] >= other[i]) {
      return false;
    }
  }
  return true;
}

template <class T, size_t N>
bool Array<T, N>::operator>(Array<T, N> &other) const {
  for (size_t i = 0; i < N; ++i) {
    if (array[i] <= other[i]) {
      return false;
    }
  }
  return true;
}

template <class T, size_t N>
bool Array<T, N>::operator==(Array<T, N> &other) const {
  for (size_t i = 0; i < N; ++i) {
    if (array[i] != other[i]) {
      return false;
    }
  }
  return true;
}

template <class T, size_t N>
bool Array<T, N>::operator<=(Array<T, N> &other) const {
  return (!(this > other));
}

template <class T, size_t N>
bool Array<T, N>::operator>=(Array<T, N> &other) const {
  return (!(this < other));
}
template <class T, size_t N>
bool Array<T, N>::operator!=(Array<T, N> &other) const {
  return (!(this == other));
}

template <class T, size_t N>
void swap(Array<T, N> &first, Array<T, N> &second) {
    first.Swap(second);
}

#endif
