
#ifndef JLANG_LANG_EQUATABLE_H
#define JLANG_LANG_EQUATABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <assert.h>
#include <type_traits>

namespace jlang {

///////////////////////////////////////////////////
// struct Equatable<T>
///////////////////////////////////////////////////

template <typename T, bool IsArithmetic = std::is_arithmetic<T>::value>
struct Equatable {
    virtual bool isEquals(T value) const = 0;
};

template <typename T>
struct Equatable<T, false> {
    virtual bool isEquals(const T & value) const = 0;
};

template <>
struct Equatable<void, true> {
};

template <>
struct Equatable<void, false> {
};

} // namespace jlang

#endif // JLANG_LANG_EQUATABLE_H
