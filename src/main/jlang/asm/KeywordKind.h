
#ifndef JLANG_ASM_KEYWORDCATEGORY_H
#define JLANG_ASM_KEYWORDCATEGORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>

namespace jlang {
namespace jasm {

struct KeywordCategory {
    enum {
        DefaultCategory     = 1 << 16,
        UserDefineCategory  = 2 << 16,
        PrecompileCategory  = 4 << 16,
        CategoryMask        = 0x7FFF0000UL,
    };
};

class KeywordKind {
private:
    uint32_t kind_;

public:
    enum Type {
        Unknown         = 0,

        // Standard
        Default         = 1 << 0,
        Section         = 1 << 1,
        Keyword         = 1 << 2,
        Others          = 1 << 3,

        // POD
        Pod             = 1 << 4,
        PodSign         = 1 << 5,

        // Normal
        Operator        = 1 << 6,
        Constant        = 1 << 7,

        TypeDef         = 1 << 8,
        InnerTypeDef    = 1 << 9,
        LogicFlow       = 1 << 10,
        Classes         = 1 << 11,

        // UserDefine
        UserDefine      = 1 << 12,

        // Precompile
        Preprocessing   = 1 << 13,
        Macro           = 1 << 14,

        // Masks
        IsType          = Pod | PodSign | TypeDef | InnerTypeDef | UserDefine,
        IsIdentifier    = Default,
        IsSection       = Section,
        IsKeyword       = Keyword | Classes | LogicFlow,
        IsOperator      = Operator,
        IsPreprocessing = Preprocessing,

        KindMask        = (int)0xFFFFFFFFFUL
    };

    KeywordKind(uint16_t kind) : kind_(static_cast<uint32_t>(kind)) {
    }
    KeywordKind(uint32_t kind = Type::Unknown) : kind_(kind) {
    }
    KeywordKind(uint64_t kind) : kind_(static_cast<uint32_t>(kind)) {
    }

    uint32_t value() const   { return this->kind_; }
    uint32_t getKind() const { return this->kind_; }
    void setKind(uint32_t kind) { this->kind_ = kind; }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_KEYWORDCATEGORY_H
