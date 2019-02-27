
#ifndef JLANG_ASM_PARSER_H
#define JLANG_ASM_PARSER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include <stdarg.h>     // For va_start(), va_end()

#include "jlang/lang/ErrorCode.h"

#include "jlang/asm/Token.h"

namespace jlang {
namespace jasm {

class Parser {
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
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_PARSER_H
