
#include "jlang/lang/Global.h"
#include "jlang/asm/Keyword.h"

using namespace jlang;

jasm::KeywordMapping * jasm::KeywordInitor::keyword_mapping              = nullptr;
jasm::KeywordMapping * jasm::KeywordInitor::prepocessing_keyword_mapping = nullptr;
LastError *            Global::last_error                                = nullptr;

///////////////////////////////////////////////////
// Global::getKeywordMapping()
///////////////////////////////////////////////////

jasm::KeywordMapping & Global::getKeywordMapping() {
    return jasm::KeywordInitor::getKeywordMapping();
}

///////////////////////////////////////////////////
// Global::getPreprocessingKeywordMapping()
///////////////////////////////////////////////////

jasm::KeywordMapping & Global::getPreprocessingKeywordMapping() {
    return jasm::KeywordInitor::getPreprocessingKeywordMapping();
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
    if (last_error) {
        delete last_error;
        last_error = nullptr;
    }
}

///////////////////////////////////////////////////
// Global::init()
///////////////////////////////////////////////////

void Global::init() {
    jasm::KeywordInitor::init();
}

///////////////////////////////////////////////////
// Global::finalize()
///////////////////////////////////////////////////

void Global::finalize() {
    jasm::KeywordInitor::finalize();
    Global::destroyLastError();
}
