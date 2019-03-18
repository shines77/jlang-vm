
#ifndef JLANG_STREAM_STREAMMARKER_H
#define JLANG_STREAM_STREAMMARKER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/stream/InputStringStream.h"
#include "jlang/stream/OutputStringStream.h"
#include "jlang/stream/StringStream.h"
#include "jlang/stream/MemoryStream.h"
#include "jlang/support/StringUtils.h"

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#define REFERENCE_CAST_TO(value, toType) \
        (*(static_cast<toType *>(&value)))

namespace jlang {

namespace detail {

template <bool _Test, typename _Type>
struct enable_if {

};

} // namespace detail

template <typename T>
class VariantReal {
public:
    typedef T value_type;

protected:
    value_type value_;

public:
    VariantReal() : value_(static_cast<value_type>(0)) {}
    VariantReal(const value_type & value) : value_(value) {}
    VariantReal(const VariantReal & src) : value_(src.value_) {}

    bool isReal() const { return true; }

    value_type get() const { return this->value_; }
    void set(value_type & value) { this->value_ = value; }

    VariantReal & operator = (const VariantReal & rhs) {
        this->value_ = rhs.value_;
        return *this;
    }

    VariantReal & operator = (const value_type & value) {
        this->value_ = value;
        return *this;
    }

    operator bool() {
        return static_cast<bool>(this->value_);
    }

    operator typename std::conditional<std::is_same<value_type, bool>::value, void, value_type>::type() {
        return this->value_;
    }
};

/*
template <>
class VariantReal<bool> {
public:
    typedef bool value_type;

protected:
    bool value_;

public:
    VariantReal() : value_(static_cast<value_type>(0)) {}
    VariantReal(const value_type & value) : value_(value) {}
    VariantReal(const VariantReal & src) : value_(src.value_) {}

    bool isReal() const { return true; }

    value_type get() const { return this->value_; }
    void set(value_type & value) { this->value_ = value; }

    VariantReal & operator = (const VariantReal & rhs) {
        this->value_ = rhs.value_;
        return *this;
    }

    VariantReal & operator = (const value_type & value) {
        this->value_ = value;
        return *this;
    }

    operator bool() {
        return this->value_;
    }
};

//*/

template <typename T>
inline
bool operator == (const VariantReal<T> & lhs, const typename VariantReal<T>::value_type rhs) {
    return (lhs.get() == rhs);
}

template <typename T>
inline
bool operator == (const typename VariantReal<T>::value_type lhs, const VariantReal<T> & rhs) {
    return (rhs.get() == lhs);
}

template <typename T>
inline
bool operator != (const VariantReal<T> & lhs, const typename VariantReal<T>::value_type rhs) {
    return (lhs.get() != rhs);
}

template <typename T>
inline
bool operator != (const typename VariantReal<T>::value_type lhs, const VariantReal<T> & rhs) {
    return (rhs.get() != lhs);
}

template <typename T, T DefaultValue = static_cast<T>(0)>
class VariantFake {
public:
    typedef T value_type;

public:
    VariantFake() {}
    VariantFake(const value_type & value) {}
    VariantFake(const VariantFake & src) {}

    bool isReal() const { return false; }

    value_type get() const { return DefaultValue; }
    void set(value_type & value) { /* Do nothing !! */ }

    VariantFake & operator = (const VariantFake & rhs) {
        return *this;
    }

    VariantFake & operator = (const value_type & value) {
        return *this;
    }

    operator bool() {
        return static_cast<bool>(DefaultValue);
    }
    
    operator typename std::conditional<std::is_same<value_type, bool>::value, void, value_type>::type() {
        return DefaultValue;
    }
};

template <typename T, T DefaultValue>
inline
bool operator == (const VariantFake<T> & lhs, const typename VariantFake<T>::value_type rhs) {
    return (lhs.get() == rhs);
}

template <typename T, T DefaultValue>
inline
bool operator == (const typename VariantFake<T>::value_type lhs, const VariantFake<T> & rhs) {
    return (rhs.get() == lhs);
}

template <typename T, T DefaultValue>
inline
bool operator != (const VariantFake<T> & lhs, const typename VariantFake<T>::value_type rhs) {
    return (lhs.get() != rhs);
}

template <typename T, T DefaultValue>
inline
bool operator != (const typename VariantFake<T>::value_type lhs, const VariantFake<T> & rhs) {
    return (rhs.get() != lhs);
}

///////////////////////////////////////////////////
// class StreamMarker
///////////////////////////////////////////////////

class StreamMarker {
private:
    StreamRoot & stream_;
    char * marker_;
#if defined(NDEBUG)
    VariantFake<bool, true> marked_;
#else
    VariantReal<bool> marked_;
#endif

public:
    StreamMarker(StringStream & steam)
        : stream_(REFERENCE_CAST_TO(steam, StreamRoot)), marker_(nullptr), marked_(false) {
        /* Do nothing !! */
    }

    StreamMarker(InputStringStream & steam)
        : stream_(REFERENCE_CAST_TO(steam, StreamRoot)), marker_(nullptr), marked_(false) {
        /* Do nothing !! */
    }

    StreamMarker(OutputStringStream & steam)
        : stream_(REFERENCE_CAST_TO(steam, StreamRoot)), marker_(nullptr), marked_(false) {
        /* Do nothing !! */
    }

    StreamMarker(MemoryStream & steam)
        : stream_(REFERENCE_CAST_TO(steam, StreamRoot)), marker_(nullptr), marked_(false) {
        /* Do nothing !! */
    }
    ~StreamMarker() {}

    StreamRoot & get_stream() { return stream_; }
    const StreamRoot & get_stream() const { return stream_; }

    char * marker() const { return this->marker_; }
    char * current() const { return this->stream_.current(); }

    bool is_marked() const { return ((this->marker_ != nullptr) && this->marked_.get()); }

    bool is_valid() const {
        return (stream_.is_valid()
            && (this->marker_ >= this->stream_.head())
            && (this->marker_ <= this->stream_.tail()));
    }

    char * start() const {
        assert(this->marker_ != nullptr);
        assert(this->is_marked());
        assert(this->marker_ >= this->stream_.head());
        return (this->marker_);
    }

    char * end() const {
        assert(this->marker_ != nullptr);
        assert(this->is_marked());
        return this->stream_.current();
    }

    intptr_t start_pos() const {
        assert(this->marker_ != nullptr);
        assert(this->is_marked());
        assert(this->marker_ >= this->stream_.head());
        return (this->marker_ - this->stream_.head());
    }

    intptr_t end_pos() const {
        assert(this->marker_ != nullptr);
        assert(this->is_marked());
        return (this->stream_.current() - this->stream_.head());
    }

    intptr_t length() const {
        assert(this->marker_ != nullptr);
        assert(this->is_marked());
        assert(this->stream_.current() >= this->marker_);
        return (this->stream_.current() - this->marker_);
    }

    char * get_marker() const { return this->start(); }
    char * get_current() const { return this->end(); }

    void set_current(char * position) {
        assert(position >= this->stream_.head() || position < this->stream_.tail());
        assert(position != nullptr);
        this->stream_.set_current(position);
    }

    void reset() {
        this->marker_ = nullptr;
        this->marked_ = false;
    }

    void remark() {
        this->marker_ = this->stream_.current();
        this->marked_ = true;
    }

    void setmark(int offset = 0) {
        if (!(this->marked_.isReal()) || !(this->marked_)) {
            this->marker_ = this->stream_.current() + offset;
            this->marked_ = true;
        }
        else {
            throw("StreamMarker:setmark(), Error: can not be set the mark two times continuous.");
        }
    }

    void rewind() {
        if (!(this->marked_.isReal()) || this->marked_) {
            assert(this->marker_ >= this->stream_.head() || this->marker_ < this->stream_.tail());
            assert(this->marker_ != nullptr);
            this->stream_.set_current(this->marker_);
            this->marker_ = nullptr;
            this->marked_ = false;
        }
        else {
            throw("StreamMarker:rewind(), Error: have no set the mark yet, can not be rewind.");
        }
    }

    intptr_t copy_string(char * substr, size_t size) {
        assert(substr != nullptr);
        assert(this->end() >= this->start());
        if (likely(this->is_marked())) {
            return StringUtils::sub_str(substr, size, this->start(), this->end());
        }
        else {
            *substr = '\0';
            return 0;
        }
    }

    template <size_t N>
    intptr_t copy_string(char (&substr)[N]) {
        return this->copy_string(substr, N);
    }

    intptr_t copy_string(std::string & substr) {
        if (likely(this->is_marked())) {
            return StringUtils::sub_str(substr, this->start(), this->end());
        }
        else {
            substr.clear();
            return 0;
        }
    }

    intptr_t append_string(std::string & substr) {
        if (likely(this->is_marked())) {
            return StringUtils::append(substr, this->start(), this->end());
        }
        else {
            substr.clear();
            return 0;
        }
    }
};

} // namespace jlang

#endif // JLANG_STREAM_STREAMMARKER_H
