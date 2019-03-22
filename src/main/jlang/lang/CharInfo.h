
#ifndef JLANG_LANG_CHARINFO_H
#define JLANG_LANG_CHARINFO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"

#include <stdint.h>
#include <assert.h>

namespace jlang {
namespace CharInfo {

    // Use in mask[256]
    enum {
        Null               = 0x0001,   // '\0'
        WhiteSpace         = 0x0002,   // ' ', \t, \v, \f
        NewLine            = 0x0004,   // \r, \n
        Alphabet           = 0x0008,   // [A-Z] and [a-z]
        UnderLine          = 0x0010,   // [_]
        Digital            = 0x0020,   // [0-9]
        HexChar            = 0x0040,   // [0-9] and ([a-f] or [A-F])
        IncludeQuote       = 0x0080,   // " >
        Operator           = 0,        // Include: ! % & * + - : < = > ? ^ | ~
        
        WhiteSpaces        = WhiteSpace | NewLine,           // ' ', \t, \v, \f and \r, \n
        IdentifierFirst    = Alphabet | UnderLine,           // [A-Z], [a-z], and [_]
        IdentifierBody     = IdentifierFirst | Digital,      // [A-Z], [a-z], [0-9] and [_]
        Identifier         = IdentifierFirst | Digital,      // [A-Z], [a-z], [0-9] and [_]

        IncludeEndOf       = Null | NewLine | IncludeQuote, // '\0', '\n', '\r', ", >

        MaskLast
    };

    static const uint8_t mask[256] = {
        /* 0x00 */
            /* '\0'  00 */ Null,                // Null
            /* '\1'  01 */ 0,
            /* '\2'  02 */ 0,
            /* '\3'  03 */ 0,
            /* '\4'  04 */ 0,
            /* '\5'  05 */ 0,
            /* '\6'  06 */ 0,
            /* '\a'  07 */ 0,                   // Bell
            /* '\b'  08 */ 0,                   // Backspace
            /* '\t'  09 */ 0 | WhiteSpace,      // Tab
            /* '\r'  0a */ 0 | NewLine,         // LF
            /* '\v'  0b */ 0 | WhiteSpace,      // VT
            /* '\f'  0c */ 0 | WhiteSpace,      // FF
            /* '\n'  0d */ 0 | NewLine,         // CR
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
            /* ' ' 20 */ WhiteSpace,
            /* '!' 21 */ Operator,
            /* '"' 22 */ 0,
            /* '#' 23 */ 0,
            /* '$' 24 */ 0,
            /* '%' 25 */ Operator,
            /* '&' 26 */ Operator,
            /* ''' 27 */ 0,
            /* '(' 28 */ 0,
            /* ')' 29 */ 0,
            /* '*' 2A */ Operator,
            /* '+' 2B */ Operator,
            /* ',' 2C */ 0,
            /* '-' 2D */ Operator,
            /* '.' 2E */ Operator,
            /* '/' 2F */ 0,
        /* 0x30 */  
            /* '0' 30 */ Digital | HexChar,
            /* '1' 31 */ Digital | HexChar,
            /* '2' 32 */ Digital | HexChar,
            /* '3' 33 */ Digital | HexChar,
            /* '4' 34 */ Digital | HexChar,
            /* '5' 35 */ Digital | HexChar,
            /* '6' 36 */ Digital | HexChar,
            /* '7' 37 */ Digital | HexChar,
            /* '8' 38 */ Digital | HexChar,
            /* '9' 39 */ Digital | HexChar,
            /* ':' 3A */ 0,
            /* ';' 3B */ 0,
            /* '<' 3C */ Operator,
            /* '=' 3D */ Operator,
            /* '>' 3E */ Operator,
            /* '?' 3F */ Operator,
        /* 40 */
            /* '@' 40 */ 0,
            /* 'A' 41 */ Alphabet | HexChar,
            /* 'B' 42 */ Alphabet | HexChar,
            /* 'C' 43 */ Alphabet | HexChar,
            /* 'D' 44 */ Alphabet | HexChar,
            /* 'E' 45 */ Alphabet | HexChar,
            /* 'F' 46 */ Alphabet | HexChar,
            /* 'G' 47 */ Alphabet,
            /* 'H' 48 */ Alphabet,
            /* 'I' 49 */ Alphabet,
            /* 'J' 4A */ Alphabet,
            /* 'K' 4B */ Alphabet,
            /* 'L' 4C */ Alphabet,
            /* 'M' 4D */ Alphabet,
            /* 'N' 4E */ Alphabet,
            /* 'O' 4F */ Alphabet,            
        /* 50 */
            /* 'P' 50 */ Alphabet,
            /* 'Q' 51 */ Alphabet,
            /* 'R' 52 */ Alphabet,
            /* 'S' 53 */ Alphabet,
            /* 'T' 54 */ Alphabet,
            /* 'U' 55 */ Alphabet,
            /* 'V' 56 */ Alphabet,
            /* 'W' 57 */ Alphabet,
            /* 'X' 58 */ Alphabet,
            /* 'Y' 59 */ Alphabet,
            /* 'Z' 5A */ Alphabet,
            /* '[' 5B */ 0,
            /* '\' 5C */ Operator,
            /* ']' 5D */ 0,
            /* '^' 5E */ Operator,
            /* '_' 5F */ UnderLine,
        /* 60 */
            /* '`' 60 */ 0,
            /* 'a' 61 */ Alphabet | HexChar,
            /* 'b' 62 */ Alphabet | HexChar,
            /* 'c' 63 */ Alphabet | HexChar,
            /* 'd' 64 */ Alphabet | HexChar,
            /* 'e' 65 */ Alphabet | HexChar,
            /* 'f' 66 */ Alphabet | HexChar,
            /* 'g' 67 */ Alphabet,
            /* 'h' 68 */ Alphabet,
            /* 'i' 69 */ Alphabet,
            /* 'j' 6A */ Alphabet,
            /* 'k' 6B */ Alphabet,
            /* 'l' 6C */ Alphabet,
            /* 'm' 6D */ Alphabet,
            /* 'n' 6E */ Alphabet,
            /* 'o' 6F */ Alphabet,
        /* 70 */
            /* 'p' 70 */ Alphabet,
            /* 'q' 71 */ Alphabet,
            /* 'r' 72 */ Alphabet,
            /* 's' 73 */ Alphabet,
            /* 't' 74 */ Alphabet,
            /* 'u' 75 */ Alphabet,
            /* 'v' 76 */ Alphabet,
            /* 'w' 77 */ Alphabet,
            /* 'x' 78 */ Alphabet,
            /* 'y' 79 */ Alphabet,
            /* 'z' 7A */ Alphabet,
            /* '{' 7B */ 0,
            /* '|' 7C */ Operator,
            /* '}' 7D */ 0,
            /* '~' 7E */ Operator,
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
        OctEscapeChar  = 254,
        NonAscii       = 255,
        AdvancedEscape = OctEscapeChar
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
            /* '\14' 0e */ OctEscapeChar,
            /* '\15' 0f */ OctEscapeChar,
        /* 0x10 */
            /* '\16' 10 */ OctEscapeChar,
            /* '\17' 11 */ OctEscapeChar,
            /* '\18' 12 */ OctEscapeChar,
            /* '\19' 13 */ OctEscapeChar,
            /* '\20' 14 */ OctEscapeChar,
            /* '\21' 15 */ OctEscapeChar,
            /* '\22' 16 */ OctEscapeChar,
            /* '\23' 17 */ OctEscapeChar,
            /* '\24' 18 */ OctEscapeChar,
            /* '\25' 19 */ OctEscapeChar,
            /* '\26' 1a */ OctEscapeChar,
            /* '\27' 1b */ OctEscapeChar,   // Escape
            /* '\28' 1c */ OctEscapeChar,
            /* '\29' 1d */ OctEscapeChar,
            /* '\30' 1e */ OctEscapeChar,
            /* '\31' 1f */ OctEscapeChar,
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
            /*  ?  7F */ NonAscii,          // Delete

        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        /* 0x80 */ NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
        /* 0x90 */ NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
        /* 0xA0 */ NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
        /* 0xB0 */ NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
        /* 0xC0 */ NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
        /* 0xD0 */ NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
        /* 0xE0 */ NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
        /* 0xF0 */ NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii,
                   NonAscii, NonAscii, NonAscii, NonAscii
    }; // escape_chars[256]

    enum {
        UnescapeError = 255,
        OctUnescapeChar = 253,
        HexUnescapeChar = 254,
        AdvancedUnescape = OctUnescapeChar
    };

    static const uint8_t unescape_chars[256] = {
        /* 0x00 */
            /* '\0'  00 */ UnescapeError,           // Null
            /* '\1'  01 */ UnescapeError,
            /* '\2'  02 */ UnescapeError,
            /* '\3'  03 */ UnescapeError,
            /* '\4'  04 */ UnescapeError,
            /* '\5'  05 */ UnescapeError,
            /* '\6'  06 */ UnescapeError,
            /* '\a'  07 */ UnescapeError,           // Bell
            /* '\b'  08 */ UnescapeError,           // Backspace
            /* '\t'  09 */ UnescapeError,           // Tab
            /* '\r'  0a */ UnescapeError,           // LF
            /* '\v'  0b */ UnescapeError,           // VT
            /* '\f'  0c */ UnescapeError,           // FF
            /* '\n'  0d */ UnescapeError,           // CR
            /* '\14' 0e */ UnescapeError,
            /* '\15' 0f */ UnescapeError,
        /* 0x10 */
            /* '\16' 10 */ UnescapeError,
            /* '\17' 11 */ UnescapeError,
            /* '\18' 12 */ UnescapeError,
            /* '\19' 13 */ UnescapeError,
            /* '\20' 14 */ UnescapeError,
            /* '\21' 15 */ UnescapeError,
            /* '\22' 16 */ UnescapeError,
            /* '\23' 17 */ UnescapeError,
            /* '\24' 18 */ UnescapeError,
            /* '\25' 19 */ UnescapeError,
            /* '\26' 1a */ UnescapeError,
            /* '\e'  1b */ UnescapeError,           // Escape
            /* '\28' 1c */ UnescapeError,
            /* '\29' 1d */ UnescapeError,
            /* '\30' 1e */ UnescapeError,
            /* '\31' 1f */ UnescapeError,
        /* 0x20 */
            /* ' ' 20 */ UnescapeError,
            /* '!' 21 */ UnescapeError,
            /* '"' 22 */ '\"',
            /* '#' 23 */ UnescapeError,
            /* '$' 24 */ UnescapeError,
            /* '%' 25 */ UnescapeError,
            /* '&' 26 */ UnescapeError,
            /* ''' 27 */ '\'',
            /* '(' 28 */ UnescapeError,
            /* ')' 29 */ UnescapeError,
            /* '*' 2A */ UnescapeError,
            /* '+' 2B */ UnescapeError,
            /* ',' 2C */ UnescapeError,
            /* '-' 2D */ UnescapeError,
            /* '.' 2E */ UnescapeError,
            /* '/' 2F */ UnescapeError,
        /* 0x30 */  
            /* '0' 30 */ OctUnescapeChar,
            /* '1' 31 */ OctUnescapeChar,
            /* '2' 32 */ OctUnescapeChar,
            /* '3' 33 */ OctUnescapeChar,
            /* '4' 34 */ OctUnescapeChar,
            /* '5' 35 */ OctUnescapeChar,
            /* '6' 36 */ OctUnescapeChar,
            /* '7' 37 */ OctUnescapeChar,
            /* '8' 38 */ UnescapeError,
            /* '9' 39 */ UnescapeError,
            /* ':' 3A */ UnescapeError,
            /* ';' 3B */ UnescapeError,
            /* '<' 3C */ UnescapeError,
            /* '=' 3D */ UnescapeError,
            /* '>' 3E */ UnescapeError,
            /* '?' 3F */ '\?',
        /* 40 */
            /* '@' 40 */ UnescapeError,
            /* 'A' 41 */ UnescapeError,
            /* 'B' 42 */ UnescapeError,
            /* 'C' 43 */ UnescapeError,
            /* 'D' 44 */ UnescapeError,
            /* 'E' 45 */ UnescapeError,
            /* 'F' 46 */ UnescapeError,
            /* 'G' 47 */ UnescapeError,
            /* 'H' 48 */ UnescapeError,
            /* 'I' 49 */ UnescapeError,
            /* 'J' 4A */ UnescapeError,
            /* 'K' 4B */ UnescapeError,
            /* 'L' 4C */ UnescapeError,
            /* 'M' 4D */ UnescapeError,
            /* 'N' 4E */ UnescapeError,
            /* 'O' 4F */ UnescapeError,            
        /* 50 */
            /* 'P' 50 */ UnescapeError,
            /* 'Q' 51 */ UnescapeError,
            /* 'R' 52 */ UnescapeError,
            /* 'S' 53 */ UnescapeError,
            /* 'T' 54 */ UnescapeError,
            /* 'U' 55 */ UnescapeError,
            /* 'V' 56 */ UnescapeError,
            /* 'W' 57 */ UnescapeError,
            /* 'X' 58 */ UnescapeError,
            /* 'Y' 59 */ UnescapeError,
            /* 'Z' 5A */ UnescapeError,
            /* '[' 5B */ UnescapeError,
            /* '\' 5C */ '\\',
            /* ']' 5D */ UnescapeError,
            /* '^' 5E */ UnescapeError,
            /* '_' 5F */ UnescapeError,
        /* 60 */
            /* '`' 60 */ UnescapeError,
            /* 'a' 61 */ '\a',
            /* 'b' 62 */ '\b',
            /* 'c' 63 */ UnescapeError,
            /* 'd' 64 */ UnescapeError,
            /* 'e' 65 */ '\33',
            /* 'f' 66 */ '\f',
            /* 'g' 67 */ UnescapeError,
            /* 'h' 68 */ UnescapeError,
            /* 'i' 69 */ UnescapeError,
            /* 'j' 6A */ UnescapeError,
            /* 'k' 6B */ UnescapeError,
            /* 'l' 6C */ UnescapeError,
            /* 'm' 6D */ UnescapeError,
            /* 'n' 6E */ '\n',
            /* 'o' 6F */ UnescapeError,
        /* 70 */
            /* 'p' 70 */ UnescapeError,
            /* 'q' 71 */ UnescapeError,
            /* 'r' 72 */ '\r',
            /* 's' 73 */ UnescapeError,
            /* 't' 74 */ '\t',
            /* 'u' 75 */ UnescapeError,
            /* 'v' 76 */ '\v',
            /* 'w' 77 */ UnescapeError,
            /* 'x' 78 */ HexUnescapeChar,
            /* 'y' 79 */ UnescapeError,
            /* 'z' 7A */ UnescapeError,
            /* '{' 7B */ UnescapeError,
            /* '|' 7C */ UnescapeError,
            /* '}' 7D */ UnescapeError,
            /* '~' 7E */ UnescapeError,
            /*  ?  7F */ UnescapeError,             // Delete

        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        /* 0x80 */ UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
        /* 0x90 */ UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
        /* 0xA0 */ UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
        /* 0xB0 */ UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
        /* 0xC0 */ UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
        /* 0xD0 */ UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
        /* 0xE0 */ UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
        /* 0xF0 */ UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError,
                   UnescapeError, UnescapeError, UnescapeError, UnescapeError
    }; // unescape_chars[256]

    enum {
        InvalidHex = 255
    };

    static const uint8_t hex_value[256] = {
        /* 0x00 */
            /* '\0'  00 */ InvalidHex,          // Null
            /* '\1'  01 */ InvalidHex,
            /* '\2'  02 */ InvalidHex,
            /* '\3'  03 */ InvalidHex,
            /* '\4'  04 */ InvalidHex,
            /* '\5'  05 */ InvalidHex,
            /* '\6'  06 */ InvalidHex,
            /* '\a'  07 */ InvalidHex,          // Bell
            /* '\b'  08 */ InvalidHex,          // Backspace
            /* '\t'  09 */ InvalidHex,          // Tab
            /* '\r'  0a */ InvalidHex,          // LF
            /* '\v'  0b */ InvalidHex,          // VT
            /* '\f'  0c */ InvalidHex,          // FF
            /* '\n'  0d */ InvalidHex,          // CR
            /* '\14' 0e */ InvalidHex,
            /* '\15' 0f */ InvalidHex,
        /* 0x10 */
            /* '\16' 10 */ InvalidHex,
            /* '\17' 11 */ InvalidHex,
            /* '\18' 12 */ InvalidHex,
            /* '\19' 13 */ InvalidHex,
            /* '\20' 14 */ InvalidHex,
            /* '\21' 15 */ InvalidHex,
            /* '\22' 16 */ InvalidHex,
            /* '\23' 17 */ InvalidHex,
            /* '\24' 18 */ InvalidHex,
            /* '\25' 19 */ InvalidHex,
            /* '\26' 1a */ InvalidHex,
            /* '\e'  1b */ InvalidHex,          // Escape
            /* '\28' 1c */ InvalidHex,
            /* '\29' 1d */ InvalidHex,
            /* '\30' 1e */ InvalidHex,
            /* '\31' 1f */ InvalidHex,
        /* 0x20 */
            /* ' ' 20 */ InvalidHex,
            /* '!' 21 */ InvalidHex,
            /* '"' 22 */ InvalidHex,
            /* '#' 23 */ InvalidHex,
            /* '$' 24 */ InvalidHex,
            /* '%' 25 */ InvalidHex,
            /* '&' 26 */ InvalidHex,
            /* ''' 27 */ InvalidHex,
            /* '(' 28 */ InvalidHex,
            /* ')' 29 */ InvalidHex,
            /* '*' 2A */ InvalidHex,
            /* '+' 2B */ InvalidHex,
            /* ',' 2C */ InvalidHex,
            /* '-' 2D */ InvalidHex,
            /* '.' 2E */ InvalidHex,
            /* '/' 2F */ InvalidHex,
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
            /* ':' 3A */ InvalidHex,
            /* ';' 3B */ InvalidHex,
            /* '<' 3C */ InvalidHex,
            /* '=' 3D */ InvalidHex,
            /* '>' 3E */ InvalidHex,
            /* '?' 3F */ InvalidHex,
        /* 40 */
            /* '@' 40 */ InvalidHex,
            /* 'A' 41 */ 10,
            /* 'B' 42 */ 11,
            /* 'C' 43 */ 12,
            /* 'D' 44 */ 13,
            /* 'E' 45 */ 14,
            /* 'F' 46 */ 15,
            /* 'G' 47 */ InvalidHex,
            /* 'H' 48 */ InvalidHex,
            /* 'I' 49 */ InvalidHex,
            /* 'J' 4A */ InvalidHex,
            /* 'K' 4B */ InvalidHex,
            /* 'L' 4C */ InvalidHex,
            /* 'M' 4D */ InvalidHex,
            /* 'N' 4E */ InvalidHex,
            /* 'O' 4F */ InvalidHex,            
        /* 50 */
            /* 'P' 50 */ InvalidHex,
            /* 'Q' 51 */ InvalidHex,
            /* 'R' 52 */ InvalidHex,
            /* 'S' 53 */ InvalidHex,
            /* 'T' 54 */ InvalidHex,
            /* 'U' 55 */ InvalidHex,
            /* 'V' 56 */ InvalidHex,
            /* 'W' 57 */ InvalidHex,
            /* 'X' 58 */ InvalidHex,
            /* 'Y' 59 */ InvalidHex,
            /* 'Z' 5A */ InvalidHex,
            /* '[' 5B */ InvalidHex,
            /* '\' 5C */ InvalidHex,
            /* ']' 5D */ InvalidHex,
            /* '^' 5E */ InvalidHex,
            /* '_' 5F */ InvalidHex,
        /* 60 */
            /* '`' 60 */ InvalidHex,
            /* 'a' 61 */ 10,
            /* 'b' 62 */ 11,
            /* 'c' 63 */ 12,
            /* 'd' 64 */ 13,
            /* 'e' 65 */ 14,
            /* 'f' 66 */ 15,
            /* 'g' 67 */ InvalidHex,
            /* 'h' 68 */ InvalidHex,
            /* 'i' 69 */ InvalidHex,
            /* 'j' 6A */ InvalidHex,
            /* 'k' 6B */ InvalidHex,
            /* 'l' 6C */ InvalidHex,
            /* 'm' 6D */ InvalidHex,
            /* 'n' 6E */ InvalidHex,
            /* 'o' 6F */ InvalidHex,
        /* 70 */
            /* 'p' 70 */ InvalidHex,
            /* 'q' 71 */ InvalidHex,
            /* 'r' 72 */ InvalidHex,
            /* 's' 73 */ InvalidHex,
            /* 't' 74 */ InvalidHex,
            /* 'u' 75 */ InvalidHex,
            /* 'v' 76 */ InvalidHex,
            /* 'w' 77 */ InvalidHex,
            /* 'x' 78 */ InvalidHex,
            /* 'y' 79 */ InvalidHex,
            /* 'z' 7A */ InvalidHex,
            /* '{' 7B */ InvalidHex,
            /* '|' 7C */ InvalidHex,
            /* '}' 7D */ InvalidHex,
            /* '~' 7E */ InvalidHex,
            /*  ?  7F */ InvalidHex,            // Delete

        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        /* 0x80 */ InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
        /* 0x90 */ InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
        /* 0xA0 */ InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
        /* 0xB0 */ InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
        /* 0xC0 */ InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
        /* 0xD0 */ InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
        /* 0xE0 */ InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
        /* 0xF0 */ InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex,
                   InvalidHex, InvalidHex, InvalidHex, InvalidHex
    }; // hex_value[256]

    // Use in ext_mask[256]
    enum {
        Null2              = 0x0001,   // '\0'
        NewLine2           = 0x0002,   // '\n', '\r'
        WhiteSpace2        = 0,
        Digital2           = 0,
        HexChar2           = 0,
        UnderLine2         = 0,
        UpperCase          = 0x0004,   // [A-Z]
        LowerCase          = 0x0008,   // [a-z]
        Delimiter          = 0x0010,   // , ;

        InvalidPath        = 0x0040,   // Include: [ : ? " * : < > | ]
        Operator2          = 0x0080,   // Include: ! % & * + - : < = > ? ^ | ~

        MaskLast2
    };

    static const uint8_t ext_mask[256] = {
        /* 0x00 */
            /* '\0'  00 */ Null2,                      // Null
            /* '\1'  01 */ 0,
            /* '\2'  02 */ 0,
            /* '\3'  03 */ 0,
            /* '\4'  04 */ 0,
            /* '\5'  05 */ 0,
            /* '\6'  06 */ 0,
            /* '\a'  07 */ 0,                           // Bell
            /* '\b'  08 */ 0,                           // Backspace
            /* '\t'  09 */ WhiteSpace2,                 // Tab
            /* '\r'  0a */ NewLine2,                    // LF
            /* '\v'  0b */ WhiteSpace2,                 // VT
            /* '\f'  0c */ WhiteSpace2,                 // FF
            /* '\n'  0d */ NewLine2,                    // CR
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
            /* ' ' 20 */ WhiteSpace2,
            /* '!' 21 */ Operator2,
            /* '"' 22 */ IncludeQuote,
            /* '#' 23 */ 0,
            /* '$' 24 */ 0,
            /* '%' 25 */ Operator2,
            /* '&' 26 */ Operator2,
            /* ''' 27 */ 0,
            /* '(' 28 */ 0,
            /* ')' 29 */ 0,
            /* '*' 2A */ Operator2,
            /* '+' 2B */ Operator2,
            /* ',' 2C */ 0,
            /* '-' 2D */ Operator2,
            /* '.' 2E */ Operator2,
            /* '/' 2F */ 0,
        /* 0x30 */  
            /* '0' 30 */ Digital2 | HexChar2,
            /* '1' 31 */ Digital2 | HexChar2,
            /* '2' 32 */ Digital2 | HexChar2,
            /* '3' 33 */ Digital2 | HexChar2,
            /* '4' 34 */ Digital2 | HexChar2,
            /* '5' 35 */ Digital2 | HexChar2,
            /* '6' 36 */ Digital2 | HexChar2,
            /* '7' 37 */ Digital2 | HexChar2,
            /* '8' 38 */ Digital2 | HexChar2,
            /* '9' 39 */ Digital2 | HexChar2,
            /* ':' 3A */ 0,
            /* ';' 3B */ 0,
            /* '<' 3C */ Operator2,
            /* '=' 3D */ Operator2,
            /* '>' 3E */ Operator2 | IncludeQuote,
            /* '?' 3F */ Operator2,
        /* 40 */
            /* '@' 40 */ 0,
            /* 'A' 41 */ UpperCase | HexChar2,
            /* 'B' 42 */ UpperCase | HexChar2,
            /* 'C' 43 */ UpperCase | HexChar2,
            /* 'D' 44 */ UpperCase | HexChar2,
            /* 'E' 45 */ UpperCase | HexChar2,
            /* 'F' 46 */ UpperCase | HexChar2,
            /* 'G' 47 */ UpperCase,
            /* 'H' 48 */ UpperCase,
            /* 'I' 49 */ UpperCase,
            /* 'J' 4A */ UpperCase,
            /* 'K' 4B */ UpperCase,
            /* 'L' 4C */ UpperCase,
            /* 'M' 4D */ UpperCase,
            /* 'N' 4E */ UpperCase,
            /* 'O' 4F */ UpperCase,            
        /* 50 */
            /* 'P' 50 */ UpperCase,
            /* 'Q' 51 */ UpperCase,
            /* 'R' 52 */ UpperCase,
            /* 'S' 53 */ UpperCase,
            /* 'T' 54 */ UpperCase,
            /* 'U' 55 */ UpperCase,
            /* 'V' 56 */ UpperCase,
            /* 'W' 57 */ UpperCase,
            /* 'X' 58 */ UpperCase,
            /* 'Y' 59 */ UpperCase,
            /* 'Z' 5A */ UpperCase,
            /* '[' 5B */ 0,
            /* '\' 5C */ Operator2,
            /* ']' 5D */ 0,
            /* '^' 5E */ Operator2,
            /* '_' 5F */ UnderLine2,
        /* 60 */
            /* '`' 60 */ 0,
            /* 'a' 61 */ LowerCase | HexChar2,
            /* 'b' 62 */ LowerCase | HexChar2,
            /* 'c' 63 */ LowerCase | HexChar2,
            /* 'd' 64 */ LowerCase | HexChar2,
            /* 'e' 65 */ LowerCase | HexChar2,
            /* 'f' 66 */ LowerCase | HexChar2,
            /* 'g' 67 */ LowerCase,
            /* 'h' 68 */ LowerCase,
            /* 'i' 69 */ LowerCase,
            /* 'j' 6A */ LowerCase,
            /* 'k' 6B */ LowerCase,
            /* 'l' 6C */ LowerCase,
            /* 'm' 6D */ LowerCase,
            /* 'n' 6E */ LowerCase,
            /* 'o' 6F */ LowerCase,
        /* 70 */
            /* 'p' 70 */ LowerCase,
            /* 'q' 71 */ LowerCase,
            /* 'r' 72 */ LowerCase,
            /* 's' 73 */ LowerCase,
            /* 't' 74 */ LowerCase,
            /* 'u' 75 */ LowerCase,
            /* 'v' 76 */ LowerCase,
            /* 'w' 77 */ LowerCase,
            /* 'x' 78 */ LowerCase,
            /* 'y' 79 */ LowerCase,
            /* 'z' 7A */ LowerCase,
            /* '{' 7B */ 0,
            /* '|' 7C */ Operator2,
            /* '}' 7D */ 0,
            /* '~' 7E */ Operator2,
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
        return ((CharInfo::mask[ch] & CharInfo::WhiteSpace) != 0);
    }

    static inline bool IsWhiteSpaces(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::WhiteSpaces) != 0);
    }

    static inline bool IsNewLine(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::NewLine) != 0);
    }

    static inline bool isAlphabet(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::Alphabet) != 0);
    }

    static inline bool IsUnderLine(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::UnderLine) != 0);
    }

    static inline bool IsDigital(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::Digital) != 0);
    }

    static inline bool IsHexChar(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::HexChar) != 0);
    }

    static inline bool IsInvalidPath(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::InvalidPath) != 0);
    }

    static inline bool IsOperator(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::Operator) != 0);
    }

    static inline bool IsIdentifierFirst(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::IdentifierFirst) != 0);
    }

    static inline bool IsIdentifierBody(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::IdentifierBody) != 0);
    }

    static inline bool IsIdentifier(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::Identifier) != 0);
    }

    static inline bool IsIncludeEndOf(unsigned char ch) {
        return ((CharInfo::mask[ch] & CharInfo::IncludeEndOf) != 0);
    }

    /* CharInfo::ext_mask[256] */
    static inline bool IsUpperCase(unsigned char ch) {
        return ((CharInfo::ext_mask[ch] & CharInfo::UpperCase) != 0);
    }

    static inline bool IsLowerCase(unsigned char ch) {
        return ((CharInfo::ext_mask[ch] & CharInfo::LowerCase) != 0);
    }

    static inline bool IsDelimiter(unsigned char ch) {
        return ((CharInfo::ext_mask[ch] & CharInfo::Delimiter) != 0);
    }

    /* Escape char */
    static inline bool NeedEscape(unsigned char ch) {
        return (CharInfo::escape_chars[ch] != 0);
    }

    static inline bool IsAdvancedEscape(unsigned char ch) {
        return (CharInfo::escape_chars[ch] >= CharInfo::AdvancedEscape);
    }

    static inline unsigned char GetEscapeChar(unsigned char ch) {
        return CharInfo::escape_chars[ch];
    }

    /* Unescape char */
    static inline bool CanUnescape(unsigned char ch) {
        return (CharInfo::unescape_chars[ch] != CharInfo::UnescapeError);
    }

    static inline bool IsAdvancedUnescape(unsigned char ch) {
        return (CharInfo::unescape_chars[ch] >= CharInfo::AdvancedUnescape);
    }

    static inline unsigned char GetUnescapeChar(unsigned char ch) {
        return CharInfo::unescape_chars[ch];
    }

    /* HexValue */
    static inline bool IsHexChars(unsigned char ch) {
        return (CharInfo::hex_value[ch] != CharInfo::InvalidHex);
    }

    static inline unsigned char GetHexValue(unsigned char ch) {
        return CharInfo::hex_value[ch];
    }

} // namespace CharInfo
} // namespace jlang

#endif // JLANG_LANG_CHARINFO_H
