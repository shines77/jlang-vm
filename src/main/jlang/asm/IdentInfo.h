
#ifndef JLANG_ASM_IDENTINFO_H
#define JLANG_ASM_IDENTINFO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/NonCopyable.h"
#include "jlang/asm/Keyword.h"
#include "jlang/lang/Global.h"

namespace jlang {
namespace jasm {

///////////////////////////////////////////////////
// class IdentInfo
///////////////////////////////////////////////////

class IdentInfo {
protected:
    std::string name_;
    intptr_t start_;
    intptr_t length_;

public:
    IdentInfo() : start_(0), length_(0) {
    }
    IdentInfo(const std::string & name, intptr_t start)
        : name_(name), start_(0), length_(name.size()) {
    }
    IdentInfo(const IdentInfo & src) {
        this->clone(src);
    }
    IdentInfo(IdentInfo && src) {
        this->swap(std::forward<IdentInfo>(src));
    }

    ~IdentInfo() {
    }

    IdentInfo & operator = (const IdentInfo & rhs) {
        this->clone(rhs);
        return *this;
    }

    IdentInfo & operator = (IdentInfo && rhs) {
        this->swap(std::forward<IdentInfo>(rhs));
        return *this;
    }

    std::string & name() { return this->name_; }
    const std::string & name() const { return this->name_; }

    void setName(const std::string & name) {
        this->name_ = name;
    }

    intptr_t start() const { return this->start_; }
    intptr_t length() const { return this->length_; }

    void setStart(intptr_t start) {
        this->start_ = start;
    }

    void setLength(intptr_t length) {
        this->length_ = length;
    }

    void setPosition(intptr_t start, intptr_t length) {
        this->start_ = start;
        this->length_ = length;
    }

    void clone(const IdentInfo & src) {
        this->name_ = src.name_;
        this->start_ = src.start_;
        this->length_ = src.length_;
    }

    void swap(IdentInfo & src) {
        this->name_.swap(src.name_);
        std::swap(this->start_, src.start_);
        std::swap(this->length_, src.length_);
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_IDENTINFO_H
