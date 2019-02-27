
#ifndef JLANG_LANG_ERRORCODE_H
#define JLANG_LANG_ERRORCODE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/NonCopyable.h"

#include <assert.h>

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

        // Single character constant errors
        IllegalSingleCharacterFormat,
        UnknownEscapedChar,
        UnknownUnescapedChar,

        // Standard errors
        Error_NullPtr,

        // vmThread
        Thread_Not_Create,
        MainProcess_Create_Failed,
        MainThread_Create_Failed,

        // vmBinary
        BinaryFile_Read_Failed,

        // Standard errors
        Unknown = -2,
        Failed = -1,
        OK = 0,
        Success = OK,
        ErrorInsufficientBuffer =  122L,
        Status_AlreadyExists = 256,
        Last
    };

private:
    Type ec_;

public:
    ErrorCode() : ec_(Type::Unknown) {}
    ErrorCode(int ec) : ec_(static_cast<Type>(ec)) {}
    ErrorCode(Type ec) : ec_(ec) {}
    ErrorCode & operator = (const ErrorCode & src) {
        this->ec_ = src.ec_;
        return *this;
    }
    ErrorCode & operator = (Type ec) {
        this->ec_ = ec;
        return *this;
    }
    ~ErrorCode() {}

    bool isOK() const {
        return (this->ec_ == Type::OK);
    }

    bool isSuccess() const {
        return (this->ec_ == Type::Success);
    }

    bool operator != (Type ec) {
        return (this->ec_ != ec);
    }
    bool operator == (Type ec) {
        return (this->ec_ == ec);
    }
    bool operator < (Type ec) {
        return (this->ec_ < ec);
    }
    bool operator > (Type ec) {
        return (this->ec_ > ec);
    }
    bool operator <= (Type ec) {
        return (this->ec_ <= ec);
    }
    bool operator >= (Type ec) {
        return (this->ec_ >= ec);
    }

    ErrorCode::Type get() const { return ec_; }
    void set(int ec) { ec_ = static_cast<Type>(ec); }
    void set(Type ec) { ec_ = ec; }

    ErrorCode::Type value() const { return ec_; }

    const char * toString() const {
        switch (ec_) {
            case Type::Unknown:
                return "Unknown";
            default:
                break;
        }
        return "Undefined Error";
    }
};

} // namespace jlang

#endif // JLANG_LANG_ERRORCODE_H
