
#ifndef JLANG_ASM_KEYWORD_H
#define JLANG_ASM_KEYWORD_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/Global.h"
#include "jlang/lang/NonCopyable.h"
#include "jlang/asm/KeywordCategory.h"
#include "jlang/asm/Token.h"
#include "jlang/jstd/min_max.h"
#include "jlang/support/HashAlgorithm.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#include <string>
#include <vector>
#include <unordered_map>

/* The word length of the keyword's hash code. */
/* The value can choose: 0, 32 or 64. */
#define KEYWORD_HASHCODE_WORDLEN    32

#define PREPROCESSING_CHAR          '#'
#define PREPROCESSING_CHAR_LEN      sizeof('#')

#define TO_STRING(name)             #name

#define MAX_IDENTIFIER_LEN          512

//////////////////////////////////////////////////////////////////////////////////////////////////////

namespace jlang {
namespace jasm {

#define JLANG_KEYWORD_ID(token, category)       token##_##category
#define JLANG_PREPROCESSING_ID(keyword)         pp_##keyword

#define KEYWORD_DEF(token, keyword, category)   JLANG_KEYWORD_ID(token, category),
#define PREPROCESSING_DEF(keyword)              JLANG_PREPROCESSING_ID(keyword),

struct KeywordId {
    enum Type {
        #include "jlang/asm/KeywordDef.h"
        MaxKeywordId
    };
};

class Keyword;

struct KeywordInfoDef {
    uint16_t id;
    uint16_t category;
    uint16_t token;
    uint16_t length;
    char name[64 - sizeof(uint16_t) * 4];   // Alignment for 64 bytes.
};

class KeywordInfo {
protected:
    uint16_t id_;
    uint16_t category_;
    uint16_t token_;
    uint16_t length_;
    std::string name_;

public:
    KeywordInfo() : id_(-1), category_(jasm::KeywordCategory::Unknown),
                    token_(jasm::Token::Unknown), length_(0) {
    }
    ~KeywordInfo() {}

    friend class Keyword;
};

#undef JLANG_KEYWORD_ID
#undef JLANG_PREPROCESSING_ID

#define JLANG_KEYWORD_ID(token, category)       KeywordId::##token##_##category
#define JLANG_PREPROCESSING_ID(keyword)         KeywordId::pp_##keyword

#define KEYWORD_DEF(token, keyword, category)  \
    { \
        (uint16_t)JLANG_KEYWORD_ID(token, category), \
        (uint16_t)jasm::KeywordCategory::category, \
        (uint16_t)jasm::Token::token, \
        (uint16_t)(sizeof(TO_STRING(keyword)) - 1), \
        TO_STRING(keyword) \
    },

#define PREPROCESSING_DEF(keyword)  \
    { \
        (uint16_t)JLANG_PREPROCESSING_ID(keyword), \
        (uint16_t)jasm::KeywordCategory::Preprocessing, \
        (uint16_t)jasm::Token::pp_##keyword, \
        (uint16_t)(sizeof(TO_STRING(keyword)) - 1), \
        TO_STRING(keyword) \
    },

static const KeywordInfoDef gKeywordList[] = {
    #include "jlang/asm/KeywordDef.h"
    {
        (uint16_t)jasm::KeywordId::Type::MaxKeywordId,
        (uint16_t)jasm::KeywordCategory::Unknown,
        (uint16_t)jasm::Token::Unknown,
        0,
        ""
    }
};

static const size_t gKeywordListSize = sizeof(gKeywordList) / sizeof(gKeywordList[0]) - 1;

///////////////////////////////////////////////////
// struct KeywordHash
///////////////////////////////////////////////////

class KeywordHash {
public:
    typedef uint32_t value_type;

private:
    value_type value_;

public:
    KeywordHash() : value_(0) {}
    KeywordHash(value_type value) {
        value_ = value;
    }

    KeywordHash(unsigned long value) {
        value_ = (value_type)value;
    }

    KeywordHash(const char * keyword) {
        value_ = HashAlgorithm::getHash(keyword);
    }

    KeywordHash(const char * keyword, size_t length) {
        value_ = HashAlgorithm::getHash(keyword, length);
    }

    KeywordHash(const std::string & keyword)
        : KeywordHash(keyword.c_str(), keyword.size()) {
    }

    value_type value() const { return value_; }
};

///////////////////////////////////////////////////
// struct KeywordHash64
///////////////////////////////////////////////////

class KeywordHash64 {
public:
    typedef uint64_t value_type;

private:
    value_type value_;

public:
    KeywordHash64() : value_(0) {}
    KeywordHash64(value_type value) {
        value_ = value;
    }

    KeywordHash64(const char * keyword) {
        value_ = HashAlgorithm::getHash64(keyword);
    }

    KeywordHash64(const char * keyword, size_t length) {
        value_ = HashAlgorithm::getHash64(keyword, length);
    }

    KeywordHash64(const std::string & keyword)
        : KeywordHash64(keyword.c_str(), keyword.size()) {
    }

    value_type value() const { return value_; }
};

///////////////////////////////////////////////////
// class Keyword
///////////////////////////////////////////////////

class Keyword : public KeywordInfo {
public:
#if (KEYWORD_HASHCODE_WORDLEN == 64)
    typedef KeywordHash64               hashcode_type;
    typedef KeywordHash64::value_type   hashvalue_type;
#else
    typedef KeywordHash                 hashcode_type;
    typedef KeywordHash::value_type     hashvalue_type;
#endif

protected:
#if (KEYWORD_HASHCODE_WORDLEN == 32) || (KEYWORD_HASHCODE_WORDLEN == 64)
    hashcode_type hashCode_;
#endif

public:
    Keyword() : KeywordInfo()
#if (KEYWORD_HASHCODE_WORDLEN == 32)
        , hashCode_(0U)
#elif (KEYWORD_HASHCODE_WORDLEN == 64)
        , hashCode_(0ULL)
#endif
    {
        /* Do nothing! */
    }

    Keyword(const KeywordInfoDef & src) {
        id_ = src.id;
        category_ = src.category;
        token_ = src.token;
        length_ = src.length;
        name_ = src.name;
#if (KEYWORD_HASHCODE_WORDLEN == 32) || (KEYWORD_HASHCODE_WORDLEN == 64)
        hashCode_ = calcHashCode();
#endif
    }

    Keyword(const KeywordInfo & src) {
        id_ = src.id_;
        category_ = src.category_;
        token_ = src.token_;
        length_ = src.length_;
        name_ = src.name_;
#if (KEYWORD_HASHCODE_WORDLEN == 32) || (KEYWORD_HASHCODE_WORDLEN == 64)
        hashCode_ = calcHashCode();
#endif
    }

    ~Keyword() {
    }

    uint32_t length() const { return length_; }

    char * c_str() { return const_cast<char *>(name_.c_str()); }
    const char * c_str() const { return name_.c_str(); }

    std::string toString() { return this->name_; }
    const std::string toString() const { return this->name_; }

    const jasm::KeywordCategory::Type getCategory() const { return jasm::KeywordCategory::Type(category_); }
    const jasm::Token::Type getToken() const { return jasm::Token::Type(token_); }

    void setCategory(uint16_t type) { category_ = type; }
    void setToken(uint16_t token) { token_ = token; }

    const std::string & getName() const { return name_; }

    void setName(const std::string & name) { name_ = name; }
    void setName(const char * name, size_t length) { name_.assign(name, length); }

    const hashcode_type getHashCode() const {
#if (KEYWORD_HASHCODE_WORDLEN == 32) || (KEYWORD_HASHCODE_WORDLEN == 64)
        return hashCode_;
#else
        return hashcode_type(0UL);
#endif
    }

    void setHashCode(hashvalue_type hash) {
#if (KEYWORD_HASHCODE_WORDLEN == 32) || (KEYWORD_HASHCODE_WORDLEN == 64)
        hashCode_ = hash;
#endif
    }

    void setHashCode(const std::string & keyword) {
#if (KEYWORD_HASHCODE_WORDLEN == 32)
        hashCode_ = HashAlgorithm::getHash(keyword.c_str(), keyword.size());
#elif (KEYWORD_HASHCODE_WORDLEN == 64)
        hashCode_ = HashAlgorithm::getHash64(keyword.c_str(), keyword.size());
#endif
    }

private:
    hashcode_type calcHashCode() {
#if (KEYWORD_HASHCODE_WORDLEN == 32) || (KEYWORD_HASHCODE_WORDLEN == 64)
        hashcode_type hash(name_);
        return hash;
#else
        return hashcode_type(0U);
#endif
    }
};

///////////////////////////////////////////////////
// struct KeywordRoot
///////////////////////////////////////////////////

struct KeywordRoot {
    enum Type {
        Default,
        Preprocessing,
        Section,
        MaxRoot
    };
};

///////////////////////////////////////////////////
// class KeywordMapping
///////////////////////////////////////////////////

class KeywordMapping {
public:
    typedef std::unordered_map<std::string, Keyword> hashtable_type;
    typedef hashtable_type::iterator                 iterator;
    typedef hashtable_type::const_iterator           const_iterator;

private:
    int root_;
    bool inited_;
    hashtable_type keywordMapping_;

public:
    KeywordMapping(int root = KeywordRoot::Default)
        : root_(root), inited_(false) { init(); }
    ~KeywordMapping() { inited_ = false; }

    bool inited() const { return inited_; }
    int root() const { return root_; }

    void set_root(int root) {
        root_ = root;
        init();
    }

    size_t size() const { return keywordMapping_.size(); }

    iterator begin() { return keywordMapping_.begin(); }
    iterator end() { return keywordMapping_.end(); }

    void reserve(size_t max_count) {
        keywordMapping_.reserve(max_count);
    }   

    Keyword & operator [](const std::string & keyword) {
        return keywordMapping_[keyword];
    }

    iterator find(const char * keyword, size_t size, size_t length) {
        length = jstd::minimum(length, size);
        return keywordMapping_.find(std::string(keyword, length));
    }

    template <size_t N>
    iterator find(char (&keyword)[N], size_t length) {
        length = jstd::minimum(length, N);
        return keywordMapping_.find(std::string(keyword, length));
    }

    iterator find(const std::string & keyword) {
        return keywordMapping_.find(keyword);
    }

private:
    void init() {
        keywordMapping_.clear();
        if (root_ == KeywordRoot::Preprocessing) {
            for (size_t i = 0; i < gKeywordListSize; ++i) {
                Keyword keyword(gKeywordList[i]);
                const std::string & keywordName = keyword.getName();
                if (keyword.getCategory() == jasm::KeywordCategory::Preprocessing) {
                    keywordMapping_.insert(std::make_pair(keywordName, keyword));
                }
            }
        }
        else if (root_ == KeywordRoot::Section) {
            for (size_t i = 0; i < gKeywordListSize; ++i) {
                Keyword keyword(gKeywordList[i]);
                const std::string & keywordName = keyword.getName();
                if (keyword.getCategory() == jasm::KeywordCategory::Section) {
                    keywordMapping_.insert(std::make_pair(keywordName, keyword));
                }
            }
        }
        else {
            for (size_t i = 0; i < gKeywordListSize; ++i) {
                Keyword keyword(gKeywordList[i]);
                const std::string & keywordName = keyword.getName();
                if (keyword.getCategory() != jasm::KeywordCategory::Preprocessing &&
                    keyword.getCategory() != jasm::KeywordCategory::Section) {
                    keywordMapping_.insert(std::make_pair(keywordName, keyword));
                }
            }
        }
        inited_ = true;
    }
};

///////////////////////////////////////////////////
// class KeywordInitor
///////////////////////////////////////////////////

class KeywordInitor : public lang::NonCopyable {
public:
    static KeywordMapping * keyword_mapping;
    static KeywordMapping * pp_keyword_mapping;
    static KeywordMapping * section_mapping;

public:
    KeywordInitor() { KeywordInitor::init(); }
    ~KeywordInitor() {}

    static bool inited() {
        KeywordMapping & keywordMapping = KeywordInitor::getKeywordMapping();
        KeywordMapping & ppKeywordMapping = KeywordInitor::getPPKeywordMapping();
        KeywordMapping & SectionMapping = KeywordInitor::getSectionMapping();
        return (keywordMapping.inited() && ppKeywordMapping.inited() && SectionMapping.inited());
    }

    static void init() {
        KeywordMapping & keywordMapping = KeywordInitor::getKeywordMapping();
        assert(keywordMapping.inited());

        KeywordMapping & ppKeywordMapping = KeywordInitor::getPPKeywordMapping();
        assert(ppKeywordMapping.inited());

        KeywordMapping & SectionMapping = KeywordInitor::getSectionMapping();
        assert(SectionMapping.inited());
    }

    static void finalize() {
        KeywordInitor::destroyKeywordMapping();
        KeywordInitor::destroyPPKeywordMapping();
        KeywordInitor::destroySectionMapping();
    }

    static KeywordMapping & getKeywordMapping() {
        if (KeywordInitor::keyword_mapping == nullptr) {
            KeywordInitor::keyword_mapping = new KeywordMapping(KeywordRoot::Default);
        }
        return *KeywordInitor::keyword_mapping;
    }

    static KeywordMapping & getPPKeywordMapping() {
        if (KeywordInitor::pp_keyword_mapping == nullptr) {
            KeywordInitor::pp_keyword_mapping = new KeywordMapping(KeywordRoot::Preprocessing);
        }
        return *KeywordInitor::pp_keyword_mapping;
    }

    static KeywordMapping & getSectionMapping() {
        if (KeywordInitor::section_mapping == nullptr) {
            KeywordInitor::section_mapping = new KeywordMapping(KeywordRoot::Section);
        }
        return *KeywordInitor::section_mapping;
    }

    static void destroyKeywordMapping() {
        if (keyword_mapping) {
            delete keyword_mapping;
            keyword_mapping = nullptr;
        }
    }

    static void destroyPPKeywordMapping() {
        if (pp_keyword_mapping) {
            delete pp_keyword_mapping;
            pp_keyword_mapping = nullptr;
        }
    }

    static void destroySectionMapping() {
        if (section_mapping) {
            delete section_mapping;
            section_mapping = nullptr;
        }
    }
};

} // namespace jasm
} // namespace jlang

#undef TO_STRING

#endif // JLANG_ASM_KEYWORD_H
