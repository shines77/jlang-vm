
#ifndef JLANG_STREAM_DETAIL_H
#define JLANG_STREAM_DETAIL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#define assert_current()                \
    do {                                \
        assert(this->current_ != nullptr);    \
        assert(this->current_ >= this->head_ && this->current_ <= this->tail_); \
    } while (0)

#define assert_current_offset(offset)   \
    assert_current();                   \
    do {                                \
        assert((this->current_ + offset) != nullptr);    \
        assert((this->current_ + offset) >= this->head_ && (this->current_ + offset) <= this->tail_); \
    } while (0)

namespace jlang {

///////////////////////////////////////////////////
// struct SeekType
///////////////////////////////////////////////////

struct SeekType {
    enum Type {
        Begin = 0,
        End = 1
    };
};

} // namespace jlang

namespace jlang {
namespace detail {

// TODO:

} // namespace detail
} // namespace jlang

#endif // JLANG_STREAM_DETAIL_H
