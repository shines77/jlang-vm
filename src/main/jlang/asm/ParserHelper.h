
#ifndef JLANG_ASM_PARSERHELPER_H
#define JLANG_ASM_PARSERHELPER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include <string>

#include "jlang/lang/CharInfo.h"
#include "jlang/lang/NonCopyable.h"

#define USE_CHARINFO_MASK   1

namespace jlang {
namespace jasm {

class ParserHelper : public lang::NonCopyable {
public:
    ParserHelper() {}
    ~ParserHelper() {}

    static inline bool isWhiteSpace(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::IsWhiteSpace(ch);
#else
        if (ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f')
            return true;
        else
            return false;
#endif
    }

    static inline bool isWhiteSpaces(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::IsWhiteSpaces(ch);
#else
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\v' || ch == '\f')
            return true;
        else
            return false;
#endif
    }

    static inline bool isNewLine(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::IsNewLine(ch);
#else
        if (ch == '\n' || ch == '\r')
            return true;
        else
            return false;
#endif
    }

    // Identifier first letter: not include any numbers, parse priority order is: 'abc_DEF'.
    static inline bool isIdentifierFirst(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::IsIdentifierFirst(ch);
#else
        if ((ch >= 'a' && ch <= 'z') || (ch == '_') || (ch >= 'A' && ch <= 'Z'))
            return true;
        else
            return false;
#endif
    }

    // Identifier body: include numbers, parse priority order is: 'abc_789DEF'.
    static inline bool isIdentifierBody(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::IsIdentifierBody(ch);
#else
        return ((ch >= 'a' && ch <= 'z') || (ch == '_') ||
                (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z'));
#endif
    }

    // Reserved keyword: parse priority order is: 'abc_DEF', not include any numbers.
    static inline bool isReservedKeyword(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::isReservedKeyword(ch);
#else
        return ((ch >= 'a' && ch <= 'z') || (ch == '_') || (ch >= 'A' && ch <= 'Z'));
#endif
    }

    static inline bool isDigital(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::IsDigital(ch);
#else
        return (ch >= '0' && ch <= '9');
#endif
    }

    static inline bool isAlphabet(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::isAlphabet(ch);
#else
        return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
#endif
    }

    static inline bool isLowerAlphabet(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::IsLowerCase(ch);
#else
        return ((ch >= 'a' && ch <= 'z'));
#endif
    }

    static inline bool isUpperAlphabet(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::IsUpperCase(ch);
#else
        return (ch >= 'A' && ch <= 'Z');
#endif
    }

    static inline bool isOperator(unsigned char ch) {
#if USE_CHARINFO_MASK
        return CharInfo::IsOperator(ch);
#else
        switch (ch) {
            case '!': case '%': case '&': case '*': case '+': 
            case '-': case ':': case '<': case '=': case '>': 
            case '?': case '^': case '|': case '~':
                return true;
            default:
                return false;
        }
#endif
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_PARSERHELPER_H
