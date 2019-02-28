
#include "jlang/lang/Global.h"
#include "jlang/asm/Keyword.h"

using namespace jlang;

jasm::KeywordMapping * jasm::KeywordInitor::keyword_mapping    = nullptr;
jasm::KeywordMapping * jasm::KeywordInitor::pp_keyword_mapping = nullptr;
jasm::KeywordMapping * jasm::KeywordInitor::section_mapping    = nullptr;
LastError *            Global::last_error                      = nullptr;

///////////////////////////////////////////////////
// Global::getKeywordMapping()
///////////////////////////////////////////////////

jasm::KeywordMapping & Global::getKeywordMapping() {
    return jasm::KeywordInitor::getKeywordMapping();
}

///////////////////////////////////////////////////
// Global::getPPKeywordMapping()
///////////////////////////////////////////////////

jasm::KeywordMapping & Global::getPPKeywordMapping() {
    return jasm::KeywordInitor::getPPKeywordMapping();
}

///////////////////////////////////////////////////
// Global::getSectionMapping()
///////////////////////////////////////////////////

jasm::KeywordMapping & Global::getSectionMapping() {
    return jasm::KeywordInitor::getSectionMapping();
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
