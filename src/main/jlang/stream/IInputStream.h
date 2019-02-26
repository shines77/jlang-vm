
#ifndef JLANG_STREAM_IINPUTSTREAMROOT_H
#define JLANG_STREAM_IINPUTSTREAMROOT_H

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
// class InputStream<T>
///////////////////////////////////////////////////

template <typename T>
class IInputStream {
public:
    IInputStream() {
        /* Do nothing!! */
    }

    virtual ~IInputStream() {
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
    bool is_null() const { return (this->get() == '\0'); }

    void move(int offset) {
        char * current = this->_current();
        current += offset;
        this->_set_current(current);
    }

    void back() {
        char * current = this->_current();
        current--;
        this->_set_current(current);
    }

   void back(int offset) {
        this->move(-offset);
    }

    void next() {
        char * current = this->_current();
        current++;
        this->_set_current(current);
    }

    void next(int offset) {
        this->move(offset);
    }

    int8_t get() const {
        return (int8_t)(*this->_current());
    }

    uint8_t getu() const {
        return (uint8_t)(*this->_current());
    }

    int8_t get(int offset) const {
        return (int8_t)(*(this->_current() + offset));
    }

    uint8_t getu(int offset) const {
        return (uint8_t)(*(this->_current() + offset));
    }
};

} // namespace jlang

#endif // JLANG_STREAM_IINPUTSTREAMROOT_H
