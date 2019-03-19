
#ifndef JLANG_ASM_INITOR_H
#define JLANG_ASM_INITOR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/NonCopyable.h"
#include "jlang/asm/Keyword.h"
#include "jlang/lang/Global.h"

#include <mutex>

namespace jlang {
namespace jasm {

///////////////////////////////////////////////////
// class Initializer
///////////////////////////////////////////////////

class Initializer : public lang::NonCopyable {
private:
    typedef std::lock_guard<std::mutex> lock_type;

    bool        inited_;
    std::mutex  mutex_;

public:
    Initializer() : inited_(false) {
        init();
    }
    ~Initializer() {
        destroy();
    }

    bool is_inited() const { return inited_; }

    void init() {
        lock_type lock(mutex_);
        if (!inited_) {
            Global::initialize();
            inited_ = true;
        }
    }

    void destroy() {
        lock_type lock(mutex_);
        if (inited_) {
            Global::finalize();
            inited_ = false;
        }
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_INITOR_H
