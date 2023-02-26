
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
#include <cstring>

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
    if (CharInfo::isUpperCase(ch)) {
        ch += ('a' - 'A');
    }
    return ch;
}

static inline
unsigned char toUpperChar(unsigned char ch)
{
    if (CharInfo::isLowerCase(ch)) {
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
size_t get_next_line_pos(const char * content, size_t length) {
    assert(content != nullptr);
    size_t offset;
    // For ppc or arm, added signed keyword.
    signed char * cur = (signed char *)content;
    const signed char * end = (signed char *)content + length;

    // For efficiency, the '\0' character is not scanned.
    while (cur < end) {
        if (windows_style) {
            // Windows style
            if (likely(*cur != '\r')) {
                if (likely(*cur != '\n')) {
                    cur++;
                }
                else {
                    // The new line char
                    break;
                }
            }
            else {
                // The new line char
                break;
            }
        }
        else {
            // Non windows style
            if (likely(*cur != '\n')) {
                cur++;
            }
            else {
                // The new line char         
                break;
            }
        }
    }
    assert(cur >= content);
    size_t offset = (size_t)((const char * )cur - content);
    return offset;
}

template <bool windows_style = true>
static inline
size_t read_a_line(char * line_buf, size_t buf_size, const char * content, size_t length) {
    assert(buf != nullptr);
    assert(content != nullptr);
    buf_size -= (buf_size != 0);
    size_t max_len = jstd::Min(buf_size, length);
    size_t line_size = get_next_line_pos<windows_style>(content, max_len);
    if (line_size > 0) {
        std::memcpy(line_buf, content, line_size);
    }
    line_buf[line_size] = '\0';
    return line_size;
}

template <bool windows_style = true>
static inline
size_t read_a_line(std::string & line, const char * content, size_t length) {
    assert(content != nullptr);
    size_t line_size = get_next_line_pos<windows_style>(content, length);
    if (line_size > 0) {
        line.append(content, line_size);
    }
    line.push_back('\0');
    return line_size;
}

static inline
intptr_t sub_str(char * text, size_t size, const char * target, size_t length) {
    assert(text != nullptr);
    assert(target != nullptr);
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
|| defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
    intptr_t copy_size = 0;
    errno_t err = ::strncpy_s(text, size, target, length);
    if (err == 0 || err == STRUNCATE) {
        text[length] = '\0';
        copy_size = (intptr_t)jstd::Min(size - 1, length);
    }
#else
    intptr_t copy_size = (intptr_t)jstd::Min(size - 1, length);
    if (likely(copy_size > 0)) {
        std::strncpy(text, target, copy_size);
        *(text + copy_size) = '\0';
    }
    else {
        *text = '\0';
    }
#endif
    return copy_size;
}

static inline
intptr_t sub_str(char * text, size_t length, const char * first, const char * last) {
    assert(last >= first);
    return StringUtils::sub_str(text, length, first, last - first);
}

template <size_t N>
static inline
intptr_t sub_str(char (&text)[N], const char * start, size_t length) {
    return StringUtils::sub_str(text, N, start, length);
}

template <size_t N>
static inline
intptr_t sub_str(char (&text)[N], const char * first, const char * last) {
    assert(last >= first);
    return StringUtils::sub_str(text, N, first, last);
}

static inline
intptr_t sub_str(std::string & str, const char * first, const char * last) {
    assert(first >= 0);
    assert(last >= first);
    str.clear();
    str.append(first, (size_t)(last - first));
    return (last - first);
}

static inline
intptr_t append(std::string & str, const char * first, const char * last) {
    assert(first >= 0);
    assert(last >= first);
    str.append(first, (size_t)(last - first));
    return (last - first);
}

} // namespace StringUtils
} // namespace jlang

#endif // JLANG_SUPPORT_STRINGUTILS_H
