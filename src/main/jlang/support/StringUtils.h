
#ifndef JLANG_SUPPORT_STRINGUTILS_H
#define JLANG_SUPPORT_STRINGUTILS_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/jstd/min_max.h"
#include "jlang/lang/CharInfo.h"

#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include <string>

namespace jlang {
namespace StringUtils {

static inline
bool isEmpty(const std::string & str) {
    for (size_t i = 0; i < str.size(); ++i) {
        if (likely(str[i] != ' ')) {
            return false;
        }
    }
    return true;
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
std::string toLowerCase(const std::string & str) {
    std::string upper;
    for (size_t i = 0; i < str.size(); ++i) {
        upper.push_back(toLowerChar(str[i]));
    }
    return upper;
}

static inline
std::string toUpperCase(const std::string & str) {
    std::string upper;
    for (size_t i = 0; i < str.size(); ++i) {
        upper.push_back(toUpperChar(str[i]));
    }
    return upper;
}

template <bool windows_style = true>
static inline
size_t next_line_pos(const char * content, size_t length) {
    size_t pos = length;
    assert(content != nullptr);
    char * cur = (char *)content;
    const char * end = content + length;
    while (cur != end) {
        // For ppc or arm, added signed keyword.
        signed char c = (signed char)*cur;
        if (windows_style) {
            // windows_style
            if (likely(c != '\n')) {
                if (likely(c != '\r')) {
                    cur++;
                }
                else {
                    // Skip '\r'
                    cur++;
                    c = (signed char)*cur;
                    if (unlikely(c == '\n')) {
                        // Get new line sign
                        assert(cur >= content);
                        pos = (size_t)(cur - content - 1);
                        break;
                    }
                }
            }
            else {
                // Get new line sign
                assert(cur >= content);
                pos = (size_t)(cur - content);
                break;
            }
        }
        else {
            // non windows_style
            if (likely(c != '\n')) {
                cur++;
            }
            else {
                // Get new line sign
                assert(cur >= content);
                pos = (size_t)(cur - content);
                break;
            }
        }
    }
    return pos;
}

template <bool windows_style = true>
static inline
int next_line(char * line, size_t size, const char * content, size_t length) {
    assert(line != nullptr);
    assert(content != nullptr);
    size_t max_size = jstd::minimum(size, length);
    size_t line_size = next_line_pos<windows_style>(content, max_size);
    assert(line_size > 0);
    ::memcpy(line, content, line_size);
    return (int)line_size;
}

static inline
bool sub_str(char * text, size_t size, const char * target, size_t length) {
    assert(text != nullptr);
    assert(target != nullptr);
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
|| defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__) 
    errno_t err = ::strncpy_s(text, size, target, length);
    if (err == 0 || err == STRUNCATE) {
        text[length] = '\0';
    }
#else
    int copy_size = jstd::minimum(size - 1, length);
    if (likely(copy_size > 0)) {
        ::strncpy(text, target, copy_size);
        *(text + copy_size) = '\0';
    }
    else {
        *text = '\0';
    }
#endif
    return true;
}

static inline
bool sub_str(char * text, size_t size, const char * first, const char * last) {
    assert(last >= first);
    return StringUtils::sub_str(text, size, first, last - first);
}

template <size_t N>
static inline
bool sub_str(char (&text)[N], const char * start, size_t length) {
    return StringUtils::sub_str(text, N, start, length);
}

template <size_t N>
static inline
bool sub_str(char (&text)[N], const char * first, const char * last) {
    assert(last >= first);
    return StringUtils::sub_str(text, N, first, last);
}

static inline
intptr_t copy(std::string & str, const char * first, const char * last) {
    assert(first >= 0);
    assert(last >= first);
    str.append(first, (size_t)(last - first));
    return (last - first);
}

} // namespace StringUtils
} // namespace jlang

#endif // JLANG_SUPPORT_STRINGUTILS_H
