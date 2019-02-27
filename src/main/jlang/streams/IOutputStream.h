
#ifndef JLANG_STREAM_IOUTPUTSTREAM_H
#define JLANG_STREAM_IOUTPUTSTREAM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/jstd/min_max.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#include <string>

namespace jlang {

///////////////////////////////////////////////////
// class OutputStream<T>
///////////////////////////////////////////////////

template <typename T>
class IOutputStream {
public:
    IOutputStream() {
        /* Do nothing!! */
    }

    virtual ~IOutputStream() {
    }

private:
    //----------------------------------------------------------------------

    T * __this() {
        return static_cast<T *>(this);
    }

    const T * __this() const {
        return (const T *)(this);
    }

    // getter
    char * _current() { return __this()->_get_current(); }
    char * _head()    { return __this()->_get_head(); }
    char * _tail()    { return __this()->_get_tail(); }

    const char * _current() const { return __this()->_get_current(); }
    const char * _head()    const { return __this()->_get_head(); }
    const char * _tail()    const { return __this()->_get_tail(); }

    // setter
    void _set_current(char * value) { __this()->_set_current(value); }
    void _set_head(char * value)    { __this()->_set_head(value); }
    void _set_tail(char * value)    { __this()->_set_tail(value); }

    //----------------------------------------------------------------------

public:
    void put(char ch) {       
        char * current = this->_current();
        *current++ = ch;
        this->_set_current(current);
    }

    void putu(unsigned char ch) {
        char * current = this->_current();
        *current++ = (signed char)ch;
        this->_set_current(current);
    }

    void put_null() {
        this->put('\0');
    }

    const char * write(const char * buf, size_t size) {
        assert(buf != nullptr);
        T * pThis = static_cast<T *>(this);
        if (pThis) {
            return pThis->write_impl(buf, size);
        }
        assert(pThis != nullptr);
        return nullptr;
    }

    template <size_t N>
    const char * write(char (&buff)[N]) {
        return this->write(buff, N);
    }

    const char * append(const std::string & str, size_t size) {
        assert(size <= str.length());
        return this->write(str.c_str(), size);
    }

    const char * append(const std::string & str) {
        return this->write(str.c_str(), str.length());
    }
};

} // namespace jlang

#endif // JLANG_STREAM_IOUTPUTSTREAM_H
