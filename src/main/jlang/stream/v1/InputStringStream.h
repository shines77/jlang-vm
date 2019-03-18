
#ifndef JLANG_STREAM_INPUTSTRINGSTREAM_H
#define JLANG_STREAM_INPUTSTRINGSTREAM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/stream/IInputStream.h"
#include "jlang/stream/IOutputStream.h"
#include "jlang/stream/StreamRoot.h"

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include <string>
#include <iosfwd>   // For std::ios, std::ios_base
#include <iostream>
#include <fstream>
#include <exception>

namespace jlang {

///////////////////////////////////////////////////
// class InputStringStream
///////////////////////////////////////////////////

class InputStringStream : public IInputStream<InputStringStream>,
                          public StreamRoot {
public:
    typedef StreamRoot root_type;

    InputStringStream() : root_type() {
    }

    ~InputStringStream() {
    }

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
};

} // namespace jlang

#endif // JLANG_STREAM_INPUTSTRINGSTREAM_H
