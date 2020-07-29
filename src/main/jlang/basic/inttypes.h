
#ifndef JLANG_BASIC_INTTYPES_H
#define JLANG_BASIC_INTTYPES_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1700)
#include "jlang/basic/msvc/inttypes.h"
#else
#include <inttypes.h>
#endif // _MSC_VER

#if !defined(__cplusplus) && (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#ifndef PRIuSIZE
#define PRIuSIZE  "zu"
#endif
#ifndef PRIsSIZE
#define PRIsSIZE  "z"
#endif
#else
#ifndef PRIuSIZE
#define PRIuSIZE  PRIuPTR
#endif
#ifndef PRIsSIZE
#define PRIsSIZE  PRIdPTR
#endif
#endif // __STDC_VERSION__ >= 199901L

#endif // JLANG_BASIC_INTTYPES_H
