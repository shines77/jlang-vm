
#ifndef JLANG_ASM_IDENTINFO_H
#define JLANG_ASM_IDENTINFO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdint.h>
#include <string>
#include <utility>  // For std::swap()

#include "jlang/basic/stddef.h"
#include "jlang/asm/Token.h"
#include "jlang/asm/Keyword.h"
#include "jlang/stream/StreamMarker.h"

namespace jlang {
namespace jasm {

///////////////////////////////////////////////////
// class IdentInfo
///////////////////////////////////////////////////

class IdentInfo {
protected:
    std::string name_;
    Token::Type token_;
    intptr_t start_;
    intptr_t length_;

public:
    IdentInfo() : token_(Token::Unknown), start_(0), length_(0) {
    }
    IdentInfo(const std::string & name, intptr_t start)
        : name_(name), token_(Token::Unknown), start_(0), length_(name.size()) {
    }
    IdentInfo(const IdentInfo & src) {
        this->copy(src);
    }
    IdentInfo(IdentInfo && src) {
        this->swap(src);
    }

    ~IdentInfo() {
    }

    IdentInfo & operator = (const IdentInfo & rhs) {
        this->copy(rhs);
        return *this;
    }

    IdentInfo & operator = (IdentInfo && rhs) {
        this->swap(rhs);
        return *this;
    }

    std::string & name() { return this->name_; }
    const std::string & name() const { return this->name_; }

    void setName(const std::string & name) {
        this->name_ = name;
    }

    Token::Type token() const { return this->token_; }

    void setToken(Token::Type type) {
        this->token_ = type;
    }

    void setToken(Token token) {
        this->token_ = token.type();
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

    void copy(const IdentInfo & src) {
        this->name_ = src.name_;
        this->start_ = src.start_;
        this->length_ = src.length_;
    }

    void swap(IdentInfo & src) {
        this->name_.swap(src.name_);
        std::swap(this->start_, src.start_);
        std::swap(this->length_, src.length_);
    }

    Keyword * getKeyword() {
        KeywordMapping & keyMapping = Global::getKeywordMapping();
        assert(keyMapping.inited());
        KeywordMapping::iterator iter = keyMapping.find(this->name_);
        if (likely(iter != keyMapping.end())) {
            Keyword * keyword = &(iter->second);
            return keyword;
        }
        else {
            return nullptr;
        }
    }

    bool getKeyword(Keyword & keyword) {
        KeywordMapping & keyMapping = Global::getKeywordMapping();
        assert(keyMapping.inited());
        KeywordMapping::iterator iter = keyMapping.find(this->name_);
        if (likely(iter != keyMapping.end())) {
            keyword = iter->second;
            return true;
        }
        else {
            return false;
        }
    }

    Keyword * getSection() {
        KeywordMapping & sectionMapping = Global::getSectionMapping();
        assert(sectionMapping.inited());
        KeywordMapping::iterator iter = sectionMapping.find(this->name_);
        if (likely(iter != sectionMapping.end())) {
            Keyword * keyword = &(iter->second);
            return keyword;
        }
        else {
            return nullptr;
        }
    }

    bool getSection(Keyword & keyword) {
        KeywordMapping & sectionMapping = Global::getSectionMapping();
        assert(sectionMapping.inited());
        KeywordMapping::iterator iter = sectionMapping.find(this->name_);
        if (likely(iter != sectionMapping.end())) {
            keyword = iter->second;
            return true;
        }
        else {
            return false;
        }
    }

    void makeIdent(const StreamMarker & marker) {
        if (likely(marker.is_marked())) {
            intptr_t length = StringUtils::sub_str(this->name_, marker.start_ptr(), marker.end_ptr());
        }
        else {
            this->name_.clear();
        }
        this->setPosition(marker.start(), marker.length());
    }

    void appendIdent(const StreamMarker & marker) {
        if (likely(marker.is_marked())) {
            intptr_t length = StringUtils::append(this->name_, marker.start_ptr(), marker.end_ptr());
        }
        else {
            this->name_.clear();
        }
        this->setPosition(marker.start(), marker.length());
    }

    bool merge(const IdentInfo & src) {
        if (likely(src.start() >= (this->start() + this->length()))) {
            this->name_ += " ";
            this->name_ += src.name();

            this->length_ = (src.start() - this->start()) + src.length();
            return true;
        }
        else {
            return false;
        }
    }
};

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_IDENTINFO_H
