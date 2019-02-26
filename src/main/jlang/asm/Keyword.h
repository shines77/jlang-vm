
#ifndef JLANG_ASM_KEYWORD_H
#define JLANG_ASM_KEYWORD_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/Global.h"
#include "jlang/lang/NonCopyable.h"
#include "jlang/asm/KeywordKind.h"
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

#define KEYWORD_error               "error"
#define KEYWORD_define              "define"
#define KEYWORD_pragma              "pragma"
#define KEYWORD_include             "include"
#define KEYWORD_warning             "warning"

#define TO_STRING(name)             #name

#define KEYWORD_LENGTH(keyword)     (sizeof(keyword) - 1)
#define KEYWORD_STRING(keyword)     keyword

#define KEYWORD_LENGTH_EX(keyword)  (sizeof(TO_STRING(keyword)) - 1)
#define KEYWORD_STRING_EX(keyword)  TO_STRING(keyword))

#define MAX_IDENTIFIER_LEN          512

namespace jlang {
namespace jasm {

#define JLANG_KEYWORD_ID(token_type, kind)      keyword_id_##token_type##_##kind
#define JLANG_PREPROCESSING_ID(keyword)         keyword_id_pp_##keyword

#define KEYWORD_DEF(token_type, keyword, kind)  \
        JLANG_KEYWORD_ID(token_type, kind),

#define PREPROCESSING_DEF(keyword)  \
        JLANG_PREPROCESSING_ID(keyword),

enum keyword_id_t {
    #include "jlang/asm/KeywordDef.h"
    KEYWORD_ID_MAX
};

#define KEYWORD_DEF(token_type, keyword, kind)  \
    { \
        (uint16_t)JLANG_KEYWORD_ID(token_type, kind), \
        (uint16_t)jasm::KeywordKind::kind, \
        (uint16_t)jasm::Token::token_type, \
        (uint16_t)(sizeof(TO_STRING(keyword)) - 1), \
        TO_STRING(keyword) \
    },

#define PREPROCESSING_DEF(keyword)  \
    { \
        (uint16_t)JLANG_PREPROCESSING_ID(keyword), \
        (uint16_t)jasm::KeywordKind::Preprocessing, \
        (uint16_t)jasm::Token::pp_##keyword, \
        (uint16_t)(sizeof(TO_STRING(keyword)) - 1), \
        TO_STRING(keyword) \
    },

struct keyword_info_t {
    uint16_t id;
    uint16_t kind;
    uint16_t token_type;
    uint16_t length;
    char name[64 - sizeof(uint16_t) * 4];   // Alignment for 64 bytes.
};

static const keyword_info_t g_keyword_list[] = {
    #include "jlang/asm/KeywordDef.h"
    {
        (uint16_t)KEYWORD_ID_MAX,
        (uint16_t)jasm::KeywordKind::Unknown,
        (uint16_t)jasm::Token::UnknownToken,
        0, ""
    }
};

static const size_t kKeywordListSize = sizeof(g_keyword_list) / sizeof(g_keyword_list[0]) - 1;

template <int keyword_id>
inline keyword_info_t getKeywordInfo()
{
    keyword_info_t keyword;
    keyword.id = keyword_id;
    keyword.kind = jasm::KeywordKind::Unknown;
    keyword.token_type = jasm::Token::UnknownToken;
    keyword.length = sizeof("Unknown keyword") - 1;
    keyword.name = "Unknown keyword";
    return keyword;
}

template <>
inline keyword_info_t getKeywordInfo<keyword_id_pp_include>()
{
    keyword_info_t keyword;
    keyword.id = keyword_id_pp_include;
    keyword.kind = jasm::KeywordKind::Others;
    keyword.token_type = jasm::Token::pp_ifndef;
    ::memcpy(keyword.name, "include", sizeof("include"));
    keyword.length = sizeof("include") - 1;
    return keyword;
}

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

class Keyword {
public:
#if (KEYWORD_HASHCODE_WORDLEN == 64)
    typedef KeywordHash64 hashcode_type;
    typedef KeywordHash64::value_type hashvalue_type;
#else
    typedef KeywordHash hashcode_type;
    typedef KeywordHash::value_type hashvalue_type;
#endif

private:
    uint16_t id_;
    uint16_t kind_;
    uint16_t token_;
    uint16_t length_;
    std::string name_;

#if (KEYWORD_HASHCODE_WORDLEN == 32) || (KEYWORD_HASHCODE_WORDLEN == 64)
    hashcode_type hashCode_;
#endif

public:
    Keyword() : id_(-1), kind_(jasm::KeywordKind::Unknown), token_(jasm::Token::UnknownToken), length_(0)
#if (KEYWORD_HASHCODE_WORDLEN == 32)
        , hashCode_(0U)
#elif (KEYWORD_HASHCODE_WORDLEN == 64)
        , hashCode_(0ULL)
#endif
    {
        /* Do nothing! */
    }

    Keyword(const keyword_info_t & keyword_detail) : Keyword() {
        id_ = keyword_detail.id;
        kind_ = keyword_detail.kind;
        token_ = keyword_detail.token_type;
        length_ = keyword_detail.length;
        name_ = keyword_detail.name;
#if (KEYWORD_HASHCODE_WORDLEN == 32) || (KEYWORD_HASHCODE_WORDLEN == 64)
        hashCode_ = calcHashCode();
#endif
    }

    ~Keyword() {
    }

    uint32_t length() const { return length_; }

    const char * c_str() const { return name_.c_str(); }
    const char * toString() const { return this->c_str(); }

    const KeywordKind::KindType getKind() const { return KeywordKind::KindType(kind_); }
    const Token::Type getToken() const { return Token::Type(token_); }

    void setKind(uint16_t type) { kind_ = type; }
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

enum KeywordRoot {
    Default,
    Preprocessing
};

///////////////////////////////////////////////////
// class KeywordMapping
///////////////////////////////////////////////////

class KeywordMapping {
public:
    typedef std::unordered_map<std::string, Keyword> hash_table_type;
    typedef hash_table_type::iterator iterator;
    typedef hash_table_type::const_iterator const_iterator;

private:
    hash_table_type keywordMapping_;
    KeywordRoot root_;
    bool inited_;

public:
    KeywordMapping(KeywordRoot root = KeywordRoot::Default)
        : root_(root), inited_(false) { init(); }
    ~KeywordMapping() { inited_ = false; }

    bool inited() const { return inited_; }
    KeywordRoot root() const { return root_; }

    void set_root(KeywordRoot root) {
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
            for (size_t i = 0; i < kKeywordListSize; ++i) {
                Keyword keyword(g_keyword_list[i]);
                const std::string & keywordName = keyword.getName();
                if (keyword.getKind() == KeywordKind::Preprocessing) {
                    keywordMapping_.insert(std::make_pair(keywordName, keyword));
                }
            }
        }
        else {
            for (size_t i = 0; i < kKeywordListSize; ++i) {
                Keyword keyword(g_keyword_list[i]);
                const std::string & keywordName = keyword.getName();
                if (keyword.getKind() != KeywordKind::Preprocessing) {
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
    static KeywordMapping * prepocessing_keyword_mapping;

public:
    KeywordInitor() { KeywordInitor::init(); }
    ~KeywordInitor() {}

    static bool inited() {
        KeywordMapping & keywordMapping = KeywordInitor::getKeywordMapping();
        KeywordMapping & preprocessingKeywordMapping = KeywordInitor::getPreprocessingKeywordMapping();
        return (keywordMapping.inited() && preprocessingKeywordMapping.inited());
    }

    static void init() {
        KeywordMapping & keywordMapping = KeywordInitor::getKeywordMapping();
        assert(keywordMapping.inited());

        KeywordMapping & preprocessingKeywordMapping = KeywordInitor::getPreprocessingKeywordMapping();
        assert(preprocessingKeywordMapping.inited());
    }

    static void finalize() {
        KeywordInitor::destroyKeywordMapping();
        KeywordInitor::destroyPreprocessingKeywordMapping();
    }

    // KeywordInitor::getKeywordMapping() implementation in Keyword.h file.
    static KeywordMapping & getKeywordMapping() {
        if (KeywordInitor::keyword_mapping == nullptr) {
            KeywordInitor::keyword_mapping = new KeywordMapping(KeywordRoot::Default);
        }
        return *KeywordInitor::keyword_mapping;
    }

    // KeywordInitor::getPreprocessingKeywordMapping() implementation in Keyword.h file.
    static KeywordMapping & getPreprocessingKeywordMapping() {
        if (KeywordInitor::prepocessing_keyword_mapping == nullptr) {
            KeywordInitor::prepocessing_keyword_mapping = new KeywordMapping(KeywordRoot::Preprocessing);
        }
        return *KeywordInitor::prepocessing_keyword_mapping;
    }

    // KeywordInitor::destroyKeywordMapping() implementation in Keyword.h file.
    static void destroyKeywordMapping() {
        if (keyword_mapping) {
            delete keyword_mapping;
            keyword_mapping = nullptr;
        }
    }

    // KeywordInitor::destroyPreprocessingKeywordMapping() implementation in Keyword.h file.
    static void destroyPreprocessingKeywordMapping() {
        if (prepocessing_keyword_mapping) {
            delete prepocessing_keyword_mapping;
            prepocessing_keyword_mapping = nullptr;
        }
    }
};

} // namespace jasm
} // namespace jlang

#undef TO_STRING

#endif // JLANG_ASM_KEYWORD_H
