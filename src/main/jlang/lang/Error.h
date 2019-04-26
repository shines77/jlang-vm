
#ifndef JLANG_LANG_ERROR_H
#define JLANG_LANG_ERROR_H

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
// class Error
///////////////////////////////////////////////////

class Error {
public:

    #define _Err(err)   err,

    enum Type {
        FirstError = -60000,
        ErrorBase = -29999,

        #include "jlang/lang/ErrorDef.h"

        // Standard errors
        KeywordNotFound = -4,
        IsNotIdentifier = -3,

        Unknown = -2,
        Failed = -1,
        Ok = 0,
        Success = Ok,
        
        ErrorInsufficientBuffer =  122L,
        Status_AlreadyExists = 256,
        LastError
    };

protected:
    Type ec_;

public:
    Error() : ec_(Type::Ok) {}
    Error(Type ec) : ec_(ec) {}
    Error(int32_t ec) : ec_(static_cast<Type>(ec)) {}
    Error(uint32_t ec) : ec_(static_cast<Type>(ec)) {}
    Error(int64_t ec) : ec_(static_cast<Type>(ec)) {}
    Error(uint64_t ec) : ec_(static_cast<Type>(ec)) {}
    Error(const Error & src) : ec_(src.ec_) {}
    ~Error() {}

    bool isOk() const {
        return (this->ec_ == Type::Ok);
    }

    bool isSuccess() const {
        return (this->ec_ == Type::Success);
    }

    bool isError() const {
        return (this->ec_ != Type::Ok);
    }

    bool isEof() const {
        return (this->ec_ == Type::EndOfFile);
    }

    bool hasError() const {
        return (this->ec_ < Type::Ok);
    }

    Error & operator = (const Error & src) {
        this->ec_ = src.ec_;
        return *this;
    }

    Error & operator = (Type ec) {
        this->ec_ = ec;
        return *this;
    }

    Error & operator = (int32_t ec) {
        this->ec_ = static_cast<Type>(this->ec_);;
        return *this;
    }

    Error & operator = (uint32_t ec) {
        this->ec_ = static_cast<Type>(this->ec_);;
        return *this;
    }

    Error & operator = (int64_t ec) {
        this->ec_ = static_cast<Type>(this->ec_);;
        return *this;
    }

    Error & operator = (uint64_t ec) {
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

    Error::Type get() const { return ec_; }
    void set(Type ec) { ec_ = ec; }

    int32_t value() const { return static_cast<int32_t>(this->ec_); }

    void setValue(int32_t ec)  { ec_ = static_cast<Type>(ec); }
    void setValue(uint32_t ec) { ec_ = static_cast<Type>(ec); }
    void setValue(int64_t ec)  { ec_ = static_cast<Type>(ec); }
    void setValue(uint64_t ec) { ec_ = static_cast<Type>(ec); }

    bool isEquals(Error ec) const      { return (this->value() == ec.value()); }
    bool isEquals(int32_t ec) const        { return (this->value() == ec); }
    bool isEquals(int64_t ec) const        { return (this->value() == static_cast<int32_t>(ec)); }

    bool isLessThan(Error ec) const    { return (this->value() <  ec.value()); }
    bool isLessThan(int32_t ec) const      { return (this->value() <  ec); }
    bool isLessThan(int64_t ec) const      { return (this->value() <  static_cast<int32_t>(ec)); }

    bool isGreaterThan(Error ec) const { return (this->value() >  ec.value()); }
    bool isGreaterThan(int32_t ec) const   { return (this->value() >  ec); }
    bool isGreaterThan(int64_t ec) const   { return (this->value() >  static_cast<int32_t>(ec)); }

    friend bool operator == (const Error & lhs, const Error & rhs) { return lhs.isEquals(rhs);   }
    friend bool operator <  (const Error & lhs, const Error & rhs) { return lhs.isLessThan(rhs); }

    friend bool operator != (const Error & lhs, const Error & rhs) { return !(lhs == rhs); }
    friend bool operator >  (const Error & lhs, const Error & rhs) { return  (rhs <  lhs); }
    friend bool operator <= (const Error & lhs, const Error & rhs) { return !(lhs >  rhs); }
    friend bool operator >= (const Error & lhs, const Error & rhs) { return !(lhs <  rhs); }

    friend bool operator == (const Error & lhs,           int32_t rhs) { return  lhs.isEquals(rhs); }
    friend bool operator == (          int32_t lhs, const Error & rhs) { return  rhs.isEquals(lhs); }
    friend bool operator == (const Error & lhs,           int64_t rhs) { return  lhs.isEquals(rhs); }
    friend bool operator == (          int64_t lhs, const Error & rhs) { return  rhs.isEquals(lhs); }

    friend bool operator != (const Error & lhs,           int32_t rhs) { return !lhs.isEquals(rhs); }
    friend bool operator != (          int32_t lhs, const Error & rhs) { return !rhs.isEquals(lhs); }
    friend bool operator != (const Error & lhs,           int64_t rhs) { return !lhs.isEquals(rhs); }
    friend bool operator != (          int64_t lhs, const Error & rhs) { return !rhs.isEquals(lhs); }

    friend bool operator <  (const Error & lhs,           int32_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator <  (          int32_t lhs, const Error & rhs) { return  rhs.isGreaterThan(lhs); }
    friend bool operator <  (const Error & lhs,           int64_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator <  (          int64_t lhs, const Error & rhs) { return  rhs.isGreaterThan(lhs); }

    friend bool operator >  (const Error & lhs,           int32_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator >  (          int32_t lhs, const Error & rhs) { return  rhs.isLessThan(lhs);    }
    friend bool operator >  (const Error & lhs,           int64_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator >  (          int64_t lhs, const Error & rhs) { return  rhs.isLessThan(lhs);    }

    friend bool operator <= (const Error & lhs,           int32_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator <= (          int32_t lhs, const Error & rhs) { return  rhs.isLessThan(lhs);    }
    friend bool operator <= (const Error & lhs,           int64_t rhs) { return  lhs.isGreaterThan(rhs); }
    friend bool operator <= (          int64_t lhs, const Error & rhs) { return  rhs.isLessThan(lhs);    }

    friend bool operator >= (const Error & lhs,           int32_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator >= (          int32_t lhs, const Error & rhs) { return  rhs.isGreaterThan(lhs); }
    friend bool operator >= (const Error & lhs,           int64_t rhs) { return  lhs.isLessThan(rhs);    }
    friend bool operator >= (          int64_t lhs, const Error & rhs) { return  rhs.isGreaterThan(lhs); }

    char * c_str() {
        return (char *)Error::format(this->ec_);
    }

    const char * c_str() const {
        return Error::format(this->ec_);
    }

    std::string toString() {
        return std::string(Error::format(this->ec_));
    }

    const std::string toString() const {
        return std::string(Error::format(this->ec_));
    }

    ///////////////////////////////////////////////////////////////////////

    #define ERROR_TO_STRING(err)    #err

    #define CASE_ERROR(err)         \
        case Type::err:             \
            return "Error::" ERROR_TO_STRING(err);

    #define _Err(err)   CASE_ERROR(err)

    ///////////////////////////////////////////////////////////////////////

    static const char * format(Type ec) {
        switch (ec) {
            CASE_ERROR(FirstError);
            CASE_ERROR(ErrorBase);

            #include "jlang/lang/ErrorDef.h"

            CASE_ERROR(Unknown);
            CASE_ERROR(Failed);
            CASE_ERROR(Ok);
            CASE_ERROR(ErrorInsufficientBuffer);
            CASE_ERROR(Status_AlreadyExists);
            CASE_ERROR(LastError);

            default:
                break;
        }

        return "Error::Undefined";
    }

    #undef ERROR_TO_STRING
    #undef CASE_ERROR
};

} // namespace jlang

#endif // JLANG_LANG_ERROR_H
