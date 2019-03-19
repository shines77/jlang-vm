
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
#include "jlang/asm/IdentInfo.h"
#include "jlang/jstd/min_max.h"
#include "jlang/jstd/SmallString.h"
#include "jlang/support/HashAlgorithm.h"

namespace jlang {
namespace jasm {

class Parser {
public:
    typedef Parser       this_type;
    typedef ParserHelper Helper;

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

    void setStream(StringStream & stream) {
        stream_.copy_from(stream);
        stream_.seek(SeekType::End, 0);
        stream_.put_null();
        stream_.reset();
    }

    void setStream(FileStringStream & fileStream) {
        StringStream & stream = fileStream.getStream();
        setStream(stream);
    }

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
    void parseIdentifier(IdentInfo & identInfo, Token & token) {
        StreamMarker marker(stream_);
        marker.setmark();
        skipIdentifier();
        assert(marker.length() > 0);

        marker.append_ident(identInfo);
        token.setToken(Token::Identifier, identInfo.start(), identInfo.length());
    }

    void parseIdentifierBody(char firstChar, IdentInfo & identInfo, Token & token) {
        assert(firstChar == stream_.get(-1));
        StreamMarker marker(stream_);
        marker.setmark(-1);
        skipIdentifier();
        assert(marker.length() > 1);

        marker.append_ident(identInfo);
        token.setToken(Token::Identifier, identInfo.start(), identInfo.length());
    }

    ErrorCode parseIdentifierStrict(IdentInfo & identInfo, Token & token) {
        ErrorCode ec;
        StreamMarker marker(stream_);
        marker.setmark();
        skipIdentifier();
        assert(marker.length() > 0);

        marker.append_ident(identInfo);
        if (identInfo.length() > 0) {
            token.setToken(Token::Identifier, identInfo.start(), identInfo.length());
        }
        else {
            token.setToken(Token::Unrecognized, identInfo.start(), identInfo.length());
            ec = ErrorCode::IllegalIdentifer;
        }
        return ec;
    }

    ErrorCode parseReservedKeyword(Token & token) {
        ErrorCode ec;
        StreamMarker marker(stream_);
        marker.setmark();
        skipIdentifier();
        assert(marker.length() > 0);

        IdentInfo keywordInfo;
        marker.make_ident(keywordInfo);
        if (keywordInfo.length() > 0) {
            std::string & keywordName = keywordInfo.name();

#if (KEYWORD_HASHCODE_WORDLEN == 32)
            uint32_t hash32 = getHash32(keywordInfo.name().c_str(), (size_t)keywordInfo.length());
#elif (KEYWORD_HASHCODE_WORDLEN == 64)
            uint64_t hash64 = getHash64(keywordInfo.name().c_str(), (size_t)keywordInfo.length());
#endif
            KeywordMapping & keyMapping = Global::getKeywordMapping();
            assert(keyMapping.inited());
            KeywordMapping::iterator iter = keyMapping.find(keywordName);
            if (iter != keyMapping.end()) {
                Keyword keyword = iter->second;
                if (keyword.getCategory() == KeywordCategory::Pod ||
                    keyword.getCategory() == KeywordCategory::TypeDef) {
                    if (keyword.getCategory() != KeywordCategory::Pod) {
                        // Parse full type define
                    }
                    skipWhiteSpace();

                    StreamMarker identMarker(stream_);
                    identMarker.setmark();
                    skipIdentifier();
                    assert(identMarker.length() > 0);

                    IdentInfo identInfo;
                    identMarker.make_ident(identInfo);

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
                            identMarker.remark();
                            skipIdentifier();
                            
                            identMarker.make_ident(identInfo);
                            if (identInfo.length() <= 0) {
                                // Type-list define failure
                                std::cout << "*** Error: Type-list define failure. *** ";
                                return false;
                            }
                            identMarker.make_ident(identInfo);

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
                else if (keyword.getCategory() == KeywordCategory::UserDefine) {
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
        }
        else {
            ec = ErrorCode::IllegalIdentifer;
        }

        return ec;
    }

    ErrorCode parseIdentifierOrKeyword(Token & token) {
        ErrorCode ec;
        StreamMarker marker(stream_);
        marker.setmark();

        skipIdentifier();
        assert(marker.length() > 0);

        IdentInfo identInfo;
        marker.make_ident(identInfo);
        if (identInfo.length() > 0) {
            std::cout << ">>> Identifier name = [" << identInfo.name().c_str() << "]" << std::endl;
        }

        return ec;
    }

    bool isPreprocessing() {
        if (stream_.get() == PREPROCESSING_CHAR)
            return true;
        else
            return false;
    }

    ErrorCode parsePreprocessing(Token & token) {
        ErrorCode ec;
        Token::Type tokenType = Token::Unknown;
        StreamMarker marker(stream_);
        marker.setmark();
        skipIdentifier();

        IdentInfo identInfo;
        marker.make_ident(identInfo);

        if (identInfo.length() > 0) {
            KeywordMapping & ppKeyMapping = Global::getPPKeywordMapping();
            KeywordMapping::iterator iter = ppKeyMapping.find(identInfo.name());
            if (iter != ppKeyMapping.end()) {
                Keyword & keyword = iter->second;
                assert(keyword.getCategory() == KeywordCategory::Preprocessing);

                tokenType = keyword.getType();
                token.setStartPos(identInfo.start());
                token.setLength(identInfo.length());

                ec = handlePreprocessingStatement(tokenType, token);
                if (ec.isOK()) {
                    token.setType(tokenType);
                }
                else {
                    // Got a error.
                }
            }
            else {
                // Error: It's a unknown preprocessing keyword.
                ec = ErrorCode::UnknownPreprocessingKeyword;
            }
        }
        else {
            // Error: It's not a preprocessing keyword.
            ec = ErrorCode::IllegalPreprocessingKeyword;
        }

        if (tokenType != Token::Unknown) {
            token.setToken(tokenType, marker.start(), marker.length());
        }
        else {
            token.setToken(tokenType, marker.start(), 0);
        }
        return ec;
    }

    ErrorCode handlePreprocessingStatement(Token::Type ppTokenType, const Token & token) {
        ErrorCode ec = ErrorCode::OK;
        switch (ppTokenType) {
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
    bool parseRadixNumberImpl(uint64_t & value) {
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

    ErrorCode parseRadixNumber(Token::Type & tokenType,
                               int & radix, uint64_t & number) {
        ErrorCode ec;
        bool is_valid;
        char ch1 = stream_.get(1);
        if (likely(ch1 == 'x' || ch1 == 'X')) {
            radix = 16;
            stream_.next(2);
            is_valid = parseRadixNumberImpl<16>(number);
            tokenType = Token::HexLiteral;
            if (!is_valid) {
                ec = ErrorCode::IllegalRadix16Number;
            }
        }
        else if (likely(ch1 == 'o' || ch1 == 'O')) {
            radix = 8;
            stream_.next(2);
            is_valid = parseRadixNumberImpl<8>(number);
            tokenType = Token::OcxLiteral;
            if (!is_valid) {
                ec = ErrorCode::IllegalRadix8Number;
            }
        }
        else if (likely(ch1 == 'b' || ch1 == 'B')) {
            radix = 2;
            stream_.next(2);
            is_valid = parseRadixNumberImpl<2>(number);
            tokenType = Token::BinaryLiteral;
            if (!is_valid) {
                ec = ErrorCode::IllegalRadix2Number;
            }
        }
        else if (likely(ch1 == 'd' || ch1 == 'D')) {
            radix = 10;
            stream_.next(2);
            is_valid = parseRadixNumberImpl<10>(number);
            tokenType = Token::DecLiteral;
            if (!is_valid) {
                ec = ErrorCode::IllegalRadix10Number;
            }
        }
        else {
            // It's not a based radix number, re-parse from old place.
            ec = ErrorCode::IllegalRadixNumber;
        }
        return ec;
    }

    ErrorCode parseDecimalNumber(uint64_t & number) {
        bool is_valid = parseRadixNumberImpl<10>(number);
        if (is_valid)
            return ErrorCode::OK;
        else
            return ErrorCode::IllegalRadix10Number;
    }

    ErrorCode parseRealNumber(Token::Type & tokenType,
                              uint64_t & integer, uint64_t & fractional,
                              int & exponent, bool & is_float) {
        ErrorCode ec;
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
                    goto parseExit;
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
                    goto parseExit;
                }
                else if (exponent_cnt <= 0) {
                    // Does not contain the exponential number, it's a illegal error.
                    ec = ErrorCode::IllegalExponentPart;
                    goto parseExit;
                }
                else {
                    // The exponent is smaller than -2147483648 or bigger than 2147483647 !
                    if ((exponent_sign != -1 && exponent_num > 2147483647) ||
                        (exponent_sign == -1 && exponent_num > 2147483648)) {
                        ec = ErrorCode::ErrorNegativeExponentPartOverflow;
                        goto parseExit;
                    }
                    // The exponent of the long double range is (1.2e-4932 ~ 1.2e+4932)
                    else if (exponent_num > 4932) {
                        ec = ErrorCode::ErrorExponentPartOutOfRange;
                        goto parseExit;
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
            goto parseExit;
        }
        // Same to fractional part
        if (integer_len > 20 || (integer_len == 20 && integer < 0xDE0B6B3A7640000ULL)) {
            // Integer part is overflow
            ec = ErrorCode::ErrorIntegerPartOverflow;
            goto parseExit;
        }

        if (ec == ErrorCode::OK) {
            if (hasDots) {
                tokenType = (isDouble) ? Token::DoubleLiteral : Token::FloatLiteral;
            }
            else {
                tokenType = Token::IntegerLiteral;
            }
        }

parseExit:
        return ec;
    }

    ErrorCode parseRealNumberSuffix(Token::Type & tokenType,
                                    uint64_t & fractional, int & exponent) {
        ErrorCode ec;
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
                goto parseExit;
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
                    goto parseExit;
                }
                else if (exponent_cnt <= 0) {
                    // Does not contain the exponential number, it's a illegal error.
                    ec = ErrorCode::IllegalExponentPart;
                    goto parseExit;
                }
                else {
                    // The exponent is smaller than -2147483648 or bigger than 2147483647 !
                    if ((exponent_sign != -1 && exponent_num > 2147483647) ||
                        (exponent_sign == -1 && exponent_num > 2147483648)) {
                        ec = ErrorCode::ErrorNegativeExponentPartOverflow;
                        goto parseExit;
                    }
                    // The exponent of the long double range is (1.2e-4932 ~ 1.2e+4932)
                    else if (exponent_num > 4932) {
                        ec = ErrorCode::ErrorExponentPartOutOfRange;
                        goto parseExit;
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
            goto parseExit;
        }

        if (ec == ErrorCode::OK) {
            tokenType = (isDouble) ? Token::DoubleLiteral : Token::FloatLiteral;
        }

parseExit:
        return ec;
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
        int skip = 0;
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
                                skip = -1;
                            }
                        }
                        else {
                            // Unescape HexChar error: The first char is not a hex chars.
                            skip = -2;
                        }
                    }
                    else {
                        // Unescape HexChar error: remain length less than 3.
                        skip = -3;
                    }
                }
                else {
                    // Unknown unescape type
                    skip = -4;
                }
            }
        }
        else {
            // Unescape error
            skip = -5;
        }
        return skip;
    }

    // Single character literal
    ErrorCode parseSingleCharLiteral(std::string & content, Token & token) {
        ErrorCode ec;
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
                // Get a unknown unescaped char
                ec = ErrorCode::UnknownUnescapedChar;
                goto parseExit;
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
            goto parseExit;
        }

        if (ec == ErrorCode::OK) {
            content = character;
        }

parseExit:
        return ec;
    }

    // Normal string literal
    ErrorCode parseStringLiteral(std::string & content, Token & token) {
        ErrorCode ec;
        int multipart_cnt = 0;
        bool completed;

        do {
            completed = false;
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
                            break;
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
                        completed = true;
                        break;
                    }
                }
            }

            // If it reach the end of file and it's not completed, exit now.
            if (!completed) {
                if (ec.isOK()) {
                    std::cout << ">>> Error: String literal is not completed!" << std::endl;
                    ec = ErrorCode::IllegalStringLiteralIsNotCompleted;
                }
                else {
                    std::cout << ">>> Error: String literal unknown error!" << std::endl;
                }
                return ec;
            }

            // Skip the white spaces between the multi-part string literal.
            skipWhiteSpaces();

            if (likely(stream_.get() != '\"')) {
                // It's the end of normal string literal or multi-part string literal.
                break;
            }
            else {
                // Find a multi-part string literal entry, skip the '"' char and continue parse.
                stream_.next();
            }
        } while (1);

        if (multipart_cnt == 1) {
            std::cout << ">>> String literal = [\n" << content << "\n];" << std::endl;
        }
        else if (multipart_cnt > 1) {
            std::cout << ">>> Multi-part (" << multipart_cnt << " parts) string literal = "
                      << "[\n" << content << "\n];" << std::endl;
        }
        else {
            std::cout << ">>> Error: String literal is not completed. (mismatch \\\") !" << std::endl;
            ec = ErrorCode::IllegalStringLiteralIsNotCompleted;
        }
        return ec;
    }

    ErrorCode parseNumberLiteral(Token & token) {
        ErrorCode ec;
        Token::Type tokenType;
        StreamMarker marker(stream_);
        marker.setmark();

        // It's a radix based number? like "0x", "0o", "0b", "0d" ...
        char ch = stream_.get();
        if (ch == '0') {
            char ch1 = stream_.get(1);
            if ((ch1 >= 'B' && ch1 <= 'x') && stream_.remain() > 2) {
                // Determine the radix for the constant
                int radix;
                uint64_t number;
                ec = parseRadixNumber(tokenType, radix, number);
                if (ec.isOK()) {
                    token.setToken(tokenType, marker.start(), marker.length());
                }
                return ec;
            }
        }

        // It's a real number? Including integer, float or double number.
        uint64_t integer;
        uint64_t fractional;
        int exponent;
        bool is_float;
        ec = parseRealNumber(tokenType, integer, fractional, exponent, is_float);
        if (ec.isOK()) {
            token.setToken(tokenType, marker.start(), marker.length());
        }

        return ec;
    }

    ErrorCode parseLiteral(std::string & content, Token & token) {
        ErrorCode ec;
        
        uint8_t ch = stream_.getu();
        if (likely(isNumber(ch))) {
            // A integer or a real number.
            // Starting with numbers: "[0-9]", or ".[0-9]"
            return parseNumberLiteral(token);
        }
        else if (likely(stream_.get() == '\"')) {
            // String literal or single char literal
            stream_.next();
            return parseStringLiteral(content, token);
        }
        else if (likely(stream_.get() == '\'')) {
            // Single character literal
            stream_.next();
            return parseSingleCharLiteral(content, token);
        }
        else if (likely(stream_.get() == '-')) {
            // Negative sign
            stream_.next();
            return parseStringLiteral(content, token);
        }
        else if (likely(stream_.get() == '+')) {
            // Positive sign
            stream_.next();
            return parseStringLiteral(content, token);
        }

        return ec;
    }

    uint64_t roundAlignedBytes(uint64_t alignedBytes) {
        static const uint64_t kMinAlignedBytes = 16;
        static const uint64_t kAlignedMask = kMinAlignedBytes - 1;
        if (likely(alignedBytes >= kMinAlignedBytes)) {
            if (likely((alignedBytes % kMinAlignedBytes) == 0)) {
                return alignedBytes;
            }
            else {
                alignedBytes = (alignedBytes + kAlignedMask) & (~kAlignedMask);
                return alignedBytes;
            }
        }
        else {
            return kMinAlignedBytes;
        }
    }

    ErrorCode handleSectionStatement(Token::Type sectionType, Token & token) {
        ErrorCode ec;

        switch (sectionType) {
        case Token::Align:
            {
                std::cout << ">>> Section [.align] begin." << std::endl;

                // Skip the leading whitespace character first.
                skipWhiteSpace();

                uint8_t ch = stream_.getu();
                if (likely(isNumber(ch))) {
ParseAlignBytes_Start:
                    uint64_t alignedBytes = 0;
                    ec = parseDecimalNumber(alignedBytes);
                    if (ec.isOK()) {
                        uint64_t newAlignedBytes = roundAlignedBytes(alignedBytes);
                        if (newAlignedBytes != alignedBytes) {
                            // Have changed to newAlignedBytes from alignedBytes.
                            std::cout << ">>> Section [.align]: alignedBytes have changed to " << newAlignedBytes;
                            std::cout << " from " << alignedBytes << " bytes" << std::endl;
                        }
                        std::cout << ">>> Section [.align]: alignedBytes = " << newAlignedBytes << " bytes" << std::endl;
                    }
                    skipWhiteSpaces();
                }
                else if (likely(isAlphabet(ch))) {
                    IdentInfo identInfo;
                    Token identToken;
                    parseIdentifier(identInfo, identToken);
                    if (identInfo.name() == "default") {   // Setting default align bytes.
                        skipWhiteSpace();
                        uint8_t ch = stream_.getu();
                        if (likely(isNumber(ch))) {
                            goto ParseAlignBytes_Start;
                        }
                        else {
                            // Got Errors, expect to a decimal integer.
                            ec = ErrorCode::UnknownError;
                        }
                    }
                }

                std::cout << ">>> Section [.align] end." << std::endl << std::endl;
            }
            break;

        case Token::Strings:
            {
                // Skip the leading whitespace character first
                skipWhiteSpace();

                uint8_t ch = stream_.getu();
                if (likely(ch == '{')) {
                    stream_.next();

                    std::cout << ">>> Section [.strings] begin." << std::endl;
                    skipWhiteSpaces();

ParseStringSection_Entry:
                    IdentInfo identInfo;
                    ec = parseIdentifierStrict(identInfo, token);
                    if (ec.isOK()) {
                        skipWhiteSpace();

                        ch = stream_.getu();
                        if (likely(ch == '\"')) {
                            stream_.next();

                            std::string stringValue;
                            ec = parseStringLiteral(stringValue, token);
                            if (ec.isOK()) {
                                skipWhiteSpaces();

                                // Parse next string or end of sign '}'.
                                ch = stream_.getu();
                                if (likely(isIdentifierFirst(ch))) {
                                    // Next string identifier
                                    goto ParseStringSection_Entry;
                                }
                                else if (likely(ch == '}')) {
                                    // End of string section
                                    std::cout << ">>> Section [.strings] end." << std::endl << std::endl;
                                }
                                else {
                                    // Got a error, expect to '}', string section ending.
                                    ec = ErrorCode::ExpectingStringSectionEnding;
                                }
                            }
                            else {
                                // Got a error, illegal string literal.
                                ec = ErrorCode::IllegalStringLiteral;
                            }
                        }
                        else {
                            // Got a error, expect to '"'.
                            ec = ErrorCode::ExpectingStringLiteral;
                        }
                    }
                }
                else {
                    // Got a error, expect to '{', string section beginning.
                    ec = ErrorCode::ExpectingStringSectionBeginning;
                }
            }
            break;

        case Token::EntryPoint:
            {
                // Skip the trailing whitespace and newline character
                skipWhiteSpaces();
            }
            break;

        default:
            {
                // Unsupported section keyword
                token.setType(Token::Unsupported);
                ec = ErrorCode::UnknownSectionStatement;
            }
            break;
        }

        return ec;
    }

    bool nextToken(Token & token, ErrorCode & ec_) {
        ErrorCode ec = ErrorCode::OK;
        StreamMarker marker(stream_);
        while (likely(stream_.has_next())) {
            marker.remark();
            Token::Type tokenType;
            char ch;
            // For ppc or arm cpu, make sure to use "signed char or int8_t".
            int8_t cur = stream_.get();
            switch (cur) {
            case '\0':
                // Set 'Eof' token's position first.
                token.setToken(Token::Eof, stream_.tell(), 0);
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
                ec = parsePreprocessing(token);
                if (unlikely(!ec.isOK())) {
                    marker.rewind();
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
                {
                    // Identifier or keyword
                    ec = parseIdentifierOrKeyword(token);
                    if (unlikely(!ec.isOK())) {
                        marker.rewind();
                        stream_.next();
                    }
                    
                    IdentInfo identInfo;
                    marker.make_ident(identInfo);

                    jstd::SmallString<256> identName;
                    identName.copy(identInfo.name());
                }
                break;

            case '0':
                {
                    char ch1 = stream_.get(1);
                    if ((ch1 >= 'B' && ch1 <= 'x') && stream_.remain() > 2) {
                        // Determine the radix for the constant
                        int radix;
                        uint64_t number;
                        ec = parseRadixNumber(tokenType, radix, number);
                        if (ec.isOK()) {
                            token.setToken(tokenType, marker.start(), marker.length());
                            return true;
                        }
                    }

                    // It's a real number? Including integer, float or double number.
                    uint64_t integer;
                    uint64_t fractional;
                    int exponent;
                    bool is_float;
                    ec = parseRealNumber(tokenType, integer, fractional, exponent, is_float);
                    if (ec.isOK()) {
                        token.setToken(tokenType, marker.start(), marker.length());
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
                    ec = parseRealNumber(tokenType, integer, fractional, exponent, is_float);
                    if (ec.isOK()) {
                        token.setToken(tokenType, marker.start(), marker.length());
                        return true;
                    }
                    token.setType(Token::IntegerLiteral);
                }
                break;

            case '.':
                stream_.next();
                {
                    ch = stream_.get();
                    if (isAlphabet(ch)) {
                        // It's a section declare
                        IdentInfo sectionInfo;
                        sectionInfo.setName(".");
                        parseIdentifier(sectionInfo, token);

                        KeywordMapping & sectionMapping = Global::getSectionMapping();
                        auto iter = sectionMapping.find(sectionInfo.name());
                        if (iter != sectionMapping.end()) {
                            Keyword section = iter->second;
                            ec = handleSectionStatement(section.getType(), token);
                            if (ec.isOK()) {
                                // success
                            }
                        }
                    }
                    else if (isDigital(ch)) {
                        // It's a float or double number.
                        uint64_t fractional;
                        int exponent;
                        ec = parseRealNumberSuffix(tokenType, fractional, exponent);
                        if (ec.isOK()) {
                            token.setToken(tokenType, marker.start(), marker.length());
                            return true;
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
                {
                    stream_.next();
                    std::string singelChar;
                    ec = parseSingleCharLiteral(singelChar, token);
                    if (unlikely(!ec.isOK())) {
                        marker.rewind();
                        stream_.next();
                    }
                }
                break;

            case '\"':  // String literal or single char literal
                {
                    stream_.next();
                    std::string stringLiteral;
                    ec = parseStringLiteral(stringLiteral, token);
                    if (unlikely(!ec.isOK())) {
                        marker.rewind();
                        stream_.next();
                    }
                }
                break;

            default:    // Internal keywords
                {
                    ec = parseReservedKeyword(token);
                    if (ec.isOK()) {
                        //stream_.next();
                        token.setType(Token::ReservedKeyword);
                    }
                    else {
                        token.setType(Token::Unrecognized);
                        if (marker.length() <= 0)
                            stream_.next();
                    }
                }
                break;
            }

            token.setStartPos(marker.start());
            token.setLength(marker.length());
            assert(token.getLength() > 0);
            if (token.getType() != Token::Unknown &&
                token.getType() != Token::Unrecognized) {
                break;
            }
        }

        if (likely(!stream_.has_next())) {
            token.setToken(Token::Eof, stream_.tell(), 0);
        }

        ec_ = ec;
        return (ec == ErrorCode::OK);
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_PARSER_H
