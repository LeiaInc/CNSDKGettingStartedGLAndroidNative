#pragma once

#include <cstdint>

namespace leia {

template <typename T>
struct Slice {
    T* data;
    size_t length;

    Slice() : data(nullptr), length(0) {}
    Slice(T* ptr, size_t length) : data(ptr), length(length) {}

    T* begin() const { return data; }
    T* end() const { return data + length; }
};

} // namespace leia
