
#include "jlang/lang/Global.h"
#include "jlang/asm/Keyword.h"

#include <assert.h>

#include <mutex>
#include <memory>

using namespace jlang;

std::unique_ptr<jasm::KeywordMapping> jasm::KeywordInitializer::keyword_mapping     = nullptr;
std::unique_ptr<jasm::KeywordMapping> jasm::KeywordInitializer::pp_keyword_mapping  = nullptr;
std::unique_ptr<jasm::KeywordMapping> jasm::KeywordInitializer::section_mapping     = nullptr;

LastError * Global::last_error = nullptr;
std::mutex  Global::s_mutex;

///////////////////////////////////////////////////
// Global::init()
///////////////////////////////////////////////////

void Global::initialize() {
    lock_type lock(s_mutex);

    jasm::KeywordInitializer::initialize();
    LastError & _lastError = Global::lastError();
    assert(_lastError.isSuccess());
}

///////////////////////////////////////////////////
// Global::finalize()
///////////////////////////////////////////////////

void Global::finalize() {
    lock_type lock(s_mutex);

    jasm::KeywordInitializer::finalize();
    Global::destroyLastError();
}

///////////////////////////////////////////////////
// Global::getKeywordMapping()
///////////////////////////////////////////////////

jasm::KeywordMapping & Global::getKeywordMapping() {
    return jasm::KeywordInitializer::getKeywordMapping();
}

///////////////////////////////////////////////////
// Global::getPPKeywordMapping()
///////////////////////////////////////////////////

jasm::KeywordMapping & Global::getPPKeywordMapping() {
    return jasm::KeywordInitializer::getPPKeywordMapping();
}

///////////////////////////////////////////////////
// Global::getSectionMapping()
///////////////////////////////////////////////////

jasm::KeywordMapping & Global::getSectionMapping() {
    return jasm::KeywordInitializer::getSectionMapping();
}

///////////////////////////////////////////////////
// Global::errorInstance()
///////////////////////////////////////////////////

LastError & Global::lastError() {
    if (Global::last_error == nullptr) {
        Global::last_error = new LastError();
    }
    return *Global::last_error;
}

///////////////////////////////////////////////////
// Global::destroyLastError()
///////////////////////////////////////////////////

void Global::destroyLastError() {
    if (Global::last_error) {
        delete Global::last_error;
        Global::last_error = nullptr;
    }
}
