
//
// This is a token define entries file,
// only allowed to be included in the /asm/Token.h file.
// Note: Don't include this file directly.
//

#ifndef JLANG_ASM_TOKEN_H

    #error "/asm/TokenDef.h only allowed to be included in the /asm/Token.h file."

#else

    #ifndef ASM_TOKEN
    #define ASM_TOKEN(type)
    #endif

    // Special Marks
    ASM_TOKEN(Eof)                      // EOF

    // White spaces and comments
    ASM_TOKEN(WhiteSpace)               // ' ', '\t', '\v', '\f'
    ASM_TOKEN(NewLine)                  // '\r', 'n'
    ASM_TOKEN(LineComment)              // ';' or "//" \n
    ASM_TOKEN(BlockComment)             // "//" \n

    // Atoms
    ASM_TOKEN(Identifier)               // ABC123, XYZ_123
    ASM_TOKEN(IntegerLiteral)           // 1234
    ASM_TOKEN(FloatLiteral)             // 12.3456
    ASM_TOKEN(DoubleLiteral)            // 12.3456f, 12.3456+E10
    ASM_TOKEN(StringLiteral)            // "1234"
    ASM_TOKEN(MultiPartStringLiteral)   // "1234" "5678" "9"
    ASM_TOKEN(HexLiteral)               // 0xFFFF     (16)
    ASM_TOKEN(DecLiteral)               // 0d1234     (10)
    ASM_TOKEN(OcxLiteral)               // 0o713      (8)
    ASM_TOKEN(BinaryLiteral)            // 0b01010101 (2)

    ASM_TOKEN(Error)                    //

    // Keywords
    ASM_TOKEN(Keyword)                  // Reserved keyword
    ASM_TOKEN(Macro)                    // Macro
    ASM_TOKEN(MacroDefine)              // Macro define
    ASM_TOKEN(MacroBody)                // Macro body

    // Math operators
    ASM_TOKEN(Star)                     // *
    ASM_TOKEN(Slash)                    // /
    ASM_TOKEN(AntiSlash)                // '\'
    ASM_TOKEN(Percent)                  // %
    ASM_TOKEN(StarStar)                 // **
    ASM_TOKEN(Ampersand)                // &, [= and]
    ASM_TOKEN(Handle)                   // @
    ASM_TOKEN(Sharp)                    // #
//  ASM_TOKEN(Dot)                      // .

    // Punctuation
    ASM_TOKEN(SingelQuote)              // '
    ASM_TOKEN(DoubleQuote)              // "
    ASM_TOKEN(EqualSign)                // =
    ASM_TOKEN(Comma)                    // ,
    ASM_TOKEN(Semicolon)                // ;
    ASM_TOKEN(LParen)                   // (
    ASM_TOKEN(RParen)                   // )
    ASM_TOKEN(LBracket)                 // [
    ASM_TOKEN(RBracket)                 // ]
    ASM_TOKEN(LBrace)                   // {
    ASM_TOKEN(RBrace)                   // }

    // Statement tokens
    ASM_TOKEN(EndStatement)             // ;
    ASM_TOKEN(Separator)                // ,
    ASM_TOKEN(RoundBracketLeft)         // (
    ASM_TOKEN(RoundBracketRight)        // )
    ASM_TOKEN(SquareBracketLeft)        // [
    ASM_TOKEN(SquareBracketRight)       // ]
    ASM_TOKEN(StatementBlockStart)      // {
    ASM_TOKEN(StatementBlockEnd)        // }

//  ASM_TOKEN(Annotation)               // @

    // Bitwise operators

    // Compare operators

    #include "jlang/asm/KeywordDef.h"

    // Classes
    ASM_TOKEN(Function)                 // function
    ASM_TOKEN(IncludeFileName)          // include file name

    ASM_TOKEN(LabelName)

    // Instruction
    ASM_TOKEN(OpImm)                    // Immediate operand number
    ASM_TOKEN(OpDataType)               // Operand data type

    // Preprocessing
    //

    ASM_TOKEN(UTF8_BOM)                 // UTF8 byte-order-mark, (0xEF, 0xBB, 0xBF)

    #undef ASM_TOKEN

#endif // JLANG_ASM_TOKEN_H
