
#ifndef JLANG_ASM_PARSER_H
#define JLANG_ASM_PARSER_H

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

namespace jlang {
namespace jasm {

enum SkipMasks {
    kSkipNone           = 0,
    kSkipWhiteSpace     = 1 << 0,
    kSkipNewLine        = 1 << 1,
    kSkipWhiteSpaces    = kSkipWhiteSpace | kSkipNewLine,
    kSkipDefault        = kSkipWhiteSpaces
};

struct IParser {
    virtual Error parseScript(bool inBlock = false) = 0;
};

class ParserBase : public IParser {
protected:
    StringScanner scanner_;
    Token token_;
    std::string filename_;
    std::string identifier_;

public:
    ParserBase() : token_(Token::Unknown) {}
    ParserBase(const std::string & filename)
        : token_(Token::Unknown), filename_(filename) {
        // Do nothing !!
    }
    virtual ~ParserBase() {}

    // NonCopyable
    ParserBase(const ParserBase & src) = delete;
    ParserBase(ParserBase && src) = delete;
    ParserBase & operator = (const ParserBase & rhs) = delete;

    void setStream(StringStream & stream) {
        scanner_.copy(stream);
    }

    void setStream(FileStringStream & fileStream) {
        StringStream & stream = fileStream.getStream();
        setStream(stream);
    }

    Error parseScript(bool inBlock = false) {
        return Error::Ok;
    }
};

class Parser : public ParserBase {
public:
    typedef Parser      this_type;
    typedef ParserBase  base_type;

public:
    Parser() : base_type() {}
    Parser(const std::string & filename)
        : base_type(filename) {
        // Do nothing !!
    }
    virtual ~Parser() {}

    // NonCopyable
    Parser(const Parser & src) = delete;
    Parser(Parser && src) = delete;
    Parser & operator = (const Parser & rhs) = delete;

public:
    void parseIdentifier(IdentInfo & identInfo) {
        StreamMarker marker(scanner_);
        scanner_.skipIdentifier();
        assert(marker.length() > 0);

        identInfo.appendIdent(marker);
    }

    void parseIdentifier(IdentInfo & identInfo, TokenInfo & ti) {
        StreamMarker marker(scanner_);
        scanner_.skipIdentifier();
        assert(marker.length() > 0);

        identInfo.appendIdent(marker);
        ti.setToken(Token::Identifier, identInfo.start(), identInfo.length());
    }

    void parseIdentifier(IdentInfo & identInfo, int offset) {
        StreamMarker marker(scanner_, false);
        marker.setmark(-1);
        scanner_.skipIdentifier();
        assert(marker.length() > 0);

        identInfo.appendIdent(marker);
    }

    void parseIdentifierBody(uint8_t firstChar, IdentInfo & identInfo) {
        assert(firstChar == scanner_.getu(-1));
        StreamMarker marker(scanner_, false);
        marker.setmark(-1);
        scanner_.skipIdentifierBody();
        assert(marker.length() > 0);

        identInfo.appendIdent(marker);
    }

    void parseIdentifierBody(uint8_t firstChar, IdentInfo & identInfo, TokenInfo & ti) {
        assert(firstChar == scanner_.getu(-1));
        StreamMarker marker(scanner_, false);
        marker.setmark(-1);
        scanner_.skipIdentifierBody();
        assert(marker.length() > 0);

        identInfo.appendIdent(marker);
        ti.setToken(Token::Identifier, identInfo.start(), identInfo.length());
    }

    Error parseIdentifierStrict(IdentInfo & identInfo) {
        Error ec;
        StreamMarker marker(scanner_);
        scanner_.skipIdentifier();
        assert(marker.length() > 0);

        identInfo.appendIdent(marker);
        if (identInfo.length() <= 0) {
            ec = Error::IllegalIdentifer;
        }
        return ec;
    }

    Error parseIdentifierStrict(IdentInfo & identInfo, TokenInfo & ti) {
        Error ec;
        StreamMarker marker(scanner_);
        scanner_.skipIdentifier();
        assert(marker.length() > 0);

        identInfo.appendIdent(marker);
        if (identInfo.length() > 0) {
            ti.setToken(Token::Identifier, identInfo.start(), identInfo.length());
        }
        else {
            ti.setToken(Token::Unrecognized, identInfo.start(), identInfo.length());
            ec = Error::IllegalIdentifer;
        }
        return ec;
    }

    Error parseStatements() {
        Error ec;

        // Skip the whitespaces at the beginning of the stream.
        scanner_.skipWhiteSpaces();

        uint8_t ch = scanner_.getu();

        // Check first non-whitespace char.
        if (likely(scanner_.isIdentifierFirst(ch))) {  // Identifier?
            scanner_.next();

            IdentInfo identInfo;
            parseIdentifierBody(ch, identInfo);

            // Expect to skip N whitespace.
            scanner_.skipWhiteSpace();

            ch = scanner_.getu();
            if (likely(ch == '=')) {
                // It's a assignment statement.
                scanner_.next();
            }
            else if (likely(ch == '+')) {
                // cnt++; or x += 1;
                scanner_.next();
            }
            else if (likely(ch == '-')) {
                // cnt--; or x -= 1;
                scanner_.next();
            }
            else if (likely(ch == '.')) {
                // object.read();
                scanner_.next();
            }
            else if (likely(ch == '-')) {
                // object->read();
                scanner_.next();
            }
            else if (likely(ch == '(')) {
                // It's a function call.
                scanner_.next();
            }
            else if (likely(ch == ':')) {
                ch = scanner_.getu(1);
                if (likely(ch == ':')) {
                    // It's a identifier namespace.
                    scanner_.next(2);
                }
                else {
                    // It's a label name.
                    scanner_.next();

                    // TODO: Append the label name.
                }
            }
        }
        else if (likely(scanner_.isWhiteSpace(ch))) {   // WhiteSpace
            scanner_.next();
        }
        else if (likely(scanner_.isDigital(ch))) {   // Digital
            scanner_.next();
        }
        else if (likely(ch == '{')) {
            // Scope begin
            scanner_.next();
        }
        else if (likely(ch == '}')) {
            // Scope end
            scanner_.next();
        }
        else if (likely(ch == '+')) {   // ++cnt;
            scanner_.next();

            ch = scanner_.getu();
            if (likely(ch == '+')) {
                // It's a ++cnt;
                scanner_.next();
            }
            else {
                // Error
                ec = Error::Unknown;
            }
        }
        else if (likely(ch == '-')) {   // --cnt;
            scanner_.next();

            ch = scanner_.getu();
            if (likely(ch == '-')) {
                // It's a --cnt;
                scanner_.next();
            }
            else {
                // Error
                ec = Error::Unknown;
            }
        }
        else if (likely(ch == '.')) {   // Dot
            scanner_.next();
        }
        else if (likely(ch == ',')) {   // Comma
            scanner_.next();
        }
        else if (likely(ch == '<')) {   // &lt;
            scanner_.next();
        }
        else if (likely(ch == '>')) {   // &gt;
            scanner_.next();
        }
        else if (likely(ch == '=')) {   // EqualSign
            scanner_.next();
        }
        else if (likely(ch == '(')) {   // (
            scanner_.next();
        }
        else if (likely(ch == ')')) {   // )
            scanner_.next();
        }
        else if (likely(ch == '?')) {   // Question
            scanner_.next();
        }
        else if (likely(ch == ';')) {   // Semicolon
            scanner_.next();

            scanner_.skipWhiteSpaces();
        }
        else {
            scanner_.next();
            ec = Error::IllegalStatement;
        }

        return ec;
    }

    Error parseFunctionBody() {
        Error ec;

        do {
            uint8_t ch = scanner_.getu();
            if (likely(ch != '}')) {
                ec = parseStatements();
                if (!ec.isOk())
                    break;
            }
            else {
                // End of function.
                scanner_.next();
                break;
            }
        } while (1);

        return ec;
    }

    Error parseFunctionBodyWrapper() {
        Error ec;
        uint8_t ch = scanner_.getu();
        if (likely(ch == '{')) {
            // It's a function body
            scanner_.next();

            ec = parseFunctionBody();
        }
        else if (likely(ch == ';')) {
            // It's a function declaration.
            scanner_.next();

            // TODO: Append the function declaration.
        }
        else {
            // Error
            scanner_.next();
            ec = Error::IllegalFunctionBody;
        }
        return ec;
    }

    typedef std::vector<std::pair<std::string, std::string>> ArgumentList;

    Error parseFunctionArgumentList() {
        Error ec;
        ArgumentList argList;

        do {
            // Argument type
            IdentInfo argType;
            parseIdentifier(argType);
            if (likely(argType.length() > 0)) {
                // Expect to skip one whitespace.
                scanner_.skipWhiteSpaces();

                // Argument name
                IdentInfo argName;
                parseIdentifier(argName);
                if (likely(argType.length() > 0)) {
                    // Append the argument list
                    argList.push_back(std::make_pair(argType.name(),
                                                     argName.name()));

                    // Expect to skip 0 whitespace.
                    //skipWhiteSpaces_0();

                    uint8_t ch = scanner_.getu();
                    if (likely(ch == ',')) {        // Argument delimiter
                        scanner_.next();

                        // Expect to skip 1 whitespace.
                        scanner_.skipWhiteSpaces_1();

                        // Continue to parse next argument
                        continue;
                    }
                    else if (likely(ch == ')')) {   // End of argument list.
                        scanner_.next();

                        // Expect to skip N whitespace.
                        scanner_.skipWhiteSpaces();

                        ec = parseFunctionBodyWrapper();
                        break;
                    }
                    else if (likely(scanner_.isWhiteSpaces(ch))) {
                        scanner_.next();

                        // Maybe have more than 1 WhiteSpaces.
                        scanner_.skipWhiteSpaces();
                    }
                    else {
                        ec = Error::IllegalArgumentDelimiter;
                        break;
                    }
                }
                else {
                    ec = Error::IllegalArgumentName;
                    break;
                }
            }
            else {
                ec = Error::IllegalArgumentType;
                break;
            }
        } while (1);

        return ec;
    }

    Error parseIdentifierDeclareName(const IdentInfo & identType) {
        Error ec;

        IdentInfo identName;
        parseIdentifier(identName);

        if (identName.length() > 0) {
            // Skip whitespaces between identifier name and ['(' or '='].
            scanner_.skipWhiteSpaces();

            uint8_t ch = scanner_.getu();
            if (likely(ch == '=')) {
                // Expression assignment
                scanner_.next();
                scanner_.skipWhiteSpaces();

                //ec = parseExpression();
            }
            else if (likely(ch == '(')) {
                // Function argument list
                scanner_.next();
                scanner_.skipWhiteSpaces();

                ec = parseFunctionArgumentList();
            }
            else {
                // Error
            }
        }
        else {
            ec = Error::Unknown;
        }

        return ec;
    }

    Error parseIdentifierDeclaration(const Keyword & keyword, IdentInfo & identInfo) {
        Error ec;
        Token signToken(Token::Unknown);

        if (keyword.getKind() == KeywordKind::PodSign) {
            // If identifier is "signed"/"unsigned", continue parse a POD type.
            signToken = keyword.getToken();
            scanner_.skipWhiteSpaces();

            IdentInfo podIdentInfo;
            parseIdentifier(podIdentInfo);
            assert(podIdentInfo.length() > 0);

            const std::string & podIdentName = podIdentInfo.name();

            KeywordMapping & keyMapping = Global::getKeywordMapping();
            KeywordMapping::iterator iter = keyMapping.find(podIdentName);
            if (iter != keyMapping.end()) {
                const Keyword & podKeyword = iter->second;
                if (podKeyword.getKind() == KeywordKind::Pod ||
                    podKeyword.getKind() == KeywordKind::TypeDef) {
                    // Merge the sign type and POD type.
                    bool merged = identInfo.merge(podIdentInfo);
                    if (!merged) {
                        ec = Error::IllegalPodType;
                        goto Parse_Exit;
                    }
                }
                else {
                    // Error: After "signed"/"unsigned", it's a unsupport POD type.
                    ec = Error::UnsupportedPodType;
                    goto Parse_Exit;
                }
            }
            else {
                // Error: After "signed"/"unsigned", it's a unknown type.
                ec = Error::UnknownPodType;
                goto Parse_Exit;
            }
        }

        scanner_.skipWhiteSpaces();

        // Parse function or identifier declare name
        ec = parseIdentifierDeclareName(identInfo);

Parse_Exit:
        return ec;
    }

    Error handleReservedKeyword(const Keyword & keyword) {
        Error ec;
        return ec;
    }

    Error parseIdentifierOrKeyword(TokenInfo & ti) {
        Error ec;

        IdentInfo identInfo;
        parseIdentifier(identInfo);
        assert(identInfo.length() > 0);

        if (identInfo.length() > 0) {
            std::cout << ">>> Identifier name = [" << identInfo.name().c_str() << "]" << std::endl;

            const std::string & identName = identInfo.name();

            KeywordMapping & keyMapping = Global::getKeywordMapping();
            assert(keyMapping.inited());
            KeywordMapping::iterator iter = keyMapping.find(identName);
            if (iter != keyMapping.end()) {
                const Keyword & keyword = iter->second;
                if (likely((keyword.getKind() & KeywordKind::IsDataType) != 0)) {
                    // Function or identifier declaration.
                    ec = parseIdentifierDeclaration(keyword, identInfo);
                }
                else if (likely((keyword.getKind() & KeywordKind::IsKeyword) != 0)) {
                    // It's a keyword
                    ec = handleReservedKeyword(keyword);
                }
            }
        }

        return ec;
    }

    Error parseReservedKeyword(TokenInfo & ti) {
        Error ec;
        StreamMarker marker(scanner_);
        scanner_.skipIdentifier();
        assert(marker.length() > 0);

        IdentInfo keywordInfo;
        keywordInfo.makeIdent(marker);
        if (keywordInfo.length() > 0) {
            std::string & keywordName = keywordInfo.name();

            KeywordMapping & keyMapping = Global::getKeywordMapping();
            assert(keyMapping.inited());
            KeywordMapping::iterator iter = keyMapping.find(keywordName);
            if (iter != keyMapping.end()) {
                Keyword keyword = iter->second;
                if (keyword.getKind() == KeywordKind::Pod ||
                    keyword.getKind() == KeywordKind::TypeDef) {
                    if (keyword.getKind() != KeywordKind::Pod) {
                        // Parse full type define
                    }
                    scanner_.skipWhiteSpace();

                    StreamMarker identMarker(scanner_);
                    scanner_.skipIdentifier();
                    assert(identMarker.length() > 0);

                    IdentInfo identInfo;
                    identInfo.makeIdent(identMarker);

                    scanner_.skipWhiteSpace();

                    //LexerLog::traceTypeDeclare(keyword_start, keyword_end, keyword_name);

                    if (scanner_.get() == '(') {
                        // It's a function
                        //LexerLog::traceFunction(identifier_start, identifier_end, identifier_name);
                    }
                    else if (scanner_.get() == ';') {
                        // It's a type define or others
                        //LexerLog::traceIdentifier(identifier_start, identifier_end, identifier_name);
                        scanner_.next();
                    }
                    else if (scanner_.get() == ',') {
                        // It's a type-list define or others list
                        //LexerLog::traceIdentifier(identifier_start, identifier_end, identifier_name);

                        do {
                            scanner_.next();
                            scanner_.skipWhiteSpace();
                            identMarker.remark();
                            scanner_.skipIdentifier();
                            
                            identInfo.makeIdent(identMarker);
                            if (identInfo.length() <= 0) {
                                // Type-list define failure
                                std::cout << "*** Error: Type-list define failure. *** ";
                                return false;
                            }
                            identInfo.makeIdent(identMarker);

                            scanner_.skipWhiteSpace();
                            //LexerLog::traceIdentifier(identifier_start, identifier_end, identifier_name);
                        } while (scanner_.get() == ',');

                        scanner_.skipWhiteSpace();
                        if (scanner_.get() == ';') {
                            scanner_.next();
                        }
                    }
                    else {
                        //
                    }
                }
                else if (keyword.getKind() == KeywordKind::UserDefine) {
                    // User define function or variant
                    scanner_.skipWhiteSpace();
                }
                else {
                    //LexerLog::traceKeyword(keyword_start, keyword_end, keyword_name);
                }
            }
            else {
                scanner_.skipWhiteSpace();
                if (scanner_.get() == '(') {
                    // It's a function call
                    //LexerLog::traceFunction(keyword_start, keyword_end, keyword_name);

                    //scanner_.next();
                }
                else {
                    // Maybe is a identifier
                    //LexerLog::traceIdentifier(keyword_start, keyword_end, keyword_name);
                }
            }
        }
        else {
            ec = Error::IllegalIdentifer;
        }

        return ec;
    }

    bool isPreprocessing() {
        if (scanner_.get() == PREPROCESSING_CHAR)
            return true;
        else
            return false;
    }

    Error parsePreprocessing(TokenInfo & ti) {
        Error ec;
        Token token = Token::Unknown;
        StreamMarker marker(scanner_);
        scanner_.skipIdentifier();

        IdentInfo identInfo;
        identInfo.makeIdent(marker);

        if (identInfo.length() > 0) {
            KeywordMapping & ppKeyMapping = Global::getPPKeywordMapping();
            KeywordMapping::iterator iter = ppKeyMapping.find(identInfo.name());
            if (iter != ppKeyMapping.end()) {
                Keyword & keyword = iter->second;
                assert(keyword.getKind() == KeywordKind::Preprocessing);

                token = keyword.getToken();
                ti.setStart(identInfo.start());
                ti.setLength(identInfo.length());

                ec = handlePreprocessingStatement(token, ti);
                if (ec.isOk()) {
                    ti.setToken(token);
                }
                else {
                    // Got a error.
                }
            }
            else {
                // Error: It's a unknown preprocessing keyword.
                ec = Error::UnknownPreprocessingKeyword;
            }
        }
        else {
            // Error: It's not a preprocessing keyword.
            ec = Error::IllegalPreprocessingKeyword;
        }

        if (token != Token::Unknown) {
            ti.setToken(token, marker.start(), marker.length());
        }
        else {
            ti.setToken(token, marker.start(), 0);
        }
        return ec;
    }

    Error handlePreprocessingStatement(Token ppToken, const TokenInfo & ti) {
        Error ec = Error::Ok;
        switch (ppToken.value()) {
        case Token::pp_if:
            //
            break;

        case Token::pp_ifdef:
            //
            break;

        case Token::pp_ifndef:
            //
            break;

        case Token::pp_else:
            //
            break;

        case Token::pp_elif:
            //
            break;

        case Token::pp_endif:
            //
            break;

        case Token::pp_define:
            {
                // #define ABC 1
                std::string macro_name;
            }
            break;

        case Token::pp_defined:
            //
            break;

        case Token::pp_include:
            {
                // #include <xxxx.h>
                std::string include_file;
            }
            break;

        default:
            ec = Error::UnknownPreprocessingKeyword;
            break;
        }
        return ec;
    }

    bool isComment() const {
        // The comment must be starting with ';' or '/' char.
        if (scanner_.get() == ';' || scanner_.get() == '/')
            return true;
        else
            return false;
    }

    bool skipLineComment() {
        while (likely(!scanner_.is_null())) {
            if (likely(!scanner_.isNewLine())) {
                scanner_.next();
            }
            else {
                scanner_.next();
                scanner_.skipNewLine();

                // Find the end of line comment.
                return true;
            }
        }

        // It's reach the end of file.
        return true;
    }

    bool skipBlockComment() {
        bool is_completed = false;
        
        while (likely(!scanner_.is_null())) {
            uint8_t ch = scanner_.getu();
            if (likely(ch != '*')) {
                scanner_.next();
            }
            else {
                scanner_.next();
                if (unlikely(scanner_.getu() == '*')) {
                    scanner_.next();
                    if (unlikely(scanner_.getu() == '/')) {
                        scanner_.next();

                        // Find the end of block comment.
                        return true;
                    }
                }
            }
        }

        // It's reach the end of file.
        return false;
    }

    bool parseLineComment(Error & ec) {
        bool is_completed = skipLineComment();
        if (is_completed)
            ec = Error::Ok;
        else 
            ec = Error::IllegalLineComment;
        return is_completed;
    }

    bool parseBlockComment(Error & ec) {
        bool is_completed = skipBlockComment();
        if (is_completed)
            ec = Error::Ok;
        else 
            ec = Error::IllegalBlockComment;
        return is_completed;
    }

    bool parseComment(TokenInfo & ti, Error & ec) {
        bool is_comments;
        uint8_t ch = scanner_.getu();
        if (likely(ch == '/')) {
            // Line comment
            scanner_.next();

            is_comments = parseLineComment(ec);
            ti.setToken(Token::LineComment);
        }
        else if (likely(ch == '*')) {
            // Block comment
            scanner_.next();

            is_comments = parseBlockComment(ec);
            ti.setToken(Token::BlockComment);
        }
        else {
            ec = Error::Ok;
            is_comments = false;
        }
        return is_comments;
    }

    bool parseNumberSuffix(int & valueType) {
        int8_t ch = scanner_.get();
        intptr_t remain = scanner_.remain();
        valueType = ValueType::Unknown;
        if (ch == 'u' || ch == 'U') {
            // u, ul, ull and U, UL, ULL
            int8_t longChar = ch - 'U' + 'L';
            if (scanner_.get(1) == '6' && scanner_.get(2) == '4' && remain >= 3) {
                valueType = ValueType::UInt64;
                scanner_.next(3);
            }
            else if (scanner_.get(1) == longChar && remain >= 2) {
                if (scanner_.get(2) == longChar && remain >= 3) {
                    valueType = ValueType::UInt64;
                    scanner_.next(3);
                }
                else {
                    valueType = ValueType::UInt32;
                    scanner_.next(2);
                }
            }
            else  {
                valueType = ValueType::UInt32;
                scanner_.next();
            }
        }
        else if (ch == 'l' || ch == 'L') {
            // l, ll and L, LL
            if (scanner_.get(1) == ch && remain >= 2) {
                valueType = ValueType::Int64;
                scanner_.next(2);
            }
            else {
                valueType = ValueType::Int32;
                scanner_.next();
            }
        }
        else if (ch == 'i' || ch == 'I') {
            // i, I, i64, I64
            if (scanner_.get(1) == '6' && scanner_.get(2) == '4' && remain >= 3) {
                valueType = ValueType::Int64;
                scanner_.next(3);
            }
            else {
                valueType = ValueType::Int32;
                scanner_.next();
            }
        }
        else if (ch == 'f' || ch == 'F') {
            // f, F
            valueType = ValueType::Fp64;
            scanner_.next();
        }
        return (valueType != ValueType::Unknown);
    }

    template <uint32_t radix = 10>
    bool parseRadixNumberImpl(uint64_t & value) {
        bool is_valid;
        value = 0;
        const char * marker = scanner_.current();
        if (radix == 16) {
            int valueType = ValueType::Unknown;
            char ch;
            while ((ch = scanner_.get()) != '\0') {
                if (ch >= '0' && ch <= '9') {
                    ch -= '0';
                }
                else if (ch >= 'A' && ch <= 'F') {
                    ch -= 'A' - 10;
                }
                else if (ch >= 'a' && ch <= 'f') {
                    ch -= 'a' - 10;
                }
                else {
                    break;
                }
                value = value * radix + ch;
                scanner_.next();
            }
            is_valid = (scanner_.current() > marker);
            parseNumberSuffix(valueType);
            scanner_.skipWhiteSpaces();
        }
        else if (radix == 10) {
            int valueType = ValueType::Unknown;
            char ch;
            while ((ch = scanner_.get()) != '\0') {
                if (ch >= '0' && ch <= '9')
                    ch -= '0';
                else
                    break;
                value = value * radix + ch;
                scanner_.next();
            }
            is_valid = (scanner_.current() > marker);
            parseNumberSuffix(valueType);
            scanner_.skipWhiteSpaces();
        }
        else if (radix == 8) {
            int valueType = ValueType::Unknown;
            char ch;
            while ((ch = scanner_.get()) != '\0') {
                if (ch >= '0' && ch <= '7')
                    ch -= '0';
                else
                    break;
                value = value * radix + ch;
                scanner_.next();
            }
            is_valid = (scanner_.current() > marker);
            parseNumberSuffix(valueType);
            scanner_.skipWhiteSpaces();
        }
        else if (radix == 2) {
            int valueType = ValueType::Unknown;
            char ch;
            while ((ch = scanner_.get()) != '\0') {
                if (ch >= '0' && ch <= '1')
                    ch -= '0';
                else
                    break;
                value = value * radix + ch;
                scanner_.next();
            }
            is_valid = (scanner_.current() > marker);
            parseNumberSuffix(valueType);
            scanner_.skipWhiteSpaces();
        }
        else {
            is_valid = false;
        }
        return is_valid;
    }

    Error parseRadixNumber(Token & token,
                               int & radix, uint64_t & number) {
        Error ec;
        bool is_valid;
        char ch1 = scanner_.get(1);
        if (likely(ch1 == 'x' || ch1 == 'X')) {
            radix = 16;
            scanner_.next(2);
            is_valid = parseRadixNumberImpl<16>(number);
            token = Token::HexLiteral;
            if (!is_valid) {
                ec = Error::IllegalRadix16Number;
            }
        }
        else if (likely(ch1 == 'o' || ch1 == 'O')) {
            radix = 8;
            scanner_.next(2);
            is_valid = parseRadixNumberImpl<8>(number);
            token = Token::OcxLiteral;
            if (!is_valid) {
                ec = Error::IllegalRadix8Number;
            }
        }
        else if (likely(ch1 == 'b' || ch1 == 'B')) {
            radix = 2;
            scanner_.next(2);
            is_valid = parseRadixNumberImpl<2>(number);
            token = Token::BinaryLiteral;
            if (!is_valid) {
                ec = Error::IllegalRadix2Number;
            }
        }
        else if (likely(ch1 == 'd' || ch1 == 'D')) {
            radix = 10;
            scanner_.next(2);
            is_valid = parseRadixNumberImpl<10>(number);
            token = Token::DecLiteral;
            if (!is_valid) {
                ec = Error::IllegalRadix10Number;
            }
        }
        else {
            // It's not a based radix number, re-parse from old place.
            ec = Error::IllegalRadixNumber;
        }
        return ec;
    }

    Error parseDecimalNumber(uint64_t & number) {
        bool is_valid = parseRadixNumberImpl<10>(number);
        if (is_valid)
            return Error::Ok;
        else
            return Error::IllegalRadix10Number;
    }

    Error parseRealNumber(Token & token,
                              uint64_t & integer, uint64_t & fractional,
                              int & exponent, bool & is_float) {
        Error ec;
        int integer_len = 0;
        int fractional_len = 0;
        integer = 0;
        fractional = 0;
        exponent = 0;

        // Whether including a decimal point?
        bool hasDots = false;
        // Whether including a exponent?
        bool hasExponent = false;
        // Whether is a double-precision floating-point number?
        bool isDouble = true;

        // Search the integer part of floating-point number or integer.
        do {
            char ch = scanner_.get();
            if (ch >= '0' && ch <= '9') {
                // Integer part
                integer = integer * 10 + ch - '0';
                integer_len++;
                scanner_.next();
            }
            else if (ch == '.') {
                hasDots = true;
                scanner_.next();
                break;
            }
            else {
                break;
            }
        } while (1);

        if (hasDots) {
            // Search the fractional part of floating-point number.
            do {
                char ch = scanner_.get();
                if (ch >= '0' && ch <= '9') {
                    // Fractional part
                    fractional = fractional * 10 + ch - '0';
                    fractional_len++;
                    scanner_.next();
                }
                else if (ch == '.') {
                    // Found the second decimal point, exit now.
                    ec = Error::IllegalFloatNumber;
                    goto Parse_Exit;
                }
                else {
                    break;
                }
            } while (1);
        }

        // Determine the exponent of floating point numbers.
        do {
            int exponent_sign = 1;
            unsigned int exponent_num = 0;
            int exponent_cnt = 0;
            char ch = scanner_.get();
            if (ch == 'e' || ch == 'E') {
                ch = scanner_.get(1);
                if (ch == '-') {
                    exponent_sign = -1;
                    scanner_.next(2);
                }
                else if (ch == '+') {
                    scanner_.next(2);
                }

                do {
                    ch = scanner_.get();
                    if (ch >= '0' && ch <= '9') {
                        exponent_num = exponent_num * 10 + ch - '0';
                        exponent_cnt++;
                        scanner_.next();
                    }
                    else {
                        break;
                    }
                } while (1);

                if (exponent_cnt > 10 || (exponent_cnt == 10 && exponent_num < 1000000000)) {
                    // Exponent part is overflow
                    ec = Error::ErrorExponentPartOverflow;
                    goto Parse_Exit;
                }
                else if (exponent_cnt <= 0) {
                    // Does not contain the exponential number, it's a illegal error.
                    ec = Error::IllegalExponentPart;
                    goto Parse_Exit;
                }
                else {
                    // The exponent is smaller than -2147483648 or bigger than 2147483647 !
                    if ((exponent_sign != -1 && exponent_num > 2147483647) ||
                        (exponent_sign == -1 && exponent_num > 2147483648)) {
                        ec = Error::ErrorNegativeExponentPartOverflow;
                        goto Parse_Exit;
                    }
                    // The exponent of the long double range is (1.2e-4932 ~ 1.2e+4932)
                    else if (exponent_num > 4932) {
                        ec = Error::ErrorExponentPartOutOfRange;
                        goto Parse_Exit;
                    } 
                    exponent = (exponent_sign != -1) ? (int)exponent_num : -(int)exponent_num;
                    hasExponent = true;
                }
            }
        } while (0);

        // Is a double-precision floating-point number?
        if (hasDots) {
            char ch = scanner_.get();
            if (ch == 'f' || ch == 'F') {
                isDouble = false;
                scanner_.next();
            }
        }

        // Contain a dot or have a exponent is a floating point number.
        is_float = (hasDots | hasExponent);

        // 0xDE0B6B3A7640000ULL = 10^19 = 10E+19
        if (fractional_len > 20 || (fractional_len == 20 && fractional < 0xDE0B6B3A7640000ULL)) {
            // Fractional part is overflow
            ec = Error::ErrorFractionalPartOverflow;
            goto Parse_Exit;
        }
        // Same to fractional part
        if (integer_len > 20 || (integer_len == 20 && integer < 0xDE0B6B3A7640000ULL)) {
            // Integer part is overflow
            ec = Error::ErrorIntegerPartOverflow;
            goto Parse_Exit;
        }

        if (ec == Error::Ok) {
            if (hasDots) {
                token = (isDouble) ? Token::DoubleLiteral : Token::FloatLiteral;
            }
            else {
                token = Token::IntegerLiteral;
            }
        }

Parse_Exit:
        return ec;
    }

    Error parseRealNumberSuffix(Token & token, uint64_t & fractional, int & exponent) {
        Error ec;
        int fractional_len = 0;
        fractional = 0;
        exponent = 0;

        // Whether including a exponent?
        bool hasExponent = false;
        // Whether is a double-precision floating-point number?
        bool isDouble = true;

        // Search the fractional part of floating-point number.
        do {
            char ch = scanner_.get();
            if (ch >= '0' && ch <= '9') {
                // Fractional part
                fractional = fractional * 10 + ch - '0';
                fractional_len++;
                scanner_.next();
            }
            else if (ch == '.') {
                // Found the second decimal point, exit now.
                ec = Error::IllegalFloatingNumber;
                goto Parse_Exit;
            }
            else {
                break;
            }
        } while (1);

        // Determine the exponent of floating point numbers.
        do {
            int exponent_sign = 1;
            unsigned int exponent_num = 0;
            int exponent_cnt = 0;
            char ch = scanner_.get();
            if (ch == 'e' || ch == 'E') {
                ch = scanner_.get(1);
                if (ch == '-') {
                    exponent_sign = -1;
                    scanner_.next(2);
                }
                else if (ch == '+') {
                    scanner_.next(2);
                }

                do {
                    ch = scanner_.get();
                    if (ch >= '0' && ch <= '9') {
                        exponent_num = exponent_num * 10 + ch - '0';
                        exponent_cnt++;
                        scanner_.next();
                    }
                    else {
                        break;
                    }
                } while (1);

                if (exponent_cnt > 10 || (exponent_cnt == 10 && exponent_num < 1000000000)) {
                    // Exponent part is overflow
                    ec = Error::ErrorExponentPartOverflow;
                    goto Parse_Exit;
                }
                else if (exponent_cnt <= 0) {
                    // Does not contain the exponential number, it's a illegal error.
                    ec = Error::IllegalExponentPart;
                    goto Parse_Exit;
                }
                else {
                    // The exponent is smaller than -2147483648 or bigger than 2147483647 !
                    if ((exponent_sign != -1 && exponent_num > 2147483647) ||
                        (exponent_sign == -1 && exponent_num > 2147483648)) {
                        ec = Error::ErrorNegativeExponentPartOverflow;
                        goto Parse_Exit;
                    }
                    // The exponent of the long double range is (1.2e-4932 ~ 1.2e+4932)
                    else if (exponent_num > 4932) {
                        ec = Error::ErrorExponentPartOutOfRange;
                        goto Parse_Exit;
                    }
                    exponent = (exponent_sign != -1) ? (int)exponent_num : -(int)exponent_num;
                    hasExponent = true;
                    UNUSED_VARIANT(hasExponent);
                }
            }
        } while (0);

        // Is a double-precision floating-point number?
        {
            char ch = scanner_.get();
            if (ch == 'f' || ch == 'F') {
                isDouble = false;
                scanner_.next();
            }
        }

        // 0xDE0B6B3A7640000ULL = 10^19 = 10E+19
        if (fractional_len > 20 || (fractional_len == 20 && fractional < 0xDE0B6B3A7640000ULL)) {
            // Fractional part is overflow
            ec = Error::ErrorFractionalPartOverflow;
            goto Parse_Exit;
        }

        if (ec == Error::Ok) {
            token = (isDouble) ? Token::DoubleLiteral : Token::FloatLiteral;
        }

Parse_Exit:
        return ec;
    }

    inline char toOcxChar(unsigned int ocx) const {
        assert(ocx >= 0 && ocx < 8);
        return ('0' + ocx);
    } 

    inline char toLowerHexChar(unsigned int dec) const {
        assert(dec >= 0 && dec < 16);
        return (dec < 10) ? ('0' + dec) : ('a' + (dec - 10));
    }

    inline char toUpperHexChar(unsigned int dec) const {
        assert(dec >= 0 && dec < 16);
        return (dec < 10) ? ('0' + dec) : ('A' + (dec - 10));
    }

    bool getEscapedChars(unsigned char ch, std::string & chars) const {
        bool success = true;
        bool needEscape = CharInfo::NeedEscape(ch);
        if (!needEscape) {
            chars = ch;
        }
        else {
            bool isAdvancedEscape = CharInfo::IsAdvancedEscape(ch);
            if (!isAdvancedEscape) {
                chars = '\\';
                chars.push_back(CharInfo::GetEscapeChar(ch));
            }
            else {
                unsigned char escapeType = CharInfo::GetEscapeChar(ch);
                if (escapeType == CharInfo::OctEscapeChar) {
                    chars = '\\';
                    chars.push_back(toOcxChar(ch / 8));
                    chars.push_back(toOcxChar(ch % 8));
                }
                else if (escapeType == CharInfo::NonAscii) {
                    chars = "\\x";
                    chars.push_back(toUpperHexChar(ch / 16));
                    chars.push_back(toUpperHexChar(ch % 16));
                }
                else {
                    // Unknown escape type
                    success = false;
                }
            }
        }
        return success;
    }

    int getUnescapedChar(StringStream & stream, unsigned char & ch) {
        int skip = 0;
        bool canUnescape = CharInfo::CanUnescape(ch);
        if (canUnescape) {
            bool isAdvencadUnescape = CharInfo::IsAdvancedUnescape(ch);
            if (!isAdvencadUnescape) {
                ch = CharInfo::GetUnescapeChar(ch);
                skip = 1;
            }
            else {
                unsigned char unescapeType = CharInfo::GetUnescapeChar(ch);
                if (unescapeType == CharInfo::OctUnescapeChar) {
                    // Oct: '\123'
                    unsigned char ch1 = stream.getu(1);
                    if (ch1 >= '0' && ch1 <= '7') {
                        unsigned char ch2 = stream.getu(2);
                        if (ch2 >= '0' && ch2 <= '7') {
                            // [\ddd] : Three octal numbers
                            ch = (ch - '0') * 64 + (ch1 - '0') * 8 + (ch2 - '0');
                            skip = 3;
                        }
                        else {
                            // [\dd] : Two octal numbers
                            ch = (ch - '0') * 8 + (ch1 - '0');
                            skip = 2;
                        }
                    }
                    else {
                        // [\d] : One octal numbers
                        ch = (ch - '0');
                        skip = 1;
                    }
                }
                else if (unescapeType == CharInfo::HexUnescapeChar) {
                    // Hex: '\xFF'
                    if ((stream.remain() >= 3)) {
                        // The first hexadecimal number
                        unsigned char ch1 = stream.getu(1);
                        if (CharInfo::IsHexChars(ch1)) {
                            ch1 = CharInfo::GetHexValue(ch1);

                            // The second hexadecimal number
                            unsigned char ch2 = stream.getu(2);
                            if (CharInfo::IsHexChars(ch2)) {
                                ch2 = CharInfo::GetHexValue(ch2);

                                ch = ch1 * 16 + ch2;
                                skip = 3;
                            }
                            else {
                                // Unescape HexChar error: The second char is not a hex chars.
                                skip = -1;
                            }
                        }
                        else {
                            // Unescape HexChar error: The first char is not a hex chars.
                            skip = -2;
                        }
                    }
                    else {
                        // Unescape HexChar error: remain length less than 3.
                        skip = -3;
                    }
                }
                else {
                    // Unknown unescape type
                    skip = -4;
                }
            }
        }
        else {
            // Unescape error
            skip = -5;
        }
        return skip;
    }

    // Single character literal
    Error parseSingleCharLiteral(std::string & content, TokenInfo & ti) {
        Error ec;
        unsigned char character;
        char ch = scanner_.get();
        if (likely(ch != '\\')) {
            // It's not an non-escaped char
            character = ch;
            scanner_.next();
        }
        else {
            // It's an non-escaped char
            scanner_.next();
            character = scanner_.get();
            int skip = getUnescapedChar(scanner_, character);
            if (likely(skip > 0)) {
                std::string escapedChars;
                getEscapedChars(character, escapedChars);
                std::cout << ">>> Single character: [UnescapedChar] - [" << escapedChars << "]" << std::endl;
                scanner_.next(skip);
            }
            else {
                // Get a unknown unescaped char
                ec = Error::UnknownUnescapedChar;
                goto Parse_Exit;
            }
        }

        if (likely(scanner_.get() == '\'')) {
            std::string escapedChars;
            getEscapedChars(character, escapedChars);
            std::cout << ">>> Single character: [" << escapedChars << "] (" << (int)character << ")" << std::endl;
            scanner_.next();
        }
        else {
            // It's a illegal single character format.
            ec = Error::IllegalSingleCharacterFormat;
            goto Parse_Exit;
        }

        if (ec == Error::Ok) {
            content = character;
        }

Parse_Exit:
        return ec;
    }

    // Normal string literal
    Error parseStringLiteral(std::string & content, TokenInfo & ti) {
        Error ec;
        int multipart_cnt = 0;
        bool completed;

        do {
            completed = false;
            char ch;
            while ((ch = scanner_.get()) != '\0') {
                if (likely(ch != '\"')) {
                    unsigned char character;
                    if (likely(ch != '\\')) {
                        // It's not an non-escaped char
                        character = ch;
                        content.append(1, character);
                        //content.push_back(character);
                        scanner_.next();
                    }
                    else {
                        // It's an non-escaped char
                        scanner_.next();
                        character = scanner_.get();
                        int skip = getUnescapedChar(scanner_, character);
                        if (likely(skip > 0)) {
                            std::string escapedChars;
                            getEscapedChars(character, escapedChars);
                            std::cout << ">>> Single character: [UnescapedChar] - [" << escapedChars << "]" << std::endl;
                            content.append(1, character);
                            //content.push_back(character);
                            scanner_.next(skip);
                        }
                        else {
                            ec = Error::UnknownUnescapedChar;
                            break;
                        }
                    }
                }
                else {
                    if (unlikely(scanner_.get(1) == '\"')) {
                        // It's [""]
                        content.append(1, '\"');
                        //content.push_back('\"');
                        scanner_.next(2);
                    }
                    else {
                        // It's the end of the normal string literal.
                        scanner_.next();
                        multipart_cnt++;
                        completed = true;
                        break;
                    }
                }
            }

            // If it reach the end of file and it's not completed, exit now.
            if (!completed) {
                if (ec.isOk()) {
                    std::cout << ">>> Error: String literal is not completed!" << std::endl;
                    ec = Error::IllegalStringLiteralIsNotCompleted;
                }
                else {
                    std::cout << ">>> Error: String literal unknown error!" << std::endl;
                }
                return ec;
            }

            // Skip the white spaces between the multi-part string literal.
            scanner_.skipWhiteSpaces();

            if (likely(scanner_.get() != '\"')) {
                // It's the end of normal string literal or multi-part string literal.
                break;
            }
            else {
                // Find a multi-part string literal entry, skip the '"' char and continue parse.
                scanner_.next();
            }
        } while (1);

        if (multipart_cnt == 1) {
            std::cout << ">>> String literal = [\n" << content << "\n];" << std::endl;
        }
        else if (multipart_cnt > 1) {
            std::cout << ">>> Multi-part (" << multipart_cnt << " parts) string literal = "
                      << "[\n" << content << "\n];" << std::endl;
        }
        else {
            std::cout << ">>> Error: String literal is not completed. (mismatch \\\") !" << std::endl;
            ec = Error::IllegalStringLiteralIsNotCompleted;
        }
        return ec;
    }

    Error parseNumberLiteral(TokenInfo & ti) {
        Error ec;
        Token token;
        StreamMarker marker(scanner_);

        // It's a radix based number? like "0x", "0o", "0b", "0d" ...
        char ch = scanner_.get();
        if (ch == '0') {
            char ch1 = scanner_.get(1);
            if ((ch1 >= 'B' && ch1 <= 'x') && scanner_.remain() > 2) {
                // Determine the radix for the constant
                int radix;
                uint64_t number;
                ec = parseRadixNumber(token, radix, number);
                if (ec.isOk()) {
                    ti.setToken(token, marker.start(), marker.length());
                }
                return ec;
            }
        }

        // It's a real number? Including integer, float or double number.
        uint64_t integer;
        uint64_t fractional;
        int exponent;
        bool is_float;
        ec = parseRealNumber(token, integer, fractional, exponent, is_float);
        if (ec.isOk()) {
            ti.setToken(token, marker.start(), marker.length());
        }

        return ec;
    }

    Error parseLiteral(std::string & content, TokenInfo & token) {
        Error ec;
        
        if (likely(scanner_.isNumber())) {
            // A integer or a real number.
            // Starting with numbers: "[0-9]", or ".[0-9]"
            return parseNumberLiteral(token);
        }
        else if (likely(scanner_.get() == '\"')) {
            // String literal or single char literal
            scanner_.next();
            return parseStringLiteral(content, token);
        }
        else if (likely(scanner_.get() == '\'')) {
            // Single character literal
            scanner_.next();
            return parseSingleCharLiteral(content, token);
        }
        else if (likely(scanner_.get() == '-')) {
            // Negative sign
            scanner_.next();
            return parseStringLiteral(content, token);
        }
        else if (likely(scanner_.get() == '+')) {
            // Positive sign
            scanner_.next();
            return parseStringLiteral(content, token);
        }

        return ec;
    }

    uint64_t roundAlignedBytes(uint64_t alignedBytes) {
        static const uint64_t kMinAlignedBytes = 16;
        static const uint64_t kAlignedMask = kMinAlignedBytes - 1;
        if (likely(alignedBytes >= kMinAlignedBytes)) {
            if (likely((alignedBytes % kMinAlignedBytes) == 0)) {
                return alignedBytes;
            }
            else {
                alignedBytes = (alignedBytes + kAlignedMask) & (~kAlignedMask);
                return alignedBytes;
            }
        }
        else {
            return kMinAlignedBytes;
        }
    }

    Error handleSectionStatement(Token sectionToken, TokenInfo & ti) {
        Error ec;

        switch (sectionToken.value()) {
        case Token::Align:
            {
                std::cout << ">>> Section [.align] begin." << std::endl;

                // Skip the leading whitespace character first.
                scanner_.skipWhiteSpace();

                if (likely(scanner_.isNumber())) {
ParseAlignBytes_Start:
                    uint64_t alignedBytes = 0;
                    ec = parseDecimalNumber(alignedBytes);
                    if (ec.isOk()) {
                        uint64_t newAlignedBytes = roundAlignedBytes(alignedBytes);
                        if (newAlignedBytes != alignedBytes) {
                            // Have changed to newAlignedBytes from alignedBytes.
                            std::cout << ">>> Section [.align]: alignedBytes have changed to " << newAlignedBytes;
                            std::cout << " from " << alignedBytes << " bytes" << std::endl;
                        }
                        std::cout << ">>> Section [.align]: alignedBytes = " << newAlignedBytes << " bytes" << std::endl;
                    }
                    scanner_.skipWhiteSpaces();
                }
                else if (likely(scanner_.isAlphabet())) {
                    IdentInfo identInfo;
                    TokenInfo identToken;
                    parseIdentifier(identInfo, identToken);
                    if (identInfo.name() == "default") {   // Setting default align bytes.
                        scanner_.skipWhiteSpace();
                        uint8_t ch = scanner_.getu();
                        if (likely(scanner_.isNumber())) {
                            goto ParseAlignBytes_Start;
                        }
                        else {
                            // Got Errors, expect to a decimal integer.
                            ec = Error::Unknown;
                        }
                    }
                }

                std::cout << ">>> Section [.align] end." << std::endl << std::endl;
            }
            break;

        case Token::Strings:
            {
                // Skip the leading whitespace character first
                scanner_.skipWhiteSpace();

                uint8_t ch = scanner_.getu();
                if (likely(ch == '{')) {
                    scanner_.next();

                    std::cout << ">>> Section [.strings] begin." << std::endl;
                    scanner_.skipWhiteSpaces();

ParseStringSection_Entry:
                    IdentInfo identInfo;
                    ec = parseIdentifierStrict(identInfo, ti);
                    if (ec.isOk()) {
                        scanner_.skipWhiteSpace();

                        ch = scanner_.getu();
                        if (likely(ch == '\"')) {
                            scanner_.next();

                            std::string stringValue;
                            ec = parseStringLiteral(stringValue, ti);
                            if (ec.isOk()) {
                                scanner_.skipWhiteSpaces();

                                // Parse next string or end of sign '}'.
                                ch = scanner_.getu();
                                if (likely(scanner_.isIdentifierFirst())) {
                                    // Next string identifier
                                    goto ParseStringSection_Entry;
                                }
                                else if (likely(ch == '}')) {
                                    // End of string section
                                    std::cout << ">>> Section [.strings] end." << std::endl << std::endl;
                                }
                                else {
                                    // Got a error, expect to '}', string section ending.
                                    ec = Error::ExpectingStringSectionEnding;
                                }
                            }
                            else {
                                // Got a error, illegal string literal.
                                ec = Error::IllegalStringLiteral;
                            }
                        }
                        else {
                            // Got a error, expect to '"'.
                            ec = Error::ExpectingStringLiteral;
                        }
                    }
                }
                else {
                    // Got a error, expect to '{', string section beginning.
                    ec = Error::ExpectingStringSectionBeginning;
                }
            }
            break;

        case Token::EntryPoint:
            {
                // Skip the trailing whitespace and newline character
                scanner_.skipWhiteSpaces();
            }
            break;

        default:
            {
                // Unsupported section keyword
                ti.setToken(Token::Unsupported);
                ec = Error::UnsupportedSectionStatement;
            }
            break;
        }
        return ec;
    }

    Error parseImport() {
        Error ec;
        return ec;
    }

    Error parseUsing() {
        Error ec;
        return ec;
    }

    Error parseNameSpace() {
        Error ec;
        return ec;
    }

    Error parseTypeDef() {
        Error ec;
        return ec;
    }

    Error parseClass() {
        Error ec;
        return ec;
    }

    Error parseStruct() {
        Error ec;
        return ec;
    }

    Error parseInterface() {
        Error ec;
        return ec;
    }

    Error parseEnum() {
        Error ec;
        return ec;
    }

    Error parseTemplate() {
        Error ec;
        return ec;
    }

    Error handleScriptKeyword(const Keyword & keyword) {
        Error ec;
        switch (keyword.token()) {
            case Token::Import:
                {
                    scanner_.skipWhiteSpaces();

                    // import std::vector;
                    ec = parseImport();
                }
                break;

            case Token::Using:
                {
                    scanner_.skipWhiteSpaces();

                    // using std::map;
                    // using namespace std;
                    // using xxxx = std::map;
                    ec = parseUsing();
                }
                break;

            case Token::NameSpace:
                {
                    scanner_.skipWhiteSpaces();

                    // namespace abcd {};
                    ec = parseNameSpace();
                }
                break;

            case Token::TypeDef:
                {
                    scanner_.skipWhiteSpaces();

                    // typedef int int32_t;
                    ec = parseTypeDef();
                }
                break;

            case Token::Class:
                {
                    scanner_.skipWhiteSpaces();

                    // class abcd {};
                    ec = parseClass();
                }
                break;

            case Token::Struct:
                {
                    scanner_.skipWhiteSpaces();

                    // struct abcd {};
                    ec = parseStruct();
                }
                break;

            case Token::Interface:
                {
                    scanner_.skipWhiteSpaces();

                    // interface abcd {};
                    ec = parseInterface();
                }
                break;

            case Token::Enum:
                {
                    scanner_.skipWhiteSpaces();

                    // enum abcd {};
                    ec = parseEnum();
                }
                break;

            case Token::Template:
                {
                    scanner_.skipWhiteSpaces();

                    // template <typename T> abcd {};
                    ec = parseTemplate();
                }
                break;

            case Token::Int:
                {
                    scanner_.skipWhiteSpaces();

                    // int foo();
                    IdentInfo identInfo;
                    ec = parseIdentifierDeclaration(keyword, identInfo);
                }
                break;

            default:
                break;
        }
        return ec;
    }

    template <size_t Property = kSkipWhiteSpaces>
    bool nextToken(TokenInfo & ti) {
        bool success = true;
        Error ec;
        StreamMarker marker(scanner_);

        // We needn't use [ !scanner_.is_null() ].
        if (likely(scanner_.has_next())) {
NextToken_Continue:
            Token token;
            marker.remark();
            uint8_t ch = scanner_.getu();
            switch (ch) {
            case '\0':
                // Set 'Eof' token's position first.
                ti.setToken(Token::Eof, scanner_.tell(), 0);
                return false;

            case '\t':  // Whitespace chars and next line
            case '\v':
            case '\f':
            case ' ':
                scanner_.next();
                scanner_.skipWhiteSpace();
                if ((Property & kSkipWhiteSpace) != 0)
                    goto NextToken_Continue;
                else
                    ti.setToken(Token::WhiteSpace, marker);
                break;

            case '\n':
            case '\r':
                scanner_.next();
                scanner_.skipNewLine();
                if ((Property & kSkipWhiteSpace) != 0)
                    goto NextToken_Continue;
                else
                    ti.setToken(Token::NewLine, marker);
                break;

            case '#':   // Preprocessing statement, example: #include <stdio.h>
                scanner_.next();
                ec = parsePreprocessing(ti);
                if (unlikely(!ec.isOk())) {
                    success = false;
                }
                break;

            case '/':   // Line comment or block comment
                {
                    scanner_.next();
                    bool is_comments = parseComment(ti, ec);
                    if (likely(is_comments)) {
                        if (!ec.isOk()) {
                            //getEngine().diagnosisComment(ec, scanner_, token);
                            return false;
                        }
                    }
                    else {
                        if (likely(scanner_.get() != '=')) {
                            // Div token, operator: /
                            ti.setToken(Token::Div);
                        }
                        else {
                            // DivEqual token, operator: /=
                            scanner_.next();
                            ti.setToken(Token::DivEqual, marker);
                        }
                    }
                }
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
                {
                    // Identifier or keyword
                    IdentInfo identInfo;
                    parseIdentifier(identInfo);

                    if (unlikely(!ec.isOk())) {
                        success = false;
                    }

                    ti.setToken(Token::Identifier, marker);
                }
                break;

            case '0':
                {
                    char ch1 = scanner_.get(1);
                    if ((ch1 >= 'B' && ch1 <= 'x') && scanner_.remain() > 2) {
                        // Determine the radix for the constant
                        int radix;
                        uint64_t number;
                        ec = parseRadixNumber(token, radix, number);
                        if (ec.isOk()) {
                            ti.setToken(token, marker);
                            return true;
                        }
                    }

                    // It's a real number? Including integer, float or double number.
                    uint64_t integer;
                    uint64_t fractional;
                    int exponent;
                    bool is_float;
                    ec = parseRealNumber(token, integer, fractional, exponent, is_float);
                    if (ec.isOk()) {
                        ti.setToken(token, marker);
                        return true;
                    }

                    scanner_.next();
                    ti.setToken(Token::IntegerLiteral, marker);
                }
                break;

            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                {
                    // It's a real number? Including integer, float or double number.
                    uint64_t integer;
                    uint64_t fractional;
                    int exponent;
                    bool is_float;
                    ec = parseRealNumber(token, integer, fractional, exponent, is_float);
                    if (ec.isOk()) {
                        ti.setToken(token, marker);
                        return true;
                    }
                    ti.setToken(Token::IntegerLiteral, marker);
                }
                break;

            case '.':
                scanner_.next();
                {
                    ch = scanner_.get();
                    if (scanner_.isAlphabet(ch)) {
                        // It's a section declare
                        IdentInfo sectionInfo;
                        sectionInfo.setName(".");
                        parseIdentifier(sectionInfo, ti);

                        KeywordMapping & sectionMapping = Global::getSectionMapping();
                        auto iter = sectionMapping.find(sectionInfo.name());
                        if (iter != sectionMapping.end()) {
                            Keyword section = iter->second;
                            ec = handleSectionStatement(section.getType(), ti);
                            if (ec.isOk()) {
                                // success
                            }
                        }
                    }
                    else if (scanner_.isDigital(ch)) {
                        // It's a float or double number.
                        uint64_t fractional;
                        int exponent;
                        ec = parseRealNumberSuffix(token, fractional, exponent);
                        if (ec.isOk()) {
                            ti.setToken(token, marker);
                            return true;
                        }
                    }
                    else {
                        ti.setToken(Token::Dot, marker);
                    }
                }
                break;

            case '!':
                scanner_.next();
                if (scanner_.get() != '=') {
                    // BoolNot token, operator: !
                    ti.setToken(Token::BoolNot, marker);
                }
                else {
                    // operator: !=
                    scanner_.next();
                    // NotEqual token
                    ti.setToken(Token::NotEqual, marker);
                }
                break;

            case '%':
                scanner_.next();
                if (scanner_.get() != '=') {
                    // Mod token, operator: %
                    ti.setToken(Token::Mod, marker);
                }
                else {
                    // operator: %=
                    scanner_.next();
                    // ModEqual token
                    ti.setToken(Token::ModEqual, marker);
                }
                break;

            case '&':
                scanner_.next();
                if (scanner_.get() != '=') {
                    if (scanner_.get() != '&') {
                        // BitAnd token, operator: &
                        ti.setToken(Token::BitAnd, marker);
                    }
                    else {
                        // BoolAnd token, operator: &&
                        ti.setToken(Token::BoolAnd, marker);
                    }
                }
                else {
                    // operator: &=
                    scanner_.next();
                    // AndEqual token
                    ti.setToken(Token::AndEqual, marker);
                }
                break;

            case '*':
                scanner_.next();
                if (scanner_.get() != '=') {
                    // Multiply token, operator: *
                    ti.setToken(Token::Multiply, marker);
                }
                else {
                    // operator: *=
                    scanner_.next();
                    // MultiplyEqual token
                    ti.setToken(Token::MultiplyEqual, marker);
                }
                break;

            case '+':
                scanner_.next();
                if (scanner_.get() != '=') {
                    if (scanner_.get() != '+') {
                        // Plus token, operator: +
                        ti.setToken(Token::Add, marker);
                    }
                    else {
                        // Increase token, operator: ++
                        ti.setToken(Token::Increase, marker);
                    }
                }
                else {
                    // operator: +=
                    scanner_.next();
                    // PlusEqual token
                    ti.setToken(Token::AddEqual, marker);
                }
                break;

            case '-':
                scanner_.next();
                if (scanner_.get() != '=') {
                    if (scanner_.get() == '>') {
                        // Pointer token, operator: ->
                        ti.setToken(Token::Pointer, marker);
                    }
                    else if (scanner_.get() != '-') {
                        // Minus token, operator: -
                        ti.setToken(Token::Sub, marker);
                    }
                    else {
                        // Decrease token, operator: --
                        ti.setToken(Token::Decrease, marker);
                    }
                }
                else {
                    // operator: -=
                    scanner_.next();
                    // MinusEqual token
                    ti.setToken(Token::SubEqual, marker);
                }
                break;

            case ':':
                scanner_.next();
                if (scanner_.get() == ':') {
                    // operator: ::
                    scanner_.next();
                    // Scope token
                    ti.setToken(Token::Scope, marker);
                }
                else {
                    // Colon token, operator: :
                    ti.setToken(Token::Colon, marker);
                }
                break;

            case '=':
                scanner_.next();
                if (scanner_.get() != '=') {
                    // Assignment token, operator: =
                    ti.setToken(Token::Assignment, marker);
                }
                else {
                    // operator: ==
                    scanner_.next();
                    // Equal token
                    ti.setToken(Token::Equal, marker);
                }
                break;

            case '?':
                scanner_.next();
                // Question token, operator: ?
                ti.setToken(Token::Question, marker);
                break;

            case '^':
                scanner_.next();
                if (scanner_.get() != '^') {
                    if (scanner_.get() != '=') {
                        // BitXor token, operator: ^
                        ti.setToken(Token::BitXor, marker);
                    }
                    else {
                        // XorEqual token, operator: ^=
                        ti.setToken(Token::XorEqual, marker);
                    }
                }
                else {
                    // operator: ^^
                    scanner_.next();
                    // BoolXor token
                    ti.setToken(Token::BoolXor, marker);
                }
                break;

            case '|':
                scanner_.next();
                if (scanner_.get() != '|') {
                    if (scanner_.get() != '=') {
                        // BitOr token, operator: |
                        ti.setToken(Token::BitOr, marker);
                    }
                    else {
                        // OrEqual token, operator: |=
                        ti.setToken(Token::OrEqual, marker);
                    }
                }
                else {
                    // operator: ||
                    scanner_.next();
                    // BoolOr token
                    ti.setToken(Token::BoolOr, marker);
                }
                break;

            case '~':
                scanner_.next();
                if (scanner_.get() != '~') {
                    if (scanner_.get() != '=') {
                        // BitNot token, operator: ~
                        ti.setToken(Token::BitNot);
                    }
                    else {
                        // NotEqual token, operator: ~=
                        ti.setToken(Token::BitNotEqual);
                    }
                }
                else {
                    // operator: ~~
                    scanner_.next();
                    // BoolNot token
                    ti.setToken(Token::BoolNot, marker);
                }
                break;

            case '@':
                // Is a annotation?
                scanner_.next();
                ti.setToken(Token::Handle, marker);
                break;

            case ',':
                scanner_.next();
                ti.setToken(Token::Comma, marker);
                break;

            case ';':
                scanner_.next();
                ti.setToken(Token::Semicolon, marker);
                break;

            case '(':
                scanner_.next();
                ti.setToken(Token::LParen, marker);
                break;

            case ')':
                scanner_.next();
                ti.setToken(Token::RParen, marker);
                break;

            case '[':
                scanner_.next();
                ti.setToken(Token::LBracket, marker);
                break;

            case ']':
                scanner_.next();
                ti.setToken(Token::RBracket, marker);
                break;

            case '{':
                scanner_.next();
                ti.setToken(Token::LBrace, marker);
                break;

            case '}':
                scanner_.next();
                ti.setToken(Token::RBrace, marker);
                break;

            case '\'':  // Single character literal
                {
                    scanner_.next();
                    std::string singelChar;
                    ec = parseSingleCharLiteral(singelChar, ti);
                    if (unlikely(!ec.isOk())) {
                        success = false;
                    }
                }
                break;

            case '\"':  // String literal or single char literal
                {
                    scanner_.next();
                    std::string stringLiteral;
                    ec = parseStringLiteral(stringLiteral, ti);
                    if (unlikely(!ec.isOk())) {
                        success = false;
                    }
                }
                break;

            default:    // Internal keywords
                {
                    ec = parseReservedKeyword(ti);
                    if (ec.isOk()) {
                        //scanner_.next();
                        ti.setToken(Token::Keyword);
                    }
                    else {
                        ti.setToken(Token::Unrecognized);
                        if (marker.length() <= 0)
                            scanner_.next();
                    }
                }
                break;
            }

            ti.setStart(marker.start());
            ti.setLength(marker.length());
            
        }

        if (likely(!scanner_.has_next())) {
            ti.setToken(Token::Eof, scanner_.tell(), 0);
        }

        return true;
    }

    bool parseToken(TokenInfo & ti, Error & ec_) {
        Error ec = Error::Ok;
        StreamMarker marker(scanner_);
        // We needn't use [ !scanner_.is_null() ].
        while (likely(scanner_.has_next())) {
            marker.remark();
            Token token;
            uint8_t ch = scanner_.getu();
            switch (ch) {
            case '\0':
                // Set 'Eof' token's position first.
                ti.setToken(Token::Eof, scanner_.tell(), 0);
                scanner_.next();
                return false;

            case '\t':  // Whitespace chars and next line
            case '\v':
            case '\f':
            case ' ':
                scanner_.next();
                scanner_.skipWhiteSpace();
                ti.setToken(Token::WhiteSpace);
                break;

            case '\n':
            case '\r':
                scanner_.next();
                scanner_.skipNewLine();
                ti.setToken(Token::NewLine);
                break;

            case '#':   // Preprocessing statement, example: #include <stdio.h>
                scanner_.next();
                ec = parsePreprocessing(ti);
                if (unlikely(!ec.isOk())) {
                    scanner_.next();
                }
                break;

            case '/':   // Line comment or block comment
                {
                    scanner_.next();
                    bool is_comments = parseComment(ti, ec);
                    if (unlikely(!is_comments)) {
                        if (scanner_.get(0) != '=') {
                            // Div token, operator: /
                            ti.setToken(Token::Div);
                        }
                        else {
                            // DivEqual token, operator: /=
                            scanner_.next();
                            ti.setToken(Token::DivEqual);
                        }
                    }
                    else {
                        if (!ec.isOk()) {
                            //this->getEngine().diagnosisComment(ec, scanner_, token);
                            return false;
                        }
                    }
                }
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
                {
                    // Identifier or keyword
                    ec = parseIdentifierOrKeyword(ti);
                    if (unlikely(!ec.isOk())) {
                        scanner_.next();
                    }
                }
                break;

            case '0':
                {
                    char ch1 = scanner_.get(1);
                    if ((ch1 >= 'B' && ch1 <= 'x') && scanner_.remain() > 2) {
                        // Determine the radix for the constant
                        int radix;
                        uint64_t number;
                        ec = parseRadixNumber(token, radix, number);
                        if (ec.isOk()) {
                            ti.setToken(token, marker.start(), marker.length());
                            return true;
                        }
                    }

                    // It's a real number? Including integer, float or double number.
                    uint64_t integer;
                    uint64_t fractional;
                    int exponent;
                    bool is_float;
                    ec = parseRealNumber(token, integer, fractional, exponent, is_float);
                    if (ec.isOk()) {
                        ti.setToken(token, marker.start(), marker.length());
                        return true;
                    }

                    scanner_.next();
                    ti.setToken(Token::IntegerLiteral);
                }
                break;

            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                {
                    // It's a real number? Including integer, float or double number.
                    uint64_t integer;
                    uint64_t fractional;
                    int exponent;
                    bool is_float;
                    ec = parseRealNumber(token, integer, fractional, exponent, is_float);
                    if (ec.isOk()) {
                        ti.setToken(token, marker.start(), marker.length());
                        return true;
                    }
                    ti.setToken(Token::IntegerLiteral);
                }
                break;

            case '.':
                scanner_.next();
                {
                    if (scanner_.isAlphabet()) {
                        // It's a section declare
                        IdentInfo sectionInfo;
                        sectionInfo.setName(".");
                        parseIdentifier(sectionInfo, ti);

                        KeywordMapping & sectionMapping = Global::getSectionMapping();
                        auto iter = sectionMapping.find(sectionInfo.name());
                        if (iter != sectionMapping.end()) {
                            Keyword section = iter->second;
                            ec = handleSectionStatement(section.getType(), ti);
                            if (ec.isOk()) {
                                // success
                            }
                        }
                    }
                    else if (scanner_.isDigital()) {
                        // It's a float or double number.
                        uint64_t fractional;
                        int exponent;
                        ec = parseRealNumberSuffix(token, fractional, exponent);
                        if (ec.isOk()) {
                            ti.setToken(token, marker.start(), marker.length());
                            return true;
                        }
                    }
                    else {
                        ti.setToken(Token::Dot);
                    }
                }
                break;

            case '!':
                scanner_.next();
                if (scanner_.get() != '=') {
                    // BoolNot token, operator: !
                    ti.setToken(Token::BoolNot);
                }
                else {
                    // operator: !=
                    scanner_.next();
                    // NotEqual token
                    ti.setToken(Token::NotEqual);
                }
                break;

            case '%':
                scanner_.next();
                if (scanner_.get() != '=') {
                    // Mod token, operator: %
                    ti.setToken(Token::Mod);
                }
                else {
                    // operator: %=
                    scanner_.next();
                    // ModEqual token
                    ti.setToken(Token::ModEqual);
                }
                break;

            case '&':
                scanner_.next();
                if (scanner_.get() != '=') {
                    if (scanner_.get() != '&') {
                        // BitAnd token, operator: &
                        ti.setToken(Token::BitAnd);
                    }
                    else {
                        // BoolAnd token, operator: &&
                        ti.setToken(Token::BoolAnd);
                    }
                }
                else {
                    // operator: &=
                    scanner_.next();
                    // AndEqual token
                    ti.setToken(Token::AndEqual);
                }
                break;

            case '*':
                scanner_.next();
                if (scanner_.get() != '=') {
                    // Multiply token, operator: *
                    ti.setToken(Token::Multiply);
                }
                else {
                    // operator: *=
                    scanner_.next();
                    // MultiplyEqual token
                    ti.setToken(Token::MultiplyEqual);
                }
                break;

            case '+':
                scanner_.next();
                if (scanner_.get() != '=') {
                    if (scanner_.get() != '+') {
                        // Plus token, operator: +
                        ti.setToken(Token::Add);
                    }
                    else {
                        // Increase token, operator: ++
                        ti.setToken(Token::Increase);
                    }
                }
                else {
                    // operator: +=
                    scanner_.next();
                    // PlusEqual token
                    ti.setToken(Token::AddEqual);
                }
                break;

            case '-':
                scanner_.next();
                if (scanner_.get() != '=') {
                    if (scanner_.get() == '>') {
                        // Pointer token, operator: ->
                        ti.setToken(Token::Pointer);
                    }
                    else if (scanner_.get() != '-') {
                        // Minus token, operator: -
                        ti.setToken(Token::Sub);
                    }
                    else {
                        // Decrease token, operator: --
                        ti.setToken(Token::Decrease);
                    }
                }
                else {
                    // operator: -=
                    scanner_.next();
                    // MinusEqual token
                    ti.setToken(Token::SubEqual);
                }
                break;

            case ':':
                scanner_.next();
                if (scanner_.get() == ':') {
                    // operator: ::
                    scanner_.next();
                    // Scope token
                    ti.setToken(Token::Scope);
                }
                else {
                    // Colon token, operator: :
                    ti.setToken(Token::Colon);
                }
                break;

            case '=':
                scanner_.next();
                if (scanner_.get() != '=') {
                    // Assignment token, operator: =
                    ti.setToken(Token::Assignment);
                }
                else {
                    // operator: ==
                    scanner_.next();
                    // Equal token
                    ti.setToken(Token::Equal);
                }
                break;

            case '?':
                scanner_.next();
                // Question token, operator: ?
                ti.setToken(Token::Question);
                break;

            case '^':
                scanner_.next();
                if (scanner_.get() != '^') {
                    if (scanner_.get() != '=') {
                        // BitXor token, operator: ^
                        ti.setToken(Token::BitXor);
                    }
                    else {
                        // XorEqual token, operator: ^=
                        ti.setToken(Token::XorEqual);
                    }
                }
                else {
                    // operator: ^^
                    scanner_.next();
                    // BoolXor token
                    ti.setToken(Token::BoolXor);
                }
                break;

            case '|':
                scanner_.next();
                if (scanner_.get() != '|') {
                    if (scanner_.get() != '=') {
                        // BitOr token, operator: |
                        ti.setToken(Token::BitOr);
                    }
                    else {
                        // OrEqual token, operator: |=
                        ti.setToken(Token::OrEqual);
                    }
                }
                else {
                    // operator: ||
                    scanner_.next();
                    // BoolOr token
                    ti.setToken(Token::BoolOr);
                }
                break;

            case '~':
                scanner_.next();
                if (scanner_.get() != '~') {
                    if (scanner_.get() != '=') {
                        // BitNot token, operator: ~
                        ti.setToken(Token::BitNot);
                    }
                    else {
                        // NotEqual token, operator: ~=
                        ti.setToken(Token::BitNotEqual);
                    }
                }
                else {
                    // operator: ~~
                    scanner_.next();
                    // BoolNot token
                    ti.setToken(Token::BoolNot);
                }
                break;

            case '@':
                // Is a annotation?
                scanner_.next();
                ti.setToken(Token::Annotation);
                break;

            case ',':
                scanner_.next();
                ti.setToken(Token::Comma);
                break;

            case ';':
                scanner_.next();
                ti.setToken(Token::Semicolon);
                break;

            case '(':
                scanner_.next();
                ti.setToken(Token::LParen);
                break;

            case ')':
                scanner_.next();
                ti.setToken(Token::RParen);
                break;

            case '[':
                scanner_.next();
                ti.setToken(Token::LBracket);
                break;

            case ']':
                scanner_.next();
                ti.setToken(Token::RBracket);
                break;

            case '{':
                scanner_.next();
                ti.setToken(Token::LBrace);
                break;

            case '}':
                scanner_.next();
                ti.setToken(Token::RBrace);
                break;

            case '\'':  // Single character literal
                {
                    scanner_.next();
                    std::string singelChar;
                    ec = parseSingleCharLiteral(singelChar, ti);
                    if (unlikely(!ec.isOk())) {
                        scanner_.next();
                    }
                }
                break;

            case '\"':  // String literal or single char literal
                {
                    scanner_.next();
                    std::string stringLiteral;
                    ec = parseStringLiteral(stringLiteral, ti);
                    if (unlikely(!ec.isOk())) {
                        scanner_.next();
                    }
                }
                break;

            default:    // Internal keywords
                {
                    ec = parseReservedKeyword(ti);
                    if (ec.isOk()) {
                        //scanner_.next();
                        ti.setToken(Token::Keyword);
                    }
                    else {
                        ti.setToken(Token::Unrecognized);
                        if (marker.length() <= 0)
                            scanner_.next();
                    }
                }
                break;
            }

            ti.setStart(marker.start());
            ti.setLength(marker.length());
            assert(ti.length() > 0);
            if (ti.token() != Token::Unknown &&
                ti.token() != Token::Unrecognized) {
                break;
            }
        }

        if (likely(!scanner_.has_next())) {
            ti.setToken(Token::Eof, scanner_.tell(), 0);
        }

        ec_ = ec;
        return (ec == Error::Ok);
    }

    // EBNF: Script = { Import | Using | Include | NameSpace | TypeDef | Class | Struct | Enum | Interface |
    //                  Template | Preprocessing | Comment | Function | FunctionDeclaration |
    //                  VariableDeclaration | IdentifierDeclaration |
    //                  ';' }
    Error parseScript(bool inBlock = false) {
        Error ec;
        StreamMarker marker(scanner_, false);
        TokenInfo ti;

        do {
            scanner_.skipWhiteSpaces();

            marker.remark();
            uint8_t ch = scanner_.get();
            if (likely(scanner_.isIdentifierFirst(ch))) {
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
            }
            else if (likely(ch == '.')) {
                // Section statement
                scanner_.next();

                IdentInfo identInfo;
                parseIdentifier(identInfo);

                if (likely(identInfo.length() > 0)) {
                    Keyword * keyword = identInfo.getKeyword();
                    if (likely(keyword != nullptr)) {
                        ec = handleSectionStatement(keyword->token(), ti);
                    }
                    else {
                        // The keyword has not Found.
                    }
                }
            }
            else if (likely(ch == '/')) {
                // Comment statement
                scanner_.next();

                bool is_comment = parseComment(ti, ec);
            }
            else if (likely(ch == '#')) {
                // Preprocessing statement
                scanner_.next();
            }
            else if (likely(ch == ';')) {
                // Semicolon
                scanner_.next();
            }
            else if (likely(ch == '\0' || !scanner_.has_next())) {
                // Eof
                break;
            }
            else {
                // Error
                break;
            }

            if (ec.isError()) {
                break;
            }
        } while (1);

        return ec;
    }

    Error parse() {
        Error ec = parseScript();
        return ec;
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_PARSER_H
