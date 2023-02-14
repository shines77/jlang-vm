
#ifndef JLANG_SYSTEM_SYSTEM_H
#define JLANG_SYSTEM_SYSTEM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdlib.h>     // For ::system("pause")
#include <stdio.h>      // For printf()

#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
#ifndef _WINDOWS_LEAN
#define _WINDOWS_LEAN
#endif // _WINDOWS_LEAN
#include <windows.h>    // For Sleep()
#else
#include <unistd.h>     // For usleep()
#endif // WIN32

#include "jlang/system/Console.h"

namespace jlang {
namespace System {

static void Sleep(unsigned int millisec)
{
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
    ::Sleep(millisec);
#else
    unsigned int seconds = millisec / 1000;
    unsigned int usec = (millisec % 1000) * 1000;
    if (seconds > 0) ::sleep(seconds);
    ::usleep(usec);
#endif
}

static void uSleep(unsigned int usec)
{
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
    ::Sleep(usec / 1000);
#else
    unsigned int seconds = usec / 1000000;
    unsigned int us = usec % 1000000;
    if (seconds > 0) ::sleep(seconds);
    // us is in the range [0, 1000000]
    ::usleep(us);
#endif
}

static void Pause()
{
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
    ::system("pause");
    printf("\n");
#else
    Console::ReadKeyLine(false, true, false);
#endif
}

} // namespace System
} // namespace jlang

#endif // JLANG_SYSTEM_SYSTEM_H
