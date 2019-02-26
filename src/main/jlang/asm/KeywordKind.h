
#ifndef JLANG_ASM_KEYWORDKIND_H
#define JLANG_ASM_KEYWORDKIND_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>

namespace jlang {
namespace jasm {

struct KeywordCategory {
    enum {
        DefaultCat      = 1 << 16,
        UserDefineCat   = 2 << 16,
        PrecompileCat   = 4 << 16,
        CategoryMasks   = 0x7FFF0000UL,
    };
};

struct KeywordKind {
    enum KindType {
        Unknown = 0,
        // Standard
        Default,
        Keywords,
        Others,
        // Normal
        Operator,
        Constant,
        PodType,
        TypeDef,
        InnerTypeDef,
        LogicFlow,
        Classes,
        // UserDefine
        UserDefine,
        // Precompile
        Preprocessing,
        Macro,
        // Others
        MaxKindType,
        TypeMasks = 0x0000FFFFUL
    };

    KeywordKind(uint16_t kind = 0) : kind_(kind) {
    }

    KeywordKind(uint32_t kind) : kind_(kind) {
    }

    uint16_t kind_;
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_KEYWORDKIND_H
