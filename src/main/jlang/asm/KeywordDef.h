
//
// This is a keyword define entries file,
// only allowed to be included in the /asm/Keyword.h or /asm/TokenDef.h file.
// Note: Don't include this file directly.
//

#if !defined(JLANG_ASM_KEYWORD_H) && !defined(JLANG_ASM_TOKEN_H)

    #error "/asm/KeywordDef.h only allowed to be included in the /asm/Keyword.h or /asm/TokenDef.h file."

#else

    #ifndef KEYWORD_DEF
    #define KEYWORD_DEF(token_type, keyword, kind)
    #endif

    #ifndef PREPROCESSING_DEF
    #define PREPROCESSING_DEF(keyword)
    #endif

    // Pod type
    KEYWORD_DEF(Void,               void,           PodType)
    KEYWORD_DEF(Bool,               bool,           PodType)
    KEYWORD_DEF(Char,               char,           PodType)
    KEYWORD_DEF(Short,              short,          PodType)
    KEYWORD_DEF(Int,                int,            PodType)
    KEYWORD_DEF(Long,               long,           PodType)
    KEYWORD_DEF(Float,              float,          PodType)
    KEYWORD_DEF(Double,             double,         PodType)
    KEYWORD_DEF(Signed,             signed,         TypeDef)
    KEYWORD_DEF(Unsigned,           unsigned,       TypeDef)

    // Standard pod typedef (Alias)
    KEYWORD_DEF(Int8,               int8,           PodType)
    KEYWORD_DEF(UInt8,              uint8,          PodType)
    KEYWORD_DEF(Int16,              int16,          PodType)
    KEYWORD_DEF(UInt16,             uint16,         PodType)
    KEYWORD_DEF(Int32,              int32,          PodType)
    KEYWORD_DEF(UInt32,             uint32,         PodType)
    KEYWORD_DEF(Int64,              int64,          PodType)
    KEYWORD_DEF(UInt64,             uint64,         PodType)

    // Reserved keywords
    KEYWORD_DEF(Namespace,          namespace,      Keywords)

    // Preprocessing
    PREPROCESSING_DEF(if)
    PREPROCESSING_DEF(ifdef)
    PREPROCESSING_DEF(ifndef)
    PREPROCESSING_DEF(elif)
    PREPROCESSING_DEF(else)
    PREPROCESSING_DEF(endif)
    PREPROCESSING_DEF(define)
    PREPROCESSING_DEF(undef)
    PREPROCESSING_DEF(defined)

    PREPROCESSING_DEF(include)
    PREPROCESSING_DEF(pragma)
    PREPROCESSING_DEF(warning)
    PREPROCESSING_DEF(error)
    PREPROCESSING_DEF(import)
    PREPROCESSING_DEF(assert)
    PREPROCESSING_DEF(unassert)

    // Operators
    KEYWORD_DEF(Add,                add,            Operator)
    KEYWORD_DEF(Sub,                sub,            Operator)
    KEYWORD_DEF(Multiply,           mul,            Operator)
    KEYWORD_DEF(Div,                div,            Operator)
    KEYWORD_DEF(Mod,                mod,            Operator)
    KEYWORD_DEF(Assignment,         assign,         Operator)

    KEYWORD_DEF(AddEqual,           add_eq,         Operator)
    KEYWORD_DEF(SubEqual,           sub_eq,         Operator)
    KEYWORD_DEF(MultiplyEqual,      mul_eq,         Operator)
    KEYWORD_DEF(DivEqual,           div_eq,         Operator)
    KEYWORD_DEF(ModEqual,           mod_eq,         Operator)

    // Bitwise operators
    KEYWORD_DEF(BitNot,             not,            Operator)
    KEYWORD_DEF(BitAnd,             and,            Operator)
    KEYWORD_DEF(BitOr,              or,             Operator)
    KEYWORD_DEF(BitXor,             xor,            Operator)
    KEYWORD_DEF(BoolNot,            bool_not,       Operator)   // (exclamation mark)
    KEYWORD_DEF(BoolAnd,            bool_and,       Operator)
    KEYWORD_DEF(BoolOr,             bool_or,        Operator)
    KEYWORD_DEF(BoolXor,            bool_xor,       Operator)
    KEYWORD_DEF(BitShiftLeft,       shl,            Operator)
    KEYWORD_DEF(BitShiftRight,      shr,            Operator)
    KEYWORD_DEF(BitRollShiftLeft,   ror,            Operator)
    KEYWORD_DEF(BitRollShiftRight,  rol,            Operator)

    KEYWORD_DEF(BitNotEqual,        not_eq,         Operator)
    KEYWORD_DEF(AndEqual,           and_eq,         Operator)
    KEYWORD_DEF(OrEqual,            or_eq,          Operator)
    KEYWORD_DEF(XorEqual,           xor_eq,         Operator)

    KEYWORD_DEF(Increase,           inc,            Operator)
    KEYWORD_DEF(Decrease,           dec,            Operator)

    // Compare operators
    KEYWORD_DEF(Compare,            cmp,            LogicFlow)
    KEYWORD_DEF(Test,               test,           LogicFlow)
                 
    // Logic flow
    KEYWORD_DEF(JmpEqual,           je,             LogicFlow)
    KEYWORD_DEF(JmpNotEqual,        jne,            LogicFlow)

    KEYWORD_DEF(JmpLessThan,        jl,             LogicFlow)
    KEYWORD_DEF(JmpGreaterThan,     jg,             LogicFlow)
    KEYWORD_DEF(JmpLessOrEqual,     jle             LogicFlow)
    KEYWORD_DEF(JmpGreaterOrEqual,  jge,            LogicFlow)

    KEYWORD_DEF(Question,           ?,              Operator)
    KEYWORD_DEF(Colon,              :,              Operator)
    KEYWORD_DEF(Scope,              ::,             Operator)

    KEYWORD_DEF(Dot,                .,              Operator)
    KEYWORD_DEF(Ellipsis,           ...,            Operator)
    KEYWORD_DEF(Pointer,            ->,             Operator)

    KEYWORD_DEF(Annotation,         @,              Operator)
    
    // Others
    KEYWORD_DEF(Include,            include,        Others)

    #undef KEYWORD_DEF
    #undef PREPROCESSING_DEF

#endif // JLANG_ASM_KEYWORD_H
