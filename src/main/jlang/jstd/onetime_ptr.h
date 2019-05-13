
#ifndef JSTD_ONETIME_PTR_H
#define JSTD_ONETIME_PTR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"
#include "jlang/jstd/allocator.h"

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <memory.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <memory>
#include <functional>

namespace jstd {

template <typename T>
struct pure_type {
    typedef typename std::remove_cv<
                typename std::remove_reference<
                    typename std::remove_pointer<T>::type
                >::type
            >::type type;
};

template <typename T>
class onetime_ptr : public pure_type<T>::type {
public:
    typedef typename pure_type<T>::type value_type;
    typedef onetime_ptr<T>              this_type;

protected:
    value_type * value_;
    bool shifted_;

public:
    onetime_ptr(value_type * value = nullptr) : value_(value), shifted_(false) {}
    onetime_ptr(this_type & src) {
        this->assign(src);
    }
    onetime_ptr(this_type && src) {
        this->assign(std::forward<this_type>(src));
    }
    virtual ~onetime_ptr() {
        this->destroy();
    }

    onetime_ptr & operator = (this_type & rhs) {
        this->copy(rhs);
        return *this;
    }

    onetime_ptr & operator = (value_type * value) {
        this->copy_from(value);
        return *this;
    }

private:
    void internal_destroy() {
        if (this->value_) {
            delete this->value_;
            this->value_ = nullptr;
        }
    }

    void destroy() {
        if (!this->shifted_) {
            this->internal_destroy();
            this->shifted_ = true;
        }
    }

    void assign(this_type && src) {
        assert(&src != this);
        this->value_ = src.value_;
        this->shifted_ = src.shifted_;
        src.set_shifted();
    }

    void assign(value_type * value) {
        assert((value == nullptr) || (value != nullptr && value != this->value));
        this->value_ = value;
        this->shifted_ = false;
    }

public:
    value_type * value() const { return this->value_; }

    template <typename ...Args>
    JM_FORCEINLINE void create_new(Args && ... args) {
        if (!this->shifted_) {
            if (this->value_) {
                delete this->value_;
            }
        }
        this->value_ = new value_type(std::forward<Args>(args)...);
        this->shifted_ = false;
    }

    value_type & get() const {
        assert(this->value_ != nullptr);
        return *(this->value_);
    }

    void reset(value_type * value = nullptr) {
        if (value != this->value_) {
            this->destroy();
            this->assign(value);
        }
    }

    void copy(this_type & src) {
        if (&src != this) {
            this->destroy();
            this->assign(src);
        }
    }

    void copy_from(value_type * value) {
        if (value != nullptr) {
            if (value != this->value_) {
                this->destroy();
                this->assign(value);
            }
        }
        else {
            this->destroy();
            this->shifted_ = false;
        }
    }

    bool is_shifted() const {
        return this->shifted_;
    }

    void set_shifted(bool shifted = true) {
        this->shifted_ = shifted;
    }

    value_type * shift() {
        this->shifted_ = true;
        return this->value_;
    }
};

template <typename T, typename Allocator = generic_allocator<T>>
class custom_onetime_ptr : public pure_type<T>::type {
public:
    typedef typename pure_type<T>::type         value_type;
    typedef Allocator                           allocator_type;
    typedef custom_onetime_ptr<T, Allocator>    this_type;

protected:
    value_type * value_;
    bool shifted_;
    allocator_type allocator_;

public:
    custom_onetime_ptr(value_type * value = nullptr) : value_(value), shifted_(false) {}
    custom_onetime_ptr(this_type & src) {
        this->assign(src);
    }
    custom_onetime_ptr(this_type && src) {
        this->assign(std::forward<this_type>(src));
    }
    virtual ~custom_onetime_ptr() {
        this->destroy();
    }

    custom_onetime_ptr & operator = (this_type & rhs) {
        this->copy(rhs);
        return *this;
    }

    custom_onetime_ptr & operator = (value_type * value) {
        this->copy_from(value);
        return *this;
    }

private:
    void internal_destroy() {
        if (this->value_) {
            this->allocator_.destroy(this->value_, sizeof(value_type));
            this->value_ = nullptr;
        }
    }

    void destroy() {
        if (!this->shifted_) {
            this->internal_destroy();
            this->shifted_ = true;
        }
    }

    void assign(this_type && src) {
        assert(&src != this);
        this->value_ = src.value_;
        this->shifted_ = src.shifted_;
        src.set_shifted();
    }

    void assign(value_type * value) {
        assert((value == nullptr) || (value != nullptr && value != this->value));
        this->value_ = value;
        this->shifted_ = false;
    }

public:
    value_type * value() const { return this->value_; }

    template <typename ...Args>
    JM_FORCEINLINE void create_new(Args && ... args) {
        if (!this->shifted_) {
            if (this->value_) {
                this->allocator_.destroy(this->value_, sizeof(value_type));
            }
        }
        this->value_ = this->allocator_.create(std::forward<Args>(args)...);
        this->shifted_ = false;
    }

    value_type & get() const {
        assert(this->value_ != nullptr);
        return *(this->value_);
    }

    void reset(value_type * value = nullptr) {
        if (value != this->value_) {
            this->destroy();
            this->assign(value);
        }
    }

    void copy(this_type & src) {
        if (&src != this) {
            this->destroy();
            this->assign(src);
        }
    }

    void copy_from(value_type * value) {
        if (value != nullptr) {
            if (value != this->value_) {
                this->destroy();
                this->assign(value);
            }
        }
        else {
            this->destroy();
            this->shifted_ = false;
        }
    }

    bool is_shifted() const {
        return this->shifted_;
    }

    void set_shifted(bool shifted = true) {
        this->shifted_ = shifted;
    }

    value_type * shift() {
        this->shifted_ = true;
        return this->value_;
    }
};

} // namespace jstd

#endif // JSTD_ONETIME_PTR_H
