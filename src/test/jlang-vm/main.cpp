
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

#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__) 
#define JLANG_SCRIPT_BASE_DIR       "..\\..\\..\\scripts\\"
#else
#define JLANG_SCRIPT_BASE_DIR       "./scripts/"
#endif
#define JLANG_SCRIPT_PATH(path)     JLANG_SCRIPT_BASE_DIR path

static const int kWarmupMillsecs = 600;

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
    printf("  test_Fibonacci32()  [native]\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    // Warm-up CPU for 500 ms.
    cpu_warmup(kWarmupMillsecs);

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
    printf("  test_Fibonacci64()  [native]\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    // Warm-up CPU for 500 ms.
    cpu_warmup(kWarmupMillsecs);

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

    // Warm-up CPU for 500 ms.
    cpu_warmup(kWarmupMillsecs);

    StopWatch sw;

    v1::Interpreter<> it;
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

    // Warm-up CPU for 500 ms.
    cpu_warmup(kWarmupMillsecs);

    StopWatch sw;

    v2::Interpreter<> it;
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

    // Warm-up CPU for 500 ms.
    cpu_warmup(kWarmupMillsecs);

    StopWatch sw;

    v3::Interpreter<> it;
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

    // Warm-up CPU for 500 ms.
    cpu_warmup(kWarmupMillsecs);

    StopWatch sw;

    v3::Interpreter<> it;
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

void test_Interpreter_v4()
{
    printf("--------------------------------------------\n");
    printf("  test_Interpreter_v4()\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    // Warm-up CPU for 500 ms.
    cpu_warmup(kWarmupMillsecs);

    StopWatch sw;

    v4::Interpreter<> it;
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

void test_Interpreter_v4_inline()
{
    printf("--------------------------------------------\n");
    printf("  test_Interpreter_v4_inline()\n");
    printf("--------------------------------------------\n\n");

    uint32_t n;
    printf("n = ? ");
#if defined(_WIN32)
    int r = scanf_s("%u", &n);
#else
    int r = scanf("%u", &n);
#endif
    printf("\n");

    // Warm-up CPU for 500 ms.
    cpu_warmup(kWarmupMillsecs);

    StopWatch sw;

    v4::Interpreter<> it;
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

void test_Assembler()
{
    printf("--------------------------------------------\n");
    printf("  test_Assembler()\n");
    printf("--------------------------------------------\n\n");

    using namespace jlang::jasm;

    TokenInfo ti(Token::Unknown);
    AsmParser parser;

    FileStringStream stream(JLANG_SCRIPT_PATH("asm/fibonacci.jasm"));
    parser.setStream(stream);

    bool success = false;
    Error ec = parser.parse();
    if (ec.isOk()) {
        printf(">>  Parser: success. [file = %s]\n", stream.filename().c_str());
    }
    else {
        printf(">>  Parser: failed. [file = %s]\n", stream.filename().c_str());
    }

    InputStringStream stream1;
    Scanner scanner(stream1);

    int keywordId = Keyword::If;

    /*
    jstd::SmallString<14> * smallString = new jstd::SmallString<14>();
    printf("\n");
    printf("alignof(jstd::SmallString<16>) = %d\n", (int)alignof(jstd::SmallString<16>));
    printf("sizeof(jstd::SmallString<16>)  = %d\n", (int)sizeof(jstd::SmallString<16>));
    printf("address_of(jstd::SmallString<16>) = 0x%p\n", smallString);
    printf("SmallString.data_[] offset = %d\n", (int)((char *)&smallString->data_[0] - (char *)smallString));
    printf("SmallString.size_ offset   = %d\n", (int)((char *)&smallString->size_ - (char *)smallString));
    //*/

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

    jasm::Initializer initializer;

    test_Assembler();

#if 0

#ifdef NDEBUG
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
    test_Fibonacci64();
#else
    test_Fibonacci32();
#endif
#endif

    //test_Interpreter_v4_inline();
    test_Interpreter_v3_inline();
    test_Interpreter_v4();
    test_Interpreter_v3();
    test_Interpreter_v2();
    test_Interpreter_v1();

#endif

    printf("\n");
    System::pause();

    return 0;
}
