
#ifndef JLANG_LANG_COMPARATORABLE_H
#define JLANG_LANG_COMPARATORABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <assert.h>
#include <type_traits>

#include "jlang/lang/Equatable.h"

namespace jlang {

///////////////////////////////////////////////////
// struct Comparatorable<T>
///////////////////////////////////////////////////

template <typename T, bool IsArithmetic = std::is_arithmetic<T>::value>
struct Comparatorable {
    virtual bool isEquals(T value) const = 0;
    virtual bool isLessThan(T value) const = 0;
    virtual bool isGreaterThan(T value) const = 0;
};

template <typename T>
struct Comparatorable<T, false> {
    virtual bool isEquals(const T & value) const = 0;
    virtual bool isLessThan(const T & value) const = 0;
    virtual bool isGreaterThan(const T & value) const = 0;
};

template <>
struct Comparatorable<void, true> {
};

template <>
struct Comparatorable<void, false> {
};

} // namespace jlang

#endif // JLANG_LANG_COMPARATORABLE_H
