
#ifndef JLANG_ASM_ASSEMBLER_H
#define JLANG_ASM_ASSEMBLER_H

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

class Assembler {
public:
    Assembler() {}
    ~Assembler() {}

    int readFromFile(const char * filename) {
        return 0;
    }

    int writeToFile(const char * filename) {
        return 0;
    }

    virtual int parse() = 0;
};

class Assembler32 : public Assembler {
public:
    Assembler32() {}
    ~Assembler32() {}

    int parse() {
        return 0;
    }
};

class Assembler32Ext : public Assembler {
public:
    Assembler32Ext() {}
    ~Assembler32Ext() {}

    int parse() {
        return 0;
    }
};

class Assembler64 : public Assembler {
public:
    Assembler64() {}
    ~Assembler64() {}

    int parse() {
        return 0;
    }
};

} // namespace jlang

#endif // JLANG_ASM_ASSEMBLER_H
