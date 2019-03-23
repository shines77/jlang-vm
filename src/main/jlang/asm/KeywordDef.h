
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
    KEYWORD_DEF(Void,               Void,           void,           Pod)
    KEYWORD_DEF(Bool,               Bool,           bool,           Pod)
    KEYWORD_DEF(Char,               Char,           char,           Pod)
    KEYWORD_DEF(Short,              Short,          short,          Pod)
    KEYWORD_DEF(Int,                Int,            int,            Pod)
    KEYWORD_DEF(Long,               Long,           long,           Pod)
    KEYWORD_DEF(Float,              Float,          float,          Pod)
    KEYWORD_DEF(Double,             Double,         double,         Pod)
    KEYWORD_DEF(Signed,             Signed,         signed,         PodSign)
    KEYWORD_DEF(Unsigned,           Unsigned,       unsigned,       PodSign)

    // Standard pod typedef (Alias)
    KEYWORD_DEF(Int8,               Int8,           int8,           Pod)
    KEYWORD_DEF(UInt8,              UInt8,          uint8,          Pod)
    KEYWORD_DEF(Int16,              Int16,          int16,          Pod)
    KEYWORD_DEF(UInt16,             UInt16,         uint16,         Pod)
    KEYWORD_DEF(Int32,              Int32,          int32,          Pod)
    KEYWORD_DEF(UInt32,             UInt32,         uint32,         Pod)
    KEYWORD_DEF(Int64,              Int64,          int64,          Pod)
    KEYWORD_DEF(UInt64,             UInt64,         uint64,         Pod)

    // Reserved keywords
    KEYWORD_DEF(Namespace,          Namespace,      namespace,      Keyword)
    KEYWORD_DEF(If,                 If,             if,             Keyword)
    KEYWORD_DEF(Else,               Else,           else,           Keyword)

    // Section
    KEYWORD_DEF(Align,              Align,          .align,         Section)
    KEYWORD_DEF(Strings,            Strings,        .strings,       Section)
    KEYWORD_DEF(EntryPoint,         EntryPoint,     .entrypoint,    Section)

    // Operators
    KEYWORD_DEF(Add,                Add,            add,            Keyword)
    KEYWORD_DEF(Sub,                Sub,            sub,            Keyword)
    KEYWORD_DEF(Multiply,           Mul,            mul,            Keyword)
    KEYWORD_DEF(Div,                Div,            div,            Keyword)
    KEYWORD_DEF(Mod,                Mod,            mod,            Keyword)
    KEYWORD_DEF(Assignment,         Assign,         assign,         Keyword)

    KEYWORD_DEF(AddEqual,           AddEqual,       add_eq,         Keyword)
    KEYWORD_DEF(SubEqual,           SubEqual,       sub_eq,         Keyword)
    KEYWORD_DEF(MultiplyEqual,      MultiplyEqual,  mul_eq,         Keyword)
    KEYWORD_DEF(DivEqual,           DivEqual,       div_eq,         Keyword)
    KEYWORD_DEF(ModEqual,           ModEqual,       mod_eq,         Keyword)

    // Bitwise operators
    KEYWORD_DEF(BitNot,             BitNot,         not,            Keyword)
    KEYWORD_DEF(BitAnd,             BitAnd,         and,            Keyword)
    KEYWORD_DEF(BitOr,              BitOr,          or,             Keyword)
    KEYWORD_DEF(BitXor,             BitXor,         xor,            Keyword)
    KEYWORD_DEF(BoolNot,            BoolNot,        bool_not,       Keyword)   // (exclamation mark)
    KEYWORD_DEF(BoolAnd,            BoolAnd,        bool_and,       Keyword)
    KEYWORD_DEF(BoolOr,             BoolOr,         bool_or,        Keyword)
    KEYWORD_DEF(BoolXor,            BoolXor,        bool_xor,       Keyword)
    KEYWORD_DEF(ShiftLeft,          Shl,            shl,            Keyword)
    KEYWORD_DEF(ShiftRight,         Shr,            shr,            Keyword)
    KEYWORD_DEF(RollShiftLeft,      Ror,            ror,            Keyword)
    KEYWORD_DEF(RollShiftRight,     Rol,            rol,            Keyword)

    KEYWORD_DEF(BitNotEqual,        BitNotEqual,    not_eq,         Keyword)
    KEYWORD_DEF(AndEqual,           AndEqual,       and_eq,         Keyword)
    KEYWORD_DEF(OrEqual,            OrEqual,        or_eq,          Keyword)
    KEYWORD_DEF(XorEqual,           XorEqual,       xor_eq,         Keyword)

    KEYWORD_DEF(Increase,           Inc,            inc,            Keyword)
    KEYWORD_DEF(Decrease,           Dec,            dec,            Keyword)

    // Compare operators
    KEYWORD_DEF(Compare,            Cmp,            cmp,            LogicFlow)
    KEYWORD_DEF(Test,               Test,           test,           LogicFlow)
                 
    // Logic flow
    KEYWORD_DEF(JmpEqual,           Je,             je,             LogicFlow)
    KEYWORD_DEF(JmpNotEqual,        Jne,            jne,            LogicFlow)

    KEYWORD_DEF(JmpLessThan,        Jl,             jl,             LogicFlow)
    KEYWORD_DEF(JmpGreaterThan,     Jg,             jg,             LogicFlow)
    KEYWORD_DEF(JmpLessOrEqual,     Jle,            jle,            LogicFlow)
    KEYWORD_DEF(JmpGreaterOrEqual,  Jge,            jge,            LogicFlow)

    // Operators
    KEYWORD_DEF(Equal,              Equal,          =,              Operator)
    KEYWORD_DEF(NotEqual,           NotEqual,       !=,             Operator)

    KEYWORD_DEF(Question,           Question,       ?,              Operator)
    KEYWORD_DEF(Colon,              Colon,          :,              Operator)
    KEYWORD_DEF(Scope,              Scope,          ::,             Operator)

    KEYWORD_DEF(Dot,                Dot,            .,              Operator)
    KEYWORD_DEF(Ellipsis,           Ellipsis,       ...,            Operator)
    KEYWORD_DEF(Pointer,            Pointer,        ->,             Operator)

    KEYWORD_DEF(Annotation,         Annotation,     @,              Operator)
    
    // Others
    KEYWORD_DEF(Include,            Include,        include,        Others)

    #undef KEYWORD_DEF
    #undef PREPROCESSING_DEF

#endif // JLANG_ASM_KEYWORD_H
