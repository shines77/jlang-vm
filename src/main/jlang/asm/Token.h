
#ifndef JLANG_ASM_TOKEN_H
#define JLANG_ASM_TOKEN_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>     // For va_start(), va_end()
#include <assert.h>

#include <string>

#include "jlang/lang/ErrorCode.h"
#include "jlang/asm/KeywordCategory.h"

#define TOKEN_DEF(token)                        token,
#define KEYWORD_DEF(token, keyword, category)   token,
#define PREPROCESSING_DEF(keyword)              pp_##keyword,

namespace jlang {
namespace jasm {

class Token {
public:
    enum Type {
        Unrecognized = -2,
        Unsupported = -1,
        Unknown = 0,

        #include "jlang/asm/TokenDef.h"

        FirstToken = Unrecognized,
        LastToken
    };

protected:
    Type type_;
    intptr_t pos_;
    intptr_t length_;

public:
    Token() : type_(Type::Unknown), pos_(0), length_(0) {}
    Token(Type type) : type_(type), pos_(0), length_(0) {}
    Token(Type type, int length, intptr_t pos) : type_(type), pos_(pos), length_(length) {}
    Token(int16_t type) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(int16_t type, int length, intptr_t pos) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(int32_t type) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(int32_t type, int length, intptr_t pos) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(int64_t type) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(int64_t type, int length, intptr_t pos) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(const Token & src) : type_(src.type_), pos_(src.pos_), length_(src.length_) {}
    Token(Token && src) : type_(src.type_), pos_(src.pos_), length_(src.length_) {}
    ~Token() {}

    Token & operator = (const Token & rhs) {
        this->copy(rhs);
        return *this;
    }

    Token & operator = (int32_t rhs) {
        this->setType(rhs);
        return *this;
    }

    Token & operator = (int64_t rhs) {
        this->setType(rhs);
        return *this;
    }

    Type getType() const { return this->type_; }
    intptr_t getStart() const { return this->pos_; }
    intptr_t getEnd() const { return (this->pos_ + this->length_); }
    intptr_t getLength() const { return this->length_; }

    void setType(Type type) {
        this->type_ = type;
    }

    void setType(int32_t type) {
        this->type_ = (Type)type;
    }

    void setType(int64_t type) {
        this->type_ = (Type)type;
    }

    void setStart(intptr_t pos) {
        this->pos_ = pos;
    }

    void setEnd(intptr_t pos) {
        assert(pos > this->pos_);
        this->length_ = (int)(pos - this->pos_);
    }

    void setLength(int32_t length) {
        assert(length >= 0);
        this->length_ = (intptr_t)length;
    }

    void setLength(int64_t length) {
        assert(length >= 0);
        this->length_ = (intptr_t)length;
    }

    void copy(const Token & src) {
        this->type_ = src.type_;
        this->pos_ = src.pos_;
        this->length_ = src.length_;
    }

    void getToken(Type & tokenType, intptr_t & start_pos, intptr_t & length) const {
        tokenType = this->getType();
        start_pos = this->getStart();
        length    = this->getLength();
    }

    void setToken(const Type tokenType, intptr_t start_pos, intptr_t length) {
        this->setType(tokenType);
        this->setStart(start_pos);
        this->setLength(length);
    }

    bool isEquals(const Token & value) const { return (this->type_ == value.type_); }
    bool isEquals(int32_t value) const       { return (this->type_ == value); }
    bool isEquals(int64_t value) const       { return (this->type_ == value); }

    bool isLessThan(const Token & value) const { return (this->type_ < value.type_); }
    bool isLessThan(int32_t value) const       { return (this->type_ < value); }
    bool isLessThan(int64_t value) const       { return (this->type_ < value); }

    bool isGreaterThan(const Token & value) const { return (this->type_ > value.type_); }
    bool isGreaterThan(int32_t value) const       { return (this->type_ > value); }
    bool isGreaterThan(int64_t value) const       { return (this->type_ > value); }

    friend bool operator == (const Token & lhs, const Token & rhs) { return lhs.isEquals(rhs);   }
    friend bool operator <  (const Token & lhs, const Token & rhs) { return lhs.isLessThan(rhs); }

    friend bool operator != (const Token & lhs, const Token & rhs) { return !(lhs == rhs); }
    friend bool operator >  (const Token & lhs, const Token & rhs) { return  (rhs <  lhs); }
    friend bool operator <= (const Token & lhs, const Token & rhs) { return !(lhs >  rhs); }
    friend bool operator >= (const Token & lhs, const Token & rhs) { return !(lhs <  rhs); }

    friend bool operator == (const Token & lhs,       int32_t rhs) { return  lhs.isEquals(rhs); }
    friend bool operator == (      int32_t lhs, const Token & rhs) { return  rhs.isEquals(lhs); }
    friend bool operator == (const Token & lhs,       int64_t rhs) { return  lhs.isEquals(rhs); }
    friend bool operator == (      int64_t lhs, const Token & rhs) { return  rhs.isEquals(lhs); }

    friend bool operator != (const Token & lhs,       int32_t rhs) { return !lhs.isEquals(rhs); }
    friend bool operator != (      int32_t lhs, const Token & rhs) { return !rhs.isEquals(lhs); }
    friend bool operator != (const Token & lhs,       int64_t rhs) { return !lhs.isEquals(rhs); }
    friend bool operator != (      int64_t lhs, const Token & rhs) { return !rhs.isEquals(lhs); }

    friend bool operator <  (const Token & lhs,       int32_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator <  (      int32_t lhs, const Token & rhs) { return  rhs.isGreaterThan(lhs); }
    friend bool operator <  (const Token & lhs,       int64_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator <  (      int64_t lhs, const Token & rhs) { return  rhs.isGreaterThan(lhs); }

    friend bool operator >  (const Token & lhs,       int32_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator >  (      int32_t lhs, const Token & rhs) { return  rhs.isLessThan(lhs);    }
    friend bool operator >  (const Token & lhs,       int64_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator >  (      int64_t lhs, const Token & rhs) { return  rhs.isLessThan(lhs);    }

    friend bool operator <= (const Token & lhs,       int32_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator <= (      int32_t lhs, const Token & rhs) { return  rhs.isLessThan(lhs);    }
    friend bool operator <= (const Token & lhs,       int64_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator <= (      int64_t lhs, const Token & rhs) { return  rhs.isLessThan(lhs);    }

    friend bool operator >= (const Token & lhs,       int32_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator >= (      int32_t lhs, const Token & rhs) { return  rhs.isGreaterThan(lhs); }
    friend bool operator >= (const Token & lhs,       int64_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator >= (      int64_t lhs, const Token & rhs) { return  rhs.isGreaterThan(lhs); }

    char * c_str() {
        return (char *)Token::format(this->type_);
    }

    const char * c_str() const {
        return Token::format(this->type_);
    }

    std::string toString() {
        return std::string(Token::format(this->type_));
    }

    const std::string toString() const {
        return std::string(Token::format(this->type_));
    }

    #define TOKEN_TO_STRING(token)  #token

    #define CASE_TOKEN(token)       \
        case Type::token:           \
            return TOKEN_TO_STRING(token);

    #define CASE_PREPROCESSING_TOKEN(token)     \
        case Type::pp_##token:                  \
            return "#" TOKEN_TO_STRING(token);

    #define TOKEN_DEF(token)            CASE_TOKEN(token)
    #define KEYWORD_DEF(token, keyword, category) \
                                        CASE_TOKEN(token)
    #define PREPROCESSING_DEF(keyword)  CASE_PREPROCESSING_TOKEN(keyword)

    static const char * format(Type token) {
        switch (token) {
            CASE_TOKEN(Unrecognized);
            CASE_TOKEN(Unknown);

            #include "jlang/asm/TokenDef.h"

            CASE_TOKEN(LastToken);
            default: break;
        }
        return "Unknown Token";
    }

    #undef TOKEN_TO_STRING
    #undef CASE_TOKEN
    #undef CASE_PREPROCESSING_TOKEN
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_TOKEN_H
