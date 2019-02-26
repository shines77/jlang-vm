
#ifndef JLANG_LANG_GLOBAL_H
#define JLANG_LANG_GLOBAL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/NonCopyable.h"
#include "jlang/asm/Keyword.h"

#include <stdint.h>

namespace jlang {

class LastError {
public:
    typedef uint32_t error_code_t;

private:
    error_code_t err_;

public:
    LastError() : err_(0) {}
    virtual ~LastError() {}

    bool isSuccess() const { return (err_ == 0); }
    bool hasErrors() const { return (err_ != 0); }

    error_code_t getLastError() { return err_; }
    void setLastError(error_code_t err) { err_ = err; }
};

///////////////////////////////////////////////////
// class Global
///////////////////////////////////////////////////

class Global : public lang::NonCopyable {
private:
    static LastError * last_error;

public:
    Global() {}
    ~Global() {}

    // Global::init() implementation in Keyword.h file.
    static void init();

    // Global::finalize() implementation in Keyword.h file.
    static void finalize();

    // Global::getKeywordMapping() implementation in Keyword.h file.
    static jasm::KeywordMapping & getKeywordMapping();

    // Global::getPreprocessingKeywordMapping() implementation in Keyword.h file.
    static jasm::KeywordMapping & getPreprocessingKeywordMapping();

    // Global::errorInstance() implementation in FileSystem.h file.
    static LastError & lastError();

    // Global::destroyLastError() implementation in Keyword.h file.
    static void destroyLastError();
};

} // namespace jlang

#endif // JLANG_LANG_GLOBAL_H
