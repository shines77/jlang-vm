
#ifndef JLANG_LANG_GLOBAL_H
#define JLANG_LANG_GLOBAL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/NonCopyable.h"
#include "jlang/lang/ErrorCode.h"
#include "jlang/asm/Keyword.h"

#include <stdint.h>

namespace jlang {

class LastError {
private:
    ErrorCode err_;

public:
    LastError() : err_(ErrorCode::OK) {}
    virtual ~LastError() {}

    bool isSuccess() const { return (err_ == ErrorCode::OK); }
    bool hasErrors() const { return (err_ != ErrorCode::OK); }

    ErrorCode getLastError() { return err_; }

    void setLastError(const ErrorCode & err) { err_ = err; }
    void setLastError(uint32_t err) { err_ = err; }
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

    // Global::getPPKeywordMapping() implementation in Keyword.h file.
    static jasm::KeywordMapping & getPPKeywordMapping();

    // Global::getSectionMapping() implementation in Keyword.h file.
    static jasm::KeywordMapping & getSectionMapping();

    // Global::errorInstance() implementation in FileSystem.h file.
    static LastError & lastError();

    // Global::destroyLastError() implementation in Keyword.h file.
    static void destroyLastError();
};

} // namespace jlang

#endif // JLANG_LANG_GLOBAL_H
