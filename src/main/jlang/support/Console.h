
#ifndef JLANG_SUPPORT_CONSOLE_H
#define JLANG_SUPPORT_CONSOLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include <stdarg.h>     // For va_start(), va_end()

#include "jlang/stream/StreamRoot.h"

#ifndef USE_DEBUG_PRINT
#ifndef NDEBUG
#define USE_DEBUG_PRINT         1
#else
#define USE_DEBUG_PRINT         0
#endif // NDEBUG
#endif // USE_DEBUG_PRINT

namespace jlang {

class Console {
protected:
    StreamRoot & stream_;

public:
    Console(StreamRoot & stream) : stream_(stream) {}
    virtual ~Console() {}

    void print(const char * info) {
        ::printf(info);
    }

    void vprintf(const char * fmt, va_list args) {
        ::vprintf(fmt, args);
    }

    void printf(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vprintf(fmt, args);
        va_end(args);
    }

    void trace(const char * fmt, ...) {
#if USE_DEBUG_PRINT
        va_list args;
        va_start(args, fmt);
        this->vprintf(fmt, args);
        va_end(args);
#endif
    }

    void trace(const char * fmt, va_list args) {
#if USE_DEBUG_PRINT
        this->vprintf(fmt, args);
#endif
    }

    void debug(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->printf("[Debug]");
        this->vprintf(fmt, args);
        va_end(args);
    }

    void debug(const char * fmt, va_list args) {
        this->vprintf(fmt, args);
    }

    void info(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vprintf(fmt, args);
        va_end(args);
    }

    void info(const char * fmt, va_list args) {
        this->vprintf(fmt, args);
    }
};

class StdConsole : public Console {
public:
    StdConsole() : Console(*(new StreamRoot())) {}

    virtual ~StdConsole() {
        this->stream_.destroy();
    }

};

static StdConsole console;

} // namespace jlang

#endif // JLANG_SUPPORT_CONSOLE_H
