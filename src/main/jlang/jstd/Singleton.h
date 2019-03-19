
#ifndef JSTD_SINGLETON_H
#define JSTD_SINGLETON_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <type_traits>

namespace jstd {

namespace v1 {

template <typename T>
class Singleton {
public:
    Singleton() {}
    ~Singleton() {}
};

} // namespace v1

template <typename T>
using Singleton = v1::Singleton<T>;

} // namespace jstd

#endif // JSTD_SINGLETON_H
