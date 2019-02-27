
#ifndef JLANG_ASM_TOKEN_H
#define JLANG_ASM_TOKEN_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include <stdarg.h>     // For va_start(), va_end()

#include <string>

#include "jlang/lang/ErrorCode.h"
#include "jlang/asm/KeywordKind.h"

#define TOKEN_DEF(token_type)                       token_type,
#define KEYWORD_DEF(token_type, keyword, kind)      token_type,
#define PREPROCESSING_DEF(keyword)                  pp_##keyword,

namespace jlang {
namespace jasm {

class Token {
public:
    enum Type {
        FirstToken = -1,
        Unrecognized = FirstToken,
        Unknown = 0,

        #include "jlang/asm/TokenDef.h"

        LastToken
    };

private:
    Type type_;
    int length_;
    intptr_t pos_;

public:
    Token() : type_(Type::Unknown), length_(0), pos_(0) {}
    Token(Type tokenType, int length, intptr_t pos)
        : type_(tokenType), length_(length), pos_(pos) {}
    Token(const Token & src) : type_(src.type_), length_(src.length_), pos_(src.pos_) {}
    ~Token() {}

    Token & operator = (const Token & rhs) {
        this->type_ = rhs.type_;
        this->length_ = rhs.length_;
        this->pos_ = rhs.pos_;
        return *this;
    }

    Type getType() const { return this->type_; }
    intptr_t getStartPos() const { return this->pos_; }
    intptr_t getEndPos() const { return (this->pos_ + this->length_); }
    int getLength() const { return this->length_; }

    void setType(const Type tokenType) {
        this->type_ = tokenType;
    }

    void setStartPos(intptr_t pos) {
        this->pos_ = pos;
    }

    void setEndPos(intptr_t pos) {
        assert(pos > this->pos_);
        this->length_ = (int)(pos - this->pos_);
    }

    void setLength(int length) {
        assert(length >= 0);
        this->length_ = length;
    }

    void setLength(int64_t length) {
        assert(length >= 0);
        this->length_ = (int)length;
    }

    Token copy() {
        Token copyToken(*this);
        return copyToken;
    }

    void getToken(Type & tokenType, intptr_t & start_pos, int & length) {
        tokenType = this->getType();
        start_pos = this->getStartPos();
        length    = this->getLength();
    }

    void getToken(Type & tokenType, intptr_t & start_pos, int64_t & length) {
        tokenType = this->getType();
        start_pos = this->getStartPos();
        length    = this->getLength();
    }

    void setToken(const Type tokenType, intptr_t start_pos, int length) {
        this->setType(tokenType);
        this->setStartPos(start_pos);
        this->setLength(length);
    }

    void setToken(const Type tokenType, intptr_t start_pos, int64_t length) {
        this->setType(tokenType);
        this->setStartPos(start_pos);
        this->setLength(length);
    }

    #define TOKEN_TO_STRING(token_str)  #token_str

    #define CASE_TOKEN(token_type)      \
        case Type::token_type:          \
            return TOKEN_TO_STRING(token_type);

    #define CASE_PREPROCESSING_TOKEN(token_type)    \
        case Type::pp_##token_type:                 \
            return "#" TOKEN_TO_STRING(token_type);

    #define TOKEN_DEF(token_type)       CASE_TOKEN(token_type)
    #define KEYWORD_DEF(token_type, keyword, kind) \
                                        CASE_TOKEN(token_type)
    #define PREPROCESSING_DEF(keyword)  CASE_PREPROCESSING_TOKEN(keyword)

    static const char * toString(Type tokenType) {
        switch (tokenType) {
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

    const char * toString() const {
        return Token::toString(this->type_);
    }

    void toString(std::string & str) {
        str = this->toString();
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_TOKEN_H
