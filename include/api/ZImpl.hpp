#pragma once

/// This class just handles the pimpl with CRTP to make it easier to use event export based classes, you can ignore it :3
template <class T>
class ZImpl {
public:
    T* get() {
        return static_cast<T*>(this);
    }
};