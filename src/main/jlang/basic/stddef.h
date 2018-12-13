
#ifndef JLANG_BASIC_STDDEF_H
#define JLANG_BASIC_STDDEF_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//
// C++ compiler macro define
// See: http://www.cnblogs.com/zyl910/archive/2012/08/02/printmacro.html
//

//
// Using unlikely/likely macros from boost?
// See: http://www.boost.org/doc/libs/1_60_0/boost/config/compiler/clang.hpp
//
// See:
//      http://www.boost.org/doc/libs/1_60_0/boost/config/compiler/gcc.hpp10
//      http://www.boost.org/doc/libs/1_60_0/boost/config/compiler/clang.hpp4
//      http://www.boost.org/doc/libs/1_60_0/boost/config/compiler/intel.hpp2
//

//
// Intel C++ compiler version
//
#if defined(__INTEL_COMPILER)
  #if (__INTEL_COMPILER == 9999)
    #define __INTEL_CXX_VERSION     1200    // Intel's bug in 12.1.
  #else
    #define __INTEL_CXX_VERSION     __INTEL_COMPILER
  #endif
#elif defined(__ICL)
#  define __INTEL_CXX_VERSION       __ICL
#elif defined(__ICC)
#  define __INTEL_CXX_VERSION       __ICC
#elif defined(__ECC)
#  define __INTEL_CXX_VERSION       __ECC
#endif

//
// Since gcc 2.96 or Intel C++ compiler 8.0
//
// I'm not sure Intel C++ compiler 8.0 was the first version to support these builtins,
// update the condition if the version is not accurate. (Andrey Semashev)
//
#if (defined(__GNUC__) && ((__GNUC__ == 2 && __GNUC_MINOR__ >= 96) || (__GNUC__ >= 3))) \
    || (defined(__GNUC__) && (__INTEL_CXX_VERSION >= 800))
  #define SUPPORT_LIKELY        1
#elif defined(__clang__)
//
// clang: GCC extensions not implemented yet
// See: http://clang.llvm.org/docs/UsersManual.html#gcc-extensions-not-implemented-yet
//
  #if defined(__has_builtin)
    #if __has_builtin(__builtin_expect)
      #define SUPPORT_LIKELY    1
    #endif // __has_builtin(__builtin_expect)
  #endif // defined(__has_builtin)
#endif // SUPPORT_LIKELY

//
// Sample: since clang 3.4
//
#if defined(__clang__) && (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 4))
    // This is a sample macro.
#endif

//
// Branch prediction hints
//
#if defined(SUPPORT_LIKELY) && (SUPPORT_LIKELY != 0)
#ifndef likely
#define likely(x)               __builtin_expect(x, 1)
#endif
#ifndef unlikely
#define unlikely(x)             __builtin_expect(x, 0)
#endif
#ifndef switch_likely
#define switch_likely(x, v)     __builtin_expect(x, v)
#endif
#else
#ifndef likely
#define likely(x)               (x)
#endif
#ifndef unlikely
#define unlikely(x)             (x)
#endif
#ifndef switch_likely
#define switch_likely(x, v)     (x)
#endif
#endif // likely() & unlikely()

//
// Branch prediction hints
//
#if defined(SUPPORT_LIKELY) && (SUPPORT_LIKELY != 0)
#define _LIKELY(x)              __builtin_expect(x, 1)
#define _UNLIKELY(x)            __builtin_expect(x, 0)
#define _SWITCH_LIKELY(x, v)    __builtin_expect(x, v)
#else
#define _LIKELY(x)              (x)
#define _UNLIKELY(x)            (x)
#define _SWITCH_LIKELY(x, v)    (x)
#endif // !SUPPORT_LIKELY

//
// Aligned prefix and suffix declare
//
#if defined(_MSC_VER) || defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
#ifndef ALIGNED_PREFIX
#define ALIGNED_PREFIX(n)       _declspec(align(n))
#endif
#ifndef ALIGNED_SUFFIX
#define ALIGNED_SUFFIX(n)
#endif
#else
#ifndef ALIGNED_PREFIX
#define ALIGNED_PREFIX(n)
#endif
#ifndef ALIGNED_SUFFIX
#define ALIGNED_SUFFIX(n)       __attribute__((aligned(n)))
#endif
#endif // ALIGNED(n)

#define STD_IOS_RIGHT(width, var) \
    std::right << std::setw(width) << (var)

#define STD_IOS_LEFT(width, var) \
    std::left << std::setw(width) << (var)

#define STD_IOS_DEFAULT() \
    std::left << std::setw(0)

#define UNUSED_VARIANT(var) \
    do { \
        (void)var; \
    } while (0)

#endif // JLANG_BASIC_STDDEF_H
