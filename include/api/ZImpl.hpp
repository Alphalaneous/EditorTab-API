#pragma once

template <class T>
class ZImpl {
public:
    T* get() {
        return static_cast<T*>(this);
    }
};