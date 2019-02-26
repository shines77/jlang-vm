
#ifndef JLANG_ASM_INITOR_H
#define JLANG_ASM_INITOR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/NonCopyable.h"
#include "jlang/asm/Keyword.h"
#include "jlang/lang/Global.h"

namespace jlang {
namespace jasm {

///////////////////////////////////////////////////
// class JLangInitor
///////////////////////////////////////////////////

class Initor : public lang::NonCopyable {
private:
    bool inited_;

public:
    Initor() : inited_(false) {
        init();
    }
    ~Initor() {
        destroy();
    }

    bool is_inited() const { return inited_; }

    void init() {
        if (!inited_) {
            Global::init();
            inited_ = true;
        }
    }

    void destroy() {
        if (inited_) {
            Global::finalize();
            inited_ = false;
        }
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_INITOR_H
