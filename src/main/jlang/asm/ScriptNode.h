
#ifndef JLANG_ASM_SCRIPTNODE_H
#define JLANG_ASM_SCRIPTNODE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"
#include "jlang/lang/NonCopyable.h"
#include "jlang/jstd/onetime_ptr.h"

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <memory.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <memory>
#include <functional>

namespace jlang {
namespace jasm {

///////////////////////////////////////////////////
// class ScriptNode
///////////////////////////////////////////////////

class ScriptNode {
private:
    bool inited_;

public:
    ScriptNode() : inited_(false) {
        init();
    }
    virtual ~ScriptNode() {
        destroy();
    }

    bool isInited() const { return inited_; }

    void init() {
        if (!inited_) {
            inited_ = true;
        }
    }

    void destroy() {
        if (inited_) {
            inited_ = false;
        }
    }

    void append(ScriptNode * node) {
        // TODO:
    }
};

typedef jstd::onetime_ptr<ScriptNode> ScriptNodePtr;

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_SCRIPTNODE_H
