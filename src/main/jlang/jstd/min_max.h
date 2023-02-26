
#ifndef JSTD_MINMAX_H
#define JSTD_MINMAX_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <type_traits>

namespace jstd {

template <typename T>
static inline
typename std::enable_if<std::is_integral<T>::value, T>::type
Min(T a, T b) {
    return (a <= b) ? a : b;
}

template <typename T>
static inline
typename std::enable_if<!std::is_integral<T>::value, T>::type
Min(T & a, T & b) {
    return (a <= b) ? a : b;
}

template <typename T>
static inline
typename std::enable_if<std::is_integral<T>::value, T>::type
Max(T a, T b) {
    return (a >= b) ? a : b;
}

template <typename T>
static inline
typename std::enable_if<!std::is_integral<T>::value, T>::type
Max(T & a, T & b) {
    return (a >= b) ? a : b;
}

} // namespace jstd

#endif // JSTD_MINMAX_H
