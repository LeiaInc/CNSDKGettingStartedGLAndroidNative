#pragma once

#include "leia/common/slice.hpp"

#include <vector>

namespace leia {

template <typename T>
Slice<T const> ToSlice(std::vector<T> const& v) {
    return {v.data(), v.size()};
}

} // namespace leia
