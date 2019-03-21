
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

struct KeywordKind {
    enum Type {
        Unknown         = 0,

        // Standard
        Default         = 0x00000001,
        Section         = 0x00000002,
        Keyword         = 0x00000004,
        Others          = 0x00000008,

        // POD
        Pod             = 0x00000010,
        PodSign         = 0x00000020,

        // Normal
        Operator        = 0x00000040,
        Constant        = 0x00000080,

        TypeDef         = 0x000000100,
        InnerTypeDef    = 0x000000200,
        LogicFlow       = 0x000000400,
        Classes         = 0x000000800,

        // UserDefine
        UserDefine      = 0x000001000,

        // Precompile
        Preprocessing   = 0x000002000,
        Macro           = 0x000004000,

        // Others
        MaxCategoryType = 0x000008000,

        // Masks
        IsType          = Pod | PodSign | TypeDef | InnerTypeDef | UserDefine,
        IsIdentifier    = Default,
        IsSection       = Section,
        IsKeyword       = Keyword | Classes | LogicFlow,
        IsOperator      = Operator,
        IsPreprocessing = Preprocessing,

        KindMask        = (int)0xFFFFFFFFFUL
    };

    KeywordKind(uint16_t category = Type::Unknown) : category_(category) {
    }

    KeywordKind(uint32_t category) : category_(static_cast<uint32_t>(category)) {
    }

    uint32_t category_;
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_KEYWORDCATEGORY_H
