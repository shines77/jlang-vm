
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

struct LogLevel {
    static const int Unknown = 0;
    static const int Trace = 1;
    static const int Info = 2;
    static const int Debug = 3;
    static const int Warn = 4;
    static const int Error = 5;
    static const int Fatal = 6;
    static const int Max = Fatal;
};

static const char * sLevelName[] = {
    "Unknown",
    "Trace",
    "Info",
    "Debug",
    "Warn",
    "Error",
    "Fatal",
};

static const char * sLevelPrefix[] = {
    "[Unknown]: ",
    "[Trace]: ",
    "[Info]:  ",
    "[Debug]: ",
    "[Warn]:  ",
    "[Error]: ",
    "[Fatal]: ",
};

class Console {
protected:
    StreamRoot & stream_;
    int level_;

public:
    Console(StreamRoot & stream) : stream_(stream), level_(LogLevel::Info) {}
    virtual ~Console() {}

    int getLevel() const { return this->level_; }

    void setLevel(int level) {
        this->level_ = level;
    }

    void nextLine() {
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
        ::printf("\r\n");
#else
        ::printf("\n");
#endif
    }

    void print(const char * info) {
        ::printf(info);
    }

    void println(const char * info) {
        ::printf(info);
        this->nextLine();
    }

    void vfprint(const char * fmt, va_list args) {
        ::vprintf(fmt, args);
    }

    void vfprintln(const char * fmt, va_list args) {
        ::vprintf(fmt, args);
        this->nextLine();
    }

    void printf(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vfprint(fmt, args);
        va_end(args);
    }

    void printfln(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vfprint(fmt, args);
        va_end(args);

        this->nextLine();
    }

    void output(int level, const char * fmt, ...) {
        va_list args;
        if (level <= this->level_) {
            va_start(args, fmt);
            if (level <= LogLevel::Max)
                this->print(sLevelPrefix[level]);
            else
                this->print("[]");
            this->vfprint(fmt, args);
            va_end(args);
        }
    }

    void outputln(int level, const char * fmt, ...) {
        va_list args;
        if (level <= this->level_) {
            va_start(args, fmt);
            if (level <= LogLevel::Max)
                this->print(sLevelPrefix[level]);
            else
                this->print("[]");
            this->vfprint(fmt, args);
            va_end(args);

            this->nextLine();
        }
    }

    void trace(const char * fmt, ...) {
#if USE_DEBUG_PRINT
        va_list args;
        va_start(args, fmt);
        this->outputln(LogLevel::Trace, fmt, args);
        va_end(args);
#endif
    }

    void debug(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->outputln(LogLevel::Debug, fmt, args);
        va_end(args);
    }

    void info(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->outputln(LogLevel::Info, fmt, args);
        va_end(args);
    }

    void error(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->outputln(LogLevel::Error, fmt, args);
        va_end(args);
    }

    void warning(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->outputln(LogLevel::Warn, fmt, args);
        va_end(args);
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
