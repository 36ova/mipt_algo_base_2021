#ifndef DEQUE_PAGE_PAGE_H
#define DEQUE_PAGE_PAGE_H
#include <iostream>

template <class T, size_t N>
class Page {
private:
    T buffer_[N];
    size_t front_ind_ = 0;
    size_t back_ind_ = N;

public:
    Page();
    Page(const Page<T, N> &other);
    Page<T, N> &operator=(const Page<T, N> &other);
    ~Page() = default;
    const T &operator[](size_t ind) const;
    T &operator[](size_t ind);
    T Front() const;
    T &Front();
    T Back() const;
    T &Back();
    bool Empty() const;
    bool Full() const;
    size_t Size() const;
    bool IsBack() const;
    bool IsFront() const;
    void PushBack(const T &value);
    void PushFront(const T &value);
    T &PopBack();
    T &PopFront();
    void Clear();
    friend void Copy(Page<T, N> &page_to, const Page<T, N> &page_from) {
        if (page_from.back_ind_ == N) {
            for (size_t i = 0; i < page_from.front_ind_; ++i) {
                page_to.buffer_[i] = page_from.buffer_[i];
            }
        } else {
            for (size_t i = N - 1; i >= page_from.back_ind_; --i) {
                page_to.buffer_[i] = page_from.buffer_[i];
            }
        }
    }
};

template <class T, size_t N>
Page<T, N>::Page() : front_ind_(0), back_ind_(N) {
}

template <class T, size_t N>
Page<T, N>::Page(const Page<T, N> &other) : front_ind_(other.front_ind_), back_ind_(other.back_ind_) {
    Copy(*this, other);
}

template <class T, size_t N>
Page<T, N> &Page<T, N>::operator=(const Page<T, N> &other) {
    if (&other != this) {
        front_ind_ = other.front_ind_;
        back_ind_ = other.back_ind_;
        Copy(*this, other);
    }
    return *this;
}

template <class T, size_t N>
const T &Page<T, N>::operator[](size_t ind) const {
    if (front_ind_ != 0) {
        return buffer_[ind];
    }
    return buffer_[back_ind_ + ind];
}

template <class T, size_t N>
T &Page<T, N>::operator[](size_t ind) {
    if (front_ind_ != 0) {
        return buffer_[ind];
    }
    return buffer_[back_ind_ + ind];
}

template <class T, size_t N>
T Page<T, N>::Front() const {
    if (front_ind_ == 0) {
        return buffer_[back_ind_];
    }
    return buffer_[0];
}

template <class T, size_t N>
T &Page<T, N>::Front() {
    if (front_ind_ == 0) {
        return buffer_[back_ind_];
    }
    return buffer_[0];
}

template <class T, size_t N>
T Page<T, N>::Back() const {
    if (front_ind_ == 0) {
        return buffer_[N - 1];
    }
    return buffer_[front_ind_ - 1];
}

template <class T, size_t N>
T &Page<T, N>::Back() {
    if (front_ind_ == 0) {
        return buffer_[N - 1];
    }
    return buffer_[front_ind_ - 1];
}

template <class T, size_t N>
bool Page<T, N>::Empty() const {
    return (front_ind_ == 0) && (back_ind_ == N);
}

template <class T, size_t N>
bool Page<T, N>::Full() const {
    return (front_ind_ == N) || (back_ind_ == 0);
}

template <class T, size_t N>
size_t Page<T, N>::Size() const {
    if (back_ind_ == N) {
        return front_ind_;
    }
    return N - back_ind_;
}

template <class T, size_t N>
bool Page<T, N>::IsBack() const {
    return (back_ind_ == N) && (front_ind_ != N);
}

template <class T, size_t N>
bool Page<T, N>::IsFront() const {
    return (front_ind_ == 0) && (back_ind_ != 0);
}

template <class T, size_t N>
void Page<T, N>::PushBack(const T &value) {
    buffer_[front_ind_] = value;
    ++front_ind_;
}

template <class T, size_t N>
void Page<T, N>::PushFront(const T &value) {
    --back_ind_;
    buffer_[back_ind_] = value;
}

template <class T, size_t N>
T &Page<T, N>::PopBack() {
    --front_ind_;
    return buffer_[front_ind_];
}

template <class T, size_t N>
T &Page<T, N>::PopFront() {
    ++back_ind_;
    return buffer_[back_ind_ - 1];
}

template <class T, size_t N>
void Page<T, N>::Clear() {
    front_ind_ = 0;
    back_ind_ = N;
}

#endif  // DEQUE_PAGE_PAGE_H
