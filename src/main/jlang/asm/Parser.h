
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

namespace jlang {
namespace jasm {

class Parser {
public:
    Parser() {}
    ~Parser() {}
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_PARSER_H
