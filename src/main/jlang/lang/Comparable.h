
#ifndef JLANG_LANG_COMPARABLE_H
#define JLANG_LANG_COMPARABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <assert.h>
#include <type_traits>

namespace jlang {

///////////////////////////////////////////////////
// struct Comparable<T>
///////////////////////////////////////////////////

template <typename T, bool IsArithmetic = std::is_arithmetic<T>::value>
struct Comparable {
    virtual bool compareTo(T value) const = 0;
};

template <typename T>
struct Comparable<T, false> {
    virtual bool compareTo(const T & value) const = 0;
};

template <>
struct Comparable<void, true> {
};

template <>
struct Comparable<void, false> {
};

} // namespace jlang

#endif // JLANG_LANG_COMPARABLE_H
