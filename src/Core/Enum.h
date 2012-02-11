#pragma once

template<typename T> class Enum {
private:
    T value;

    //Enum() {}
protected:
    Enum(const T& value) {
        this->value = value;
    }

    Enum(const Enum& that) {
        this->value = that.value;
    }

public:
    bool operator ==(const Enum<T>& e) const {
        return value == e.value;
    }

    operator const T&() const {
        return this->value;
    }
};
