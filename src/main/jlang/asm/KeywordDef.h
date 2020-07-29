
//
// This is a keyword define entries file,
// only allowed to be included in the /asm/Keyword.h or /asm/TokenDef.h file.
// Note: Don't include this file directly.
//

#if !defined(JLANG_ASM_KEYWORD_H) && !defined(JLANG_ASM_TOKEN_H)

    #error "/asm/KeywordDef.h only allowed to be included in the /asm/Keyword.h or /asm/TokenDef.h file."

#else

    #ifndef ASM_KEYWORD
    #define ASM_KEYWORD(token, keywordId, keyword, kind)
    #endif

    #ifndef ASM_PREPROCESSING
    #define ASM_PREPROCESSING(keyword)
    #endif

    // Preprocessing
    ASM_PREPROCESSING(if)
    ASM_PREPROCESSING(ifdef)
    ASM_PREPROCESSING(ifndef)
    ASM_PREPROCESSING(elif)
    ASM_PREPROCESSING(else)
    ASM_PREPROCESSING(endif)
    ASM_PREPROCESSING(define)
    ASM_PREPROCESSING(undef)
    ASM_PREPROCESSING(defined)

    ASM_PREPROCESSING(include)
    ASM_PREPROCESSING(pragma)
    ASM_PREPROCESSING(warning)
    ASM_PREPROCESSING(error)

    // Pod type
    ASM_KEYWORD(Void,               Void,           void,           Pod)
    ASM_KEYWORD(Bool,               Bool,           bool,           Pod)
    ASM_KEYWORD(Char,               Char,           char,           Pod)
    ASM_KEYWORD(Short,              Short,          short,          Pod)
    ASM_KEYWORD(Int,                Int,            int,            Pod)
    ASM_KEYWORD(Long,               Long,           long,           Pod)
    ASM_KEYWORD(Float,              Float,          float,          Pod)
    ASM_KEYWORD(Double,             Double,         double,         Pod)
    ASM_KEYWORD(Signed,             Signed,         signed,         PodSign)
    ASM_KEYWORD(Unsigned,           Unsigned,       unsigned,       PodSign)

    // Standard pod typedef (Alias)
    ASM_KEYWORD(Int8,               Int8,           int8,           Pod)
    ASM_KEYWORD(UInt8,              UInt8,          uint8,          Pod)
    ASM_KEYWORD(Int16,              Int16,          int16,          Pod)
    ASM_KEYWORD(UInt16,             UInt16,         uint16,         Pod)
    ASM_KEYWORD(Int32,              Int32,          int32,          Pod)
    ASM_KEYWORD(UInt32,             UInt32,         uint32,         Pod)
    ASM_KEYWORD(Int64,              Int64,          int64,          Pod)
    ASM_KEYWORD(UInt64,             UInt64,         uint64,         Pod)

    // Reserved keywords
    
    ASM_KEYWORD(Import,             Import,         import,         Keyword)
    ASM_KEYWORD(Using,              Using,          using,          Keyword)
    ASM_KEYWORD(NameSpace,          NameSpace,      namespace,      Keyword)
    ASM_KEYWORD(TypeDef,            TypeDef,        typedef,        Keyword)
    ASM_KEYWORD(Class,              Class,          class,          Keyword)
    ASM_KEYWORD(Struct,             Struct,         struct,         Keyword)
    ASM_KEYWORD(Interface,          Interface,      interface,      Keyword)
    ASM_KEYWORD(Enum,               Enum,           enum,           Keyword)
    ASM_KEYWORD(Template,           Template,       template,       Keyword)

    ASM_KEYWORD(If,                 If,             if,             Keyword)
    ASM_KEYWORD(Else,               Else,           else,           Keyword)

    ASM_KEYWORD(Do,                 Do,             do,             Keyword)
    ASM_KEYWORD(While,              While,          while,          Keyword)
    ASM_KEYWORD(Continue,           Continue,       continue,       Keyword)

    ASM_KEYWORD(For,                For,            for,            Keyword)

    ASM_KEYWORD(Switch,             Switch,         switch,         Keyword)
    ASM_KEYWORD(Case,               Case,           case,           Keyword)
    ASM_KEYWORD(Default,            Default,        default,        Keyword)

    ASM_KEYWORD(Break,              Break,          break,          Keyword)
    ASM_KEYWORD(Goto,               Goto,           goto,           Keyword)

    // Section
    ASM_KEYWORD(Align,              Align,          .align,         Section)
    ASM_KEYWORD(Strings,            Strings,        .strings,       Section)
    ASM_KEYWORD(EntryPoint,         EntryPoint,     .entrypoint,    Section)

    // Stack about
    ASM_KEYWORD(InstPush,           InstPush,       push,           Instruction)
    ASM_KEYWORD(InstPop,            InstPop,        pop,            Instruction)
    ASM_KEYWORD(InstCall,           InstCall,       call,           Instruction)
    ASM_KEYWORD(InstReturn,         InstReturn,     ret,            Instruction)

    // Load and store
    ASM_KEYWORD(InstMove,           InstMove,       mov,            Instruction)
    ASM_KEYWORD(InstLoad,           InstLoad,       load,           Instruction)
    ASM_KEYWORD(InstStore,          InstStore,      store,          Instruction)

    // Operators
    ASM_KEYWORD(Add,                Add,            +,              Operator)
    ASM_KEYWORD(Sub,                Sub,            -,              Operator)
    ASM_KEYWORD(Multiply,           Multiply,       *,              Operator)
    ASM_KEYWORD(Div,                Div,            /,              Operator)
    ASM_KEYWORD(Mod,                Mod,            %,              Operator)

    ASM_KEYWORD(InstAdd,            InstAdd,        add,            Instruction)
    ASM_KEYWORD(InstSub,            InstSub,        sub,            Instruction)
    ASM_KEYWORD(InstMul,            InstMul,        mul,            Instruction)
    ASM_KEYWORD(InstDiv,            InstDiv,        div,            Instruction)
    ASM_KEYWORD(InstMod,            InstMod,        mod,            Instruction)
    ASM_KEYWORD(Assignment,         Assign,         assign,         Keyword)

    ASM_KEYWORD(AddEqual,           AddEqual,       add_eq,         Keyword)
    ASM_KEYWORD(SubEqual,           SubEqual,       sub_eq,         Keyword)
    ASM_KEYWORD(MultiplyEqual,      MultiplyEqual,  mul_eq,         Keyword)
    ASM_KEYWORD(DivEqual,           DivEqual,       div_eq,         Keyword)
    ASM_KEYWORD(ModEqual,           ModEqual,       mod_eq,         Keyword)

    // Bitwise operators
    ASM_KEYWORD(BitNot,             BitNot,         !,              Operator)
    ASM_KEYWORD(BitAnd,             BitAnd,         &,              Operator)
    ASM_KEYWORD(BitOr,              BitOr,          |,              Operator)
    ASM_KEYWORD(BitXor,             BitXor,         ^,              Operator)

    ASM_KEYWORD(InstNot,            InstNot,        not,            Instruction)
    ASM_KEYWORD(InstAnd,            InstAnd,        and,            Instruction)
    ASM_KEYWORD(InstOr,             InstOr,         or,             Instruction)
    ASM_KEYWORD(InstXor,            InstXor,        xor,            Instruction)

    ASM_KEYWORD(BoolNot,            BoolNot,        bool_not,       Keyword)   // (exclamation mark)
    ASM_KEYWORD(BoolAnd,            BoolAnd,        bool_and,       Keyword)
    ASM_KEYWORD(BoolOr,             BoolOr,         bool_or,        Keyword)
    ASM_KEYWORD(BoolXor,            BoolXor,        bool_xor,       Keyword)

    ASM_KEYWORD(InstShl,            InstShl,        shl,            Instruction)
    ASM_KEYWORD(InstShr,            InstShr,        shr,            Instruction)
    ASM_KEYWORD(InstRor,            InstRor,        ror,            Instruction)
    ASM_KEYWORD(InstRol,            InstRol,        rol,            Instruction)

    ASM_KEYWORD(BitNotEqual,        BitNotEqual,    not_eq,         Keyword)
    ASM_KEYWORD(AndEqual,           AndEqual,       and_eq,         Keyword)
    ASM_KEYWORD(OrEqual,            OrEqual,        or_eq,          Keyword)
    ASM_KEYWORD(XorEqual,           XorEqual,       xor_eq,         Keyword)

    ASM_KEYWORD(Increase,           Increase,       +,              Operator)
    ASM_KEYWORD(Decrease,           Decrease,       -,              Operator)

    ASM_KEYWORD(InstInc,            InstInc,        inc,            Instruction)
    ASM_KEYWORD(InstDec,            InstDec,        dec,            Instruction)

    // Compare operators
    ASM_KEYWORD(InstCmp,            InstCmp,        cmp,            Instruction)
    ASM_KEYWORD(InstTest,           InstTest,       test,           Instruction)
                 
    // Logic flow
    ASM_KEYWORD(InstJe,             InstJe,         je,             Instruction)
    ASM_KEYWORD(InstJne,            InstJne,        jne,            Instruction)

    ASM_KEYWORD(InstJl,             InstJl,         jl,             Instruction)
    ASM_KEYWORD(InstJg,             InstJg,         jg,             Instruction)
    ASM_KEYWORD(InstJle,            InstJle,        jle,            Instruction)
    ASM_KEYWORD(InstJge,            InstJge,        jge,            Instruction)

    // Instruction operands keyword
    ASM_KEYWORD(OpArgs,             OpArgs,         args,           Keyword)
    ASM_KEYWORD(OpVars,             OpVars,         vars,           Keyword)
    ASM_KEYWORD(OpSkip,             OpSkip,         skip,           Keyword)

    ASM_KEYWORD(OpEAX,              OpEAX,          eax,            Keyword)
    ASM_KEYWORD(OpEBX,              OpEBX,          ebx,            Keyword)
    ASM_KEYWORD(OpECX,              OpECX,          ecx,            Keyword)
    ASM_KEYWORD(OpEDX,              OpEDX,          edx,            Keyword)

    // Instruction datatype
    ASM_KEYWORD(OpInt8,             OpInt8,         i1,             Keyword)
    ASM_KEYWORD(OpInt16,            OpInt16,        i2,             Keyword)
    ASM_KEYWORD(OpInt32,            OpInt32,        i4,             Keyword)
    ASM_KEYWORD(OpInt64,            OpInt64,        i8,             Keyword)
    ASM_KEYWORD(OpInt128,           OpInt128,       i16,            Keyword)

    // Operators
    ASM_KEYWORD(Equal,              Equal,          =,              Operator)
    ASM_KEYWORD(NotEqual,           NotEqual,       !=,             Operator)

    ASM_KEYWORD(Question,           Question,       ?,              Operator)
    ASM_KEYWORD(Colon,              Colon,          :,              Operator)
    ASM_KEYWORD(Scope,              Scope,          ::,             Operator)

    ASM_KEYWORD(Dot,                Dot,            .,              Operator)
    ASM_KEYWORD(Ellipsis,           Ellipsis,       ...,            Operator)
    ASM_KEYWORD(Pointer,            Pointer,        ->,             Operator)

    ASM_KEYWORD(Annotation,         Annotation,     @,              Operator)
    
    // Others
    ASM_KEYWORD(Include,            Include,        include,        Others)

    #undef ASM_KEYWORD
    #undef ASM_PREPROCESSING

#endif // JLANG_ASM_KEYWORD_H
