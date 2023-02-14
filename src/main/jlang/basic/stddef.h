
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
#define likely(x)               __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)             __builtin_expect(!!(x), 0)
#endif
#ifndef switch_likely
#define switch_likely(x, v)     __builtin_expect(!!(x), (v))
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
// Aligned prefix and suffix declare
//
#if defined(_MSC_VER) || defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)
#ifndef ALIGNED_PREFIX
#define ALIGNED_PREFIX(n)       __declspec(align(n))
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

#if defined(__GNUC__) && !defined(__GNUC_STDC_INLINE__) && !defined(__GNUC_GNU_INLINE__)
  #define __GNUC_GNU_INLINE__   1
#endif

/**
 * For inline, force-inline and no-inline define.
 */
#if defined(_MSC_VER) || defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC)

#define JM_HAS_INLINE                       1

#define JM_CRT_INLINE                       extern __inline
#define JM_CRT_FORCEINLINE                  extern __forceinline
#define JM_CRT_NOINLINE                     extern __declspec(noinline)

#define JM_CRT_INLINE_DECLARE(decl)         extern __inline decl
#define JM_CRT_FORCEINLINE_DECLARE(decl)    extern __forceinline decl
#define JM_CRT_NOINLINE_DECLARE(decl)       extern __declspec(noinline) decl

#ifdef __cplusplus

  #define JM_INLINE                         __inline
  #define JM_FORCEINLINE                    __forceinline
  #define JM_NOINLINE                       __declspec(noinline)

  #define JM_INLINE_DECLARE(decl)           __inline decl
  #define JM_FORCEINLINE_DECLARE(decl)      __forceinline decl
  #define JM_NOINLINE_DECLARE(decl)         __declspec(noinline) decl

#else

  #define JM_INLINE                         JM_CRT_INLINE
  #define JM_FORCEINLINE                    JM_CRT_FORCEINLINE
  #define JM_NOINLINE                       JM_CRT_FORCEINLINE

  #define JM_INLINE_DECLARE(decl)           JM_CRT_INLINE_DECLARE(decl)
  #define JM_FORCEINLINE_DECLARE(decl)      JM_CRT_FORCEINLINE_DECLARE(decl)
  #define JM_NOINLINE_DECLARE(decl)         JM_CRT_NOINLINE_DECLARE(decl)

#endif // __cplusplus

#define JM_RESTRICT                         __restrict

#elif defined(__GNUC__) || defined(__clang__) || defined(__MINGW32__) || defined(__CYGWIN__) || defined(__linux__)

#define JM_HAS_INLINE                       1

#define JM_CRT_INLINE                       extern inline __attribute__((gnu_inline))
#define JM_CRT_FORCEINLINE                  extern inline __attribute__((always_inline))
#define JM_CRT_NOINLINE                     extern __attribute__((noinline))

#define JM_CRT_INLINE_DECLARE(decl)         extern inline __attribute__((gnu_inline)) decl
#define JM_CRT_FORCEINLINE_DECLARE(decl)    extern inline __attribute__((always_inline)) decl
#define JM_CRT_NOINLINE_DECLARE(decl)       extern __attribute__((noinline)) decl

#if __GNUC__ && !__GNUC_STDC_INLINE__

  #define JM_INLINE                         JM_CRT_INLINE
  #define JM_FORCEINLINE                    JM_CRT_FORCEINLINE
  #define JM_NOINLINE                       JM_CRT_FORCEINLINE

  #define JM_INLINE_DECLARE(decl)           JM_CRT_INLINE_DECLARE(decl)
  #define JM_FORCEINLINE_DECLARE(decl)      JM_CRT_FORCEINLINE_DECLARE(decl)
  #define JM_NOINLINE_DECLARE(decl)         JM_CRT_NOINLINE_DECLARE(decl)

#else

  #define JM_INLINE                         inline __attribute__((gnu_inline))
  #define JM_FORCEINLINE                    inline __attribute__((always_inline))
  #define JM_NOINLINE                       __attribute__((noinline))

  #define JM_INLINE_DECLARE(decl)           inline __attribute__((gnu_inline)) decl
  #define JM_FORCEINLINE_DECLARE(decl)      inline __attribute__((always_inline)) decl
  #define JM_NOINLINE_DECLARE(decl)         __attribute__((noinline)) decl

#endif // __GNUC__ && !__GNUC_STDC_INLINE__

#define JM_RESTRICT                         __restrict__

#else

#define JM_CRT_INLINE                       extern inline
#define JM_CRT_FORCEINLINE                  extern inline
#define JM_CRT_NOINLINE                     extern

#define JM_CRT_INLINE_DECLARE(decl)         extern inline decl
#define JM_CRT_FORCEINLINE_DECLARE(decl)    extern inline decl
#define JM_CRT_NOINLINE_DECLARE(decl)       extern decl

#ifdef __cplusplus

  #define JM_INLINE                         inline
  #define JM_FORCEINLINE                    inline
  #define JM_NOINLINE

  #define JM_INLINE_DECLARE(decl)           inline decl
  #define JM_FORCEINLINE_DECLARE(decl)      inline decl
  #define JM_NOINLINE_DECLARE(decl)         decl

#else

  #define JM_INLINE                         JM_CRT_INLINE
  #define JM_FORCEINLINE                    JM_CRT_FORCEINLINE
  #define JM_NOINLINE                       JM_CRT_FORCEINLINE

  #define JM_INLINE_DECLARE(decl)           JM_CRT_INLINE_DECLARE(decl)
  #define JM_FORCEINLINE_DECLARE(decl)      JM_CRT_FORCEINLINE_DECLARE(decl)
  #define JM_NOINLINE_DECLARE(decl)         JM_CRT_NOINLINE_DECLARE(decl)

#endif // __cplusplus

#define JM_RESTRICT

#endif

/**
 * For exported func
 */
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    #define JM_EXPORTED_FUNC      __cdecl
    #define JM_EXPORTED_METHOD    __thiscall
#else
    #define JM_EXPORTED_FUNC
    #define JM_EXPORTED_METHOD
#endif

#define JLANG_TO_STRING(str)    #str

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
