
#ifndef JLANG_LANG_MEMORYSTREAM_H
#define JLANG_LANG_MEMORYSTREAM_H

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
// class MemoryStream
///////////////////////////////////////////////////

class MemoryStream : public IInputStream<MemoryStream>,
                     public IOutputStream<MemoryStream>,
                     public StreamRoot {
public:
    typedef StreamRoot root_type;

    MemoryStream() : root_type() {
    }

    virtual ~MemoryStream() {
    }

#if 0
    //----------------------------------------------------------------------

    root_type * __this() {
        return static_cast<root_type *>(this);
    }

    const root_type * __this() const {
        return (const root_type *)(this);
    }

    // getter
    char * _current() { return root_type::_get_current(); }
    char * _head()    { return root_type::_get_head(); }
    char * _tail()    { return root_type::_get_tail(); }

    const char * _current() const { return root_type::_get_current(); }
    const char * _head()    const { return root_type::_get_head(); }
    const char * _tail()    const { return root_type::_get_tail(); }

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
        if (this->current_) {
            if (this->current_ + size <= this->tail_) {
                ::memcpy(this->current_, buf, size);
                this->current_ += size;
            }
            else {
                ::memcpy(this->current_, buf, this->tail_ - this->current_);
                this->current_ =this->tail_;
            }
        }
        return this->current_;
    }

private:
    // NonCopyable
    MemoryStream(const MemoryStream & src) = delete;
    MemoryStream & operator = (const MemoryStream & rhs) {
        return *this;
    }
};

} // namespace jlang

#endif // JLANG_LANG_MEMORYSTREAM_H
