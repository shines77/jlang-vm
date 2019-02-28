
#ifndef JLANG_LANG_ERRORCODE_H
#define JLANG_LANG_ERRORCODE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

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

    bool isEquals(ErrorCode ec) const    { return (this->value() == ec.value()); }
    bool isNotEquals(ErrorCode ec) const { return (this->value() != ec.value()); }

    bool isLessThan(ErrorCode ec) const    { return (this->value() < ec.value()); }
    bool isGreaterThan(ErrorCode ec) const { return (this->value() > ec.value()); }

    bool isLessThanAndEqual(ErrorCode ec) const    { return (this->value() <= ec.value()); }
    bool isGreaterThanAndEqual(ErrorCode ec) const { return (this->value() >= ec.value()); }

    bool isEquals(int32_t ec) const    { return (this->value() == ec); }
    bool isNotEquals(int32_t ec) const { return (this->value() != ec); }

    bool isLessThan(int32_t ec) const    { return (this->value() < ec); }
    bool isGreaterThan(int32_t ec) const { return (this->value() > ec); }

    bool isLessThanAndEqual(int32_t ec) const    { return (this->value() <= ec); }
    bool isGreaterThanAndEqual(int32_t ec) const { return (this->value() >= ec); }

    bool isEquals(int64_t ec) const    { return (this->value() == static_cast<int>(ec)); }
    bool isNotEquals(int64_t ec) const { return (this->value() != static_cast<int>(ec)); }

    bool isLessThan(int64_t ec) const    { return (this->value() < static_cast<int>(ec)); }
    bool isGreaterThan(int64_t ec) const { return (this->value() > static_cast<int>(ec)); }

    bool isLessThanAndEqual(int64_t ec) const    { return (this->value() <= static_cast<int>(ec)); }
    bool isGreaterThanAndEqual(int64_t ec) const { return (this->value() >= static_cast<int>(ec)); }

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

    static const char * toString(Type ec) {
        switch (ec) {
            case Type::Unknown:
                return "Unknown";
            default:
                break;
        }
        return "Undefined Error";
    }

    char * toString() {
        return (char *)ErrorCode::toString(this->ec_);
    }

    const char * toString() const {
        return ErrorCode::toString(this->ec_);
    }

    void toString(std::string & str) {
        str = this->toString();
    }
};

// operator == 

inline bool operator == (const ErrorCode & lhs, const ErrorCode & rhs) {
    return lhs.isEquals(rhs);
}

inline bool operator == (int lhs, const ErrorCode & rhs) {
    return rhs.isEquals(lhs);
}

inline bool operator == (const ErrorCode & lhs, int rhs) {
    return lhs.isEquals(rhs);
}

inline bool operator == (int64_t lhs, const ErrorCode & rhs) {
    return rhs.isEquals(lhs);
}

inline bool operator == (const ErrorCode & lhs, int64_t rhs) {
    return lhs.isEquals(rhs);
}

// operator !=

inline bool operator != (const ErrorCode & lhs, const ErrorCode & rhs) {
    return lhs.isNotEquals(rhs);
}

inline bool operator != (int lhs, const ErrorCode & rhs) {
    return rhs.isNotEquals(lhs);
}

inline bool operator != (const ErrorCode & lhs, int rhs) {
    return lhs.isNotEquals(rhs);
}

inline bool operator != (int64_t lhs, const ErrorCode & rhs) {
    return rhs.isNotEquals(lhs);
}

inline bool operator != (const ErrorCode & lhs, int64_t rhs) {
    return lhs.isNotEquals(rhs);
}

// operator <

inline bool operator < (const ErrorCode & lhs, const ErrorCode & rhs) {
    return lhs.isLessThan(rhs);
}

inline bool operator < (int lhs, const ErrorCode & rhs) {
    return rhs.isLessThan(lhs);
}

inline bool operator < (const ErrorCode & lhs, int rhs) {
    return lhs.isLessThan(rhs);
}

inline bool operator < (int64_t lhs, const ErrorCode & rhs) {
    return rhs.isLessThan(lhs);
}

inline bool operator < (const ErrorCode & lhs, int64_t rhs) {
    return lhs.isLessThan(rhs);
}

// operator >

inline bool operator > (const ErrorCode & lhs, const ErrorCode & rhs) {
    return lhs.isGreaterThan(rhs);
}

inline bool operator > (int lhs, const ErrorCode & rhs) {
    return rhs.isGreaterThan(lhs);
}

inline bool operator > (const ErrorCode & lhs, int rhs) {
    return lhs.isGreaterThan(rhs);
}

inline bool operator > (int64_t lhs, const ErrorCode & rhs) {
    return rhs.isGreaterThan(lhs);
}

inline bool operator > (const ErrorCode & lhs, int64_t rhs) {
    return lhs.isGreaterThan(rhs);
}

// operator <=

inline bool operator <= (const ErrorCode & lhs, const ErrorCode & rhs) {
    return lhs.isLessThanAndEqual(rhs);
}

inline bool operator <= (int lhs, const ErrorCode & rhs) {
    return rhs.isLessThanAndEqual(lhs);
}

inline bool operator <= (const ErrorCode & lhs, int rhs) {
    return lhs.isLessThanAndEqual(rhs);
}

inline bool operator <= (int64_t lhs, const ErrorCode & rhs) {
    return rhs.isLessThanAndEqual(lhs);
}

inline bool operator <= (const ErrorCode & lhs, int64_t rhs) {
    return lhs.isLessThanAndEqual(rhs);
}

// operator >=

inline bool operator >= (const ErrorCode & lhs, const ErrorCode & rhs) {
    return lhs.isGreaterThanAndEqual(rhs);
}

inline bool operator >= (int lhs, const ErrorCode & rhs) {
    return rhs.isGreaterThanAndEqual(lhs);
}

inline bool operator >= (const ErrorCode & lhs, int rhs) {
    return lhs.isGreaterThanAndEqual(rhs);
}

inline bool operator >= (int64_t lhs, const ErrorCode & rhs) {
    return rhs.isGreaterThanAndEqual(lhs);
}

inline bool operator >= (const ErrorCode & lhs, int64_t rhs) {
    return lhs.isGreaterThanAndEqual(rhs);
}

} // namespace jlang

#endif // JLANG_LANG_ERRORCODE_H
