/**
 * @file common_lib.hpp
 *
 * @brief A common utility library
 *
 * @author Milind Singh (milind345@gmail.com)
 */

#pragma once

#include <iostream>
#include <bitset>
#include <cstdint>

namespace utils {

template<typename T>
inline T clear_bits(const T& val, const T& bits)
{
    return (val & ~(bits));
}

template<typename T>
inline T set_bits(const T& val, const T& bits)
{
    return (val | bits);
}

inline void print_bits_1b(uint8_t n);

} // namespace utils
