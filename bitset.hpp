// kate: replace-tabs on; indent-width 4; indent-mode cstyle;
#pragma once
#include <bitset>
#include <type_traits>

template <typename T> std::bitset<sizeof(T) * 8> as_bitset(T x) {
    static_assert(std::is_integral_v<T>, "as_bitset() only takes whole numbers!");
    return std::bitset<sizeof(T) * 8>(x);
}
