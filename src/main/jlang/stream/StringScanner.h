
#ifndef JLANG_STREAM_STRINGSCANNER_H
#define JLANG_STREAM_STRINGSCANNER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"
#include "jlang/lang/Char.h"
#include "jlang/stream/StringStream.h"

#include <stddef.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <assert.h>

namespace jlang {

class StringScanner : public StringStream {
public:
    StringScanner() {}
    StringScanner(const StringScanner & scanner) : StringStream(scanner.getStream()) {}
    StringScanner(StringScanner && scanner) : StringStream(std::forward<StringStream>(scanner.getStream())) {}
    StringScanner(const StringStream & stream) : StringStream(stream) {}
    StringScanner(StringStream && stream) : StringStream(std::forward<StringStream>(stream)) {}
    virtual ~StringScanner() {
    }

    /* WhiteSpace */

    bool isWhiteSpace(uint8_t ch) const {
        return Char::isWhiteSpace(ch);
    }

    bool isWhiteSpace() const {
        uint8_t ch = this->getu();
        return Char::isWhiteSpace(ch);
    }

    void skipWhiteSpace() {
        do {
            if (likely(isWhiteSpace()))
                this->next();
            else
                break;
        } while (1);
    }

    void skipWhiteSpace_0() {
        do {
            if (likely(!isWhiteSpace()))
                break;
            else
                this->next();
        } while (1);
    }

    void skipWhiteSpace_1() {
        if (likely(isWhiteSpace())) {
            this->next();
            // Expect to skip 0 WhiteSpace char.
            skipWhiteSpace_0();
        }
    }

    /* WhiteSpaces */

    bool isWhiteSpaces(uint8_t ch) {
        return Char::isWhiteSpaces(ch);
    }

    bool isWhiteSpaces() {
        uint8_t ch = this->getu();
        return Char::isWhiteSpaces(ch);
    }

    void skipWhiteSpaces() {
        do {
            if (likely(isWhiteSpaces()))
                this->next();
            else
                break;
        } while (1);
    }

    void skipWhiteSpaces_0() {
        do {
            if (likely(!isWhiteSpaces()))
                break;
            else
                this->next();
        } while (1);
    }

    void skipWhiteSpaces_1() {
        if (likely(isWhiteSpaces())) {
            this->next();
            // Expect to skip 0 WhiteSpaces char.
            skipWhiteSpaces_0();
        }
    }

    /* NewLine */

    bool isNewLine(uint8_t ch) {
        return Char::isNewLine(ch);
    }

    bool isNewLine() {
        uint8_t ch = this->getu();
        return Char::isNewLine(ch);
    }

    void skipNewLine() {
        do {
            if (isNewLine())
                this->next();
            else
                break;
        } while (1);
    }

    void skipToNewLine() {
        while (!this->is_null()) {
            if (!isNewLine())
                this->next();
            else
                break;
        }
    }

    /* Identifier */

    bool isIdentifierFirst(uint8_t ch) {
        return Char::isIdentifierFirst(ch);
    }

    // Identifier first letter: can not be a number, parse priority order: 'abcDEF_'.
    bool isIdentifierFirst() {
        uint8_t ch = this->getu();
        return Char::isIdentifierFirst(ch);
    }

    // Check if is identifier first letter and skip to next.
    bool skipIdentifierFirst() {
        if (isIdentifierFirst()) {
            this->next();
            return true;
        }
        return false;
    }

    bool isIdentifierBody(uint8_t ch) const {
        return Char::isIdentifierBody(ch);
    }

    // Identifier body: include numbers, parse priority order: 'abc789_DEF'.
    bool isIdentifierBody() const {
        uint8_t ch = this->getu();
        return Char::isIdentifierBody(ch);
    }

    // Skip the identifier body.
    void skipIdentifierBody() {
        do {
            if (isIdentifierBody())
                this->next();
            else
                break;
        } while (1);
    }

    bool isIdentifier(uint8_t ch) {
        // Identifier first char can not be a number.
        return isIdentifierFirst(ch);
    }

    bool isIdentifier() {
        // Identifier first char can not be a number.
        return isIdentifierFirst();
    }

    void skipIdentifier() { 
        // Identifier first char can not be a number.
        if (isIdentifierFirst()) {
            this->next();
            skipIdentifierBody();
        }
    }

    /* Number */

    bool isDigital(uint8_t ch) const {
        return Char::isDigital(ch);
    }

    bool isDigital() const {
        uint8_t ch = this->getu();
        return Char::isDigital(ch);
    }

    // Starting with numbers: "[0-9]", or ".[0-9]"
    inline bool isNumber() {
        uint8_t ch = this->getu();
        return (Char::isDigital(ch) || (ch == '.' && this->remain() > 1 &&
                Char::isDigital(this->getu(1))));
    }

    // Skip the numbers.
    void skipNumbers() {
        uint8_t ch1 = this->getu();
        uint8_t ch2 = this->getu(1);
        if ((ch1 == '0' && ch2 == '.') ||
            (ch1 == '.' && Char::isDigital(ch2))) {
            this->next(2);
        }

        do {
            if (likely(isDigital()))
                this->next();
            else
                break;
        } while (1);
    }

    // Skip the signed numbers.
    int skipSignedNumbers() {
        uint8_t ch = this->getu();
        int positive;
        if (likely(ch != '-')) {
            if (likely(ch != '+')) {
                positive = 0;
            } else {
                this->next();
                positive = 1;
            }
        } else {
            this->next();
            positive = -1;
        }
        skipNumbers();
        return positive;
    }

    /* Alphabet */

    bool isAlphabet(uint8_t ch) const {
        return Char::isAlphabet(ch);
    }

    bool isAlphabet() const {
        uint8_t ch = this->getu();
        return Char::isAlphabet(ch);
    }

    bool isLowerAlphabet(uint8_t ch) const {
        return Char::isLowerAlphabet(ch);
    }

    bool isLowerAlphabet() const {
        uint8_t ch = this->getu();
        return Char::isLowerAlphabet(ch);
    }

    bool isUpperAlphabet(uint8_t ch) const {
        return Char::isUpperAlphabet(ch);
    }

    bool isUpperAlphabet() const {
        uint8_t ch = this->getu();
        return Char::isUpperAlphabet(ch);
    }

    void skipAlphabet() {
        do {
            if (likely(isAlphabet()))
                this->next();
            else
                break;
        } while (1);
    }

    void skipLowerAlphabet() {
        do {
            if (likely(isLowerAlphabet()))
                this->next();
            else
                break;
        } while (1);
    }

    void skipUpperAlphabet() {
        do {
            if (likely(isUpperAlphabet()))
                this->next();
            else
                break;
        } while (1);
    }

    void skipExpression() {
        skipToNewLine();
    }
};

} // namespace jlang

#endif // JLANG_STREAM_STRINGSCANNER_H
