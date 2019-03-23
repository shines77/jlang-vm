
//
// This is a keyword define entries file,
// only allowed to be included in the /asm/Keyword.h or /asm/TokenDef.h file.
// Note: Don't include this file directly.
//

#if !defined(JLANG_ASM_KEYWORD_H) && !defined(JLANG_ASM_TOKEN_H)

    #error "/asm/KeywordDef.h only allowed to be included in the /asm/Keyword.h or /asm/TokenDef.h file."

#else

    #ifndef KEYWORD_DEF
    #define KEYWORD_DEF(token, keywordId, keyword, kind)
    #endif

    #ifndef PREPROCESSING_DEF
    #define PREPROCESSING_DEF(keyword)
    #endif

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

    // Pod type
    KEYWORD_DEF(Void,               void,           void,           Pod)
    KEYWORD_DEF(Bool,               bool,           bool,           Pod)
    KEYWORD_DEF(Char,               char,           char,           Pod)
    KEYWORD_DEF(Short,              short,          short,          Pod)
    KEYWORD_DEF(Int,                int,            int,            Pod)
    KEYWORD_DEF(Long,               long,           long,           Pod)
    KEYWORD_DEF(Float,              float,          float,          Pod)
    KEYWORD_DEF(Double,             double,         double,         Pod)
    KEYWORD_DEF(Signed,             signed,         signed,         PodSign)
    KEYWORD_DEF(Unsigned,           unsigned,       unsigned,       PodSign)

    // Standard pod typedef (Alias)
    KEYWORD_DEF(Int8,               int8,           int8,           Pod)
    KEYWORD_DEF(UInt8,              uint8,          uint8,          Pod)
    KEYWORD_DEF(Int16,              int16,          int16,          Pod)
    KEYWORD_DEF(UInt16,             uint16,         uint16,         Pod)
    KEYWORD_DEF(Int32,              int32,          int32,          Pod)
    KEYWORD_DEF(UInt32,             uint32,         uint32,         Pod)
    KEYWORD_DEF(Int64,              int64,          int64,          Pod)
    KEYWORD_DEF(UInt64,             uint64,         uint64,         Pod)

    // Reserved keywords
    KEYWORD_DEF(Namespace,          namespace,      namespace,      Keyword)
    KEYWORD_DEF(If,                 if,             if,             Keyword)
    KEYWORD_DEF(Else,               else,           else,           Keyword)

    // Section
    KEYWORD_DEF(Align,              align,          .align,         Section)
    KEYWORD_DEF(Strings,            strings,        .strings,       Section)
    KEYWORD_DEF(EntryPoint,         entrypoint,     .entrypoint,    Section)

    // Operators
    KEYWORD_DEF(Add,                add,            add,            Keyword)
    KEYWORD_DEF(Sub,                sub,            sub,            Keyword)
    KEYWORD_DEF(Multiply,           mul,            mul,            Keyword)
    KEYWORD_DEF(Div,                div,            div,            Keyword)
    KEYWORD_DEF(Mod,                mod,            mod,            Keyword)
    KEYWORD_DEF(Assignment,         assign,         assign,         Keyword)

    KEYWORD_DEF(AddEqual,           add_eq,         add_eq,         Keyword)
    KEYWORD_DEF(SubEqual,           sub_eq,         sub_eq,         Keyword)
    KEYWORD_DEF(MultiplyEqual,      mul_eq,         mul_eq,         Keyword)
    KEYWORD_DEF(DivEqual,           div_eq,         div_eq,         Keyword)
    KEYWORD_DEF(ModEqual,           mod_eq,         mod_eq,         Keyword)

    // Bitwise operators
    KEYWORD_DEF(BitNot,             not,            not,            Keyword)
    KEYWORD_DEF(BitAnd,             and,            and,            Keyword)
    KEYWORD_DEF(BitOr,              or,             or,             Keyword)
    KEYWORD_DEF(BitXor,             xor,            xor,            Keyword)
    KEYWORD_DEF(BoolNot,            bool_not,       bool_not,       Keyword)   // (exclamation mark)
    KEYWORD_DEF(BoolAnd,            bool_and,       bool_and,       Keyword)
    KEYWORD_DEF(BoolOr,             bool_or,        bool_or,        Keyword)
    KEYWORD_DEF(BoolXor,            bool_xor,       bool_xor,       Keyword)
    KEYWORD_DEF(BitShiftLeft,       shl,            shl,            Keyword)
    KEYWORD_DEF(BitShiftRight,      shr,            shr,            Keyword)
    KEYWORD_DEF(BitRollShiftLeft,   ror,            ror,            Keyword)
    KEYWORD_DEF(BitRollShiftRight,  rol,            rol,            Keyword)

    KEYWORD_DEF(BitNotEqual,        not_eq,         not_eq,         Keyword)
    KEYWORD_DEF(AndEqual,           and_eq,         and_eq,         Keyword)
    KEYWORD_DEF(OrEqual,            or_eq,          or_eq,          Keyword)
    KEYWORD_DEF(XorEqual,           xor_eq,         xor_eq,         Keyword)

    KEYWORD_DEF(Increase,           inc,            inc,            Keyword)
    KEYWORD_DEF(Decrease,           dec,            dec,            Keyword)

    // Compare operators
    KEYWORD_DEF(Compare,            cmp,            cmp,            LogicFlow)
    KEYWORD_DEF(Test,               test,           test,           LogicFlow)
                 
    // Logic flow
    KEYWORD_DEF(JmpEqual,           je,             je,             LogicFlow)
    KEYWORD_DEF(JmpNotEqual,        jne,            jne,            LogicFlow)

    KEYWORD_DEF(JmpLessThan,        jl,             jl,             LogicFlow)
    KEYWORD_DEF(JmpGreaterThan,     jg,             jg,             LogicFlow)
    KEYWORD_DEF(JmpLessOrEqual,     jle,            jle,            LogicFlow)
    KEYWORD_DEF(JmpGreaterOrEqual,  jge,            jge,            LogicFlow)

    // Operators
    KEYWORD_DEF(Equal,              equal,          =,              Operator)
    KEYWORD_DEF(NotEqual,           not_equal,      !=,             Operator)

    KEYWORD_DEF(Question,           question,       ?,              Operator)
    KEYWORD_DEF(Colon,              colon,          :,              Operator)
    KEYWORD_DEF(Scope,              scope,          ::,             Operator)

    KEYWORD_DEF(Dot,                dot,            .,              Operator)
    KEYWORD_DEF(Ellipsis,           ellipsis,       ...,            Operator)
    KEYWORD_DEF(Pointer,            pointer,        ->,             Operator)

    KEYWORD_DEF(Annotation,         annotation,     @,              Operator)
    
    // Others
    KEYWORD_DEF(Include,            include,        include,        Others)

    #undef KEYWORD_DEF
    #undef PREPROCESSING_DEF

#endif // JLANG_ASM_KEYWORD_H
