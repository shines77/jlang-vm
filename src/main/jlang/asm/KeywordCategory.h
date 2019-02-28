
#ifndef JLANG_ASM_KEYWORDCATEGORY_H
#define JLANG_ASM_KEYWORDCATEGORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>

namespace jlang {
namespace jasm {

struct KeywordMasks {
    enum {
        DefaultCategory     = 1 << 16,
        UserDefineCategory  = 2 << 16,
        PrecompileCategory  = 4 << 16,
        CategoryMask        = 0x7FFF0000UL,
    };
};

struct KeywordCategory {
    enum Type {
        Unknown = 0,

        // Standard
        Default,
        Section,
        Keywords,
        Others,

        // Normal
        Operator,
        Constant,
        Pod,
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
        MaxCategoryType,
        CategoryMask = 0x0000FFFFUL
    };

    KeywordCategory(uint16_t kind = Type::Unknown) : kind_(kind) {
    }

    KeywordCategory(uint32_t kind) : kind_(static_cast<uint16_t>(kind)) {
    }

    uint16_t kind_;
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_KEYWORDCATEGORY_H
