#ifndef SHARED_PTR_SHARED_PTR_H
#define SHARED_PTR_SHARED_PTR_H

#include <iostream>

class BadWeakPtr : public std::runtime_error {
public:
    BadWeakPtr() : std::runtime_error("BadWeakPtr") {
    }
};

template <class T>
class WeakPtr;

struct Counter {
    size_t weak;
    size_t shared;
    Counter() : weak(0), shared(0) {
    }
};

template <class T>
class SharedPtr {
private:
    T* ptr_;
    Counter* counter_;

public:
    SharedPtr();
    SharedPtr(T* ptr);  // NOLINT
    SharedPtr(const SharedPtr<T>& other);
    SharedPtr(SharedPtr<T>&& other) noexcept;
    SharedPtr(const WeakPtr<T>& other, bool except = true);  // NOLINT
    ~SharedPtr();

    SharedPtr<T>& operator=(const SharedPtr<T>& other);
    SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept;

    void Destroy();
    void Reset(T* ptr = nullptr);
    void Swap(SharedPtr<T>& other);
    T* Get() const;
    Counter* GetCounter() const;
    size_t UseCount() const;

    T& operator*() const;
    T* operator->() const;
    explicit operator bool() const;
};

template <class T>
SharedPtr<T>::SharedPtr() : ptr_(nullptr), counter_(nullptr) {
}

template <class T>
SharedPtr<T>::SharedPtr(T* ptr) : ptr_(ptr) {
    if (ptr == nullptr) {
        counter_ = nullptr;
    } else {
        counter_ = new Counter;
        counter_->weak = 0;
        counter_->shared = 1;
    }
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), counter_(other.counter_) {
    if (ptr_ != nullptr) {
        ++counter_->shared;
    }
}

template <class T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept : ptr_(nullptr), counter_(nullptr) {
    std::swap(ptr_, other.ptr_);
    std::swap(counter_, other.counter_);
}

template <class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& other, bool except) {
    if (other.Expired()) {
        if (except) {
            throw BadWeakPtr{};
        }
        ptr_ = nullptr;
        counter_ = nullptr;
        return;
    }
    ptr_ = other.Get();
    counter_ = other.GetCounter();
    ++counter_->shared;
}

template <class T>
SharedPtr<T>::~SharedPtr() {
    Destroy();
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
    if (&other != this) {
        Destroy();
        ptr_ = other.ptr_;
        counter_ = other.counter_;
        ++counter_->shared;
    }
    return *this;
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept {
    if (&other != this) {
        if (ptr_ == nullptr) {
            std::swap(ptr_, other.ptr_);
            std::swap(counter_, other.counter_);
        } else {
            Destroy();
            ptr_ = other.ptr_;
            counter_ = other.counter_;
            other.ptr_ = nullptr;
            other.counter_ = nullptr;
        }
    }
    return *this;
}

template <class T>
void SharedPtr<T>::Destroy() {
    if (counter_ != nullptr) {
        --counter_->shared;
        if (counter_->shared == 0) {
            delete ptr_;
            if (counter_->weak == 0) {
                delete counter_;
            }
        }
    }
}

template <class T>
void SharedPtr<T>::Reset(T* ptr) {
    if (ptr_ != nullptr) {
        Destroy();
    }
    ptr_ = ptr;
    if (ptr == nullptr) {
        counter_ = nullptr;
    } else {
        counter_ = new Counter;
        counter_->shared = 1;
    }
}

template <class T>
void SharedPtr<T>::Swap(SharedPtr<T>& other) {
    if (&other != this) {
        std::swap(ptr_, other.ptr_);
        std::swap(counter_, other.counter_);
    }
}

template <class T>
T* SharedPtr<T>::Get() const {
    return ptr_;
}

template <class T>
Counter* SharedPtr<T>::GetCounter() const {
    return counter_;
}

template <class T>
size_t SharedPtr<T>::UseCount() const {
    return counter_ == nullptr ? 0 : counter_->shared;
}

template <class T>
T& SharedPtr<T>::operator*() const {
    return *ptr_;
}

template <class T>
T* SharedPtr<T>::operator->() const {
    return ptr_;
}

template <class T>
SharedPtr<T>::operator bool() const {
    return ptr_ != nullptr;
}

template <class T>
class WeakPtr {
private:
    T* ptr_;
    Counter* counter_;

public:
    WeakPtr();
    WeakPtr(const WeakPtr<T>& other);
    WeakPtr(WeakPtr<T>&& other) noexcept;
    WeakPtr(const SharedPtr<T>& other);  // NOLINT
    ~WeakPtr();

    WeakPtr<T>& operator=(const WeakPtr<T>& other);
    WeakPtr<T>& operator=(WeakPtr<T>&& other) noexcept;

    void Destroy();
    T* Get() const;
    Counter* GetCounter() const;
    void Swap(WeakPtr<T>* other);
    void Reset(T* ptr = nullptr);
    size_t UseCount() const;
    bool Expired() const;
    SharedPtr<T> Lock() const;
};

template <class T>
WeakPtr<T>::WeakPtr() : ptr_(nullptr), counter_(nullptr) {
}

template <class T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& other) : ptr_(other.ptr_), counter_(other.counter_) {
    if (ptr_ != nullptr) {
        ++counter_->weak;
    }
}

template <class T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& other) noexcept : ptr_(nullptr), counter_(nullptr) {
    std::swap(ptr_, other.ptr_);
    std::swap(counter_, other.counter_);
}

template <class T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& other) : ptr_(other.Get()), counter_(other.GetCounter()) {
    if (ptr_ != nullptr) {
        ++counter_->weak;
    }
}

template <class T>
WeakPtr<T>::~WeakPtr() {
    Destroy();
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other) {
    if (&other != this) {
        ptr_ = other.ptr_;
        counter_ = other.counter_;
        ++counter_->weak;
    }
    return *this;
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other) noexcept {
    if (&other != this) {
        if (ptr_ == nullptr) {
            std::swap(ptr_, other.ptr_);
            std::swap(counter_, other.counter_);
        } else {
            Destroy();
            ptr_ = other.ptr_;
            counter_ = other.counter_;
            other.ptr_ = nullptr;
            other.counter_ = nullptr;
        }
    }
    return *this;
}

template <class T>
void WeakPtr<T>::Destroy() {
    if (counter_ != nullptr) {
        --counter_->weak;
        if (counter_->shared == 0 && counter_->weak == 0) {
            delete counter_;
        }
    }
}

template <class T>
T* WeakPtr<T>::Get() const {
    return ptr_;
}

template <class T>
Counter* WeakPtr<T>::GetCounter() const {
    return counter_;
}

template <class T>
void WeakPtr<T>::Swap(WeakPtr<T>* other) {
    if (&other != this) {
        std::swap(ptr_, other->ptr_);
        std::swap(counter_, other->counter_);
    }
}

template <class T>
void WeakPtr<T>::Reset(T* ptr) {
    Destroy();
    ptr_ = ptr;
    if (ptr == nullptr) {
        counter_ = nullptr;
    } else {
        counter_ = new Counter;
        counter_->weak = 1;
    }
}

template <class T>
size_t WeakPtr<T>::UseCount() const {
    return counter_ == nullptr ? 0 : counter_->shared;
}

template <class T>
bool WeakPtr<T>::Expired() const {
    return (ptr_ == nullptr || counter_->shared == 0);
}

template <class T>
SharedPtr<T> WeakPtr<T>::Lock() const {
    SharedPtr<T> result = SharedPtr<T>(*this, false);
    return result;
}

#endif  // SHARED_PTR_SHARED_PTR_H