
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

    // Logic flow
    KEYWORD_DEF(If,                 if,             LogicFlow)
    KEYWORD_DEF(Else,               else,           LogicFlow)
    KEYWORD_DEF(For,                for,            LogicFlow)
    KEYWORD_DEF(ForEach,            for_each,       LogicFlow)
    KEYWORD_DEF(Do,                 do,             LogicFlow)
    KEYWORD_DEF(While,              while,          LogicFlow)
    KEYWORD_DEF(Until,              until,          LogicFlow)
    KEYWORD_DEF(Switch,             switch,         LogicFlow)
    KEYWORD_DEF(Case,               case,           LogicFlow)
    KEYWORD_DEF(Default,            default,        LogicFlow)
    KEYWORD_DEF(Break,              break,          LogicFlow)
    KEYWORD_DEF(Continue,           continue,       LogicFlow)
    KEYWORD_DEF(Goto,               goto,           LogicFlow)

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

    // Type define
    KEYWORD_DEF(TypeDef,            typedef,        TypeDef)
    KEYWORD_DEF(True,               true,           Constant)
    KEYWORD_DEF(False,              false,          Constant)
    KEYWORD_DEF(NullPtr,            nullptr,        Constant)

    // Reserved keywords
    KEYWORD_DEF(Namespace,          namespace,      Keywords)
    KEYWORD_DEF(Using,              using,          Keywords)
    KEYWORD_DEF(Import,             import,         Keywords)
    KEYWORD_DEF(As,                 as,             Keywords)

    // Language
    KEYWORD_DEF(Auto,               auto,           Keywords)

    // Constant
    KEYWORD_DEF(Static,             static,         Classes)
    KEYWORD_DEF(Const,              const,          TypeDef)

    // Classes
    KEYWORD_DEF(Class,              class,          Classes)
    KEYWORD_DEF(Interface,          interface,      Classes)
    KEYWORD_DEF(Enum,               enum,           Classes)
    KEYWORD_DEF(Struct,             struct,         Classes)
    KEYWORD_DEF(Private,            private,        Classes)
    KEYWORD_DEF(Protected,          protected,      Classes)
    KEYWORD_DEF(Public,             public,         Classes)
    KEYWORD_DEF(Virtual,            virtual,        Classes)
    KEYWORD_DEF(Override,           override,       Classes)
    KEYWORD_DEF(Final,              final,          Classes)
    KEYWORD_DEF(Abstract,           abstract,       Classes)
    KEYWORD_DEF(Mutable,            mutable,        Classes)

    KEYWORD_DEF(This,               this,           Classes)
    KEYWORD_DEF(Super,              super,          Classes)
    KEYWORD_DEF(In,                 in,             Classes)

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
    KEYWORD_DEF(Plus,               +,              Operator)
    KEYWORD_DEF(Minus,              -,              Operator)
    KEYWORD_DEF(Multiply,           *,              Operator)
    KEYWORD_DEF(Div,                /,              Operator)
    KEYWORD_DEF(Mod,                %,              Operator)
    KEYWORD_DEF(Assignment,         =,              Operator)

    KEYWORD_DEF(PlusEqual,          +=,             Operator)
    KEYWORD_DEF(MinusEqual,         -=,             Operator)
    KEYWORD_DEF(MultiplyEqual,      *=,             Operator)
    KEYWORD_DEF(DivEqual,           /=,             Operator)
    KEYWORD_DEF(ModEqual,           %=,             Operator)

    // Bitwise operators
    KEYWORD_DEF(BitNot,             ~,              Operator)
    KEYWORD_DEF(BitAnd,             &,              Operator)
    KEYWORD_DEF(BitOr,              |,              Operator)
    KEYWORD_DEF(BitXor,             ^,              Operator)
    KEYWORD_DEF(BoolNot,            !,              Operator)   // (exclamation mark)
    KEYWORD_DEF(BoolAnd,            &&,             Operator)
    KEYWORD_DEF(BoolOr,             ||,             Operator)
    KEYWORD_DEF(BoolXor,            ^^,             Operator)
    KEYWORD_DEF(BitShiftLeft,       <<,             Operator)
    KEYWORD_DEF(BitShiftRight,      >>,             Operator)

    KEYWORD_DEF(BitNotEqual,        ~=,             Operator)
    KEYWORD_DEF(AndEqual,           &=,             Operator)
    KEYWORD_DEF(OrEqual,            |=,             Operator)
    KEYWORD_DEF(XorEqual,           ^=,             Operator)

    KEYWORD_DEF(Increase,           ++,             Operator)
    KEYWORD_DEF(Decrease,           --,             Operator)
                 
    // Compare operators
    KEYWORD_DEF(Equal,              ==,             Operator)
    KEYWORD_DEF(NotEqual,           !=,             Operator)

    KEYWORD_DEF(LessThan,           <,              Operator)
    KEYWORD_DEF(GreaterThan,        >,              Operator)
    KEYWORD_DEF(LessThanOrEqual,    <=,             Operator)
    KEYWORD_DEF(GreaterThanOrEqual, >=,             Operator)

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
