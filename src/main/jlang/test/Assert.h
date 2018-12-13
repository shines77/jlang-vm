
#ifndef JLANG_TEST_ASSERT_H
#define JLANG_TEST_ASSERT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <iostream>
#include <string>

#define JLANG_TO_STRING(str)    #str

#define JLANG_ASSERT_EQUAL(expr) \
    Assert::Equal(!!(expr), JLANG_TO_STRING(expr))

#define JLANG_ASSERT_EQUAL_EX(expr, info) \
    Assert::Equal(!!(expr), info)

namespace jlang {
namespace Assert {

static void Equal(bool value, const std::string & info)
{
    std::cout << "[" << info.c_str() << "] -- ";
    if (value)
        std::cout << "Passed";
    else
        std::cout << "Failed";
    std::cout << std::endl;
}

} // namespace Assert
} // namespace jlang

#endif // JLANG_TEST_ASSERT_H
