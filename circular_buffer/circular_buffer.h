#ifndef DEQUE_CIRCULAR_BUFFER_H
#define DEQUE_CIRCULAR_BUFFER_H

#include "cstddef"
#include <iostream>

template <class T>
void Swap(T &first, T &second) {
    T temp = first;
    first = second;
    second = temp;
}

template <class T>
void Copy(T *buffer_to, const T *buffer_from, const size_t capacity, const size_t head, const size_t tail) {
    if (head < tail) {
        for (size_t i = 0; i < tail - head; ++i) {
            buffer_to[i] = buffer_from[head + i];
        }
    } else {
        for (size_t i = 0; i < capacity - head; ++i) {
            buffer_to[i] = buffer_from[head + i];
        }
        for (size_t i = 0; i < tail; ++i) {
            buffer_to[capacity - head + i] = buffer_from[i];
        }
    }
}

template <class T>
class CircularBuffer {
private:
    T *buffer_;
    const static size_t kIncreaseFactor = 2;
    size_t capacity_;
    size_t head_;
    size_t tail_;
    size_t size_;

public:
    CircularBuffer();
    explicit CircularBuffer(const size_t size);  //  NOLINT
    CircularBuffer(const CircularBuffer<T> &other);
    CircularBuffer<T> &operator=(const CircularBuffer<T> &other);
    ~CircularBuffer();
    const T &operator[](size_t idx) const;
    T &operator[](size_t idx);
    T Front() const;
    T &Front();
    T Back() const;
    T &Back();
    bool Empty() const;
    size_t Size() const;
    size_t Capacity() const;
    void PushBack(const T &value);
    void PushFront(const T &value);
    T &PopBack();
    T &PopFront();
    void Clear();
    void Reserve(const size_t new_capacity);
    void Swap(CircularBuffer<T> &other);
    size_t IncreaseCapacity() const;
    void BufferReallocation(const size_t new_capacity, bool needs_copy);
};

template <class T>
CircularBuffer<T>::CircularBuffer() : buffer_(nullptr), capacity_(0), head_(0), tail_(0), size_(0) {
}

template <class T>
CircularBuffer<T>::CircularBuffer(const size_t capacity) : capacity_(capacity), head_(0), tail_(0), size_(0) {
    buffer_ = new T[capacity_];
}

template <class T>
CircularBuffer<T>::CircularBuffer(const CircularBuffer<T> &other)
    : capacity_(other.capacity_), head_(0), tail_(other.Size()), size_(other.size_) {
    buffer_ = new T[capacity_];
    Copy(buffer_, other.buffer_, capacity_, other.head_, other.tail_);
}

template <class T>
CircularBuffer<T> &CircularBuffer<T>::operator=(const CircularBuffer<T> &other) {
    if (&other != this) {
        BufferReallocation(other.capacity_, false);
        Copy(buffer_, other.buffer_, capacity_, other.head_, other.tail_);
        head_ = 0;
        size_ = tail_ = other.Size();
    }
    return *this;
}

template <class T>
CircularBuffer<T>::~CircularBuffer() {
    delete[] buffer_;
}

template <class T>
const T &CircularBuffer<T>::operator[](size_t idx) const {
    return buffer_[(head_ + idx) % capacity_];
}

template <class T>
T &CircularBuffer<T>::operator[](size_t idx) {
    return buffer_[(head_ + idx) % capacity_];
}

template <class T>
T CircularBuffer<T>::Front() const {
    return buffer_[head_];
}

template <class T>
T &CircularBuffer<T>::Front() {
    return buffer_[head_];
}

template <class T>
T CircularBuffer<T>::Back() const {
    return buffer_[tail_ - 1];
}

template <class T>
T &CircularBuffer<T>::Back() {
    return buffer_[tail_ - 1];
}

template <class T>
bool CircularBuffer<T>::Empty() const {
    return (size_ == 0);
}

template <class T>
size_t CircularBuffer<T>::Size() const {
    return size_;
}

template <class T>
size_t CircularBuffer<T>::Capacity() const {
    return capacity_;
}

template <class T>
void CircularBuffer<T>::PushBack(const T &value) {
    if (size_ == capacity_) {
        BufferReallocation(IncreaseCapacity(), true);
    }
    if (tail_ == capacity_) {
        tail_ = 0;
    }
    buffer_[tail_] = value;
    ++tail_;
    ++size_;
}

template <class T>
void CircularBuffer<T>::PushFront(const T &value) {
    if (size_ == capacity_) {
        BufferReallocation(IncreaseCapacity(), true);
    }
    if (head_ == 0) {
        head_ = capacity_;
    }
    --head_;
    buffer_[head_] = value;
    ++size_;
}

template <class T>
T &CircularBuffer<T>::PopBack() {
    T &result = buffer_[tail_ - 1];
    if (tail_ == 1) {
        tail_ = capacity_ + 1;
    }
    --tail_;
    --size_;
    return result;
}

template <class T>
T &CircularBuffer<T>::PopFront() {
    T &result = buffer_[head_];
    if (head_ == capacity_ - 1) {
        head_ = 0;
    } else {
        ++head_;
    }
    --size_;
    return result;
}

template <class T>
void CircularBuffer<T>::Clear() {
    size_ = head_ = 0;
    tail_ = 1;
}

template <class T>
void CircularBuffer<T>::Reserve(const size_t new_capacity) {
    BufferReallocation(std::max(capacity_, new_capacity), true);
}

template <class T>
void CircularBuffer<T>::Swap(CircularBuffer<T> &other) {
    std::swap(buffer_, other.buffer_);
    ::Swap(capacity_, other.capacity_);
    ::Swap(head_, other.head_);
    ::Swap(tail_, other.tail_);
    ::Swap(size_, other.size_);
}

template <class T>
size_t CircularBuffer<T>::IncreaseCapacity() const {
    return (capacity_ == 0) ? 1 : capacity_ * kIncreaseFactor;
}

template <class T>
void CircularBuffer<T>::BufferReallocation(const size_t new_capacity, bool needs_copy) {
    T *new_buffer = (new_capacity == 0) ? nullptr : new T[new_capacity];
    if (new_capacity >= capacity_) {
        if (needs_copy) {
            Copy(new_buffer, buffer_, capacity_, head_, tail_);
            tail_ = Size();
            head_ = 0;
        }
    }
    delete[] buffer_;
    capacity_ = new_capacity;
    buffer_ = new_buffer;
}

#endif  // DEQUE_CIRCULAR_BUFFER_H
