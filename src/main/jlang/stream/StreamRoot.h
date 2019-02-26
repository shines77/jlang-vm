
#ifndef JLANG_STREAM_ISTREAMROOT_H
#define JLANG_STREAM_ISTREAMROOT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/jstd/min_max.h"
#include "jlang/stream/StreamDetail.h"
#include "jlang/support/StringUtils.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#include <string>

namespace jlang {

///////////////////////////////////////////////////
// class StreamRoot
///////////////////////////////////////////////////

class StreamRoot {
protected:
    char * current_;
    char * head_;
    char * tail_;

public:
    StreamRoot() : current_(nullptr), head_(nullptr), tail_(nullptr) {
        /* Do nothing!! */
    }

    virtual ~StreamRoot() {
        this->destroy();
    }

public:
    //----------------------------------------------------------------------

    // getter
    char * _get_current() { return this->current_; }
    char * _get_head() { return this->head_; }
    char * _get_tail() { return this->tail_; }

    const char * _get_current() const { return this->current_; }
    const char * _get_head() const { return this->head_; }
    const char * _get_tail() const { return this->tail_; }

    // setter
    void _set_current(char * value) { this->current_ = value; }
    void _set_head(char * value) { this->head_ = value; }
    void _set_tail(char * value) { this->tail_ = value; }

    //----------------------------------------------------------------------

public:
    char * head() { return this->head_; }
    char * tail() { return this->tail_; }
    const char * head() const { return this->head_; }
    const char * tail() const { return this->tail_; }

    const char * chead() const { return this->head_; }
    const char * ctail() const { return this->tail_; }

    char * begin() { return this->head(); }
    char * end() { return this->tail(); }
    const char * begin() const { return this->head(); }
    const char * end() const { return this->tail(); }

    const char * cbegin() const { return this->chead(); }
    const char * cend() const { return this->ctail(); }

    char * current() { return this->current_; }
    const char * current() const { return this->current_; }

    void set_current(char * current) { this->current_ = current; }

    char * data() { return this->head_; }
    const char * data() const { return this->head_; }

    char * c_str() { return this->data(); }
    const char * c_str() const { return this->data(); }

    size_t sizes() const {
        assert(this->tail_ >= this->head_);
        return (size_t)(this->tail_ - this->head_);
    }
    size_t length() const { return this->sizes(); }
    size_t size_of() const { return (this->sizes() + 1); }

    bool is_alive() const { return (this->head_ != nullptr && this->tail_ != nullptr); }
    bool is_valid() const { return (this->current_ != nullptr); }

    bool is_head() const { return (this->current_ == this->head_); }
    bool is_tail() const { return (this->current_ == this->tail_); }
    bool is_bof()  const { return (this->current_ == this->head_); }
    bool is_eof()  const { return (this->current_ == this->tail_); }

    bool is_underflow() const { return (this->current_ <  this->head_); }
    bool is_overflow()  const { return (this->current_ >= this->tail_); }

    void reset() { this->current_ = this->head_; }
    
    intptr_t tell() const { return (this->current_ - this->head_); }
    intptr_t remain() const { return (this->tail_ - this->current_); }

    size_t remain_sizes() const {
        if (this->current_ < this->tail_)
            return this->remain();
        else
            return 0;
    }

    void destroy() {
        if (this->head_) {
            ::free(this->head_);
        }
        this->current_ = nullptr;
        this->head_ = nullptr;
        this->tail_ = nullptr;
    }

    void clear() {
        this->destroy();
    }

    void clear_data() {
        assert(this->head_ != nullptr);
        assert(this->head_ <= this->tail_);
        ::memset(this->head_, 0, this->sizes());
    }

    bool reserve(size_t size, bool need_clear = false) {
        char * new_data = (char *)::malloc(size + 1);
        if (new_data) {
            if (need_clear && this->head_) {
                ::free(this->head_);
            }
            this->current_ = new_data;
            this->head_ = new_data;
            this->tail_ = new_data + size;
            return true;
        }
        return false;
    }

    bool resize(size_t size, bool need_clear = false) {
        char * new_data = (char *)::realloc(this->head_, size + 1);
        if (new_data) {
            if (need_clear) {
                if ((size > this->sizes())) {
                    ::memset(this->head_ + this->sizes(), 0, size - this->sizes() + 1);
                }
                else if (new_data != this->head_) {
                    ::memset(new_data, 0, size + 1);
                }
            }
            this->current_ = new_data;
            this->head_ = new_data;
            this->tail_ = new_data + size;
            return true;
        }
        return false;
    }

    intptr_t copy(std::string & str, intptr_t first_pos, intptr_t last_pos) {
        assert(first_pos >= 0);
        assert(last_pos >= first_pos);
        str.append((const char *)(this->head_ + first_pos), (size_t)(last_pos - first_pos));
        return (last_pos - first_pos);
    }

    template <int offset = 0>
    void seek(const SeekType::enum_type type) {
        if (offset == 0) {
            if (type == SeekType::Begin)
                this->current_ = this->head_;
            else if (type == SeekType::End)
                this->current_ = this->tail_;
        }
        else {
            if (type == SeekType::Begin)
                this->current_ = this->head_ + offset;
            else if (type == SeekType::End)
                this->current_ = this->tail_ + offset;
            else
                this->current_ = offset;
        }
    }

    void seek(const SeekType::enum_type type, int offset = 0) {
        if (offset == 0) {
            if (type == SeekType::Begin)
                this->current_ = this->head_;
            else if (type == SeekType::End)
                this->current_ = this->tail_;
        }
        else {
            if (type == SeekType::Begin)
                this->current_ = this->head_ + offset;
            else if (type == SeekType::End)
                this->current_ = this->tail_ + offset;
            else
                this->current_ += offset;
        }
    }

    void seek_limit(const SeekType::enum_type type, int offset = 0) {
        if (type == SeekType::Begin) {
            if (offset >= 0) {
                if ((size_t)offset < this->sizes())
                    this->current_ = this->head_ + offset;
                else
                    this->current_ = this->tail_;
            }
            else {
                this->current_ = this->head_;
            }
        }
        else if (type == SeekType::End) {
            if (offset >= 0) {
                this->current_ = this->tail_;
            }
            else if ((size_t)-offset > this->sizes()) {
                this->current_ = this->tail_ + offset;
            }
            else {
                this->current_ = this->head_;
            }
        }
        else {
            if (offset != 0) {
                this->current_ += offset;
                if (this->current_ < this->head_)
                    this->current_ = this->head_;
                else if (this->current_ > this->tail_)
                    this->current_ = this->tail_;
            }
        }
    }

    int read_line(char * buf, size_t size) const {
        int line_size = StringUtils::read_line(buf, size, this->current_, this->remain());
        return line_size;
    }
};

} // namespace jlang

#endif // JLANG_STREAM_ISTREAMROOT_H
