
#ifndef JLANG_STREAM_STRINGSTREAM_H
#define JLANG_STREAM_STRINGSTREAM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/stream/StreamStorage.h"
#include "jlang/stream/IInputStream.h"
#include "jlang/stream/IOutputStream.h"
#include "jlang/stream/StreamRoot.h"

#include <stddef.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <assert.h>

namespace jlang {

///////////////////////////////////////////////////
// class StringStream
///////////////////////////////////////////////////

class StringStream : public IInputStream<StringStream>,
                     public IOutputStream<StringStream>,
                     public StreamRoot {
public:
    typedef StreamRoot root_type;

    StringStream() : root_type() {
    }

    virtual ~StringStream() {
    }

#if 1
    //----------------------------------------------------------------------

    root_type * __this() {
        return static_cast<root_type *>(this);
    }

    const root_type * __this() const {
        return (const root_type *)(this);
    }

    // getter
    char * _get_current() { return root_type::_get_current(); }
    char * _get_head()    { return root_type::_get_head(); }
    char * _get_tail()    { return root_type::_get_tail(); }

    const char * _get_current() const { return root_type::_get_current(); }
    const char * _get_head()    const { return root_type::_get_head(); }
    const char * _get_tail()    const { return root_type::_get_tail(); }

    // setter
    void _set_current(char * value) { root_type::_set_current(value); }
    void _set_head(char * value)    { root_type::_set_head(value); }
    void _set_tail(char * value)    { root_type::_set_tail(value); }

    //----------------------------------------------------------------------
#endif

    const char * write_impl(const char * buf, size_t size) {
        assert(buf != nullptr);
        assert(this->current_ != nullptr);
        assert(!this->is_overflow());
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
        if (this->current_ + size <= this->tail_) {
            errno_t err = ::memcpy_s(this->current_, size, buf, size);
            if (err == 0 || err == STRUNCATE) {
                // No error
            }
            this->current_ += size;
        }
        else {
            errno_t err = ::memcpy_s(this->current_, this->remain_sizes(), buf, this->remain_sizes());
            if (err == 0 || err == STRUNCATE) {
                // No error
            }
            this->current_ = this->tail_;
        }
#else
        if (this->current_ + size <= this->tail_) {
            ::memcpy(this->current_, buf, size);
            this->current_ += size;
        }
        else {
            ::memcpy(this->current_, buf, this->remain_sizes());
            this->current_ = this->tail_;
        }
#endif
        return this->current_;
    }

private:
    // NonCopyable
    StringStream(const StringStream & src) = delete;
    StringStream(StringStream && src) = delete;
    StringStream & operator = (const StringStream & rhs) = delete;
};

} // namespace jlang

#endif // JLANG_STREAM_STRINGSTREAM_H
