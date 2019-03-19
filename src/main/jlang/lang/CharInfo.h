
#ifndef JLANG_LANG_CHARINFO_H
#define JLANG_LANG_CHARINFO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"

#include <stdint.h>
#include <assert.h>

namespace jlang {

///////////////////////////////////////////////////
// namespace CharInfo
///////////////////////////////////////////////////

namespace CharInfo {
    // Use in mask[256]
    enum {
        kNull               = 0x0001,   // '\0'
        kWhiteSpace         = 0x0002,   // ' ', \t, \v, \f
        kNewLine            = 0x0004,   // \r, \n
        kAlphabet           = 0x0008,   // [A-Z] and [a-z]
        kUnderLine          = 0x0010,   // [_]
        kDigital            = 0x0020,   // [0-9]
        kHexChar            = 0x0040,   // [0-9] and ([a-f] or [A-F])
        kIncludeQuote       = 0x0080,   // " >
        kOperator           = 0,        // Include: ! % & * + - : < = > ? ^ | ~
        
        kWhiteSpaces        = kWhiteSpace | kNewLine,           // ' ', \t, \v, \f and \r, \n
        kIdentifierFirst    = kAlphabet | kUnderLine,           // [A-Z], [a-z], and [_]
        kIdentifierBody     = kIdentifierFirst | kDigital,      // [A-Z], [a-z], [0-9] and [_]
        kIdentifier         = kIdentifierFirst | kDigital,      // [A-Z], [a-z], [0-9] and [_]

        kIncludeEndOf       = kNull | kNewLine | kIncludeQuote, // '\0', '\n', '\r', ", >

        kMaskLast
    };

    static const uint8_t mask[256] = {
        /* 0x00 */
            /* '\0'  00 */ kNull,               // Null
            /* '\1'  01 */ 0,
            /* '\2'  02 */ 0,
            /* '\3'  03 */ 0,
            /* '\4'  04 */ 0,
            /* '\5'  05 */ 0,
            /* '\6'  06 */ 0,
            /* '\a'  07 */ 0,                   // Bell
            /* '\b'  08 */ 0,                   // Backspace
            /* '\t'  09 */ 0 | kWhiteSpace,     // Tab
            /* '\r'  0a */ 0 | kNewLine,        // LF
            /* '\v'  0b */ 0 | kWhiteSpace,     // VT
            /* '\f'  0c */ 0 | kWhiteSpace,     // FF
            /* '\n'  0d */ 0 | kNewLine,        // CR
            /* '\14' 0e */ 0,
            /* '\15' 0f */ 0,
        /* 0x10 */
            /* '\16' 10 */ 0,
            /* '\17' 11 */ 0,
            /* '\18' 12 */ 0,
            /* '\19' 13 */ 0,
            /* '\20' 14 */ 0,
            /* '\21' 15 */ 0,
            /* '\22' 16 */ 0,
            /* '\23' 17 */ 0,
            /* '\24' 18 */ 0,
            /* '\25' 19 */ 0,
            /* '\26' 1a */ 0,
            /* '\e'  1b */ 0,                   // Escape
            /* '\28' 1c */ 0,
            /* '\29' 1d */ 0,
            /* '\30' 1e */ 0,
            /* '\31' 1f */ 0,
        /* 0x20 */
            /* ' ' 20 */ kWhiteSpace,
            /* '!' 21 */ kOperator,
            /* '"' 22 */ 0,
            /* '#' 23 */ 0,
            /* '$' 24 */ 0,
            /* '%' 25 */ kOperator,
            /* '&' 26 */ kOperator,
            /* ''' 27 */ 0,
            /* '(' 28 */ 0,
            /* ')' 29 */ 0,
            /* '*' 2A */ kOperator,
            /* '+' 2B */ kOperator,
            /* ',' 2C */ 0,
            /* '-' 2D */ kOperator,
            /* '.' 2E */ kOperator,
            /* '/' 2F */ 0,
        /* 0x30 */  
            /* '0' 30 */ kDigital | kHexChar,
            /* '1' 31 */ kDigital | kHexChar,
            /* '2' 32 */ kDigital | kHexChar,
            /* '3' 33 */ kDigital | kHexChar,
            /* '4' 34 */ kDigital | kHexChar,
            /* '5' 35 */ kDigital | kHexChar,
            /* '6' 36 */ kDigital | kHexChar,
            /* '7' 37 */ kDigital | kHexChar,
            /* '8' 38 */ kDigital | kHexChar,
            /* '9' 39 */ kDigital | kHexChar,
            /* ':' 3A */ 0,
            /* ';' 3B */ 0,
            /* '<' 3C */ kOperator,
            /* '=' 3D */ kOperator,
            /* '>' 3E */ kOperator,
            /* '?' 3F */ kOperator,
        /* 40 */
            /* '@' 40 */ 0,
            /* 'A' 41 */ kAlphabet | kHexChar,
            /* 'B' 42 */ kAlphabet | kHexChar,
            /* 'C' 43 */ kAlphabet | kHexChar,
            /* 'D' 44 */ kAlphabet | kHexChar,
            /* 'E' 45 */ kAlphabet | kHexChar,
            /* 'F' 46 */ kAlphabet | kHexChar,
            /* 'G' 47 */ kAlphabet,
            /* 'H' 48 */ kAlphabet,
            /* 'I' 49 */ kAlphabet,
            /* 'J' 4A */ kAlphabet,
            /* 'K' 4B */ kAlphabet,
            /* 'L' 4C */ kAlphabet,
            /* 'M' 4D */ kAlphabet,
            /* 'N' 4E */ kAlphabet,
            /* 'O' 4F */ kAlphabet,            
        /* 50 */
            /* 'P' 50 */ kAlphabet,
            /* 'Q' 51 */ kAlphabet,
            /* 'R' 52 */ kAlphabet,
            /* 'S' 53 */ kAlphabet,
            /* 'T' 54 */ kAlphabet,
            /* 'U' 55 */ kAlphabet,
            /* 'V' 56 */ kAlphabet,
            /* 'W' 57 */ kAlphabet,
            /* 'X' 58 */ kAlphabet,
            /* 'Y' 59 */ kAlphabet,
            /* 'Z' 5A */ kAlphabet,
            /* '[' 5B */ 0,
            /* '\' 5C */ kOperator,
            /* ']' 5D */ 0,
            /* '^' 5E */ kOperator,
            /* '_' 5F */ kUnderLine,
        /* 60 */
            /* '`' 60 */ 0,
            /* 'a' 61 */ kAlphabet | kHexChar,
            /* 'b' 62 */ kAlphabet | kHexChar,
            /* 'c' 63 */ kAlphabet | kHexChar,
            /* 'd' 64 */ kAlphabet | kHexChar,
            /* 'e' 65 */ kAlphabet | kHexChar,
            /* 'f' 66 */ kAlphabet | kHexChar,
            /* 'g' 67 */ kAlphabet,
            /* 'h' 68 */ kAlphabet,
            /* 'i' 69 */ kAlphabet,
            /* 'j' 6A */ kAlphabet,
            /* 'k' 6B */ kAlphabet,
            /* 'l' 6C */ kAlphabet,
            /* 'm' 6D */ kAlphabet,
            /* 'n' 6E */ kAlphabet,
            /* 'o' 6F */ kAlphabet,
        /* 70 */
            /* 'p' 70 */ kAlphabet,
            /* 'q' 71 */ kAlphabet,
            /* 'r' 72 */ kAlphabet,
            /* 's' 73 */ kAlphabet,
            /* 't' 74 */ kAlphabet,
            /* 'u' 75 */ kAlphabet,
            /* 'v' 76 */ kAlphabet,
            /* 'w' 77 */ kAlphabet,
            /* 'x' 78 */ kAlphabet,
            /* 'y' 79 */ kAlphabet,
            /* 'z' 7A */ kAlphabet,
            /* '{' 7B */ 0,
            /* '|' 7C */ kOperator,
            /* '}' 7D */ 0,
            /* '~' 7E */ kOperator,
            /*  ?  7F */ 0,                     // Delete

        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        /* 0x80 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0x90 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xA0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xB0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xC0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xD0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xE0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xF0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }; // mask[256]

    enum {
        kOctEscapeChar  = 254,
        kNonAscii       = 255,
        kAdvancedEscape = kOctEscapeChar
    };

    static const uint8_t escape_chars[256] = {
        /* 0x00 */
            /* '\0'  00 */ '0',             // Null
            /* '\1'  01 */ '1',
            /* '\2'  02 */ '2',
            /* '\3'  03 */ '3',
            /* '\4'  04 */ '4',
            /* '\5'  05 */ '5',
            /* '\6'  06 */ '6',
            /* '\a'  07 */ 'a',             // Bell
            /* '\b'  08 */ 'b',             // Backspace
            /* '\t'  09 */ 't',             // Tab
            /* '\r'  0a */ 'r',             // LF
            /* '\v'  0b */ 'v',             // VT
            /* '\f'  0c */ 'f',             // FF
            /* '\n'  0d */ 'n',             // CR
            /* '\14' 0e */ kOctEscapeChar,
            /* '\15' 0f */ kOctEscapeChar,
        /* 0x10 */
            /* '\16' 10 */ kOctEscapeChar,
            /* '\17' 11 */ kOctEscapeChar,
            /* '\18' 12 */ kOctEscapeChar,
            /* '\19' 13 */ kOctEscapeChar,
            /* '\20' 14 */ kOctEscapeChar,
            /* '\21' 15 */ kOctEscapeChar,
            /* '\22' 16 */ kOctEscapeChar,
            /* '\23' 17 */ kOctEscapeChar,
            /* '\24' 18 */ kOctEscapeChar,
            /* '\25' 19 */ kOctEscapeChar,
            /* '\26' 1a */ kOctEscapeChar,
            /* '\27' 1b */ kOctEscapeChar,  // Escape
            /* '\28' 1c */ kOctEscapeChar,
            /* '\29' 1d */ kOctEscapeChar,
            /* '\30' 1e */ kOctEscapeChar,
            /* '\31' 1f */ kOctEscapeChar,
        /* 0x20 */
            /* ' ' 20 */ 0,
            /* '!' 21 */ 0,
            /* '"' 22 */ '\"',
            /* '#' 23 */ 0,
            /* '$' 24 */ 0,
            /* '%' 25 */ 0,
            /* '&' 26 */ 0,
            /* ''' 27 */ '\'',
            /* '(' 28 */ 0,
            /* ')' 29 */ 0,
            /* '*' 2A */ 0,
            /* '+' 2B */ 0,
            /* ',' 2C */ 0,
            /* '-' 2D */ 0,
            /* '.' 2E */ 0,
            /* '/' 2F */ 0,
        /* 0x30 */  
            /* '0' 30 */ 0,
            /* '1' 31 */ 0,
            /* '2' 32 */ 0,
            /* '3' 33 */ 0,
            /* '4' 34 */ 0,
            /* '5' 35 */ 0,
            /* '6' 36 */ 0,
            /* '7' 37 */ 0,
            /* '8' 38 */ 0,
            /* '9' 39 */ 0,
            /* ':' 3A */ 0,
            /* ';' 3B */ 0,
            /* '<' 3C */ 0,
            /* '=' 3D */ 0,
            /* '>' 3E */ 0,
            /* '?' 3F */ 0,
        /* 40 */
            /* '@' 40 */ 0,
            /* 'A' 41 */ 0,
            /* 'B' 42 */ 0,
            /* 'C' 43 */ 0,
            /* 'D' 44 */ 0,
            /* 'E' 45 */ 0,
            /* 'F' 46 */ 0,
            /* 'G' 47 */ 0,
            /* 'H' 48 */ 0,
            /* 'I' 49 */ 0,
            /* 'J' 4A */ 0,
            /* 'K' 4B */ 0,
            /* 'L' 4C */ 0,
            /* 'M' 4D */ 0,
            /* 'N' 4E */ 0,
            /* 'O' 4F */ 0,            
        /* 50 */
            /* 'P' 50 */ 0,
            /* 'Q' 51 */ 0,
            /* 'R' 52 */ 0,
            /* 'S' 53 */ 0,
            /* 'T' 54 */ 0,
            /* 'U' 55 */ 0,
            /* 'V' 56 */ 0,
            /* 'W' 57 */ 0,
            /* 'X' 58 */ 0,
            /* 'Y' 59 */ 0,
            /* 'Z' 5A */ 0,
            /* '[' 5B */ 0,
            /* '\' 5C */ '\\',
            /* ']' 5D */ 0,
            /* '^' 5E */ 0,
            /* '_' 5F */ 0,
        /* 60 */
            /* '`' 60 */ 0,
            /* 'a' 61 */ 0,
            /* 'b' 62 */ 0,
            /* 'c' 63 */ 0,
            /* 'd' 64 */ 0,
            /* 'e' 65 */ 0,
            /* 'f' 66 */ 0,
            /* 'g' 67 */ 0,
            /* 'h' 68 */ 0,
            /* 'i' 69 */ 0,
            /* 'j' 6A */ 0,
            /* 'k' 6B */ 0,
            /* 'l' 6C */ 0,
            /* 'm' 6D */ 0,
            /* 'n' 6E */ 0,
            /* 'o' 6F */ 0,
        /* 70 */
            /* 'p' 70 */ 0,
            /* 'q' 71 */ 0,
            /* 'r' 72 */ 0,
            /* 's' 73 */ 0,
            /* 't' 74 */ 0,
            /* 'u' 75 */ 0,
            /* 'v' 76 */ 0,
            /* 'w' 77 */ 0,
            /* 'x' 78 */ 0,
            /* 'y' 79 */ 0,
            /* 'z' 7A */ 0,
            /* '{' 7B */ 0,
            /* '|' 7C */ 0,
            /* '}' 7D */ 0,
            /* '~' 7E */ 0,
            /*  ?  7F */ kNonAscii,         // Delete

        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        /* 0x80 */ kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
        /* 0x90 */ kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
        /* 0xA0 */ kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
        /* 0xB0 */ kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
        /* 0xC0 */ kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
        /* 0xD0 */ kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
        /* 0xE0 */ kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
        /* 0xF0 */ kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii,
                   kNonAscii, kNonAscii, kNonAscii, kNonAscii
    }; // escape_chars[256]

    enum {
        kUnescapeError = 255,
        kOctUnescapeChar = 253,
        kHexUnescapeChar = 254,
        kAdvancedUnescape = kOctUnescapeChar
    };

    static const uint8_t unescape_chars[256] = {
        /* 0x00 */
            /* '\0'  00 */ kUnescapeError,          // Null
            /* '\1'  01 */ kUnescapeError,
            /* '\2'  02 */ kUnescapeError,
            /* '\3'  03 */ kUnescapeError,
            /* '\4'  04 */ kUnescapeError,
            /* '\5'  05 */ kUnescapeError,
            /* '\6'  06 */ kUnescapeError,
            /* '\a'  07 */ kUnescapeError,          // Bell
            /* '\b'  08 */ kUnescapeError,          // Backspace
            /* '\t'  09 */ kUnescapeError,          // Tab
            /* '\r'  0a */ kUnescapeError,          // LF
            /* '\v'  0b */ kUnescapeError,          // VT
            /* '\f'  0c */ kUnescapeError,          // FF
            /* '\n'  0d */ kUnescapeError,          // CR
            /* '\14' 0e */ kUnescapeError,
            /* '\15' 0f */ kUnescapeError,
        /* 0x10 */
            /* '\16' 10 */ kUnescapeError,
            /* '\17' 11 */ kUnescapeError,
            /* '\18' 12 */ kUnescapeError,
            /* '\19' 13 */ kUnescapeError,
            /* '\20' 14 */ kUnescapeError,
            /* '\21' 15 */ kUnescapeError,
            /* '\22' 16 */ kUnescapeError,
            /* '\23' 17 */ kUnescapeError,
            /* '\24' 18 */ kUnescapeError,
            /* '\25' 19 */ kUnescapeError,
            /* '\26' 1a */ kUnescapeError,
            /* '\e'  1b */ kUnescapeError,          // Escape
            /* '\28' 1c */ kUnescapeError,
            /* '\29' 1d */ kUnescapeError,
            /* '\30' 1e */ kUnescapeError,
            /* '\31' 1f */ kUnescapeError,
        /* 0x20 */
            /* ' ' 20 */ kUnescapeError,
            /* '!' 21 */ kUnescapeError,
            /* '"' 22 */ '\"',
            /* '#' 23 */ kUnescapeError,
            /* '$' 24 */ kUnescapeError,
            /* '%' 25 */ kUnescapeError,
            /* '&' 26 */ kUnescapeError,
            /* ''' 27 */ '\'',
            /* '(' 28 */ kUnescapeError,
            /* ')' 29 */ kUnescapeError,
            /* '*' 2A */ kUnescapeError,
            /* '+' 2B */ kUnescapeError,
            /* ',' 2C */ kUnescapeError,
            /* '-' 2D */ kUnescapeError,
            /* '.' 2E */ kUnescapeError,
            /* '/' 2F */ kUnescapeError,
        /* 0x30 */  
            /* '0' 30 */ kOctUnescapeChar,
            /* '1' 31 */ kOctUnescapeChar,
            /* '2' 32 */ kOctUnescapeChar,
            /* '3' 33 */ kOctUnescapeChar,
            /* '4' 34 */ kOctUnescapeChar,
            /* '5' 35 */ kOctUnescapeChar,
            /* '6' 36 */ kOctUnescapeChar,
            /* '7' 37 */ kOctUnescapeChar,
            /* '8' 38 */ kUnescapeError,
            /* '9' 39 */ kUnescapeError,
            /* ':' 3A */ kUnescapeError,
            /* ';' 3B */ kUnescapeError,
            /* '<' 3C */ kUnescapeError,
            /* '=' 3D */ kUnescapeError,
            /* '>' 3E */ kUnescapeError,
            /* '?' 3F */ '\?',
        /* 40 */
            /* '@' 40 */ kUnescapeError,
            /* 'A' 41 */ kUnescapeError,
            /* 'B' 42 */ kUnescapeError,
            /* 'C' 43 */ kUnescapeError,
            /* 'D' 44 */ kUnescapeError,
            /* 'E' 45 */ kUnescapeError,
            /* 'F' 46 */ kUnescapeError,
            /* 'G' 47 */ kUnescapeError,
            /* 'H' 48 */ kUnescapeError,
            /* 'I' 49 */ kUnescapeError,
            /* 'J' 4A */ kUnescapeError,
            /* 'K' 4B */ kUnescapeError,
            /* 'L' 4C */ kUnescapeError,
            /* 'M' 4D */ kUnescapeError,
            /* 'N' 4E */ kUnescapeError,
            /* 'O' 4F */ kUnescapeError,            
        /* 50 */
            /* 'P' 50 */ kUnescapeError,
            /* 'Q' 51 */ kUnescapeError,
            /* 'R' 52 */ kUnescapeError,
            /* 'S' 53 */ kUnescapeError,
            /* 'T' 54 */ kUnescapeError,
            /* 'U' 55 */ kUnescapeError,
            /* 'V' 56 */ kUnescapeError,
            /* 'W' 57 */ kUnescapeError,
            /* 'X' 58 */ kUnescapeError,
            /* 'Y' 59 */ kUnescapeError,
            /* 'Z' 5A */ kUnescapeError,
            /* '[' 5B */ kUnescapeError,
            /* '\' 5C */ '\\',
            /* ']' 5D */ kUnescapeError,
            /* '^' 5E */ kUnescapeError,
            /* '_' 5F */ kUnescapeError,
        /* 60 */
            /* '`' 60 */ kUnescapeError,
            /* 'a' 61 */ '\a',
            /* 'b' 62 */ '\b',
            /* 'c' 63 */ kUnescapeError,
            /* 'd' 64 */ kUnescapeError,
            /* 'e' 65 */ '\33',
            /* 'f' 66 */ '\f',
            /* 'g' 67 */ kUnescapeError,
            /* 'h' 68 */ kUnescapeError,
            /* 'i' 69 */ kUnescapeError,
            /* 'j' 6A */ kUnescapeError,
            /* 'k' 6B */ kUnescapeError,
            /* 'l' 6C */ kUnescapeError,
            /* 'm' 6D */ kUnescapeError,
            /* 'n' 6E */ '\n',
            /* 'o' 6F */ kUnescapeError,
        /* 70 */
            /* 'p' 70 */ kUnescapeError,
            /* 'q' 71 */ kUnescapeError,
            /* 'r' 72 */ '\r',
            /* 's' 73 */ kUnescapeError,
            /* 't' 74 */ '\t',
            /* 'u' 75 */ kUnescapeError,
            /* 'v' 76 */ '\v',
            /* 'w' 77 */ kUnescapeError,
            /* 'x' 78 */ kHexUnescapeChar,
            /* 'y' 79 */ kUnescapeError,
            /* 'z' 7A */ kUnescapeError,
            /* '{' 7B */ kUnescapeError,
            /* '|' 7C */ kUnescapeError,
            /* '}' 7D */ kUnescapeError,
            /* '~' 7E */ kUnescapeError,
            /*  ?  7F */ kUnescapeError,            // Delete

        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        /* 0x80 */ kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
        /* 0x90 */ kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
        /* 0xA0 */ kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
        /* 0xB0 */ kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
        /* 0xC0 */ kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
        /* 0xD0 */ kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
        /* 0xE0 */ kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
        /* 0xF0 */ kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError,
                   kUnescapeError, kUnescapeError, kUnescapeError, kUnescapeError
    }; // unescape_chars[256]

    enum {
        kInvalidHex = 255
    };

    static const uint8_t hex_value[256] = {
        /* 0x00 */
            /* '\0'  00 */ kInvalidHex,          // Null
            /* '\1'  01 */ kInvalidHex,
            /* '\2'  02 */ kInvalidHex,
            /* '\3'  03 */ kInvalidHex,
            /* '\4'  04 */ kInvalidHex,
            /* '\5'  05 */ kInvalidHex,
            /* '\6'  06 */ kInvalidHex,
            /* '\a'  07 */ kInvalidHex,          // Bell
            /* '\b'  08 */ kInvalidHex,          // Backspace
            /* '\t'  09 */ kInvalidHex,          // Tab
            /* '\r'  0a */ kInvalidHex,          // LF
            /* '\v'  0b */ kInvalidHex,          // VT
            /* '\f'  0c */ kInvalidHex,          // FF
            /* '\n'  0d */ kInvalidHex,          // CR
            /* '\14' 0e */ kInvalidHex,
            /* '\15' 0f */ kInvalidHex,
        /* 0x10 */
            /* '\16' 10 */ kInvalidHex,
            /* '\17' 11 */ kInvalidHex,
            /* '\18' 12 */ kInvalidHex,
            /* '\19' 13 */ kInvalidHex,
            /* '\20' 14 */ kInvalidHex,
            /* '\21' 15 */ kInvalidHex,
            /* '\22' 16 */ kInvalidHex,
            /* '\23' 17 */ kInvalidHex,
            /* '\24' 18 */ kInvalidHex,
            /* '\25' 19 */ kInvalidHex,
            /* '\26' 1a */ kInvalidHex,
            /* '\e'  1b */ kInvalidHex,          // Escape
            /* '\28' 1c */ kInvalidHex,
            /* '\29' 1d */ kInvalidHex,
            /* '\30' 1e */ kInvalidHex,
            /* '\31' 1f */ kInvalidHex,
        /* 0x20 */
            /* ' ' 20 */ kInvalidHex,
            /* '!' 21 */ kInvalidHex,
            /* '"' 22 */ kInvalidHex,
            /* '#' 23 */ kInvalidHex,
            /* '$' 24 */ kInvalidHex,
            /* '%' 25 */ kInvalidHex,
            /* '&' 26 */ kInvalidHex,
            /* ''' 27 */ kInvalidHex,
            /* '(' 28 */ kInvalidHex,
            /* ')' 29 */ kInvalidHex,
            /* '*' 2A */ kInvalidHex,
            /* '+' 2B */ kInvalidHex,
            /* ',' 2C */ kInvalidHex,
            /* '-' 2D */ kInvalidHex,
            /* '.' 2E */ kInvalidHex,
            /* '/' 2F */ kInvalidHex,
        /* 0x30 */  
            /* '0' 30 */ 0,
            /* '1' 31 */ 1,
            /* '2' 32 */ 2,
            /* '3' 33 */ 3,
            /* '4' 34 */ 4,
            /* '5' 35 */ 5,
            /* '6' 36 */ 6,
            /* '7' 37 */ 7,
            /* '8' 38 */ 8,
            /* '9' 39 */ 9,
            /* ':' 3A */ kInvalidHex,
            /* ';' 3B */ kInvalidHex,
            /* '<' 3C */ kInvalidHex,
            /* '=' 3D */ kInvalidHex,
            /* '>' 3E */ kInvalidHex,
            /* '?' 3F */ kInvalidHex,
        /* 40 */
            /* '@' 40 */ kInvalidHex,
            /* 'A' 41 */ 10,
            /* 'B' 42 */ 11,
            /* 'C' 43 */ 12,
            /* 'D' 44 */ 13,
            /* 'E' 45 */ 14,
            /* 'F' 46 */ 15,
            /* 'G' 47 */ kInvalidHex,
            /* 'H' 48 */ kInvalidHex,
            /* 'I' 49 */ kInvalidHex,
            /* 'J' 4A */ kInvalidHex,
            /* 'K' 4B */ kInvalidHex,
            /* 'L' 4C */ kInvalidHex,
            /* 'M' 4D */ kInvalidHex,
            /* 'N' 4E */ kInvalidHex,
            /* 'O' 4F */ kInvalidHex,            
        /* 50 */
            /* 'P' 50 */ kInvalidHex,
            /* 'Q' 51 */ kInvalidHex,
            /* 'R' 52 */ kInvalidHex,
            /* 'S' 53 */ kInvalidHex,
            /* 'T' 54 */ kInvalidHex,
            /* 'U' 55 */ kInvalidHex,
            /* 'V' 56 */ kInvalidHex,
            /* 'W' 57 */ kInvalidHex,
            /* 'X' 58 */ kInvalidHex,
            /* 'Y' 59 */ kInvalidHex,
            /* 'Z' 5A */ kInvalidHex,
            /* '[' 5B */ kInvalidHex,
            /* '\' 5C */ kInvalidHex,
            /* ']' 5D */ kInvalidHex,
            /* '^' 5E */ kInvalidHex,
            /* '_' 5F */ kInvalidHex,
        /* 60 */
            /* '`' 60 */ kInvalidHex,
            /* 'a' 61 */ 10,
            /* 'b' 62 */ 11,
            /* 'c' 63 */ 12,
            /* 'd' 64 */ 13,
            /* 'e' 65 */ 14,
            /* 'f' 66 */ 15,
            /* 'g' 67 */ kInvalidHex,
            /* 'h' 68 */ kInvalidHex,
            /* 'i' 69 */ kInvalidHex,
            /* 'j' 6A */ kInvalidHex,
            /* 'k' 6B */ kInvalidHex,
            /* 'l' 6C */ kInvalidHex,
            /* 'm' 6D */ kInvalidHex,
            /* 'n' 6E */ kInvalidHex,
            /* 'o' 6F */ kInvalidHex,
        /* 70 */
            /* 'p' 70 */ kInvalidHex,
            /* 'q' 71 */ kInvalidHex,
            /* 'r' 72 */ kInvalidHex,
            /* 's' 73 */ kInvalidHex,
            /* 't' 74 */ kInvalidHex,
            /* 'u' 75 */ kInvalidHex,
            /* 'v' 76 */ kInvalidHex,
            /* 'w' 77 */ kInvalidHex,
            /* 'x' 78 */ kInvalidHex,
            /* 'y' 79 */ kInvalidHex,
            /* 'z' 7A */ kInvalidHex,
            /* '{' 7B */ kInvalidHex,
            /* '|' 7C */ kInvalidHex,
            /* '}' 7D */ kInvalidHex,
            /* '~' 7E */ kInvalidHex,
            /*  ?  7F */ kInvalidHex,            // Delete

        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        /* 0x80 */ kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
        /* 0x90 */ kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
        /* 0xA0 */ kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
        /* 0xB0 */ kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
        /* 0xC0 */ kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
        /* 0xD0 */ kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
        /* 0xE0 */ kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
        /* 0xF0 */ kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex,
                   kInvalidHex, kInvalidHex, kInvalidHex, kInvalidHex
    }; // hex_value[256]

    // Use in ext_mask[256]
    enum {
        kNull2              = 0x0001,   // '\0'
        kNewLine2           = 0x0002,   // '\n', '\r'
        kWhiteSpace2        = 0,
        kDigital2           = 0,
        kHexChar2           = 0,
        kUnderLine2         = 0,
        kUpperCase          = 0x0004,   // [A-Z]
        kLowerCase          = 0x0008,   // [a-z]
        kDelimiter          = 0x0010,   // , ;

        kInvalidPath        = 0x0040,   // Include: [ : ? " * : < > | ]
        kOperator2          = 0x0080,   // Include: ! % & * + - : < = > ? ^ | ~

        kMaskLast2
    };

    static const uint8_t ext_mask[256] = {
        /* 0x00 */
            /* '\0'  00 */ kNull2,                      // Null
            /* '\1'  01 */ 0,
            /* '\2'  02 */ 0,
            /* '\3'  03 */ 0,
            /* '\4'  04 */ 0,
            /* '\5'  05 */ 0,
            /* '\6'  06 */ 0,
            /* '\a'  07 */ 0,                           // Bell
            /* '\b'  08 */ 0,                           // Backspace
            /* '\t'  09 */ kWhiteSpace2,                // Tab
            /* '\r'  0a */ kNewLine2,                   // LF
            /* '\v'  0b */ kWhiteSpace2,                // VT
            /* '\f'  0c */ kWhiteSpace2,                // FF
            /* '\n'  0d */ kNewLine2,                   // CR
            /* '\14' 0e */ 0,
            /* '\15' 0f */ 0,
        /* 0x10 */
            /* '\16' 10 */ 0,
            /* '\17' 11 */ 0,
            /* '\18' 12 */ 0,
            /* '\19' 13 */ 0,
            /* '\20' 14 */ 0,
            /* '\21' 15 */ 0,
            /* '\22' 16 */ 0,
            /* '\23' 17 */ 0,
            /* '\24' 18 */ 0,
            /* '\25' 19 */ 0,
            /* '\26' 1a */ 0,
            /* '\e'  1b */ 0,                           // Escape
            /* '\28' 1c */ 0,
            /* '\29' 1d */ 0,
            /* '\30' 1e */ 0,
            /* '\31' 1f */ 0,
        /* 0x20 */
            /* ' ' 20 */ kWhiteSpace2,
            /* '!' 21 */ kOperator2,
            /* '"' 22 */ kIncludeQuote,
            /* '#' 23 */ 0,
            /* '$' 24 */ 0,
            /* '%' 25 */ kOperator2,
            /* '&' 26 */ kOperator2,
            /* ''' 27 */ 0,
            /* '(' 28 */ 0,
            /* ')' 29 */ 0,
            /* '*' 2A */ kOperator2,
            /* '+' 2B */ kOperator2,
            /* ',' 2C */ 0,
            /* '-' 2D */ kOperator2,
            /* '.' 2E */ kOperator2,
            /* '/' 2F */ 0,
        /* 0x30 */  
            /* '0' 30 */ kDigital2 | kHexChar2,
            /* '1' 31 */ kDigital2 | kHexChar2,
            /* '2' 32 */ kDigital2 | kHexChar2,
            /* '3' 33 */ kDigital2 | kHexChar2,
            /* '4' 34 */ kDigital2 | kHexChar2,
            /* '5' 35 */ kDigital2 | kHexChar2,
            /* '6' 36 */ kDigital2 | kHexChar2,
            /* '7' 37 */ kDigital2 | kHexChar2,
            /* '8' 38 */ kDigital2 | kHexChar2,
            /* '9' 39 */ kDigital2 | kHexChar2,
            /* ':' 3A */ 0,
            /* ';' 3B */ 0,
            /* '<' 3C */ kOperator2,
            /* '=' 3D */ kOperator2,
            /* '>' 3E */ kOperator2 | kIncludeQuote,
            /* '?' 3F */ kOperator2,
        /* 40 */
            /* '@' 40 */ 0,
            /* 'A' 41 */ kUpperCase | kHexChar2,
            /* 'B' 42 */ kUpperCase | kHexChar2,
            /* 'C' 43 */ kUpperCase | kHexChar2,
            /* 'D' 44 */ kUpperCase | kHexChar2,
            /* 'E' 45 */ kUpperCase | kHexChar2,
            /* 'F' 46 */ kUpperCase | kHexChar2,
            /* 'G' 47 */ kUpperCase,
            /* 'H' 48 */ kUpperCase,
            /* 'I' 49 */ kUpperCase,
            /* 'J' 4A */ kUpperCase,
            /* 'K' 4B */ kUpperCase,
            /* 'L' 4C */ kUpperCase,
            /* 'M' 4D */ kUpperCase,
            /* 'N' 4E */ kUpperCase,
            /* 'O' 4F */ kUpperCase,            
        /* 50 */
            /* 'P' 50 */ kUpperCase,
            /* 'Q' 51 */ kUpperCase,
            /* 'R' 52 */ kUpperCase,
            /* 'S' 53 */ kUpperCase,
            /* 'T' 54 */ kUpperCase,
            /* 'U' 55 */ kUpperCase,
            /* 'V' 56 */ kUpperCase,
            /* 'W' 57 */ kUpperCase,
            /* 'X' 58 */ kUpperCase,
            /* 'Y' 59 */ kUpperCase,
            /* 'Z' 5A */ kUpperCase,
            /* '[' 5B */ 0,
            /* '\' 5C */ kOperator2,
            /* ']' 5D */ 0,
            /* '^' 5E */ kOperator2,
            /* '_' 5F */ kUnderLine2,
        /* 60 */
            /* '`' 60 */ 0,
            /* 'a' 61 */ kLowerCase | kHexChar2,
            /* 'b' 62 */ kLowerCase | kHexChar2,
            /* 'c' 63 */ kLowerCase | kHexChar2,
            /* 'd' 64 */ kLowerCase | kHexChar2,
            /* 'e' 65 */ kLowerCase | kHexChar2,
            /* 'f' 66 */ kLowerCase | kHexChar2,
            /* 'g' 67 */ kLowerCase,
            /* 'h' 68 */ kLowerCase,
            /* 'i' 69 */ kLowerCase,
            /* 'j' 6A */ kLowerCase,
            /* 'k' 6B */ kLowerCase,
            /* 'l' 6C */ kLowerCase,
            /* 'm' 6D */ kLowerCase,
            /* 'n' 6E */ kLowerCase,
            /* 'o' 6F */ kLowerCase,
        /* 70 */
            /* 'p' 70 */ kLowerCase,
            /* 'q' 71 */ kLowerCase,
            /* 'r' 72 */ kLowerCase,
            /* 's' 73 */ kLowerCase,
            /* 't' 74 */ kLowerCase,
            /* 'u' 75 */ kLowerCase,
            /* 'v' 76 */ kLowerCase,
            /* 'w' 77 */ kLowerCase,
            /* 'x' 78 */ kLowerCase,
            /* 'y' 79 */ kLowerCase,
            /* 'z' 7A */ kLowerCase,
            /* '{' 7B */ 0,
            /* '|' 7C */ kOperator2,
            /* '}' 7D */ 0,
            /* '~' 7E */ kOperator2,
            /*  ?  7F */ 0,                 // Delete

        /* 0x80 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0x90 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xA0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xB0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xC0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xD0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xE0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 0xF0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }; // ext_mask[256]

    //
    // is_operator_chars:
    //     !   %   &   *   +   -   :   <   =   >   ?   ^   |   ~
    //    21  25  26  2A  2B  2D  3A  3C  3D  3E  3F  5E  7C  7E
    //    33  37  38  42  43  45  58  60  61  62  63  94 124 126
    //
    static const unsigned char is_operator_chars[256] = {
        //       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        /* 00 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 10 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 20 */ 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0,
        /* 30 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1,
        /* 40 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 50 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        /* 60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,

        /* 80 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 90 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* A0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* B0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* C0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* D0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* E0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* F0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    /* CharInfo::mask[256] */
    static inline bool IsWhiteSpace(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kWhiteSpace) != 0);
    }

    static inline bool IsWhiteSpaces(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kWhiteSpaces) != 0);
    }

    static inline bool IsNewLine(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kNewLine) != 0);
    }

    static inline bool isAlphabet(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kAlphabet) != 0);
    }

    static inline bool IsUnderLine(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kUnderLine) != 0);
    }

    static inline bool IsDigital(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kDigital) != 0);
    }

    static inline bool IsHexChar(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kHexChar) != 0);
    }

    static inline bool IsInvalidPath(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kInvalidPath) != 0);
    }

    static inline bool IsOperator(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kOperator) != 0);
    }

    static inline bool IsIdentifierFirst(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kIdentifierFirst) != 0);
    }

    static inline bool IsIdentifierBody(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kIdentifierBody) != 0);
    }

    static inline bool IsIdentifier(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kIdentifier) != 0);
    }

    static inline bool IsIncludeEndOf(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::kIncludeEndOf) != 0);
    }

    /* CharInfo::ext_mask[256] */
    static inline bool IsUpperCase(unsigned char ch) {
        return ((CharInfo::ext_mask[ch] & CharInfo::kUpperCase) != 0);
    }

    static inline bool IsLowerCase(unsigned char ch) {
        return ((CharInfo::ext_mask[ch] & CharInfo::kLowerCase) != 0);
    }

    static inline bool IsDelimiter(unsigned char ch) {
        return ((CharInfo::ext_mask[ch] & CharInfo::kDelimiter) != 0);
    }

    /* Escape char */
    static inline bool NeedEscape(unsigned char ch) {
        return (CharInfo::escape_chars[ch] != 0);
    }

    static inline bool IsAdvancedEscape(unsigned char ch) {
        return (CharInfo::escape_chars[ch] >= CharInfo::kAdvancedEscape);
    }

    static inline unsigned char GetEscapeChar(unsigned char ch) {
        return CharInfo::escape_chars[ch];
    }

    /* Unescape char */
    static inline bool CanUnescape(unsigned char ch) {
        return (CharInfo::unescape_chars[ch] != CharInfo::kUnescapeError);
    }

    static inline bool IsAdvancedUnescape(unsigned char ch) {
        return (CharInfo::unescape_chars[ch] >= CharInfo::kAdvancedUnescape);
    }

    static inline unsigned char GetUnescapeChar(unsigned char ch) {
        return CharInfo::unescape_chars[ch];
    }

    /* HexValue */
    static inline bool IsHexChars(unsigned char ch) {
        return (CharInfo::hex_value[ch] != CharInfo::kInvalidHex);
    }

    static inline unsigned char GetHexValue(unsigned char ch) {
        return CharInfo::hex_value[ch];
    }
};

} // namespace jlang

#endif // JLANG_LANG_CHARINFO_H
