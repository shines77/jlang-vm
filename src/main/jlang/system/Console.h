
#ifndef JLANG_SYSTEM_CONSOLE_H
#define JLANG_SYSTEM_CONSOLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"
#include "jlang/lang/Logger.h"
#include "jlang/support/StopWatch.h"

#include <stdio.h>
#include <stdarg.h>

#ifdef _MSC_VER
#include <conio.h>
#endif // _MSC_VER

namespace jlang {

void cpu_warmup(int delayTime)
{
#if defined(NDEBUG)
    double startTime, stopTime;
    double delayTimeLimit = (double)delayTime / 1000.0;
    volatile int sum = 0;

    printf("CPU warm-up begin ...\n");
    fflush(stdout);
    startTime = StopWatch::timestamp();
    double elapsedTime;
    do {
        for (int i = 0; i < 500; ++i) {
            sum += i;
            for (int j = 5000; j >= 0; --j) {
                sum -= j;
            }
        }
        stopTime = StopWatch::timestamp();
        elapsedTime = stopTime - startTime;
    } while (elapsedTime < delayTimeLimit);

    printf("sum = %u, time: %0.3f ms\n", sum, elapsedTime * 1000.0);
    printf("CPU warm-up end   ... \n\n");
    fflush(stdout);
#endif // !_DEBUG
}

} // namespace jlang

namespace jlang {
namespace Console {

static jlang::ConsoleLogger<false> & getConsoleLogger()
{
    static jlang::ConsoleLogger<false> s_consoleLogger;
    return s_consoleLogger;
}

static void print(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    getConsoleLogger().print_args(fmt, args);
    va_end(args);
}

static void print_ln(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    getConsoleLogger().print_args_ln(fmt, args);
    va_end(args);
}

static void trace(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    getConsoleLogger().trace_args(fmt, args);
    va_end(args);
}

static void write(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    getConsoleLogger().print_args(fmt, args);
    va_end(args);
}

static void writeLine(const char * fmt = NULL, ...)
{
    if (fmt != NULL) {
        va_list args;
        va_start(args, fmt);
        getConsoleLogger().print_args_ln(fmt, args);
        va_end(args);
    } else {
        getConsoleLogger().new_line();
    }
}

static int readKey(bool enabledCpuWarmup = false, bool displayTips = true,
                   bool echoInput = false, bool newLine = false) {
    int keyCode;
    if (displayTips) {
        printf("Press any key to continue ...");

        keyCode = getchar();
        printf("\n");
    }
    else {
        keyCode = getchar();
        if (echoInput) {
            if (keyCode != EOF)
                printf("%c", (char)keyCode);
            else
                printf("EOF: (%d)", keyCode);
        }
    }

    if (newLine) {
        printf("\n");
    }

    // After call jimi_getch(), warm up the CPU again, at least 500 ms.
    if (enabledCpuWarmup) {
        // Run the code in a loop for a while, to warm up the CPU.
        cpu_warmup(500);
    }
    return keyCode;
}

static int readKeyLine(bool enabledCpuWarmup = false, bool displayTips = true,
                       bool echoInput = false) {
    return readKey(false, displayTips, echoInput, true);
}

} // namespace Console
} // namespace jlang

#define jlConsole jlang::Console

#endif // !JLANG_SYSTEM_CONSOLE_H
