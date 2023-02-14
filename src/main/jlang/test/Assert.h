
#ifndef JLANG_TEST_ASSERT_H
#define JLANG_TEST_ASSERT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <iostream>
#include <string>

#include "jlang/basic/stddef.h"

// JLANG_ASSERT_TRUE

#define JLANG_ASSERT_TRUE1(expr) \
    jlang::Assert::IsTrue(!!(expr), JLANG_TO_STRING(expr))

#define JLANG_ASSERT_TRUE(expr, info) \
    jlang::Assert::IsTrue(!!(expr), info)

#define JLANG_ASSERT_TRUE_EX(expr, info, msg) \
    jlang::Assert::IsTrue(!!(expr), info, msg)

// JLANG_ASSERT_FALSE

#define JLANG_ASSERT_FALSE1(expr) \
    jlang::Assert::IsFalse(!!(expr), JLANG_TO_STRING(expr))

#define JLANG_ASSERT_FALSE(expr, info) \
    jlang::Assert::IsFalse(!!(expr), info)

#define JLANG_ASSERT_FALSE_EX(expr, info, msg) \
    jlang::Assert::IsFalse(!!(expr), info, msg)

// JLANG_ASSERT_EQUAL

#define JLANG_ASSERT_EQUAL2(expr1, expr2) \
    jlang::Assert::Equal(!!(expr1), !!(expr2), JLANG_TO_STRING(expr1) " == " JLANG_TO_STRING(expr2))

#define JLANG_ASSERT_EQUAL(expr1, expr2, info) \
    jlang::Assert::Equal(!!(expr1), !!(expr2), info)

#define JLANG_ASSERT_EQUAL_EX(expr1, expr2, info, msg) \
    jlang::Assert::Equal(!!(expr1), !!(expr2), info, msg)

// JLANG_ASSERT_NOTEQUAL

#define JLANG_ASSERT_NOTEQUAL2(expr1, expr2) \
    jlang::Assert::Equal(!!(expr1), !!(expr2), JLANG_TO_STRING(expr1) " != " JLANG_TO_STRING(expr2))

#define JLANG_ASSERT_NOTEQUAL(expr1, expr2, info) \
    jlang::Assert::Equal(!!(expr1), !!(expr2), info)

#define JLANG_ASSERT_NOTEQUAL_EX(expr1, expr2, info, msg) \
    jlang::Assert::Equal(!!(expr1), !!(expr2), info, msg)

namespace jlang {
namespace Assert {

static const char * kSuccessStr = "Success";
static const char * kPassedStr = "Passed";
static const char * kFailedStr = "Failed";

static void printSuccess()
{
    std::cout << kSuccessStr;
}

static void printPassed()
{
    std::cout << kPassedStr;
}

static void printFailed()
{
    std::cout << kFailedStr;
}

static void printlnSuccess()
{
    printSuccess();
    std::cout << std::endl;
}

static void printlnPassed()
{
    printPassed();
    std::cout << std::endl;
}

static void printlnFailed()
{
    printFailed();
    std::cout << std::endl;
}

static void IsTrue(bool value, const std::string & info)
{
    std::cout << "[" << info << "] -- ";
    if (value)
        printlnPassed();
    else
        printlnFailed();
}

static void IsTrue(bool value, const std::string & info, const std::string & message)
{
    std::cout << "[" << info << "] -- ";
    if (value) {
        printlnPassed();
    }
    else {
        printlnFailed();
        std::cout << message << std::endl;
    }
}

static void IsFalse(bool value, const std::string & info)
{
    std::cout << "[" << info << "] -- ";
    if (!value)
        printlnPassed();
    else
        printlnFailed();
}

static void IsFalse(bool value, const std::string & info, const std::string & message)
{
    std::cout << "[" << info << "] -- ";
    if (!value) {
        printlnPassed();
    }
    else {
        printlnFailed();
        std::cout << message << std::endl;
    }
}

static void Equal(bool value1, bool value2, const std::string & info)
{
    std::cout << "[" << info << "] -- ";
    if (value1 == value2)
        printlnPassed();
    else
        printlnFailed();
}

static void Equal(bool value1, bool value2, const std::string & info, const std::string & message)
{
    std::cout << "[" << info << "] -- ";
    if (value1 == value2) {
        printlnPassed();
    }
    else {
        printlnFailed();
        std::cout << message << std::endl;
    }
}

static void NotEqual(bool value1, bool value2, const std::string & info)
{
    std::cout << "[" << info << "] -- ";
    if (value1 != value2)
        printlnPassed();
    else
        printlnFailed();
}

static void NotEqual(bool value1, bool value2, const std::string & info, const std::string & message)
{
    std::cout << "[" << info << "] -- ";
    if (value1 != value2) {
        printlnPassed();
    }
    else {
        printlnFailed();
        std::cout << message << std::endl;
    }
}

} // namespace Assert
} // namespace jlang

#endif // JLANG_TEST_ASSERT_H
