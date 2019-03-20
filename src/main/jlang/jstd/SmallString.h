
#ifndef JSTD_SMALLSTRING_H
#define JSTD_SMALLSTRING_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <string.h>
#include <memory.h>
#include <assert.h>

#include <cstdint>  // For std::size_t
#include <string>   // For std::string

#ifndef PTR_SIZE
#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__)
#define PTR_SIZE    8
#else
#define PTR_SIZE    4
#endif
#endif // PTR_SIZE

namespace jstd {

struct do_nothing_t {};

static do_nothing_t do_nothing;

#pragma pack(push, PTR_SIZE)

template <std::size_t Capacity, typename CharTy = char>
class alignas(PTR_SIZE) SmallString {
public:
    typedef CharTy          char_type;
    typedef std::size_t     size_type;

    typedef CharTy *        pointer;
    typedef const CharTy *  const_pointer;

    static const std::size_t kCapacity = Capacity;

protected:
    char_type data_[kCapacity];
    size_type size_;

public:
    SmallString() : size_(0) {
        this->data_[0] = static_cast<char_type>('\0');
    }
    SmallString(const jstd::do_nothing_t &) {
        /* Do nothing ! */
    }
    SmallString(const std::string & src) {
        this->copy(src);
    }
    SmallString(const SmallString & src) {
        this->copy(src);
    }
    ~SmallString() {
    }

    SmallString & operator = (const SmallString & rhs) {
        this->copy(rhs);
        return *this;
    }

    pointer data() { return &(this->data_[0]); }
    pointer c_str() { return this->data(); }

    const_pointer data() const { return &(this->data_[0]); }
    const_pointer c_str() const { return this->data(); }

    size_type size() const { return this->size_; }
    size_type capacity() const { return kCapacity; }

    template <typename U>
    void copy(const U & src) {
        static_assert(kCapacity > 0, "Capacity must be greater than 0.");
        size_type copySize = jstd::minimum(kCapacity - 1, src.size());
        ::memcpy(this->data_, src.data(), copySize);
        this->data_[copySize] = static_cast<char_type>('\0');
        this->size_ = src.size();
    }
};

#pragma pack(pop)

} // namespace jstd

#endif // JSTD_SMALLSTRING_H
