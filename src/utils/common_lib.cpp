/**
 * @file common_lib.cpp
 *
 * @brief A common utility library
 *
 * @author Milind Singh (milind345@gmail.com)
 */

#include <iostream>
#include <bitset>
#include <cstdint>

#include "utils/common_lib.hpp"

namespace utils {

/**
 * @brief Helper to print bits of a byte
 * @param n
 */
inline void print_bits_1b(uint8_t n)
{
    std::bitset<8u> b(n);
    std::cout << b;
}

} // namespace utils