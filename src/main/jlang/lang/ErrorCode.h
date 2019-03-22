
#ifndef JLANG_LANG_ERRORCODE_H
#define JLANG_LANG_ERRORCODE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <string>

namespace jlang {

///////////////////////////////////////////////////
// class ErrorCode
///////////////////////////////////////////////////

class ErrorCode {
public:
    enum Type {
        First = -60000,
        ErrorFirst = -30000,

        // FileSystem
        IllegalPathOrFilename,

        // Preprocessing statement errors
        IllegalPreprocessingKeyword,
        UnknownPreprocessingKeyword,

        // Include file errors
        IncludeFile_UnknownQuote,
        IncludeFile_SuffixQuoteMismatch,
        IllegalPath,
        IllegalDirectory,
        IllegalFilename,
        IllegalDirectoryOrFilename,
        IllegalDirectoryOrFilenameCharacters,

        // Macro errors
        IllegalMacroDefineName,
        IllegalMacroDefineBody,

        // Standard errors
        IllegalDoubleDot,
        IllegalFourDot,

        // Comment errors
        IllegalLineComment,
        IllegalBlockComment,
        IllegalNestedBlockComment,
        IllegalCommentIsNotCompleted,

        // Constant errors
        IllegalNumber,
        IllegalRadixNumber,
        IllegalRadix2Number,
        IllegalRadix8Number,
        IllegalRadix10Number,
        IllegalRadix16Number,
        IllegalFloatingNumber,
        IllegalFloatNumber,
        IllegalDoubleNumber,

        // Real number errors
        ErrorIntegerPartOverflow,
        ErrorFractionalPartOverflow,
        ErrorExponentPartOverflow,
        ErrorNegativeIntegerPartOverflow,
        ErrorNegativeFractionalPartOverflow,
        ErrorNegativeExponentPartOverflow,
        ErrorExponentPartOutOfRange,
        IllegalExponentPart,

        // String constant errors

        // Pod type
        IllegalPodType,
        UnknownPodType,
        UnsupportPodType,

        // Single character constant errors
        IllegalSingleCharacterFormat,
        UnknownEscapedChar,
        UnknownUnescapedChar,

        // String literal
        ExpectingStringLiteral,
        IllegalStringLiteral,
        IllegalStringLiteralIsNotCompleted,

        // String section
        UnknownSectionStatement,
        IllegalStringSection,
        ExpectingStringSectionBeginning,
        ExpectingStringSectionEnding,
        IllegalStringSectionIdentifier,
        IllegalStringSectionStringLiteral,

        // Statements
        IllegalStatement,

        // Identifer
        IllegalIdentifer,

        // Function argument list
        IllegalArgumentType,
        IllegalArgumentName,
        IllegalArgumentDelimiter,

        // Function body
        IllegalFunctionBody,

        // Standard errors
        Error_NullPtr,

        // vmThread
        Thread_Not_Create,
        MainProcess_Create_Failed,
        MainThread_Create_Failed,

        // vmBinary
        BinaryFile_Read_Failed,

        // Standard errors
        UnknownError = -2,
        Failed = -1,
        OK = 0,
        Success = OK,
        ErrorInsufficientBuffer =  122L,
        Status_AlreadyExists = 256,
        Last
    };

protected:
    Type ec_;

public:
    ErrorCode() : ec_(Type::OK) {}
    ErrorCode(Type ec) : ec_(ec) {}
    ErrorCode(int32_t ec) : ec_(static_cast<Type>(ec)) {}
    ErrorCode(uint32_t ec) : ec_(static_cast<Type>(ec)) {}
    ErrorCode(int64_t ec) : ec_(static_cast<Type>(ec)) {}
    ErrorCode(uint64_t ec) : ec_(static_cast<Type>(ec)) {}
    ErrorCode(const ErrorCode & src) : ec_(src.ec_) {}
    ~ErrorCode() {}

    bool isOK() const {
        return (this->ec_ == Type::OK);
    }

    bool isSuccess() const {
        return (this->ec_ == Type::Success);
    }

    bool hasErrors() const {
        return (this->ec_ != Type::OK);
    }

    ErrorCode & operator = (const ErrorCode & src) {
        this->ec_ = src.ec_;
        return *this;
    }

    ErrorCode & operator = (Type ec) {
        this->ec_ = ec;
        return *this;
    }

    ErrorCode & operator = (int32_t ec) {
        this->ec_ = static_cast<Type>(this->ec_);;
        return *this;
    }

    ErrorCode & operator = (uint32_t ec) {
        this->ec_ = static_cast<Type>(this->ec_);;
        return *this;
    }

    ErrorCode & operator = (int64_t ec) {
        this->ec_ = static_cast<Type>(this->ec_);;
        return *this;
    }

    ErrorCode & operator = (uint64_t ec) {
        this->ec_ = static_cast<Type>(this->ec_);;
        return *this;
    }

    operator int32_t () {
        return static_cast<int32_t>(this->ec_);
    }

    operator uint32_t () {
        return static_cast<uint32_t>(this->ec_);
    }

    operator int64_t () {
        return static_cast<int64_t>(this->ec_);
    }

    operator uint64_t () {
        return static_cast<uint64_t>(this->ec_);
    }

    ErrorCode::Type get() const { return ec_; }
    void set(Type ec) { ec_ = ec; }

    void setValue(int32_t ec)  { ec_ = static_cast<Type>(ec); }
    void setValue(uint32_t ec) { ec_ = static_cast<Type>(ec); }
    void setValue(int64_t ec)  { ec_ = static_cast<Type>(ec); }
    void setValue(uint64_t ec) { ec_ = static_cast<Type>(ec); }

    int32_t value() const { return static_cast<int32_t>(this->ec_); }

    bool isEquals(ErrorCode ec) const      { return (this->value() == ec.value()); }
    bool isEquals(int32_t ec) const        { return (this->value() == ec); }
    bool isEquals(int64_t ec) const        { return (this->value() == static_cast<int32_t>(ec)); }

    bool isLessThan(ErrorCode ec) const    { return (this->value() <  ec.value()); }
    bool isLessThan(int32_t ec) const      { return (this->value() <  ec); }
    bool isLessThan(int64_t ec) const      { return (this->value() <  static_cast<int32_t>(ec)); }

    bool isGreaterThan(ErrorCode ec) const { return (this->value() >  ec.value()); }
    bool isGreaterThan(int32_t ec) const   { return (this->value() >  ec); }
    bool isGreaterThan(int64_t ec) const   { return (this->value() >  static_cast<int32_t>(ec)); }

    friend bool operator == (const ErrorCode & lhs, const ErrorCode & rhs) { return lhs.isEquals(rhs);   }
    friend bool operator <  (const ErrorCode & lhs, const ErrorCode & rhs) { return lhs.isLessThan(rhs); }

    friend bool operator != (const ErrorCode & lhs, const ErrorCode & rhs) { return !(lhs == rhs); }
    friend bool operator >  (const ErrorCode & lhs, const ErrorCode & rhs) { return  (rhs <  lhs); }
    friend bool operator <= (const ErrorCode & lhs, const ErrorCode & rhs) { return !(lhs >  rhs); }
    friend bool operator >= (const ErrorCode & lhs, const ErrorCode & rhs) { return !(lhs <  rhs); }

    friend bool operator == (const ErrorCode & lhs,           int32_t rhs) { return  lhs.isEquals(rhs); }
    friend bool operator == (          int32_t lhs, const ErrorCode & rhs) { return  rhs.isEquals(lhs); }
    friend bool operator == (const ErrorCode & lhs,           int64_t rhs) { return  lhs.isEquals(rhs); }
    friend bool operator == (          int64_t lhs, const ErrorCode & rhs) { return  rhs.isEquals(lhs); }

    friend bool operator != (const ErrorCode & lhs,           int32_t rhs) { return !lhs.isEquals(rhs); }
    friend bool operator != (          int32_t lhs, const ErrorCode & rhs) { return !rhs.isEquals(lhs); }
    friend bool operator != (const ErrorCode & lhs,           int64_t rhs) { return !lhs.isEquals(rhs); }
    friend bool operator != (          int64_t lhs, const ErrorCode & rhs) { return !rhs.isEquals(lhs); }

    friend bool operator <  (const ErrorCode & lhs,           int32_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator <  (          int32_t lhs, const ErrorCode & rhs) { return  rhs.isGreaterThan(lhs); }
    friend bool operator <  (const ErrorCode & lhs,           int64_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator <  (          int64_t lhs, const ErrorCode & rhs) { return  rhs.isGreaterThan(lhs); }

    friend bool operator >  (const ErrorCode & lhs,           int32_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator >  (          int32_t lhs, const ErrorCode & rhs) { return  rhs.isLessThan(lhs);    }
    friend bool operator >  (const ErrorCode & lhs,           int64_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator >  (          int64_t lhs, const ErrorCode & rhs) { return  rhs.isLessThan(lhs);    }

    friend bool operator <= (const ErrorCode & lhs,           int32_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator <= (          int32_t lhs, const ErrorCode & rhs) { return  rhs.isLessThan(lhs);    }
    friend bool operator <= (const ErrorCode & lhs,           int64_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator <= (          int64_t lhs, const ErrorCode & rhs) { return  rhs.isLessThan(lhs);    }

    friend bool operator >= (const ErrorCode & lhs,           int32_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator >= (          int32_t lhs, const ErrorCode & rhs) { return  rhs.isGreaterThan(lhs); }
    friend bool operator >= (const ErrorCode & lhs,           int64_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator >= (          int64_t lhs, const ErrorCode & rhs) { return  rhs.isGreaterThan(lhs); }

    static const char * format(Type ec) {
        switch (ec) {
            case Type::UnknownError:
                return "Unknown";
            default:
                break;
        }
        return "Undefined Error";
    }

    char * c_str() {
        return (char *)ErrorCode::format(this->ec_);
    }

    const char * c_str() const {
        return ErrorCode::format(this->ec_);
    }

    std::string toString() {
        return std::string(ErrorCode::format(this->ec_));
    }

    const std::string toString() const {
        return std::string(ErrorCode::format(this->ec_));
    }
};

} // namespace jlang

#endif // JLANG_LANG_ERRORCODE_H
