
//
// This is a token define entries file,
// only allowed to be included in the /asm/Token.h file.
// Note: Don't include this file directly.
//

#ifndef JLANG_ASM_TOKEN_H

    #error "/asm/TokenDef.h only allowed to be included in the /asm/Token.h file."

#else

    #ifndef TOKEN_DEF
    #define TOKEN_DEF(token_type)
    #endif

    // Special Marks
    TOKEN_DEF(EndOfFile)                // EOF

    // White spaces and comments
    TOKEN_DEF(WhiteSpace)               // ' ', '\t', '\v', '\f'
    TOKEN_DEF(NewLine)                  // '\r', 'n'
    TOKEN_DEF(LineComment)              // "//" \n
    TOKEN_DEF(BlockComment)             // "/* xxxxx */"
    TOKEN_DEF(NestedBlockComment)       // "/{ xxxxxx }/"

    // Atoms
    TOKEN_DEF(Identifier)               // ABC123, XYZ_123
    TOKEN_DEF(IntegerLiteral)           // 1234
    TOKEN_DEF(FloatLiteral)             // 12.3456
    TOKEN_DEF(DoubleLiteral)            // 12.3456f, 12.3456+E10
    TOKEN_DEF(StringLiteral)            // "1234"
    TOKEN_DEF(MultiPartStringLiteral)   // "1234" "5678" "9"
    TOKEN_DEF(HexLiteral)               // 0xFFFF     (16)
    TOKEN_DEF(DecLiteral)               // 0d1234     (10)
    TOKEN_DEF(OcxLiteral)               // 0o713      (8)
    TOKEN_DEF(BinaryLiteral)            // 0b01010101 (2)

    TOKEN_DEF(IllegalFloatNumber)       //
    TOKEN_DEF(IllegalNumberOverflow)    //
    TOKEN_DEF(Error)                    //

    // Keywords
    TOKEN_DEF(InternalKeyword)          // for reserved keywords
    TOKEN_DEF(Macro)                    // Macro
    TOKEN_DEF(MacroDefine)              // Macro define
    TOKEN_DEF(MacroBody)                // Macro body

    // Math operators
    //TOKEN_DEF(Annotation)             // @

    TOKEN_DEF(Star)                     // *
    TOKEN_DEF(Slash)                    // /
    TOKEN_DEF(AntiSlash)                // '\'
    TOKEN_DEF(Percent)                  // %
    TOKEN_DEF(StarStar)                 // **
    TOKEN_DEF(Ampersand)                // &, [= and]
    TOKEN_DEF(Handle)                   // @
    TOKEN_DEF(Sharp)                    // #

    // Punctuation
    TOKEN_DEF(SQoute)                   // '
    TOKEN_DEF(EqualSign)                // =
    TOKEN_DEF(Comma)                    // ,
    TOKEN_DEF(Semi)                     // ;
    TOKEN_DEF(LParen)                   // (
    TOKEN_DEF(RParen)                   // )
    TOKEN_DEF(LBracket)                 // [
    TOKEN_DEF(RBracket)                 // ]
    TOKEN_DEF(LBrace)                   // {
    TOKEN_DEF(RBrace)                   // }

    // Statement tokens
    TOKEN_DEF(EndStatement)             // ;
    TOKEN_DEF(Separator)                // ,
    TOKEN_DEF(RoundBracketLeft)         // (
    TOKEN_DEF(RoundBracketRight)        // )
    TOKEN_DEF(SquareBracketLeft)        // [
    TOKEN_DEF(SquareBracketRight)       // ]
    TOKEN_DEF(StatementBlockStart)      // {
    TOKEN_DEF(StatementBlockEnd)        // }

    // Bitwise operators

    // Compare operators

    #include "jlang/asm/KeywordDef.h"

    // Classes
    TOKEN_DEF(Function)                 // function
    TOKEN_DEF(IncludeFileName)          // include file name    

    // Preprocessing
    //

    TOKEN_DEF(Utf8_BOM)                 // UTF8 byte-order-mark, (0xEF, 0xBB, 0xBF)

    #undef TOKEN_DEF

#endif // JLANG_ASM_TOKEN_H
