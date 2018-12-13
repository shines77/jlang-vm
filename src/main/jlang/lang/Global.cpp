
#include "jlang/lang/Global.h"

using namespace jlang;

LastError * Global::last_error = nullptr;

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
// Global::finalize()
///////////////////////////////////////////////////

void Global::finalize() {
    if (last_error) {
        delete last_error;
        last_error = nullptr;
    }
}
