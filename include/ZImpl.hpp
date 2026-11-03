#pragma once

template <class T>
class ZImpl {
public:
    T* operator*() {
        return static_cast<T*>(this);
    }
};