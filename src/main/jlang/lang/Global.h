
#ifndef JLANG_LANG_GLOBAL_H
#define JLANG_LANG_GLOBAL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/NonCopyable.h"
#include "jlang/lang/Error.h"
#include "jlang/asm/Keyword.h"
#include "jlang/jstd/Singleton.h"

#include <stdint.h>
#include <mutex>

namespace jlang {

class LastError {
private:
    Error err_;

public:
    LastError() : err_(Error::OK) {}
    virtual ~LastError() {}

    bool isSuccess() const { return (err_ == Error::OK); }
    bool hasErrors() const { return (err_ != Error::OK); }

    Error getLastError() { return err_; }

    void setLastError(const Error & err) { err_ = err; }
    void setLastError(uint32_t err) { err_ = err; }
};

///////////////////////////////////////////////////
// class Global
///////////////////////////////////////////////////

class Global : public NonCopyable {
private:
    typedef std::lock_guard<std::mutex> lock_type;

    static LastError * last_error;
    static std::mutex  s_mutex;

public:
    Global() {}
    ~Global() {}

    // Global::initialize() implementation in Keyword.h file.
    static void initialize();

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
