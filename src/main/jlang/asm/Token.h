
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

private:
    Type type_;
    intptr_t pos_;
    intptr_t length_;

public:
    Token() : type_(Type::Unknown), pos_(0), length_(0) {}
    Token(Type type) : type_(type), pos_(0), length_(0) {}
    Token(Type type, int length, intptr_t pos) : type_(type), pos_(pos), length_(length) {}
    Token(uint16_t type) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(uint16_t type, int length, intptr_t pos) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(uint32_t type) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(uint32_t type, int length, intptr_t pos) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(uint64_t type) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(uint64_t type, int length, intptr_t pos) : type_(static_cast<Type>(type)), pos_(0), length_(0) {}
    Token(const Token & src) : type_(src.type_), pos_(src.pos_), length_(src.length_) {}
    Token(Token && src) : type_(src.type_), pos_(src.pos_), length_(src.length_) {}
    ~Token() {}

    Token & operator = (const Token & rhs) {
        this->type_ = rhs.type_;
        this->pos_ = rhs.pos_;
        this->length_ = rhs.length_;
        return *this;
    }

    Type getType() const { return this->type_; }
    intptr_t getStartPos() const { return this->pos_; }
    intptr_t getEndPos() const { return (this->pos_ + this->length_); }
    intptr_t getLength() const { return this->length_; }

    void setType(Type type) {
        this->type_ = type;
    }

    void setType(int type) {
        this->type_ = (Type)type;
    }

    void setType(int64_t type) {
        this->type_ = (Type)type;
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
        this->length_ = (intptr_t)length;
    }

    void setLength(int64_t length) {
        assert(length >= 0);
        this->length_ = (intptr_t)length;
    }

    bool isEquals(const Token & value) const {
        return (this->type_ == value.type_);
    }

    bool isNotEquals(const Token & value) const {
        return (this->type_ != value.type_);
    }

    bool isEquals(int value) const {
        return (this->type_ == value);
    }

    bool isNotEquals(int value) const {
        return (this->type_ != value);
    }

    bool isEquals(int64_t value) const {
        return (this->type_ == value);
    }

    bool isNotEquals(int64_t value) const {
        return (this->type_ != value);
    }

    Token copy() {
        Token copyToken(*this);
        return copyToken;
    }

    void getToken(Type & tokenType, intptr_t & start_pos, intptr_t & length) {
        tokenType = this->getType();
        start_pos = this->getStartPos();
        length    = this->getLength();
    }

    void setToken(const Type tokenType, intptr_t start_pos, intptr_t length) {
        this->setType(tokenType);
        this->setStartPos(start_pos);
        this->setLength(length);
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

    static const char * toString(Type token) {
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

    char * toString() {
        return (char *)Token::toString(this->type_);
    }

    const char * toString() const {
        return Token::toString(this->type_);
    }

    void toString(std::string & str) {
        str = this->toString();
    }
};

// operator == 

inline bool operator == (const Token & lhs, const Token & rhs) {
    return lhs.isEquals(rhs);
}

inline bool operator == (int32_t lhs, const Token & rhs) {
    return rhs.isEquals(lhs);
}

inline bool operator == (const Token & lhs, int32_t rhs) {
    return lhs.isEquals(rhs);
}

inline bool operator == (int64_t lhs, const Token & rhs) {
    return rhs.isEquals(lhs);
}

inline bool operator == (const Token & lhs, int64_t rhs) {
    return lhs.isEquals(rhs);
}

// operator !=

inline bool operator != (const Token & lhs, const Token & rhs) {
    return lhs.isNotEquals(rhs);
}

inline bool operator != (int32_t lhs, const Token & rhs) {
    return rhs.isNotEquals(lhs);
}

inline bool operator != (const Token & lhs, int32_t rhs) {
    return lhs.isNotEquals(rhs);
}

inline bool operator != (int64_t lhs, const Token & rhs) {
    return rhs.isNotEquals(lhs);
}

inline bool operator != (const Token & lhs, int64_t rhs) {
    return lhs.isNotEquals(rhs);
}

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_TOKEN_H
