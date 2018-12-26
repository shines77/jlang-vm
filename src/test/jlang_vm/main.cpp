
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <iostream>
#include <iomanip>  // std::setw(), std::setfill(), std::setprecision().
#include <sstream>
#include <string>
#include <utility>

#include <jlang/jlang.h>

using namespace jlang;

uint32_t fibonacci32(uint32_t n)
{
    if (n >= 3)
        return fibonacci32(n - 1) + fibonacci32(n - 2);
    else
        return 1;
}

uint64_t fibonacci64(uint32_t n)
{
    if (n >= 3)
        return fibonacci64(n - 1) + fibonacci64(n - 2);
    else
        return 1;
}

void test_Fibonacci32()
{
    printf("--------------------------------------------\n");
    printf("  test_Fibonacci32()-[native]\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    StopWatch sw;

    sw.start();
    uint32_t fib_n = fibonacci32(n);
    sw.stop();

    printf("  fibonacci(%u) = %u\n\n", n, fib_n);

    double elapsed_time = sw.getElapsedMillisec();
    printf("  elapsed time:  %0.3f ms\n", elapsed_time);
    printf("\n");
}

void test_Fibonacci64()
{
    printf("--------------------------------------------\n");
    printf("  test_Fibonacci64()-[native]\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    StopWatch sw;

    sw.start();
    uint64_t fib_n = fibonacci64(n);
    sw.stop();

    printf("  fibonacci(%u) = %llu\n\n", n, fib_n);

    double elapsed_time = sw.getElapsedMillisec();
    printf("  elapsed time:  %0.3f ms\n", elapsed_time);
    printf("\n");
}

void test_Interpreter_v1()
{
    printf("--------------------------------------------\n");
    printf("  test_Interpreter_v1()\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    StopWatch sw;

    Interpreter_v1<> it;
    vmReturn<> retVal;
    retVal.setDataType(vmReturn<>::Basic);
    retVal.setValue(n);
    
    int ec = it.create();

    sw.start();
    ec = it.run(retVal);
    if (ec >= 0) {
        sw.stop();
        if (retVal.isValid()) {
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
            printf("  fibonacci(%u) = %llu\n", n, retVal.getValue());
#else
            printf("  fibonacci(%u) = %u\n", n, retVal.getValue());
#endif
        }
    }
    printf("\n");

    double elapsed_time = sw.getElapsedMillisec();
    printf("  elapsed time:  %0.3f ms\n", elapsed_time);
    printf("\n");
}

void test_Interpreter_v2()
{
    printf("--------------------------------------------\n");
    printf("  test_Interpreter_v2()\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    StopWatch sw;

    Interpreter_v2<> it;
    vmReturn<> retVal;
    retVal.setDataType(vmReturn<>::Basic);
    retVal.setValue(n);
    
    int ec = it.create();

    sw.start();
    ec = it.run(retVal);
    if (ec >= 0) {
        sw.stop();
        if (retVal.isValid()) {
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
            printf("  fibonacci(%u) = %llu\n", n, retVal.getValue());
#else
            printf("  fibonacci(%u) = %u\n", n, retVal.getValue());
#endif
        }
    }
    printf("\n");

    double elapsed_time = sw.getElapsedMillisec();
    printf("  elapsed time:  %0.3f ms\n", elapsed_time);
    printf("\n");
}

void test_Interpreter_v3()
{
    printf("--------------------------------------------\n");
    printf("  test_Interpreter_v3()\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    StopWatch sw;

    Interpreter_v3<> it;
    vmReturn<> retVal;
    retVal.setDataType(vmReturn<>::Basic);
    retVal.setValue(n);
    
    int ec = it.create();

    sw.start();
    ec = it.run(retVal);
    if (ec >= 0) {
        sw.stop();
        if (retVal.isValid()) {
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
            printf("  fibonacci(%u) = %llu\n", n, retVal.getValue());
#else
            printf("  fibonacci(%u) = %u\n", n, retVal.getValue());
#endif
        }
    }
    printf("\n");

    double elapsed_time = sw.getElapsedMillisec();
    printf("  elapsed time:  %0.3f ms\n", elapsed_time);
    printf("\n");
}

void test_Interpreter_v3_inline()
{
    printf("--------------------------------------------\n");
    printf("  test_Interpreter_v3_inline()\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    StopWatch sw;

    Interpreter_v3<> it;
    vmReturn<> retVal;
    retVal.setDataType(vmReturn<>::Basic);
    retVal.setValue(n);
    
    int ec = it.create();

    sw.start();
    ec = it.run_inline(retVal);
    if (ec >= 0) {
        sw.stop();
        if (retVal.isValid()) {
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
            printf("  fibonacci(%u) = %llu\n", n, retVal.getValue());
#else
            printf("  fibonacci(%u) = %u\n", n, retVal.getValue());
#endif
        }
    }
    printf("\n");

    double elapsed_time = sw.getElapsedMillisec();
    printf("  elapsed time:  %0.3f ms\n", elapsed_time);
    printf("\n");
}

void print_version()
{
    std::cout << std::endl;
    std::cout << "jlang-vm: version 1.0" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char * argv[])
{
    print_version();

#ifdef NDEBUG
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    test_Fibonacci64();
#else
    test_Fibonacci32();
#endif
#endif
    test_Interpreter_v3_inline();
    test_Interpreter_v3();
    test_Interpreter_v2();
    test_Interpreter_v1();

    printf("\n");
    System::pause();

    return 0;
}
