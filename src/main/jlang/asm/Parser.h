
#ifndef JLANG_ASM_PARSER_H
#define JLANG_ASM_PARSER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <memory.h>

#include <string>

#include "jlang/lang/ErrorCode.h"
#include "jlang/lang/CharInfo.h"
#include "jlang/asm/Keyword.h"
#include "jlang/asm/Token.h"
#include "jlang/stream/StreamMarker.h"
#include "jlang/asm/ParserHelper.h"
#include "jlang/support/HashAlgorithm.h"

namespace jlang {
namespace jasm {

class Parser {
public:
    typedef Parser          this_type;
    typedef ParserHelper    Helper;

protected:
    StringStream stream_;
    std::string filename_;
    Token token_;
    std::string identifier_;

public:
    Parser() : token_(Token::Unknown, 0, 0) {}
    Parser(const std::string & filename)
        : filename_(filename), token_(Token::Unknown, 0, 0) {
        // Do nothing !!
    }
    virtual ~Parser() {}

    // NonCopyable
    Parser(const Parser & src) = delete;
    Parser(Parser && src) = delete;
    Parser & operator = (const Parser & rhs) = delete;

private:
    uint32_t getHash32(const char * keyword) const {
        return HashAlgorithm::getHash(keyword);
    }

    uint32_t getHash32(const char * keyword, size_t length) const {
        return HashAlgorithm::getHash(keyword, length);
    }

    uint64_t getHash64(const char * keyword) const {
        return HashAlgorithm::getHash64(keyword);
    }

    uint64_t getHash64(const char * keyword, size_t length) const {
        return HashAlgorithm::getHash64(keyword, length);
    }

    bool isWhiteSpace(uint8_t ch) const {
        return Helper::isWhiteSpace(ch);
    }

    void skipWhiteSpace() {
        do {
            uint8_t ch = stream_.getu();
            if (isWhiteSpace(ch))
                stream_.next();
            else
                break;
        } while (1);
    }

    bool isWhiteSpaces(uint8_t ch) {
        return Helper::isWhiteSpaces(ch);
    }

    void skipWhiteSpaces() {
        do {
            uint8_t ch = stream_.getu();
            if (isWhiteSpaces(ch))
                stream_.next();
            else
                break;
        } while (1);
    }

    bool isNewLine(uint8_t ch) {
        return Helper::isNewLine(ch);
    }

    void skipNewLine() {
        do {
            uint8_t ch = stream_.getu();
            if (isNewLine(ch))
                stream_.next();
            else
                break;
        } while (1);
    }

    void skipToNewLine() {
        while (!stream_.is_eof()) {
            uint8_t ch = stream_.getu();
            if (!isNewLine(ch))
                stream_.next();
            else
                break;
        }
    }

    // Identifier first letter: can not be a number, parse priority order: 'abcDEF_'.
    bool isIdentifierFirst(uint8_t ch) {
        return Helper::isIdentifierFirst(ch);
    }

    // Check if is identifier first letter and skip to next.
    bool skipIdentifierFirst() {
        uint8_t ch = stream_.getu();
        if (isIdentifierFirst(ch)) {
            stream_.next();
            return true;
        }
        return false;
    }

    // Identifier body: include numbers, parse priority order: 'abc789_DEF'.
    inline bool isIdentifierBody(uint8_t ch) const {
        return Helper::isIdentifierBody(ch);
    }

    // Skip the identifier body.
    void skipIdentifierBody() {
        do {
            uint8_t ch = stream_.getu();
            if (isIdentifierBody(ch))
                stream_.next();
            else
                break;
        } while (1);
    }

    bool isIdentifier(uint8_t ch) {
        // Identifier first char can not be a number.
        if (isIdentifierFirst(ch)) {
            stream_.next();
            skipIdentifierBody();
            return true;
        }
        return false;
    }

    void skipIdentifier() { 
        // Identifier first char can not be a number.
        uint8_t ch = stream_.getu();
        if (isIdentifierFirst(ch)) {
            stream_.next();
            skipIdentifierBody();
        }
    }

    // Identifier keyword: parse priority order: 'abc_DEF', have no numbers.
    inline bool isReservedKeyword(uint8_t ch) const {
        return Helper::isReservedKeyword(ch);
    }

    // Skip the internal keyword.
    void skipReservedKeyword() {
        do {
            uint8_t ch = stream_.getu();
            if (isReservedKeyword(ch))
                stream_.next();
            else
                break;
        } while (1);
    }

    bool isDigital(uint8_t ch) const {
        return Helper::isDigital(ch);
    }

    // Starting with numbers: "[0-9]", or ".[0-9]"
    inline bool isNumber(uint8_t ch) {
        return (isDigital(ch) || (ch == '.' && stream_.remain() > 1 &&
                isDigital(stream_.getu(1))));
    }

    // Skip the numbers.
    void skipNumbers() {
        uint8_t ch = stream_.getu();
        if ((ch == '0' && stream_.get(1) == '.') ||
            (ch == '.' && isDigital(stream_.get(1)))) {
            stream_.next(2);
        }

        do {
            uint8_t ch = stream_.getu();
            if (isDigital(ch))
                stream_.next();
            else
                break;
        } while (1);
    }

    // Skip the signed numbers.
    int skipSignedNumbers() {
        uint8_t ch = stream_.getu();
        int positive;
        if (likely(ch != '-')) {
            if (likely(ch != '+')) {
                positive = 0;
            } else {
                stream_.next();
                positive = 1;
            }
        } else {
            stream_.next();
            positive = -1;
        }
        skipNumbers();
        return positive;
    }

    inline bool isAlphabet(uint8_t ch) const {
        return Helper::isAlphabet(ch);
    }

    inline bool isLowerAlphabet(uint8_t ch) const {
        return Helper::isLowerAlphabet(ch);
    }

    inline bool isUpperAlphabet(uint8_t ch) const {
        return Helper::isUpperAlphabet(ch);
    }

    void skipAlphabet() {
        do {
            uint8_t ch = stream_.getu();
            if (isAlphabet(ch))
                stream_.next();
            else
                break;
        } while (1);
    }

    void skipLowerAlphabet() {
        do {
            uint8_t ch = stream_.getu();
            if (isLowerAlphabet(ch))
                stream_.next();
            else
                break;
        } while (1);
    }

    void skipUpperAlphabet() {
        do {
            uint8_t ch = stream_.getu();
            if (isUpperAlphabet(ch))
                stream_.next();
            else
                break;
        } while (1);
    }

    void skipExpression() {
        skipToNewLine();
    }

public:
    bool parseIdentifier(ErrorCode & ec) {
        StringStreamMarker marker(stream_);
        marker.remark();
        skipIdentifier();
        intptr_t ident_length = marker.length();
        if (ident_length > 0) {
            intptr_t ident_start = marker.start_pos();
            intptr_t ident_end = marker.end_pos();
            char ident_name[MAX_IDENTIFIER_LEN];
            marker.copy_string(ident_name);

            KeywordMapping & keyMapping = Global::getKeywordMapping();
            assert(keyMapping.inited());
            KeywordMapping::iterator iter = keyMapping.find(ident_name, (size_t)ident_length);
            if (iter != keyMapping.end()) {
                Keyword keyword = iter->second;
                return true;
            }
        }
        return false;
    }

    bool parseReservedKeyword(ErrorCode & ec) {
        StringStreamMarker marker(stream_);
        marker.remark();
        skipReservedKeyword();
        intptr_t keyword_length = marker.length();
        if (keyword_length > 0) {
            intptr_t keyword_start = marker.start_pos();
            intptr_t keyword_end = marker.end_pos();
            char keyword_name[MAX_IDENTIFIER_LEN];
            marker.copy_string(keyword_name);

#if (KEYWORD_HASHCODE_WORDLEN == 32)
            uint32_t hash32 = getHash32(keyword_name, (size_t)keyword_length);
#elif (KEYWORD_HASHCODE_WORDLEN == 64)
            uint64_t hash64 = getHash64(keyword_name, (size_t)keyword_length);
#endif
            KeywordMapping & keyMapping = Global::getKeywordMapping();
            assert(keyMapping.inited());
            KeywordMapping::iterator iter = keyMapping.find(keyword_name, (size_t)keyword_length);
            if (iter != keyMapping.end()) {
                Keyword keyword = iter->second;
                if (keyword.getKind() == KeywordKind::PodType ||
                    keyword.getKind() == KeywordKind::TypeDef) {
                    if (keyword.getKind() != KeywordKind::PodType) {
                        // Parse full type define
                    }
                    skipWhiteSpace();

                    char identifier_name[MAX_IDENTIFIER_LEN];
                    StringStreamMarker identifierMarker(stream_);
                    intptr_t identifier_start, identifier_end;

                    identifierMarker.remark();
                    skipIdentifier();
                    identifier_start = identifierMarker.start_pos();
                    identifier_end = identifierMarker.end_pos();
                    assert(identifier_end > identifier_start);
                    identifierMarker.copy_string(identifier_name);

                    skipWhiteSpace();

                    //LexerLog::traceTypeDeclare(keyword_start, keyword_end, keyword_name);

                    if (stream_.get() == '(') {
                        // It's a function
                        //LexerLog::traceFunction(identifier_start, identifier_end, identifier_name);
                    }
                    else if (stream_.get() == ';') {
                        // It's a type define or others
                        //LexerLog::traceIdentifier(identifier_start, identifier_end, identifier_name);
                        stream_.next();
                    }
                    else if (stream_.get() == ',') {
                        // It's a type-list define or others list
                        //LexerLog::traceIdentifier(identifier_start, identifier_end, identifier_name);

                        do {
                            stream_.next();
                            skipWhiteSpace();
                            identifierMarker.remark();
                            skipIdentifier();
                            identifier_start = identifierMarker.start_pos();
                            identifier_end = identifierMarker.end_pos();
                            if (identifier_end <= identifier_start) {
                                // Type-list define failure
                                std::cout << "*** Error: Type-list define failure. *** ";
                                return false;
                            }
                            assert(identifier_end > identifier_start);
                            identifierMarker.copy_string(identifier_name);
                            skipWhiteSpace();

                            //LexerLog::traceIdentifier(identifier_start, identifier_end, identifier_name);
                        } while (stream_.get() == ',');

                        skipWhiteSpace();
                        if (stream_.get() == ';') {
                            stream_.next();
                        }
                    }
                    else {
                        //
                    }
                }
                else if (keyword.getKind() == KeywordKind::UserDefine) {
                    // User define function or variant
                    skipWhiteSpace();
                }
                else {
                    //LexerLog::traceKeyword(keyword_start, keyword_end, keyword_name);
                }
            }
            else {
                skipWhiteSpace();
                if (stream_.get() == '(') {
                    // It's a function call
                    //LexerLog::traceFunction(keyword_start, keyword_end, keyword_name);

                    //stream_.next();
                }
                else {
                    // Maybe is a identifier
                    //LexerLog::traceIdentifier(keyword_start, keyword_end, keyword_name);
                }
            }
            return true;
        }
        return false;
    }

    bool isPreprocessing() {
        if (stream_.get() == PREPROCESSING_CHAR)
            return true;
        else
            return false;
    }

    bool parsePreprocessing(Token & token, ErrorCode & ec) {
        bool is_ok = true;
        Token::Type tokenType = Token::Unknown;
        ec = ErrorCode::OK;
        StringStreamMarker marker(stream_);
        marker.set_mark();
        skipReservedKeyword();
        intptr_t keyword_length = marker.length();
        if (keyword_length > 0) {
            char * keyword_start = marker.start();
            std::string keyword_name(keyword_start, (size_t)keyword_length);
            KeywordMapping & ppKeyMapping = Global::getPreprocessingKeywordMapping();
            KeywordMapping::iterator iter = ppKeyMapping.find(keyword_name);
            if (iter != ppKeyMapping.end()) {
                Keyword & keyword = iter->second;
                assert(keyword.getKind() == KeywordKind::Preprocessing);
                tokenType = keyword.getToken();
                token.setStartPos(marker.start_pos());
                token.setLength(keyword_length);
                ec = handlePreprocessingToken(tokenType, token);
                if (ec.isOK()) {
                    token.setType(tokenType);
                }
                else {
                    // Have some errors.
                    is_ok = false;
                }
            }
            else {
                // Error: It's a unknown preprocessing keyword.
                is_ok = false;
                ec = ErrorCode::UnknownPreprocessingKeyword;
            }
        }
        else {
            // Error: It's not a normal preprocessing keyword.
            is_ok = false;
            ec = ErrorCode::IllegalPreprocessingKeyword;
        }
        if (tokenType != Token::Unknown) {
            token.setToken(tokenType, marker.start_pos(), marker.length());
        }
        else {
            token.setToken(tokenType, marker.start_pos(), 0);
        }
        return is_ok;
    }

    ErrorCode handlePreprocessingToken(Token::Type tokenType, const Token & token) {
        ErrorCode ec = ErrorCode::OK;
        switch (tokenType) {
        case Token::pp_if:
            //
            break;

        case Token::pp_ifdef:
            //
            break;

        case Token::pp_ifndef:
            //
            break;

        case Token::pp_else:
            //
            break;

        case Token::pp_elif:
            //
            break;

        case Token::pp_endif:
            //
            break;

        case Token::pp_define:
            {
                // #define ABC 1
                std::string macro_name;
            }
            break;

        case Token::pp_defined:
            //
            break;

        case Token::pp_include:
            {
                // #include <xxxx.h>
                std::string include_file;
            }
            break;

        default:
            ec = ErrorCode::UnknownPreprocessingKeyword;
            break;
        }
        return ec;
    }

    bool isComment() const {
        // The comment must be starting with ';' or '/' char.
        if (stream_.get() == ';' || stream_.get() == '/')
            return true;
        else
            return false;
    }

    bool skipLineComment() {
        bool is_completed = false;
        char cur;
        while ((cur = stream_.get()) != '\0') {
            if (likely(cur != '\n' && cur != '\r')) {
                stream_.next();
            }
            else {
                stream_.next();
                // Find the end of line comment.
                is_completed = true;
                break;
            }
        }
        return is_completed;
    }

    bool skipBlockComment() {
        bool is_completed = false;
        char cur;
        while ((cur = stream_.get()) != '\0') {
            if (likely(cur != '*')) {
                stream_.next();
            }
            else {
                stream_.next();
                if (stream_.get() == '/') {
                    stream_.next();
                    // Find the end of block comment.
                    is_completed = true;
                    break;
                }
            }
        }
        return is_completed;
    }

    bool parseComment(Token & token, ErrorCode & ec) {
        bool is_comments;
        char ch = stream_.get();
        if (ch == ';' || ch == '/') {
            // Line comment
            stream_.next();
            bool is_completed = skipLineComment();
            if (is_completed)
                ec = ErrorCode::OK;
            else 
                ec = ErrorCode::IllegalLineComment;
            token.setType(Token::LineComment);
            is_comments = true;
        }
        else if (ch == '*') {
            // Block comment
            stream_.next();
            bool is_completed = skipBlockComment();
            if (is_completed)
                ec = ErrorCode::OK;
            else 
                ec = ErrorCode::IllegalBlockComment;
            token.setType(Token::BlockComment);
            is_comments = true;
        }
        else {
            ec = ErrorCode::OK;
            is_comments = false;
        }
        return is_comments;
    }

    bool parseNumberSuffix(int & valueType) {
        int8_t ch = stream_.get();
        intptr_t remain = stream_.remain();
        valueType = ValueType::Unknown;
        if (ch == 'u' || ch == 'U') {
            // u, ul, ull and U, UL, ULL
            int8_t longChar = ch - 'U' + 'L';
            if (stream_.get(1) == '6' && stream_.get(2) == '4' && remain >= 3) {
                valueType = ValueType::UInt64;
                stream_.next(3);
            }
            else if (stream_.get(1) == longChar && remain >= 2) {
                if (stream_.get(2) == longChar && remain >= 3) {
                    valueType = ValueType::UInt64;
                    stream_.next(3);
                }
                else {
                    valueType = ValueType::UInt32;
                    stream_.next(2);
                }
            }
            else  {
                valueType = ValueType::UInt32;
                stream_.next();
            }
        }
        else if (ch == 'l' || ch == 'L') {
            // l, ll and L, LL
            if (stream_.get(1) == ch && remain >= 2) {
                valueType = ValueType::Int64;
                stream_.next(2);
            }
            else {
                valueType = ValueType::Int32;
                stream_.next();
            }
        }
        else if (ch == 'i' || ch == 'I') {
            // i, I, i64, I64
            if (stream_.get(1) == '6' && stream_.get(2) == '4' && remain >= 3) {
                valueType = ValueType::Int64;
                stream_.next(3);
            }
            else {
                valueType = ValueType::Int32;
                stream_.next();
            }
        }
        else if (ch == 'f' || ch == 'F') {
            // f, F
            valueType = ValueType::Fp64;
            stream_.next();
        }
        return (valueType != ValueType::Unknown);
    }

    template <uint32_t radix = 10>
    bool parseRadixNumber(uint64_t & value) {
        bool is_valid;
        value = 0;
        const char * marker = stream_.current();
        if (radix == 16) {
            int valueType = ValueType::Unknown;
            char ch;
            while ((ch = stream_.get()) != '\0') {
                if (ch >= '0' && ch <= '9') {
                    ch -= '0';
                }
                else if (ch >= 'A' && ch <= 'F') {
                    ch -= 'A' - 10;
                }
                else if (ch >= 'a' && ch <= 'f') {
                    ch -= 'a' - 10;
                }
                else {
                    break;
                }
                value = value * radix + ch;
                stream_.next();
            }
            is_valid = (stream_.current() > marker);
            parseNumberSuffix(valueType);
            skipWhiteSpaces();
        }
        else if (radix == 10) {
            int valueType = ValueType::Unknown;
            char ch;
            while ((ch = stream_.get()) != '\0') {
                if (ch >= '0' && ch <= '9')
                    ch -= '0';
                else
                    break;
                value = value * radix + ch;
                stream_.next();
            }
            is_valid = (stream_.current() > marker);
            parseNumberSuffix(valueType);
            skipWhiteSpaces();
        }
        else if (radix == 8) {
            int valueType = ValueType::Unknown;
            char ch;
            while ((ch = stream_.get()) != '\0') {
                if (ch >= '0' && ch <= '7')
                    ch -= '0';
                else
                    break;
                value = value * radix + ch;
                stream_.next();
            }
            is_valid = (stream_.current() > marker);
            parseNumberSuffix(valueType);
            skipWhiteSpaces();
        }
        else if (radix == 2) {
            int valueType = ValueType::Unknown;
            char ch;
            while ((ch = stream_.get()) != '\0') {
                if (ch >= '0' && ch <= '1')
                    ch -= '0';
                else
                    break;
                value = value * radix + ch;
                stream_.next();
            }
            is_valid = (stream_.current() > marker);
            parseNumberSuffix(valueType);
            skipWhiteSpaces();
        }
        else {
            is_valid = false;
        }
        return is_valid;
    }

    bool parseRadixNumber(Token::Type & tokenType, ErrorCode & ec,
                          int & radix, uint64_t & number) {
        bool parse_ok = true;
        bool is_valid;
        char ch1 = stream_.get(1);
        if (ch1 == 'x' || ch1 == 'X') {
            radix = 16;
            stream_.next(2);
            is_valid = parseRadixNumber<16>(number);
            tokenType = Token::HexLiteral;
            if (!is_valid) {
                ec = ErrorCode::IllegalRadix16Number;
            }
        }
        else if (ch1 == 'o' || ch1 == 'O') {
            radix = 8;
            stream_.next(2);
            is_valid = parseRadixNumber<8>(number);
            tokenType = Token::OcxLiteral;
            if (!is_valid) {
                ec = ErrorCode::IllegalRadix8Number;
            }
        }
        else if (ch1 == 'b' || ch1 == 'B') {
            radix = 2;
            stream_.next(2);
            is_valid = parseRadixNumber<2>(number);
            tokenType = Token::BinaryLiteral;
            if (!is_valid) {
                ec = ErrorCode::IllegalRadix2Number;
            }
        }
        else if (ch1 == 'd' || ch1 == 'D') {
            radix = 10;
            stream_.next(2);
            is_valid = parseRadixNumber<10>(number);
            tokenType = Token::DecLiteral;
            if (!is_valid) {
                ec = ErrorCode::IllegalRadix10Number;
            }
        }
        else {
            // It's not a based number, re-parse from old place.
            parse_ok = false;
            is_valid = false;
        }
        return parse_ok;
    }

    bool parseRealNumber(Token::Type & tokenType, ErrorCode & ec,
                         uint64_t & integer, uint64_t & fractional,
                         int & exponent, bool & is_float) {
        bool is_ok;
        int integer_len = 0;
        int fractional_len = 0;
        integer = 0;
        fractional = 0;
        exponent = 0;

        // Whether including a decimal point?
        bool hasDots = false;
        // Whether including a exponent?
        bool hasExponent = false;
        // Whether is a double-precision floating-point number?
        bool isDouble = true;

        // Search the integer part of floating-point number or integer.
        do {
            char ch = stream_.get();
            if (ch >= '0' && ch <= '9') {
                // Integer part
                integer = integer * 10 + ch - '0';
                integer_len++;
                stream_.next();
            }
            else if (ch == '.') {
                hasDots = true;
                stream_.next();
                break;
            }
            else {
                break;
            }
        } while (1);

        if (hasDots) {
            // Search the fractional part of floating-point number.
            do {
                char ch = stream_.get();
                if (ch >= '0' && ch <= '9') {
                    // Fractional part
                    fractional = fractional * 10 + ch - '0';
                    fractional_len++;
                    stream_.next();
                }
                else if (ch == '.') {
                    // Found the second decimal point, exit now.
                    ec = ErrorCode::IllegalFloatNumber;
                    break;
                }
                else {
                    break;
                }
            } while (1);
        }

        // Determine the exponent of floating point numbers.
        do {
            int exponent_sign = 1;
            unsigned int exponent_num = 0;
            int exponent_cnt = 0;
            char ch = stream_.get();
            if (ch == 'e' || ch == 'E') {
                ch = stream_.get(1);
                if (ch == '-') {
                    exponent_sign = -1;
                    stream_.next(2);
                }
                else if (ch == '+') {
                    stream_.next(2);
                }

                do {
                    ch = stream_.get();
                    if (ch >= '0' && ch <= '9') {
                        exponent_num = exponent_num * 10 + ch - '0';
                        exponent_cnt++;
                        stream_.next();
                    }
                    else {
                        break;
                    }
                } while (1);

                if (exponent_cnt > 10 || (exponent_cnt == 10 && exponent_num < 1000000000)) {
                    // Exponent part is overflow
                    ec = ErrorCode::ErrorExponentPartOverflow;
                }
                else if (exponent_cnt <= 0) {
                    // Does not contain the exponential number, it's a illegal error.
                    ec = ErrorCode::IllegalExponentPart;
                }
                else {
                    // The exponent is smaller than -2147483648 or bigger than 2147483647 !
                    if ((exponent_sign != -1 && exponent_num > 2147483647) ||
                        (exponent_sign == -1 && exponent_num > 2147483648)) {
                        ec = ErrorCode::ErrorNegativeExponentPartOverflow;
                    }
                    // The exponent of the long double range is (1.2e-4932 ~ 1.2e+4932)
                    else if (exponent_num > 4932) {
                        ec = ErrorCode::ErrorExponentPartOutOfRange;
                    } 
                    exponent = (exponent_sign != -1) ? (int)exponent_num : -(int)exponent_num;
                    hasExponent = true;
                }
            }
        } while (0);

        // Is a double-precision floating-point number?
        if (hasDots) {
            char ch = stream_.get();
            if (ch == 'f' || ch == 'F') {
                isDouble = false;
                stream_.next();
            }
        }

        // Contain a dot or have a exponent is a floating point number.
        is_float = (hasDots | hasExponent);

        // 0xDE0B6B3A7640000ULL = 10^19 = 10E+19
        if (fractional_len > 20 || (fractional_len == 20 && fractional < 0xDE0B6B3A7640000ULL)) {
            // Fractional part is overflow
            ec = ErrorCode::ErrorFractionalPartOverflow;
        }
        // Same to fractional part
        if (integer_len > 20 || (integer_len == 20 && integer < 0xDE0B6B3A7640000ULL)) {
            // Integer part is overflow
            ec = ErrorCode::ErrorIntegerPartOverflow;
        }

        if (ec == ErrorCode::OK) {
            if (hasDots) {
                tokenType = (isDouble) ? Token::DoubleLiteral : Token::FloatLiteral;
            }
            else {
                tokenType = Token::IntegerLiteral;
            }
            is_ok = true;
        }
        else {
            is_ok = false;
        }
        return is_ok;
    }

    bool parseRealNumberSuffix(Token::Type & tokenType, ErrorCode & ec,
                               uint64_t & fractional, int & exponent) {
        bool is_ok;
        int fractional_len = 0;
        fractional = 0;
        exponent = 0;

        // Whether including a exponent?
        bool hasExponent = false;
        // Whether is a double-precision floating-point number?
        bool isDouble = true;

        // Search the fractional part of floating-point number.
        do {
            char ch = stream_.get();
            if (ch >= '0' && ch <= '9') {
                // Fractional part
                fractional = fractional * 10 + ch - '0';
                fractional_len++;
                stream_.next();
            }
            else if (ch == '.') {
                // Found the second decimal point, exit now.
                ec = ErrorCode::IllegalFloatingNumber;
                break;
            }
            else {
                break;
            }
        } while (1);

        // Determine the exponent of floating point numbers.
        do {
            int exponent_sign = 1;
            unsigned int exponent_num = 0;
            int exponent_cnt = 0;
            char ch = stream_.get();
            if (ch == 'e' || ch == 'E') {
                ch = stream_.get(1);
                if (ch == '-') {
                    exponent_sign = -1;
                    stream_.next(2);
                }
                else if (ch == '+') {
                    stream_.next(2);
                }

                do {
                    ch = stream_.get();
                    if (ch >= '0' && ch <= '9') {
                        exponent_num = exponent_num * 10 + ch - '0';
                        exponent_cnt++;
                        stream_.next();
                    }
                    else {
                        break;
                    }
                } while (1);

                if (exponent_cnt > 10 || (exponent_cnt == 10 && exponent_num < 1000000000)) {
                    // Exponent part is overflow
                    ec = ErrorCode::ErrorExponentPartOverflow;
                }
                else if (exponent_cnt <= 0) {
                    // Does not contain the exponential number, it's a illegal error.
                    ec = ErrorCode::IllegalExponentPart;
                }
                else {
                    // The exponent is smaller than -2147483648 or bigger than 2147483647 !
                    if ((exponent_sign != -1 && exponent_num > 2147483647) ||
                        (exponent_sign == -1 && exponent_num > 2147483648)) {
                        ec = ErrorCode::ErrorNegativeExponentPartOverflow;
                    }
                    // The exponent of the long double range is (1.2e-4932 ~ 1.2e+4932)
                    else if (exponent_num > 4932) {
                        ec = ErrorCode::ErrorExponentPartOutOfRange;
                    }
                    exponent = (exponent_sign != -1) ? (int)exponent_num : -(int)exponent_num;
                    hasExponent = true;
                    UNUSED_VARIANT(hasExponent);
                }
            }
        } while (0);

        // Is a double-precision floating-point number?
        {
            char ch = stream_.get();
            if (ch == 'f' || ch == 'F') {
                isDouble = false;
                stream_.next();
            }
        }

        // 0xDE0B6B3A7640000ULL = 10^19 = 10E+19
        if (fractional_len > 20 || (fractional_len == 20 && fractional < 0xDE0B6B3A7640000ULL)) {
            // Fractional part is overflow
            ec = ErrorCode::ErrorFractionalPartOverflow;
        }

        is_ok = (ec == ErrorCode::OK);
        if (is_ok) {
            tokenType = (isDouble) ? Token::DoubleLiteral : Token::FloatLiteral;
        }
        return is_ok;
    }

    inline char toOcxChar(unsigned int ocx) const {
        assert(ocx >= 0 && ocx < 8);
        return ('0' + ocx);
    } 

    inline char toLowerHexChar(unsigned int dec) const {
        assert(dec >= 0 && dec < 16);
        return (dec < 10) ? ('0' + dec) : ('a' + (dec - 10));
    }

    inline char toUpperHexChar(unsigned int dec) const {
        assert(dec >= 0 && dec < 16);
        return (dec < 10) ? ('0' + dec) : ('A' + (dec - 10));
    }

    bool getEscapedChars(unsigned char ch, std::string & chars) const {
        bool success = true;
        bool needEscape = CharInfo::NeedEscape(ch);
        if (!needEscape) {
            chars = ch;
        }
        else {
            bool isAdvancedEscape = CharInfo::IsAdvancedEscape(ch);
            if (!isAdvancedEscape) {
                chars = '\\';
                chars.push_back(CharInfo::GetEscapeChar(ch));
            }
            else {
                unsigned char escapeType = CharInfo::GetEscapeChar(ch);
                if (escapeType == CharInfo::kOctEscapeChar) {
                    chars = '\\';
                    chars.push_back(toOcxChar(ch / 8));
                    chars.push_back(toOcxChar(ch % 8));
                }
                else if (escapeType == CharInfo::kNonAscii) {
                    chars = "\\x";
                    chars.push_back(toUpperHexChar(ch / 16));
                    chars.push_back(toUpperHexChar(ch % 16));
                }
                else {
                    // Unknown escape type
                    success = false;
                }
            }
        }
        return success;
    }

    int getUnescapedChar(StringStream & stream, unsigned char & ch) {
        int skip;
        bool canUnescape = CharInfo::CanUnescape(ch);
        if (canUnescape) {
            bool isAdvencadUnescape = CharInfo::IsAdvancedUnescape(ch);
            if (!isAdvencadUnescape) {
                ch = CharInfo::GetUnescapeChar(ch);
                skip = 1;
            }
            else {
                unsigned char unescapeType = CharInfo::GetUnescapeChar(ch);
                if (unescapeType == CharInfo::kOctUnescapeChar) {
                    // Oct: '\123'
                    unsigned char ch1 = stream.getu(1);
                    if (ch1 >= '0' && ch1 <= '7') {
                        unsigned char ch2 = stream.getu(2);
                        if (ch2 >= '0' && ch2 <= '7') {
                            // [\ddd] : Three octal numbers
                            ch = (ch - '0') * 64 + (ch1 - '0') * 8 + (ch2 - '0');
                            skip = 3;
                        }
                        else {
                            // [\dd] : Two octal numbers
                            ch = (ch - '0') * 8 + (ch1 - '0');
                            skip = 2;
                        }
                    }
                    else {
                        // [\d] : One octal numbers
                        ch = (ch - '0');
                        skip = 1;
                    }
                }
                else if (unescapeType == CharInfo::kHexUnescapeChar) {
                    // Hex: '\xFF'
                    if ((stream.remain() >= 3)) {
                        // The first hexadecimal number
                        unsigned char ch1 = stream.getu(1);
                        if (CharInfo::IsHexChars(ch1)) {
                            ch1 = CharInfo::GetHexValue(ch1);

                            // The second hexadecimal number
                            unsigned char ch2 = stream.getu(2);
                            if (CharInfo::IsHexChars(ch2)) {
                                ch2 = CharInfo::GetHexValue(ch2);

                                ch = ch1 * 16 + ch2;
                                skip = 3;
                            }
                            else {
                                // Unescape HexChar error: The second char is not a hex chars.
                                skip = 0;
                            }
                        }
                        else {
                            // Unescape HexChar error: The first char is not a hex chars.
                            skip = 0;
                        }
                    }
                    else {
                        // Unescape HexChar error: remain length less than 3.
                        skip = 0;
                    }
                }
                else {
                    // Unknown unescape type
                    skip = 0;
                }
            }
        }
        else {
            // Unescape error
            skip = 0;
        }
        return skip;
    }

    // Single character literal
    bool parseSingleCharLiteral(Token & token, ErrorCode & ec) {
        ec = ErrorCode::OK;
        unsigned char character;
        char ch = stream_.get();
        if (likely(ch != '\\')) {
            // It's not an non-escaped char
            character = ch;
            stream_.next();
        }
        else {
            // It's an non-escaped char
            stream_.next();
            character = stream_.get();
            int skip = getUnescapedChar(stream_, character);
            if (likely(skip > 0)) {
                std::string escapedChars;
                getEscapedChars(character, escapedChars);
                std::cout << ">>> Single character: [UnescapedChar] - [" << escapedChars << "]" << std::endl;
                stream_.next(skip);
            }
            else {
                ec = ErrorCode::UnknownUnescapedChar;
            }
        }
        if (likely(stream_.get() == '\'')) {
            std::string escapedChars;
            getEscapedChars(character, escapedChars);
            std::cout << ">>> Single character: [" << escapedChars << "] (" << (int)character << ")" << std::endl;
            stream_.next();
        }
        else {
            // It's a illegal single character format.
            ec = ErrorCode::IllegalSingleCharacterFormat;
        }
        return (ec == ErrorCode::OK);
    }

    // Normal string literal
    bool parseStringLiteral(Token & token, ErrorCode & ec) {
        ec = ErrorCode::OK;
        std::string content;
        int multipart_cnt = 0;
        do {
            char ch;
            while ((ch = stream_.get()) != '\0') {
                if (likely(ch != '\"')) {
                    unsigned char character;
                    if (likely(ch != '\\')) {
                        // It's not an non-escaped char
                        character = ch;
                        content.append(1, character);
                        //content.push_back(character);
                        stream_.next();
                    }
                    else {
                        // It's an non-escaped char
                        stream_.next();
                        character = stream_.get();
                        int skip = getUnescapedChar(stream_, character);
                        if (likely(skip > 0)) {
                            std::string escapedChars;
                            getEscapedChars(character, escapedChars);
                            std::cout << ">>> Single character: [UnescapedChar] - [" << escapedChars << "]" << std::endl;
                            content.append(1, character);
                            //content.push_back(character);
                            stream_.next(skip);
                        }
                        else {
                            ec = ErrorCode::UnknownUnescapedChar;
                        }
                    }
                }
                else {
                    if (unlikely(stream_.get(1) == '\"')) {
                        // It's [""]
                        content.append(1, '\"');
                        //content.push_back('\"');
                        stream_.next(2);
                    }
                    else {
                        // It's the end of the normal string literal.
                        stream_.next();
                        multipart_cnt++;
                        break;
                    }
                }
            }
            skipWhiteSpaces();
            if (likely(stream_.get() != '\"')) {
                // It's the end of normal string literal or multi-part string literal.
                break;
            }
            else {
                stream_.next();
            }
        } while (1);

        if (multipart_cnt == 1) {
            std::cout << ">>> Normal string literal = [\n" << content << "\n];" << std::endl;
        }
        else if (multipart_cnt > 1) {
            std::cout << ">>> Multi-part (" << multipart_cnt << " parts) string literal = "
                      << "[\n" << content << "\n];" << std::endl;
        }
        else {
            std::cout << ">>> Error: String literal mismatch \\\" !" << std::endl;
        }
        return (ec == ErrorCode::OK);
    }

    bool parseLiteral(Token & token, ErrorCode & ec) {
        bool parse_ok;
        Token::Type tokenType;
        StringStreamMarker marker(stream_);
        marker.set_mark();
        // Starting with numbers: "[0-9]", or ".[0-9]"
        uint8_t ch = stream_.getu();
        if (isNumber(ch)) {
            ec = ErrorCode::OK;
            // It's a radix based number? like "0x", "0o", "0b", "0d" ...
            char ch = stream_.get();
            if (ch == '0') {
                char ch1 = stream_.get(1);
                if ((ch1 >= 'B' && ch1 <= 'x') && stream_.remain() > 2) {
                    // Determine the radix for the constant
                    int radix;
                    uint64_t number;
                    parse_ok = parseRadixNumber(tokenType, ec, radix, number);
                    if (parse_ok) {
                        token.setToken(tokenType, marker.start_pos(), marker.length());
                        return true;
                    }
                }
            }

            // It's a real number? Including integer, float or double number.
            uint64_t integer;
            uint64_t fractional;
            int exponent;
            bool is_float;
            parse_ok = parseRealNumber(tokenType, ec, integer, fractional, exponent, is_float);
            if (parse_ok) {
                token.setToken(tokenType, marker.start_pos(), marker.length());
                return true;
            }
        }

        if (stream_.get() == '\"') {
            // String literal or single char literal
            stream_.next();
            return parseStringLiteral(token, ec);
        }
        else if (stream_.get() == '\'') {
            // Single character literal
            stream_.next();
            return parseSingleCharLiteral(token, ec);
        }
        return false;
    }

    bool Parser::nextToken(Token & token, ErrorCode & ec) {
        ec = ErrorCode::OK;
        token.setType(Token::Unknown);
        StringStreamMarker marker(stream_);
        while (likely(!stream_.is_eof())) {
            marker.remark();
            Token::Type tokenType;
            bool success;
            char ch;
            // For ppc or arm cpu, make sure to use "signed char or int8_t".
            int8_t cur = stream_.get();
            switch (cur) {
            case '\0':
                // Set 'EndOfFile' token's position first.
                token.setToken(Token::EndOfFile, stream_.tell(), 0);
                stream_.next();
                return false;

            case '\t':  // Whitespace chars and next line
            case '\v':
            case '\f':
            case ' ':
                stream_.next();
                skipWhiteSpace();
                token.setType(Token::WhiteSpace);
                break;

            case '\n':
            case '\r':
                stream_.next();
                skipNewLine();
                token.setType(Token::NewLine);
                break;

            case '#':   // Preprocessing statement, example: #include <stdio.h>
                stream_.next();
                success = parsePreprocessing(token, ec);
                if (unlikely(!success)) {
                    marker.restore();
                    stream_.next();
                }
                break;

            case '/':   // Line comment or block comment
                {
                    stream_.next();
                    bool is_comments = parseComment(token, ec);
                    if (unlikely(!is_comments)) {
                        if (stream_.get(0) != '=') {
                            // Div token, operator: /
                            token.setType(Token::Div);
                        }
                        else {
                            // DivEqual token, operator: /=
                            stream_.next();
                            token.setType(Token::DivEqual);
                        }
                    }
                    else {
                        if (!ec.isOK()) {
                            //this->getEngine().diagnosisComment(ec, stream_, token);
                            return false;
                        }
                    }
                }
                break;

            case 'A': case 'B': case 'C': case 'D': case 'E':
            case 'F': case 'G': case 'H': case 'I': case 'J':
            case 'K': case 'L': case 'M': case 'N': case 'O':
            case 'P': case 'Q': case 'R': case 'S': case 'T':
            case 'U': case 'V': case 'W': case 'X': case 'Y':
            case 'Z':
            case 'a': case 'b': case 'c': case 'd': case 'e':
            case 'f': case 'g': case 'h': case 'i': case 'j':
            case 'k': case 'l': case 'm': case 'n': case 'o':
            case 'p': case 'q': case 'r': case 's': case 't':
            case 'u': case 'v': case 'w': case 'x': case 'y':
            case 'z':
            case '_':
                // Identifier or keywords
                if (parseIdentifier(ec)) {
                    //stream_.next();
                    token.setType(Token::Identifier);
                }
                else {
                    token.setType(Token::Unrecognized);
                    if (marker.length() <= 0)
                        stream_.next();
                }
                break;

            case '0':
                {
                    char ch1 = stream_.get(1);
                    if ((ch1 >= 'B' && ch1 <= 'x') && stream_.remain() > 2) {
                        // Determine the radix for the constant
                        int radix;
                        uint64_t number;
                        success = parseRadixNumber(tokenType, ec, radix, number);
                        if (success) {
                            token.setToken(tokenType, marker.start_pos(), marker.length());
                            return true;
                        }
                    }

                    // It's a real number? Including integer, float or double number.
                    uint64_t integer;
                    uint64_t fractional;
                    int exponent;
                    bool is_float;
                    success = parseRealNumber(tokenType, ec, integer, fractional, exponent, is_float);
                    if (success) {
                        token.setToken(tokenType, marker.start_pos(), marker.length());
                        return true;
                    }

                    stream_.next();
                    token.setType(Token::IntegerLiteral);
                }
                break;

            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                {
                    // It's a real number? Including integer, float or double number.
                    uint64_t integer;
                    uint64_t fractional;
                    int exponent;
                    bool is_float;
                    success = parseRealNumber(tokenType, ec, integer, fractional, exponent, is_float);
                    if (success) {
                        token.setToken(tokenType, marker.start_pos(), marker.length());
                        return true;
                    }
                    token.setType(Token::IntegerLiteral);
                }
                break;

            case '.':
                stream_.next();
                {
                    ch = stream_.get();
                    if (Helper::isDigital(ch)) {
                        // It's a float or double number.
                        uint64_t fractional;
                        int exponent;
                        success = parseRealNumberSuffix(tokenType, ec, fractional, exponent);
                        if (success) {
                            token.setToken(tokenType, marker.start_pos(), marker.length());
                            return true;
                        }
                    }
                    else if (ch == '.') {
                        char ch1 = stream_.get(1);
                        if (ch1 == '.') {
                            char ch2 = stream_.get(2);
                            if (ch2 != '.') {
                                stream_.next(2);
                                token.setType(Token::Ellipsis);
                            }
                            else {
                                // Error: [....]
                                ec = ErrorCode::IllegalFourDot;
                            }
                        }
                        else {
                            // Error: [..]
                            ec = ErrorCode::IllegalDoubleDot;
                        }
                    }
                    else {
                        token.setType(Token::Dot);
                    }
                }
                break;

            case '!':
                stream_.next();
                if (stream_.get() != '=') {
                    // BoolNot token, operator: !
                    token.setType(Token::BoolNot);
                }
                else {
                    // operator: !=
                    stream_.next();
                    // NotEqual token
                    token.setType(Token::NotEqual);
                }
                break;

            case '%':
                stream_.next();
                if (stream_.get() != '=') {
                    // Mod token, operator: %
                    token.setType(Token::Mod);
                }
                else {
                    // operator: %=
                    stream_.next();
                    // ModEqual token
                    token.setType(Token::ModEqual);
                }
                break;

            case '&':
                stream_.next();
                if (stream_.get() != '=') {
                    if (stream_.get() != '&') {
                        // BitAnd token, operator: &
                        token.setType(Token::BitAnd);
                    }
                    else {
                        // BoolAnd token, operator: &&
                        token.setType(Token::BoolAnd);
                    }
                }
                else {
                    // operator: &=
                    stream_.next();
                    // AndEqual token
                    token.setType(Token::AndEqual);
                }
                break;

            case '*':
                stream_.next();
                if (stream_.get() != '=') {
                    // Multiply token, operator: *
                    token.setType(Token::Multiply);
                }
                else {
                    // operator: *=
                    stream_.next();
                    // MultiplyEqual token
                    token.setType(Token::MultiplyEqual);
                }
                break;

            case '+':
                stream_.next();
                if (stream_.get() != '=') {
                    if (stream_.get() != '+') {
                        // Plus token, operator: +
                        token.setType(Token::Add);
                    }
                    else {
                        // Increase token, operator: ++
                        token.setType(Token::Increase);
                    }
                }
                else {
                    // operator: +=
                    stream_.next();
                    // PlusEqual token
                    token.setType(Token::AddEqual);
                }
                break;

            case '-':
                stream_.next();
                if (stream_.get() != '=') {
                    if (stream_.get() == '>') {
                        // Pointer token, operator: ->
                        token.setType(Token::Pointer);
                    }
                    else if (stream_.get() != '-') {
                        // Minus token, operator: -
                        token.setType(Token::Sub);
                    }
                    else {
                        // Decrease token, operator: --
                        token.setType(Token::Decrease);
                    }
                }
                else {
                    // operator: -=
                    stream_.next();
                    // MinusEqual token
                    token.setType(Token::SubEqual);
                }
                break;

            case ':':
                stream_.next();
                if (stream_.get() == ':') {
                    // operator: ::
                    stream_.next();
                    // Scope token
                    token.setType(Token::Scope);
                }
                else {
                    // Colon token, operator: :
                    token.setType(Token::Colon);
                }
                break;

            case '=':
                stream_.next();
                if (stream_.get() != '=') {
                    // Assignment token, operator: =
                    token.setType(Token::Assignment);
                }
                else {
                    // operator: ==
                    stream_.next();
                    // Equal token
                    token.setType(Token::Equal);
                }
                break;

            case '?':
                stream_.next();
                // Question token, operator: ?
                token.setType(Token::Question);
                break;

            case '^':
                stream_.next();
                if (stream_.get() != '^') {
                    if (stream_.get() != '=') {
                        // BitXor token, operator: ^
                        token.setType(Token::BitXor);
                    }
                    else {
                        // XorEqual token, operator: ^=
                        token.setType(Token::XorEqual);
                    }
                }
                else {
                    // operator: ^^
                    stream_.next();
                    // BoolXor token
                    token.setType(Token::BoolXor);
                }
                break;

            case '|':
                stream_.next();
                if (stream_.get() != '|') {
                    if (stream_.get() != '=') {
                        // BitOr token, operator: |
                        token.setType(Token::BitOr);
                    }
                    else {
                        // OrEqual token, operator: |=
                        token.setType(Token::OrEqual);
                    }
                }
                else {
                    // operator: ||
                    stream_.next();
                    // BoolOr token
                    token.setType(Token::BoolOr);
                }
                break;

            case '~':
                stream_.next();
                if (stream_.get() != '~') {
                    if (stream_.get() != '=') {
                        // BitNot token, operator: ~
                        token.setType(Token::BitNot);
                    }
                    else {
                        // NotEqual token, operator: ~=
                        token.setType(Token::BitNotEqual);
                    }
                }
                else {
                    // operator: ~~
                    stream_.next();
                    // BoolNot token
                    token.setType(Token::BoolNot);
                }
                break;

            case '@':
                // Is a annotation?
                stream_.next();
                token.setType(Token::Annotation);
                break;

            case ',':
                stream_.next();
                token.setType(Token::Comma);
                break;

            case ';':
                stream_.next();
                token.setType(Token::Semi);
                break;

            case '(':
                stream_.next();
                token.setType(Token::LParen);
                break;

            case ')':
                stream_.next();
                token.setType(Token::RParen);
                break;

            case '[':
                stream_.next();
                token.setType(Token::LBracket);
                break;

            case ']':
                stream_.next();
                token.setType(Token::RBracket);
                break;

            case '{':
                stream_.next();
                token.setType(Token::LBrace);
                break;

            case '}':
                stream_.next();
                token.setType(Token::RBrace);
                break;

            case '\'':  // Single character literal
                stream_.next();
                success = parseSingleCharLiteral(token, ec);
                if (unlikely(!success)) {
                    marker.restore();
                    stream_.next();
                }
                break;

            case '\"':  // String literal or single char literal
                stream_.next();
                success = parseStringLiteral(token, ec);
                if (unlikely(!success)) {
                    marker.restore();
                    stream_.next();
                }
                break;

            default:    // Internal keywords
                if (parseReservedKeyword(ec)) {
                    //stream_.next();
                    token.setType(Token::ReservedKeyword);
                }
                else {
                    token.setType(Token::Unrecognized);
                    if (marker.length() <= 0)
                        stream_.next();
                }
                break;
            }

            token.setStartPos(marker.start_pos());
            token.setLength(marker.length());
            assert(token.getLength() > 0);
            if (token.getType() != Token::Unknown &&
                token.getType() != Token::Unrecognized) {
                break;
            }
        }

        if (likely(stream_.is_eof())) {
            token.setToken(Token::EndOfFile, stream_.tell(), 0);
            return false;
        }
        return true;
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_PARSER_H
