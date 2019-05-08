
#ifndef JLANG_ASM_SCRIPTNODE_H
#define JLANG_ASM_SCRIPTNODE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"
#include "jlang/lang/NonCopyable.h"

#include <type_traits>
#include <functional>

namespace jlang {
namespace jasm {

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
    typedef typename pure_type<T>::type element_type;

    typedef std::function<void(element_type *, size_t)> deleter_type;

protected:
    element_type * value_;
    bool shifted_;

public:
    onetime_ptr(element_type * value = nullptr) : value_(value), shifted_(false) {}
    onetime_ptr(onetime_ptr & src) {
        this->assign(src);
    }
    onetime_ptr(onetime_ptr && src) {
        this->assign(std::forward<onetime_ptr>(src));
    }
    ~onetime_ptr() {
        this->destroy();
    }

    onetime_ptr & operator = (onetime_ptr & rhs) {
        this->copy(rhs);
        return *this;
    }

    onetime_ptr & operator = (element_type * value) {
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

    void assign(onetime_ptr & src) {
        assert(&src != this);
        this->value_ = src.value_;
        this->shifted_ = src.shifted_;
        src.set_shifted();
    }

    void assign(element_type * value) {
        assert((value == nullptr) || (value != nullptr && value != this->value));
        this->value_ = value;
        this->shifted_ = false;
    }

public:
    element_type * value() const { return this->value_; }

    element_type & get() const {
        assert(this->value_ != nullptr);
        return *(this->value_);
    }

    void reset(element_type * value) {
        if (value != this->value_) {
            this->destroy();
            this->shifted_ = false;
            this->value_ = value_;
        }
    }

    void copy(onetime_ptr & src) {
        if (&src != this) {
            this->destroy();
            this->assign(src);
        }
    }

    void copy_from(element_type * value) {
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

    void set_shifted(bool isShifted = true) {
        this->shifted_ = isShifted;
    }

    element_type * shift() {
        this->shifted_ = true;
        return this->value_;
    }
};

template <typename T>
class custom_onetime_ptr : public pure_type<T>::type {
public:
    typedef typename pure_type<T>::type element_type;

    typedef std::function<void(element_type *, size_t)> deleter_type;

protected:
    element_type * value_;
    bool shifted_;
    deleter_type deleter_;

public:
    custom_onetime_ptr(element_type * value = nullptr) : value_(value), shifted_(false) {}
    custom_onetime_ptr(const deleter_type & deleter) : value_(nullptr),
        shifted_(false), deleter_(deleter) {}
    custom_onetime_ptr(element_type * value, const deleter_type & deleter)
        : value_(value), shifted_(false), deleter_(deleter) {}
    custom_onetime_ptr(custom_onetime_ptr & src) {
        this->assign(src);
    }
    custom_onetime_ptr(custom_onetime_ptr && src) {
        this->assign(std::forward<custom_onetime_ptr>(src));
    }
    ~custom_onetime_ptr() {
        this->destroy();
    }

    custom_onetime_ptr & operator = (custom_onetime_ptr & rhs) {
        this->copy(rhs);
        return *this;
    }

    custom_onetime_ptr & operator = (element_type * value) {
        this->copy_from(value);
        return *this;
    }

private:
    void internal_destroy() {
        if (this->value_) {
            if (this->deleter_) {
                this->deleter_(this->value_, sizeof(element_type));
            }
            this->value_ = nullptr;
        }
    }

    void destroy() {
        if (!this->shifted_) {
            this->internal_destroy();
            this->shifted_ = true;
        }
    }

    void assign(custom_onetime_ptr & src) {
        assert(&src != this);
        this->value_ = src.value_;
        this->shifted_ = src.shifted_;
        this->deleter_ = src.deleter_;
        src.set_shifted();
    }

    void assign(element_type * value) {
        assert((value == nullptr) || (value != nullptr && value != this->value));
        this->value_ = value;
        this->shifted_ = false;
    }

public:
    element_type * value() const { return this->value_; }
    element_type * deleter() const { return this->deleter_; }

    element_type & get() const {
        assert(this->value_ != nullptr);
        return *(this->value_);
    }

    void reset(element_type * value) {
        if (value != this->value_) {
            this->destroy();
            this->shifted_ = false;
            this->value_ = value_;
        }
    }

    void reset(element_type * value, const deleter_type & deleter) {
        if (value != this->value_) {
            this->destroy();
            this->shifted_ = false;
            this->value_ = value_;
            this->deleter_ = deleter;
        }
    }

    void copy(custom_onetime_ptr & src) {
        if (&src != this) {
            this->destroy();
            this->assign(src);
        }
    }

    void copy_from(element_type * value) {
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

    void set_deleter(const deleter_type & deleter) {
        this->deleter_ = deleter;
    }

    bool is_shifted() const {
        return this->shifted_;
    }

    void set_shifted(bool isShifted = true) {
        this->shifted_ = isShifted;
    }

    element_type * shift() {
        this->shifted_ = true;
        return this->value_;
    }
};

///////////////////////////////////////////////////
// class ScriptNode
///////////////////////////////////////////////////

class ScriptNode {
private:
    bool inited_;

public:
    ScriptNode() : inited_(false) {
        init();
    }
    virtual ~ScriptNode() {
        destroy();
    }

    bool isInited() const { return inited_; }

    void init() {
        if (!inited_) {
            inited_ = true;
        }
    }

    void destroy() {
        if (inited_) {
            inited_ = false;
        }
    }

    void append(ScriptNode * node) {
        // TODO:
    }
};

typedef onetime_ptr<ScriptNode> ScriptNodePtr;

} // namespace jasm
} // namespace jlang

#endif // JLANG_ASM_SCRIPTNODE_H
