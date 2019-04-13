
#ifndef JLANG_ASM_ASMPARSER_H
#define JLANG_ASM_ASMPARSER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <memory.h>

#include <string>
#include <vector>   // For std::vector<T>
#include <utility>  // For std::pair<T1, T2>

#include "jlang/basic/stddef.h"
#include "jlang/lang/Error.h"
#include "jlang/lang/Char.h"
#include "jlang/asm/Keyword.h"
#include "jlang/asm/Token.h"
#include "jlang/asm/TokenInfo.h"
#include "jlang/asm/IdentInfo.h"
#include "jlang/stream/StringScanner.h"
#include "jlang/stream/StringStream.h"
#include "jlang/stream/StreamMarker.h"
#include "jlang/jstd/min_max.h"
#include "jlang/jstd/SmallString.h"
#include "jlang/support/HashAlgorithm.h"

#include "jlang/asm/Parser.h"

namespace jlang {
namespace jasm {

class AsmParser : public Parser {
public:
    typedef AsmParser this_type;
    typedef Parser    base_type;

    AsmParser() : base_type() {}
    AsmParser(const std::string & filename)
        : base_type(filename) {
        // Do nothing !!
    }
    virtual ~AsmParser() {}

    // NonCopyable
    AsmParser(const AsmParser & src) = delete;
    AsmParser(AsmParser && src) = delete;
    AsmParser & operator = (const AsmParser & rhs) = delete;

public:
    bool skipLineComment() {
        return base_type::skipLineComment();
    }

    bool skipBlockComment() {
        // It's not implement.
        return false;
    }

    bool parseLineComment(Error & ec) {
        return base_type::parseLineComment(ec);
    }

    bool parseBlockComment(Error & ec) {
        // It's not implement.
        return false;
    }

    bool parseComment(TokenInfo & ti, Error & ec) {
        bool is_comments;
        uint8_t ch = scanner_.getu();
        if (likely(ch == ';')) {
            // Line comment
            scanner_.next();

            is_comments = base_type::parseLineComment(ec);
            ti.setToken(Token::LineComment);
        }
        else {
            ec = Error::Ok;
            is_comments = false;
        }
        return is_comments;
    }

    // EBNF: Script = { Include | Preprocessing | Comment | Function | FunctionDeclaration
    //                  AlignmentStatement | EntryPointStatement | StringsDeclaration
    //                  ';' }
    Error parseScript(bool inBlock = false) {
        Error ec;
        StreamMarker marker(scanner_, false);
        TokenInfo ti;
        bool isEof = false;

        while (scanner_.has_next()) {
            scanner_.skipWhiteSpaces();

            marker.remark();
            uint8_t ch = scanner_.get();
            switch (ch) {
            case '\0':
                // Eof
                isEof = true;
                break;

            case 'A': case 'B': case 'C': case 'D': case 'E':
            case 'F': case 'G': case 'H': case 'I': case 'J':
            case 'K': case 'L': case 'M': case 'N': case 'O':
            case 'P': case 'Q': case 'R': case 'S': case 'T':
            case 'U': case 'V': case 'W': case 'X': case 'Y':
            case 'Z':
            case 'a': case 'b': case 'c': case 'd': case 'e':
            case 'f': case 'g': case 'h': case 'i': case 'j':
            case 'k': case 'l': case 'm': case 'n': case 'o':
            case 'p': case 'q': case 'r': case 's': case 't':
            case 'u': case 'v': case 'w': case 'x': case 'y':
            case 'z':
            case '_':
                // Identifier
                {
                    scanner_.next();

                    // Parse identifier body
                    scanner_.skipIdentifierBody();

                    IdentInfo identInfo;
                    identInfo.makeIdent(marker);
                    assert(identInfo.length() > 0);

                    std::cout << ">>> Identifier name = [" << identInfo.name().c_str() << "]" << std::endl;

                    const std::string & identName = identInfo.name();

                    Keyword * keyword = identInfo.getKeyword();
                    if (likely(keyword != nullptr)) {
                        if (likely((keyword->getKind() & KeywordKind::IsDataType) != 0)) {
                            // It's a function or identifier declaration.
                            ec = parseIdentifierDeclaration(*keyword, identInfo);
                        }
                        else if (likely((keyword->getKind() & KeywordKind::IsKeyword) != 0)) {
                            // It's a keyword
                            ec = handleScriptKeyword(*keyword);
                        }
                        else if (likely(keyword->id() == Keyword::NotFound)) {
                            // Not found
                        }
                        else {
                            // Error
                        }
                    }
                    else {
                        // The keyword has not found.
                    }
                }
                break;

            case '.':
                // Section statement
                scanner_.next();

                {
                    IdentInfo identInfo;
                    parseIdentifier(identInfo, -1);

                    if (likely(identInfo.length() > 0)) {
                        Keyword * section = identInfo.getSection();
                        if (likely(section != nullptr)) {
                            ec = handleSectionStatement(section->token(), ti);
                        }
                        else {
                            // The section keywrod has not found.
                            ec = Error::UnsupportedSectionStatement;
                        }
                    }
                }
                break;

            case '#':
                // Preprocessing statement
                scanner_.next();

                {
                    //
                }
                break;

            case ';':
                // Comment statement (Semicolon)
                scanner_.next();

                {
                    bool is_comment = parseLineComment(ec);
                    ti.setToken(Token::LineComment);
                }
                break;

            default:
                // Unknown
                {
                    scanner_.next();
                    scanner_.skipWhiteSpace();
                }
                break;
            }

            if (ec.isError()) {
                break;
            }
        }

        return ec;
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_ASMPARSER_H
