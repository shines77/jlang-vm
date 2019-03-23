
#ifndef JLANG_ASM_KEYWORD_H
#define JLANG_ASM_KEYWORD_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"
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

#include <mutex>
#include <memory>

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
    uint16_t token;
    uint32_t kind;
    uint16_t length;
    char name[64 - sizeof(uint16_t) * 5];   // Alignment for 64 bytes.
};

class KeywordInfo {
protected:
    uint16_t id_;
    uint16_t token_;
    uint32_t kind_;
    uint16_t length_;
    std::string name_;

public:
    KeywordInfo() : id_(-1), token_(jasm::Token::Unknown),
                    kind_(jasm::KeywordKind::Unknown),
                    length_(0) {
    }
    ~KeywordInfo() {}

    friend class Keyword;
};

#undef JLANG_KEYWORD_ID
#undef JLANG_PREPROCESSING_ID

#define JLANG_KEYWORD_ID(token, kind)    jasm::KeywordId::token##_##kind
#define JLANG_PREPROCESSING_ID(keyword)  jasm::KeywordId::pp_##keyword

#define KEYWORD_DEF(token, keyword, kind)  \
    { \
        (uint16_t)JLANG_KEYWORD_ID(token, kind), \
        (uint16_t)jasm::Token::token, \
        (uint32_t)jasm::KeywordKind::kind, \
        (uint16_t)(sizeof(TO_STRING(keyword)) - 1), \
        TO_STRING(keyword) \
    },

#define PREPROCESSING_DEF(keyword)  \
    { \
        (uint16_t)JLANG_PREPROCESSING_ID(keyword), \
        (uint16_t)jasm::Token::pp_##keyword, \
        (uint16_t)jasm::KeywordKind::Preprocessing, \
        (uint16_t)(sizeof(TO_STRING(keyword)) - 1), \
        TO_STRING(keyword) \
    },

static const KeywordInfoDef gKeywordList[] = {
    #include "jlang/asm/KeywordDef.h"
    {
        (uint16_t)jasm::KeywordId::Type::MaxKeywordId,
        (uint16_t)jasm::Token::Unknown,
        (uint32_t)jasm::KeywordKind::Unknown,
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
        kind_ = src.kind;
        token_ = src.token;
        length_ = src.length;
        name_ = src.name;
#if (KEYWORD_HASHCODE_WORDLEN == 32) || (KEYWORD_HASHCODE_WORDLEN == 64)
        hashCode_ = calcHashCode();
#endif
    }

    Keyword(const KeywordInfo & src) {
        id_ = src.id_;
        kind_ = src.kind_;
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

    const jasm::KeywordKind::Type getKind() const { return jasm::KeywordKind::Type(kind_); }
    const jasm::Token getToken() const { return jasm::Token(token_); }
    const jasm::Token::Type getType() const { return jasm::Token::Type(token_); }

    void setKind(uint16_t type) { kind_ = type; }
    void setToken(const jasm::Token & token) { token_ = token.value(); }

    void setType(int16_t token) { token_ = static_cast<uint16_t>(token); }
    void setType(int32_t token) { token_ = static_cast<uint16_t>(token); }
    void setType(int64_t token) { token_ = static_cast<uint16_t>(token); }
    void setType(jasm::Token::Type token) { token_ = static_cast<uint16_t>(token); }

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
    typedef std::lock_guard<std::mutex>              lock_type;

private:
    int root_;
    bool inited_;
    hashtable_type keywordMapping_;
    std::mutex mutex_;

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
    iterator end()   { return keywordMapping_.end(); }

    void destroy() {
        keywordMapping_.clear();
    }

    void _destroy() {
        lock_type lock(mutex_);
        destroy();
    }

    void reserve(size_t max_count) {
        keywordMapping_.reserve(max_count);
    }   

    Keyword & operator [](const std::string & keyword) {
        return keywordMapping_[keyword];
    }

    iterator find(const char * keyword, size_t capacity, size_t length) {
        length = jstd::minimum(length, capacity);
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
                if (keyword.getKind() == jasm::KeywordKind::Preprocessing) {
                    keywordMapping_.insert(std::make_pair(keywordName, keyword));
                }
            }
        }
        else if (root_ == KeywordRoot::Section) {
            for (size_t i = 0; i < gKeywordListSize; ++i) {
                Keyword keyword(gKeywordList[i]);
                const std::string & keywordName = keyword.getName();
                if (keyword.getKind() == jasm::KeywordKind::Section) {
                    keywordMapping_.insert(std::make_pair(keywordName, keyword));
                }
            }
        }
        else {
            for (size_t i = 0; i < gKeywordListSize; ++i) {
                Keyword keyword(gKeywordList[i]);
                const std::string & keywordName = keyword.getName();
                if (keyword.getKind() != jasm::KeywordKind::Preprocessing &&
                    keyword.getKind() != jasm::KeywordKind::Section) {
                    keywordMapping_.insert(std::make_pair(keywordName, keyword));
                }
            }
        }
        inited_ = true;
    }
};

///////////////////////////////////////////////////
// class KeywordInitializer
///////////////////////////////////////////////////

class KeywordInitializer : public NonCopyable {
public:
    static std::unique_ptr<KeywordMapping> keyword_mapping;
    static std::unique_ptr<KeywordMapping> pp_keyword_mapping;
    static std::unique_ptr<KeywordMapping> section_mapping;

public:
    KeywordInitializer() { KeywordInitializer::initialize(); }
    ~KeywordInitializer() {}

    static bool inited() {
        return KeywordInitializer::initialize();
    }

    static bool initialize() {
        KeywordMapping & keywordMapping = KeywordInitializer::getKeywordMapping();
        assert(keywordMapping.inited());

        KeywordMapping & ppKeywordMapping = KeywordInitializer::getPPKeywordMapping();
        assert(ppKeywordMapping.inited());

        KeywordMapping & SectionMapping = KeywordInitializer::getSectionMapping();
        assert(SectionMapping.inited());

        return (keywordMapping.inited() && ppKeywordMapping.inited() && SectionMapping.inited());
    }

    static void finalize() {
        KeywordInitializer::destroyKeywordMapping();
        KeywordInitializer::destroyPPKeywordMapping();
        KeywordInitializer::destroySectionMapping();
    }

    static KeywordMapping & getKeywordMapping() {
        if (unlikely(KeywordInitializer::keyword_mapping.get() == nullptr)) {
            KeywordInitializer::keyword_mapping.reset(new KeywordMapping(KeywordRoot::Default));
        }
        return *KeywordInitializer::keyword_mapping;
    }

    static KeywordMapping & getPPKeywordMapping() {
        if (unlikely(KeywordInitializer::pp_keyword_mapping.get() == nullptr)) {
            KeywordInitializer::pp_keyword_mapping.reset(new KeywordMapping(KeywordRoot::Preprocessing));
        }
        return *KeywordInitializer::pp_keyword_mapping;
    }

    static KeywordMapping & getSectionMapping() {
        if (unlikely(KeywordInitializer::section_mapping.get() == nullptr)) {
            KeywordInitializer::section_mapping.reset(new KeywordMapping(KeywordRoot::Section));
        }
        return *KeywordInitializer::section_mapping;
    }

    static void destroyKeywordMapping() {
        if (KeywordInitializer::keyword_mapping.get() != nullptr) {
            KeywordInitializer::keyword_mapping->destroy();
            KeywordInitializer::keyword_mapping.reset();
        }
    }

    static void destroyPPKeywordMapping() {
        if (KeywordInitializer::pp_keyword_mapping.get() != nullptr) {
            KeywordInitializer::pp_keyword_mapping->destroy();
            KeywordInitializer::pp_keyword_mapping.reset();
        }
    }

    static void destroySectionMapping() {
        if (KeywordInitializer::section_mapping.get() != nullptr) {
            KeywordInitializer::section_mapping->destroy();
            KeywordInitializer::section_mapping.reset();
        }
    }
};

} // namespace jasm
} // namespace jlang

#undef TO_STRING

#endif // JLANG_ASM_KEYWORD_H
