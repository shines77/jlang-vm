
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
    
    KEYWORD_DEF(Import,             Import,         import,         Keyword)
    KEYWORD_DEF(Using,              Using,          using,          Keyword)
    KEYWORD_DEF(NameSpace,          NameSpace,      namespace,      Keyword)
    KEYWORD_DEF(TypeDef,            TypeDef,        typedef,        Keyword)
    KEYWORD_DEF(Class,              Class,          class,          Keyword)
    KEYWORD_DEF(Struct,             Struct,         struct,         Keyword)
    KEYWORD_DEF(Interface,          Interface,      interface,      Keyword)
    KEYWORD_DEF(Enum,               Enum,           enum,           Keyword)
    KEYWORD_DEF(Template,           Template,       template,       Keyword)

    KEYWORD_DEF(If,                 If,             if,             Keyword)
    KEYWORD_DEF(Else,               Else,           else,           Keyword)

    KEYWORD_DEF(Do,                 Do,             do,             Keyword)
    KEYWORD_DEF(While,              While,          while,          Keyword)
    KEYWORD_DEF(Continue,           Continue,       continue,       Keyword)

    KEYWORD_DEF(For,                For,            for,            Keyword)

    KEYWORD_DEF(Switch,             Switch,         switch,         Keyword)
    KEYWORD_DEF(Case,               Case,           case,           Keyword)
    KEYWORD_DEF(Default,            Default,        default,        Keyword)

    KEYWORD_DEF(Break,              Break,          break,          Keyword)
    KEYWORD_DEF(Goto,               Goto,           goto,           Keyword)

    // Section
    KEYWORD_DEF(Align,              Align,          .align,         Section)
    KEYWORD_DEF(Strings,            Strings,        .strings,       Section)
    KEYWORD_DEF(EntryPoint,         EntryPoint,     .entrypoint,    Section)

    // Stack about
    KEYWORD_DEF(InstPush,           InstPush,       push,           Instruction)
    KEYWORD_DEF(InstPop,            InstPop,        pop,            Instruction)
    KEYWORD_DEF(InstCall,           InstCall,       call,           Instruction)
    KEYWORD_DEF(InstReturn,         InstReturn,     ret,            Instruction)

    // Load and store
    KEYWORD_DEF(InstMove,           InstMove,       mov,            Instruction)
    KEYWORD_DEF(InstLoad,           InstLoad,       load,           Instruction)
    KEYWORD_DEF(InstStore,          InstStore,      store,          Instruction)

    // Operators
    KEYWORD_DEF(Add,                Add,            +,              Operator)
    KEYWORD_DEF(Sub,                Sub,            -,              Operator)
    KEYWORD_DEF(Multiply,           Multiply,       *,              Operator)
    KEYWORD_DEF(Div,                Div,            /,              Operator)
    KEYWORD_DEF(Mod,                Mod,            %,              Operator)

    KEYWORD_DEF(InstAdd,            InstAdd,        add,            Instruction)
    KEYWORD_DEF(InstSub,            InstSub,        sub,            Instruction)
    KEYWORD_DEF(InstMul,            InstMul,        mul,            Instruction)
    KEYWORD_DEF(InstDiv,            InstDiv,        div,            Instruction)
    KEYWORD_DEF(InstMod,            InstMod,        mod,            Instruction)
    KEYWORD_DEF(Assignment,         Assign,         assign,         Keyword)

    KEYWORD_DEF(AddEqual,           AddEqual,       add_eq,         Keyword)
    KEYWORD_DEF(SubEqual,           SubEqual,       sub_eq,         Keyword)
    KEYWORD_DEF(MultiplyEqual,      MultiplyEqual,  mul_eq,         Keyword)
    KEYWORD_DEF(DivEqual,           DivEqual,       div_eq,         Keyword)
    KEYWORD_DEF(ModEqual,           ModEqual,       mod_eq,         Keyword)

    // Bitwise operators
    KEYWORD_DEF(BitNot,             BitNot,         !,              Operator)
    KEYWORD_DEF(BitAnd,             BitAnd,         &,              Operator)
    KEYWORD_DEF(BitOr,              BitOr,          |,              Operator)
    KEYWORD_DEF(BitXor,             BitXor,         ^,              Operator)

    KEYWORD_DEF(InstNot,            InstNot,        not,            Instruction)
    KEYWORD_DEF(InstAnd,            InstAnd,        and,            Instruction)
    KEYWORD_DEF(InstOr,             InstOr,         or,             Instruction)
    KEYWORD_DEF(InstXor,            InstXor,        xor,            Instruction)

    KEYWORD_DEF(BoolNot,            BoolNot,        bool_not,       Keyword)   // (exclamation mark)
    KEYWORD_DEF(BoolAnd,            BoolAnd,        bool_and,       Keyword)
    KEYWORD_DEF(BoolOr,             BoolOr,         bool_or,        Keyword)
    KEYWORD_DEF(BoolXor,            BoolXor,        bool_xor,       Keyword)

    KEYWORD_DEF(InstShl,            InstShl,        shl,            Instruction)
    KEYWORD_DEF(InstShr,            InstShr,        shr,            Instruction)
    KEYWORD_DEF(InstRor,            InstRor,        ror,            Instruction)
    KEYWORD_DEF(InstRol,            InstRol,        rol,            Instruction)

    KEYWORD_DEF(BitNotEqual,        BitNotEqual,    not_eq,         Keyword)
    KEYWORD_DEF(AndEqual,           AndEqual,       and_eq,         Keyword)
    KEYWORD_DEF(OrEqual,            OrEqual,        or_eq,          Keyword)
    KEYWORD_DEF(XorEqual,           XorEqual,       xor_eq,         Keyword)

    KEYWORD_DEF(Increase,           Increase,       +,              Operator)
    KEYWORD_DEF(Decrease,           Decrease,       -,              Operator)

    KEYWORD_DEF(InstInc,            InstInc,        inc,            Instruction)
    KEYWORD_DEF(InstDec,            InstDec,        dec,            Instruction)

    // Compare operators
    KEYWORD_DEF(InstCmp,            InstCmp,        cmp,            Instruction)
    KEYWORD_DEF(InstTest,           InstTest,       test,           Instruction)
                 
    // Logic flow
    KEYWORD_DEF(InstJe,             InstJe,         je,             Instruction)
    KEYWORD_DEF(InstJne,            InstJne,        jne,            Instruction)

    KEYWORD_DEF(InstJl,             InstJl,         jl,             Instruction)
    KEYWORD_DEF(InstJg,             InstJg,         jg,             Instruction)
    KEYWORD_DEF(InstJle,            InstJle,        jle,            Instruction)
    KEYWORD_DEF(InstJge,            InstJge,        jge,            Instruction)

    // Instruction operands keyword
    KEYWORD_DEF(OpArgs,             OpArgs,         args,           Keyword)
    KEYWORD_DEF(OpVars,             OpVars,         vars,           Keyword)
    KEYWORD_DEF(OpSkip,             OpSkip,         skip,           Keyword)

    KEYWORD_DEF(OpEAX,              OpEAX,          eax,            Keyword)
    KEYWORD_DEF(OpEBX,              OpEBX,          ebx,            Keyword)
    KEYWORD_DEF(OpECX,              OpECX,          ecx,            Keyword)
    KEYWORD_DEF(OpEDX,              OpEDX,          edx,            Keyword)

    // Instruction datatype
    KEYWORD_DEF(OpInt8,             OpInt8,         i1,             Keyword)
    KEYWORD_DEF(OpInt16,            OpInt16,        i2,             Keyword)
    KEYWORD_DEF(OpInt32,            OpInt32,        i4,             Keyword)
    KEYWORD_DEF(OpInt64,            OpInt64,        i8,             Keyword)
    KEYWORD_DEF(OpInt128,           OpInt128,       i16,            Keyword)

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
