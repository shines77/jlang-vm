
#ifndef JLANG_STREAM_STREAMMARKER_H
#define JLANG_STREAM_STREAMMARKER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/stream/StringStream.h"
#include "jlang/stream/MemoryStream.h"
#include "jlang/support/StringUtils.h"

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

namespace jlang {

///////////////////////////////////////////////////
// class StreamMarker
///////////////////////////////////////////////////

template <typename T>
class StreamMarker {
public:
    typedef T stream_type;

private:
    stream_type & stream_;
    char * marker_;
#if !defined(NDEBUG) || defined(_DEBUG)
    bool marked_;
#endif

public:
    StreamMarker(stream_type & steam)
        : stream_(steam), marker_(nullptr)
#if !defined(NDEBUG) || defined(_DEBUG)
        , marked_(false)
#endif
    {
        /* Do nothing!! */
    }
    ~StreamMarker() {}

    stream_type & get_stream() { return stream_; }
    const stream_type & get_stream() const { return stream_; }

    char * marker() const { return this->marker_; }
    char * current() const { return this->stream_.current(); }

#if !defined(NDEBUG) || defined(_DEBUG)
    bool has_marked() const { return ((marker_ != nullptr) && this->marked_); }
#else
    bool has_marked() const { return (marker_ != nullptr); }
#endif

    bool is_valid() const {
        return (stream_.is_valid()
            && (this->marker_ >= this->stream_.head())
            && (this->marker_ <= this->stream_.tail()));
    }

    char * start() const {
        assert(this->marker_ != nullptr);
        assert(this->has_marked());
        assert(this->marker_ >= this->stream_.head());
        return (this->marker_);
    }

    char * end() const {
        assert(this->marker_ != nullptr);
        assert(this->has_marked());
        return this->stream_.current();
    }

    intptr_t start_pos() const {
        assert(this->marker_ != nullptr);
        assert(this->has_marked());
        assert(this->marker_ >= this->stream_.head());
        return (this->marker_ - this->stream_.head());
    }

    intptr_t end_pos() const {
        assert(this->marker_ != nullptr);
        assert(this->has_marked());
        return (this->stream_.current() - this->stream_.head());
    }

    intptr_t length() const {
        assert(this->marker_ != nullptr);
        assert(this->has_marked());
        assert(this->stream_.current() >= this->marker_);
        return (this->stream_.current() - this->marker_);
    }

    char * get_marker() const { return this->start(); }
    char * get_current() const { return this->end(); }

    void set_current(const char * position) {
        assert(position >= this->stream_.head() || position < this->stream_.tail());
        assert(position != nullptr);
        this->stream_.set_current(position);
    }

    void reset() {
        this->marker_ = nullptr;
#if !defined(NDEBUG) || defined(_DEBUG)
        this->marked_ = false;
#endif
    }

    void remark() {
        this->marker_ = this->stream_.current();
#if !defined(NDEBUG) || defined(_DEBUG)
        this->marked_ = true;
#endif
    }

    void set_mark(int offset = 0) {
#if !defined(NDEBUG) || defined(_DEBUG)
        if (!(this->marked_)) {
#else
        if (true) {
#endif
            this->marker_ = this->stream_.current() + offset;
#if !defined(NDEBUG) || defined(_DEBUG)
            this->marked_ = true;
#endif
        }
        else {
            throw("StreamMarker<T>:set_mark(), Error: can not be set the mark two times continuous.");
        }
    }

    void restore() {
#if !defined(NDEBUG) || defined(_DEBUG)
        if (this->marked_) {
#else
        if (true) {
#endif
            assert(this->marker_ >= this->stream_.head() || this->marker_ < this->stream_.tail());
            assert(this->marker_ != nullptr);
            this->stream_.set_current(marker_);
            this->marker_ = nullptr;
#if !defined(NDEBUG) || defined(_DEBUG)
            this->marked_ = false;
#endif
        }
        else {
            throw("StreamMarker<T>:restore(), Error: have no set the mark yet, can not be restore.");
        }
    }

    bool copy_string(char * word, size_t size) {
        assert(word != nullptr);
        assert(this->end() >= this->start());
        if (this->has_marked()) {
            return StringUtils::sub_str(word, size, this->start(), this->end());
        }
        else {
            *word = '\0';
            return false;
        }
    }

    template <size_t N>
    bool copy_string(char (&word)[N]) {
        return this->copy_string(word, N);
    }
};

typedef StreamMarker<StringStream> StringStreamMarker;
typedef StreamMarker<MemoryStream> MemoryStreamMarker;

} // namespace jlang

#endif // JLANG_STREAM_STREAMMARKER_H
