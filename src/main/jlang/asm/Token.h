
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

#include "jlang/lang/Comparatorable.h"

namespace jlang {
namespace jasm {

class Token : public Comparatorable<Token>,
              public Comparatorable<int32_t>,
              public Comparatorable<int64_t> {
public:
    #define TOKEN_DEF(token)                                token,
    #define KEYWORD_DEF(token, keywordId, keyword, kind)    token,
    #define PREPROCESSING_DEF(keyword)                      pp_##keyword,

    enum Type {
        Unrecognized = -2,
        Unsupported = -1,
        Unknown = 0,

        #include "jlang/asm/TokenDef.h"

        FirstToken = Unrecognized,
        LastToken
    };

protected:
    Type token_;

public:
    Token() : token_(Token::Unknown) {}
    Token(Type token) : token_(token) {}
    Token(int16_t token) : token_(static_cast<Type>(token)) {}
    Token(int32_t token) : token_(static_cast<Type>(token)) {}
    Token(int64_t token) : token_(static_cast<Type>(token)) {}
    Token(const Token & src) : token_(src.token_) {}
    Token(Token && src) : token_(src.token_) {}
    ~Token() {}

    Token & operator = (const Token & rhs) {
        this->copy(rhs);
        return *this;
    }

    Token & operator = (int16_t rhs) {
        this->set(rhs);
        return *this;
    }

    Token & operator = (int32_t rhs) {
        this->set(rhs);
        return *this;
    }

    Token & operator = (int64_t rhs) {
        this->set(rhs);
        return *this;
    }

    Type get() const { return this->token_; }

    void set(Type token)     { this->token_ = token; }

    void set(int16_t token)  { this->token_ = (Type)token; }
    void set(int32_t token)  { this->token_ = (Type)token; }
    void set(int64_t token)  { this->token_ = (Type)token; }

    int32_t value() const  { return static_cast<int32_t>(this->token_); }

    Type type() const { return this->token_; }    

    void setType(Type token) { this->token_ = token; }

    void setType(int16_t type) { this->token_ = (Type)type; }
    void setType(int32_t type) { this->token_ = (Type)type; }
    void setType(int64_t type) { this->token_ = (Type)type; }

    void copy(const Token & src) {
        this->token_ = src.token_;
    }

    bool isEquals(const Token & value) const override { return (this->token_ == value.token_); }
    bool isEquals(int32_t value) const override       { return (this->token_ == value); }
    bool isEquals(int64_t value) const override       { return (this->token_ == value); }

    bool isLessThan(const Token & value) const override { return (this->token_ < value.token_); }
    bool isLessThan(int32_t value) const override       { return (this->token_ < value); }
    bool isLessThan(int64_t value) const override       { return (this->token_ < value); }

    bool isGreaterThan(const Token & value) const override { return (this->token_ > value.token_); }
    bool isGreaterThan(int32_t value) const override       { return (this->token_ > value); }
    bool isGreaterThan(int64_t value) const override       { return (this->token_ > value); }

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
        return (char *)Token::format(this->token_);
    }

    const char * c_str() const {
        return Token::format(this->token_);
    }

    std::string toString() {
        return std::string(Token::format(this->token_));
    }

    const std::string toString() const {
        return std::string(Token::format(this->token_));
    }

    ///////////////////////////////////////////////////////////////////////

    #define TOKEN_DEF(token)            CASE_TOKEN(token)
    #define KEYWORD_DEF(token, keywordId, keyword, kind) \
                                        CASE_TOKEN(token)
    #define PREPROCESSING_DEF(keyword)  CASE_PREPROCESSING_TOKEN(keyword)

    ///////////////////////////////////////////////////////////////////////

    #define TOKEN_TO_STRING(token)  #token

    #define CASE_TOKEN(token)       \
        case Type::token:           \
            return TOKEN_TO_STRING(token);

    #define CASE_PREPROCESSING_TOKEN(token)     \
        case Type::pp_##token:                  \
            return "Token::" TOKEN_TO_STRING(token);

    ///////////////////////////////////////////////////////////////////////

    static const char * format(Type token) {
        switch (token) {
            CASE_TOKEN(Unrecognized);
            CASE_TOKEN(Unknown);

            #include "jlang/asm/TokenDef.h"

            CASE_TOKEN(LastToken);

            default: break;
        }

        return "Token::Undefined";
    }

    #undef TOKEN_TO_STRING
    #undef CASE_TOKEN
    #undef CASE_PREPROCESSING_TOKEN
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_TOKEN_H
