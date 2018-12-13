
#ifndef JLANG_SYSTEM_SYSTEM_H
#define JLANG_SYSTEM_SYSTEM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
#ifndef _WINDOWS_LEAN
#define _WINDOWS_LEAN
#endif // _WINDOWS_LEAN
#include <windows.h>    // For Sleep()
#else
#include <unistd.h>     // For usleep()
#endif // WIN32

namespace jlang {

struct System {
    static void sleep(int millisec) {
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
        ::Sleep(millisec);
#else
        usleep(millisec * 1000);
#endif
    }

    static void pause() {
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
    ::system("pause");
    printf("\n");
#endif
    }

}; // class System

} // namespace jlang

#endif // JLANG_SYSTEM_SYSTEM_H
