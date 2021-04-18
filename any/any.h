#ifndef ANY_ANY_H
#define ANY_ANY_H

#include <iostream>
#include <memory>

class BadAnyCast : public std::bad_cast {
public:
    const char* what() const noexcept override {
        return "bad_any_cast caught";
    }
};

class IBase {
public:
    virtual std::unique_ptr<IBase> Clone() const = 0;
    virtual ~IBase() = default;
};

template <class T>
class Derived : public IBase {
public:
    T value;
    explicit Derived(T type);
    std::unique_ptr<IBase> Clone() const override;
};

template <class T>
Derived<T>::Derived(T type) : value(std::move(type)) {
}

template <class T>
std::unique_ptr<IBase> Derived<T>::Clone() const {
    return std::make_unique<Derived>(value);
}

class Any {
protected:
    std::unique_ptr<IBase> base_ptr_;

public:
    Any() = default;

    template <class T>
    Any(T object) : base_ptr_(std::make_unique<Derived<T>>(std::move(object))) {  // NOLINT
    }

    Any(const Any& other) : base_ptr_(other.base_ptr_->Clone()) {
    }

    Any(Any&& value) {
        Swap(value);
    }

    Any& operator=(Any other) {
        Swap(other);
        return *this;
    }

    void Swap(Any& other) {
        base_ptr_.swap(other.base_ptr_);
    }

    void Reset() {
        base_ptr_.reset();
    }

    bool HasValue() const {
        return base_ptr_ != nullptr;
    }

    template <class T>
    friend T AnyCast(const Any& value);
};

template <class T>
T AnyCast(const Any& value) {
    auto* result = dynamic_cast<Derived<T>*>(value.base_ptr_.get());
    if (result == nullptr) {
        throw BadAnyCast();
    }
    return result->value;
}

#endif  // ANY_ANY_H
