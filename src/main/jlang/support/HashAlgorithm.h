
#ifndef JLANG_SUPPORT_HASHALGORITHM_H
#define JLANG_SUPPORT_HASHALGORITHM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stddef.h>
#include <stdint.h>

///////////////////////////////////////////////////
// namespace jlang::HashAlgorithm
///////////////////////////////////////////////////

namespace jlang {
namespace HashAlgorithm {

//
// BKDR Hash Function (seed = 31) -- Times31, use on Java string hashCode.
//
//   hash = hash + src[0] * seed;
//
static inline uint32_t getHash(const char * key) {
    static const uint32_t seed = 31U;   // 31, 33, 131, 1313, 13131, 131313, etc ...

    register const unsigned char * src = (const unsigned char *)key;
    register uint32_t hash = 0;

    while (*src != '\0') {
        hash += (*src) * seed;
        src++;
    }
    return hash;
}

//
// BKDR Hash Function (seed = 31) -- Times31, use on Java string hashCode.
//
//   hash = hash * seed^4 + a * seed^3 + b * seed^2 + c * seed + d;
//
static inline uint32_t getHash(const char * key, size_t len)
{
    static const uint32_t seed   = 31U;   // 31, 33, 131, 1313, 13131, 131313, etc ...
    static const uint32_t seed_2 = seed * seed;
    static const uint32_t seed_3 = seed * seed_2;
    static const uint32_t seed_4 = seed_2 * seed_2;

    register const unsigned char * src = (const unsigned char *)key;
    register const unsigned char * end = src + len;
    register uint32_t hash = 0;

    register const unsigned char * limit = src + (len & size_t(~(size_t)3U));
    while (src < limit) {
        hash = hash * seed_4 + src[0] * seed_3 + src[1] * seed_2 + src[2] * seed + src[3];
        src += 4;
    }
    while (src != end) {
        hash = hash * seed + (*src);
        src++;
    }

    return hash;
}

static inline uint64_t getHash64(const char * key) {
    static const uint64_t seed = 31ULL;   // 31, 33, 131, 1313, 13131, 131313, etc ...

    register const unsigned char * src = (const unsigned char *)key;
    register uint64_t hash = 0;

    while (*src != '\0') {
        hash += (*src) * seed;
        src++;
    }
    return hash;
}

static inline uint64_t getHash64(const char * key, size_t len)
{
    static const uint64_t seed   = 31U;   // 31, 33, 131, 1313, 13131, 131313, etc ...
    static const uint64_t seed_2 = seed * seed;
    static const uint64_t seed_3 = seed * seed_2;
    static const uint64_t seed_4 = seed_2 * seed_2;

    register const unsigned char * src = (const unsigned char *)key;
    register const unsigned char * end = src + len;
    register uint64_t hash = 0;

    register const unsigned char * limit = src + (len & size_t(~(size_t)3U));
    while (src < limit) {
        hash = hash * seed_4 + src[0] * seed_3 + src[1] * seed_2 + src[2] * seed + src[3];
        src += 4;
    }
    while (src != end) {
        hash = hash * seed + (*src);
        src++;
    }

    return hash;
}

} // namespace HashAlgorithm
} // namespace jlang

#endif // JLANG_SUPPORT_HASHALGORITHM_H
