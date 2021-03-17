#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H
#include <iostream>

class VectorOutOfRange : public std::out_of_range {
public:
    VectorOutOfRange() : std::out_of_range("VectorOutOfRange") {
    }
};

template <class T>
void Copy(T *buffer_to, const T *buffer_from, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        buffer_to[i] = buffer_from[i];
    }
}

template <class T>
void Swap(T &a, T &b) {
    T c = a;
    a = b;
    b = c;
}

template <class T>
class Vector {
private:
    T *buffer_;
    size_t size_;
    size_t capacity_;
    const static size_t kIncreaseFactor = 2;

public:
    Vector();
    explicit Vector(const size_t size);
    Vector(const size_t size, const T &value);
    Vector(const Vector<T> &other);
    Vector<T> &operator=(const Vector<T> &other);
    ~Vector();
    void Clear();
    void PushBack(const T &value);
    T &PopBack();
    void Resize(const size_t new_size);
    void Resize(const size_t new_size, const T &value);
    void Reserve(const size_t new_capacity);
    void ShrinkToFit();
    void Swap(Vector<T> &other);
    const T &operator[](size_t idx) const;
    T &operator[](size_t idx);
    const T At(size_t idx) const;
    T &At(size_t idx);
    T Front() const;
    T &Front();
    T Back() const;
    T &Back();
    bool Empty() const;
    size_t Size() const;
    size_t Capacity() const;
    const T *Data() const;
    T *Data();
    bool operator<(const Vector<T> &other) const;
    bool operator>(const Vector<T> &other) const;
    bool operator==(const Vector<T> &other) const;
    bool operator<=(const Vector<T> &other) const;
    bool operator>=(const Vector<T> &other) const;
    bool operator!=(const Vector<T> &other) const;
    void Fill(const size_t start, const size_t end, const T &value);
    size_t IncreaseCapacity() const;
    void BufferReallocation(const size_t new_capacity);
};

template <class T>
Vector<T>::Vector() : buffer_(nullptr), size_(0), capacity_(0) {
}

template <class T>
Vector<T>::Vector(const size_t size) : size_(size), capacity_(size) {
    buffer_ = new T[capacity_];
}

template <class T>
Vector<T>::Vector(const size_t size, const T &value) : size_(size), capacity_(size) {
    buffer_ = new T[capacity_];
    Fill(0, size_, value);
}

template <class T>
Vector<T>::Vector(const Vector<T> &other) : size_(other.size_), capacity_(other.capacity_) {
    buffer_ = new T[capacity_];
    Copy(buffer_, other.buffer_, size_);
}

template <class T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
    if (&other != this) {
        BufferReallocation(other.capacity_);
        size_ = other.size_;
        Copy(buffer_, other.buffer_, size_);
    }
    return *this;
}

template <class T>
Vector<T>::~Vector() {
    delete[] buffer_;
}

template <class T>
void Vector<T>::Clear() {
    size_ = 0;
}

template <class T>
void Vector<T>::PushBack(const T &value) {
    if (size_ == capacity_) {
        BufferReallocation(IncreaseCapacity());
    }
    buffer_[size_] = value;
    ++size_;
}

template <class T>
T &Vector<T>::PopBack() {
    T &result = buffer_[size_];
    --size_;
    return result;
}

template <class T>
void Vector<T>::Resize(const size_t new_size) {
    if (new_size > capacity_) {
        BufferReallocation(new_size);
    }
    size_ = new_size;
}

template <class T>
void Vector<T>::Resize(const size_t new_size, const T &value) {
    if (new_size > capacity_) {
        BufferReallocation(new_size);
        Fill(size_, new_size, value);
        size_ = new_size;
    } else {
        if (new_size > size_) {
            Fill(size_, new_size, value);
        }
        size_ = new_size;
    }
}

template <class T>
void Vector<T>::Reserve(const size_t new_capacity) {
    BufferReallocation(std::max(capacity_, new_capacity));
}

template <class T>
void Vector<T>::ShrinkToFit() {
    if (size_ != capacity_) {
        BufferReallocation(size_);
    }
}

template <class T>
void Vector<T>::Swap(Vector<T> &other) {
    std::swap(buffer_, other.buffer_);
    ::Swap(capacity_, other.capacity_);
    ::Swap(size_, other.size_);
}

template <class T>
const T &Vector<T>::operator[](size_t idx) const {
    return buffer_[idx];
}

template <class T>
T &Vector<T>::operator[](size_t idx) {
    return buffer_[idx];
}

template <class T>
const T Vector<T>::At(size_t idx) const {
    if (idx >= size_) {
        throw VectorOutOfRange{};
    }
    return buffer_[idx];
}

template <class T>
T &Vector<T>::At(size_t idx) {
    if (idx >= size_) {
        throw VectorOutOfRange{};
    }
    return buffer_[idx];
}

template <class T>
T Vector<T>::Front() const {
    return buffer_[0];
}

template <class T>
T &Vector<T>::Front() {
    return buffer_[0];
}

template <class T>
T Vector<T>::Back() const {
    return buffer_[size_ - 1];
}

template <class T>
T &Vector<T>::Back() {
    return buffer_[size_ - 1];
}

template <class T>
bool Vector<T>::Empty() const {
    return size_ == 0;
}

template <class T>
size_t Vector<T>::Size() const {
    return size_;
}

template <class T>
size_t Vector<T>::Capacity() const {
    return capacity_;
}

template <class T>
const T *Vector<T>::Data() const {
    return &(buffer_[0]);
}

template <class T>
T *Vector<T>::Data() {
    return &(buffer_[0]);
}

template <class T>
bool Vector<T>::operator<(const Vector<T> &other) const {
    size_t end = std::min(size_, other.size_);
    for (size_t i = 0; i < end; ++i) {
        if (buffer_[i] < other.buffer_[i]) {
            return true;
        }
        if (buffer_[i] > other.buffer_[i]) {
            return false;
        }
    }
    return (size_ < other.size_);
}

template <class T>
bool Vector<T>::operator>(const Vector<T> &other) const {
    return (other < *this);
}

template <class T>
bool Vector<T>::operator==(const Vector<T> &other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (size_t i = 0; i < size_; ++i) {
        if (buffer_[i] != other.buffer_[i]) {
            return false;
        }
    }
    return true;
}

template <class T>
bool Vector<T>::operator<=(const Vector<T> &other) const {
    return !(other < *this);
}

template <class T>
bool Vector<T>::operator>=(const Vector<T> &other) const {
    return !(*this < other);
}

template <class T>
bool Vector<T>::operator!=(const Vector<T> &other) const {
    return !(*this == other);
}

template <class T>
void Vector<T>::Fill(const size_t start, const size_t end, const T &value) {
    for (size_t i = start; i < end; ++i) {
        buffer_[i] = value;
    }
}

template <class T>
size_t Vector<T>::IncreaseCapacity() const {
    return (capacity_ == 0) ? 1 : capacity_ * kIncreaseFactor;
}

template <class T>
void Vector<T>::BufferReallocation(const size_t new_capacity) {
    T *new_buffer = (new_capacity == 0) ? nullptr : new T[new_capacity];
    size_ = std::min(size_, new_capacity);
    Copy(new_buffer, buffer_, size_);
    delete[] buffer_;
    capacity_ = new_capacity;
    buffer_ = new_buffer;
}

#endif  // VECTOR_VECTOR_H
