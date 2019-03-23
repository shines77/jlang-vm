
#ifndef JLANG_ASM_TOKENINFO_H
#define JLANG_ASM_TOKENINFO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>     // For va_start(), va_end()
#include <assert.h>

#include <string>

#include "jlang/asm/Token.h"
#include "jlang/asm/KeywordKind.h"
#include "jlang/lang/Error.h"

namespace jlang {
namespace jasm {

class TokenInfo {
protected:
    Token token_;
    intptr_t pos_;
    intptr_t length_;

public:
    TokenInfo() : token_(Token::Unknown), pos_(0), length_(0) {}
    TokenInfo(Token token) : token_(token), pos_(0), length_(0) {}
    TokenInfo(Token token, intptr_t length, intptr_t pos) : token_(token), pos_(pos), length_(length) {}
    TokenInfo(int16_t token) : token_(token), pos_(0), length_(0) {}
    TokenInfo(int16_t token, intptr_t length, intptr_t pos) : token_(token), pos_(0), length_(0) {}
    TokenInfo(int32_t token) : token_(token), pos_(0), length_(0) {}
    TokenInfo(int32_t token, intptr_t length, intptr_t pos) : token_(token), pos_(0), length_(0) {}
    TokenInfo(int64_t token) : token_(token), pos_(0), length_(0) {}
    TokenInfo(int64_t token, intptr_t length, intptr_t pos) : token_(token), pos_(0), length_(0) {}
    TokenInfo(const TokenInfo & src) : token_(src.token_), pos_(src.pos_), length_(src.length_) {}
    TokenInfo(TokenInfo && src) : token_(src.token_), pos_(src.pos_), length_(src.length_) {}
    ~TokenInfo() {}

    TokenInfo & operator = (const TokenInfo & rhs) {
        this->copy(rhs);
        return *this;
    }

    TokenInfo & operator = (int16_t rhs) {
        this->setToken(rhs);
        return *this;
    }

    TokenInfo & operator = (int32_t rhs) {
        this->setToken(rhs);
        return *this;
    }

    TokenInfo & operator = (int64_t rhs) {
        this->setToken(rhs);
        return *this;
    }

    Token token() const { return this->token_; }
    Token getToken() const { return this->token_; }

    void setToken(Token type)   { this->token_ = type; }
    void setToken(int16_t type) { this->token_ = type; }
    void setToken(int32_t type) { this->token_ = type; }
    void setToken(int64_t type) { this->token_ = type; }

    intptr_t getStart() const  { return this->pos_; }
    intptr_t getEnd() const    { return (this->pos_ + this->length_); }
    intptr_t getLength() const { return this->length_; }

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

    void copy(const TokenInfo & src) {
        this->token_ = src.token_;
        this->pos_ = src.pos_;
        this->length_ = src.length_;
    }

    void getToken(Token & token, intptr_t & start_pos, intptr_t & length) const {
        token     = this->getToken();
        start_pos = this->getStart();
        length    = this->getLength();
    }

    void setToken(Token token, intptr_t start_pos, intptr_t length) {
        this->setToken(token);
        this->setStart(start_pos);
        this->setLength(length);
    }

    bool isEquals(const TokenInfo & value) const { return (this->token_ == value.token_); }
    bool isEquals(int32_t value) const           { return (this->token_ == value); }
    bool isEquals(int64_t value) const           { return (this->token_ == value); }

    bool isLessThan(const TokenInfo & value) const { return (this->token_ < value.token_); }
    bool isLessThan(int32_t value) const           { return (this->token_ < value); }
    bool isLessThan(int64_t value) const           { return (this->token_ < value); }

    bool isGreaterThan(const TokenInfo & value) const { return (this->token_ > value.token_); }
    bool isGreaterThan(int32_t value) const           { return (this->token_ > value); }
    bool isGreaterThan(int64_t value) const           { return (this->token_ > value); }

    friend bool operator == (const TokenInfo & lhs, const TokenInfo & rhs) { return lhs.isEquals(rhs);   }
    friend bool operator <  (const TokenInfo & lhs, const TokenInfo & rhs) { return lhs.isLessThan(rhs); }

    friend bool operator != (const TokenInfo & lhs, const TokenInfo & rhs) { return !(lhs == rhs); }
    friend bool operator >  (const TokenInfo & lhs, const TokenInfo & rhs) { return  (rhs <  lhs); }
    friend bool operator <= (const TokenInfo & lhs, const TokenInfo & rhs) { return !(lhs >  rhs); }
    friend bool operator >= (const TokenInfo & lhs, const TokenInfo & rhs) { return !(lhs <  rhs); }

    friend bool operator == (const TokenInfo & lhs,           int32_t rhs) { return  lhs.isEquals(rhs); }
    friend bool operator == (          int32_t lhs, const TokenInfo & rhs) { return  rhs.isEquals(lhs); }
    friend bool operator == (const TokenInfo & lhs,           int64_t rhs) { return  lhs.isEquals(rhs); }
    friend bool operator == (          int64_t lhs, const TokenInfo & rhs) { return  rhs.isEquals(lhs); }

    friend bool operator != (const TokenInfo & lhs,           int32_t rhs) { return !lhs.isEquals(rhs); }
    friend bool operator != (          int32_t lhs, const TokenInfo & rhs) { return !rhs.isEquals(lhs); }
    friend bool operator != (const TokenInfo & lhs,           int64_t rhs) { return !lhs.isEquals(rhs); }
    friend bool operator != (          int64_t lhs, const TokenInfo & rhs) { return !rhs.isEquals(lhs); }

    friend bool operator <  (const TokenInfo & lhs,           int32_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator <  (          int32_t lhs, const TokenInfo & rhs) { return  rhs.isGreaterThan(lhs); }
    friend bool operator <  (const TokenInfo & lhs,           int64_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator <  (          int64_t lhs, const TokenInfo & rhs) { return  rhs.isGreaterThan(lhs); }

    friend bool operator >  (const TokenInfo & lhs,           int32_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator >  (          int32_t lhs, const TokenInfo & rhs) { return  rhs.isLessThan(lhs);    }
    friend bool operator >  (const TokenInfo & lhs,           int64_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator >  (          int64_t lhs, const TokenInfo & rhs) { return  rhs.isLessThan(lhs);    }

    friend bool operator <= (const TokenInfo & lhs,           int32_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator <= (          int32_t lhs, const TokenInfo & rhs) { return  rhs.isLessThan(lhs);    }
    friend bool operator <= (const TokenInfo & lhs,           int64_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator <= (          int64_t lhs, const TokenInfo & rhs) { return  rhs.isLessThan(lhs);    }

    friend bool operator >= (const TokenInfo & lhs,           int32_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator >= (          int32_t lhs, const TokenInfo & rhs) { return  rhs.isGreaterThan(lhs); }
    friend bool operator >= (const TokenInfo & lhs,           int64_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator >= (          int64_t lhs, const TokenInfo & rhs) { return  rhs.isGreaterThan(lhs); }

    char * c_str() {
        return this->token_.c_str();
    }

    const char * c_str() const {
        return this->token_.c_str();
    }

    std::string toString() {
        return this->token_.toString();
    }

    const std::string toString() const {
        return this->token_.toString();
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_TOKENINFO_H
