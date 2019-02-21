
#ifndef JLANG_SUPPORT_STRINGUTILS_H
#define JLANG_SUPPORT_STRINGUTILS_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/CharInfo.h"

#include <stdint.h>
#include <string>

namespace jlang {
namespace StringUtils {

static inline
bool IsEmpty(const std::string & str) {
    bool isEmpty = true;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] != ' ') {
            isEmpty = false;
            break;
        }
    }
    return isEmpty;
}

static inline
unsigned char toLowerChar(unsigned char ch)
{
    if (CharInfo::IsUpperCase(ch)) {
        ch += ('a' - 'A');
    }
    return ch;
}

static inline
unsigned char toUpperChar(unsigned char ch)
{
    if (CharInfo::IsLowerCase(ch)) {
        ch -= ('a' - 'A');
    }
    return ch;
}

static inline
std::string ToLowerCase(const std::string & str) {
    std::string upper;
    for (size_t i = 0; i < str.size(); ++i) {
        upper.push_back(toLowerChar(str[i]));
    }
    return upper;
}

static inline
std::string ToUpperCase(const std::string & str) {
    std::string upper;
    for (size_t i = 0; i < str.size(); ++i) {
        upper.push_back(toUpperChar(str[i]));
    }
    return upper;
}

} // namespace StringUtils
} // namespace jlang

#endif // JLANG_SUPPORT_STRINGUTILS_H
