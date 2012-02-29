#pragma once

template<typename T> class Enum{
protected:
    T value;

    Enum(const T& value) {
        this->value = value;
    }
public:
    Enum() {}

    Enum(const Enum<T>& that) {
        this->value = that.value;
    }
    
    bool operator ==(const Enum<T>& e) const {
        return value == e.value;
    }
    
    bool operator !=(const Enum<T>& e) const {
        return value != e.value;
    }

    operator const T&() const {
        return this->value;
    }
};
