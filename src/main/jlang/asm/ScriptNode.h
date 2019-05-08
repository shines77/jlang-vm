
#ifndef JLANG_ASM_SCRIPTNODE_H
#define JLANG_ASM_SCRIPTNODE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"
#include "jlang/lang/NonCopyable.h"

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <memory.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <memory>
#include <xmemory>
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
    virtual ~onetime_ptr() {
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

    void assign(onetime_ptr && src) {
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

    template <typename ...Args>
    JM_FORCEINLINE void create_new(Args && ... args) {
        if (!this->shifted_) {
            if (this->value_) {
                delete this->value_;
            }
        }
        this->value_ = new element_type(std::forward<Args>(args)...);
        this->shifted_ = false;
    }

    element_type & get() const {
        assert(this->value_ != nullptr);
        return *(this->value_);
    }

    void reset(element_type * value = nullptr) {
        if (value != this->value_) {
            this->destroy();
            this->assign(value);
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

inline
void * _Allocate(std::size_t element_count, std::size_t element_size,
		         bool try_aligned_allocation = true,
                 std::size_t alignment = 8) {
    void * ptr = nullptr;

    if (element_count == 0)
        return ptr;

    const std::size_t total_size = element_size * element_count;
    if (try_aligned_allocation) {
#ifdef _WIN32
        ptr = _aligned_malloc(total_size, alignment);
#else
        ptr = memalign(alignment, total_size);
#endif
    }
    else {
        ptr = ::operator new(total_size);
    }
    return ptr;
}

template <typename T>
class allocator {
public:
    typedef typename std::remove_const<
                typename std::remove_pointer<T>::type
            >::type value_type;

    typedef value_type *        pointer;
    typedef const value_type *  const_pointer;

    typedef value_type &        reference;
    typedef const value_type &  const_reference;

    typedef std::size_t         size_type;
    typedef std::ptrdiff_t      difference_type;

    typedef true_type           propagate_on_container_move_assignment;
    typedef true_type           is_always_equal;

    typedef allocator<T>        this_type;

public:
    allocator() {}
    allocator(const this_type &) {}

    template <typename Other>
    allocator(const allocator<Other> &) {}

    template <typename Other>
    this_type & operator = (const allocator<Other> &) {
        return *this;
    }

    ~allocator() {}

    pointer address(reference value) const {
        return std::addressof(value);
    }

    const_pointer address(const_reference value) const {
        return std::addressof(value);
    }

    pointer allocate(size_type count) {
        // TODO:
        return static_cast<pointer>(_Allocate(count, sizeof(value_type)));
    }

    pointer allocate(size_type count, const void *) {
        return this->allocate(count);
    }

    void deallocate(pointer ptr, size_type count) {
        // TODO:
        //_Deallocate(ptr, count, sizeof(value_type));
    }

    inline pointer create() {
        pointer value = new value_type();
        return value;
    }

    template <typename ...Args>
    inline pointer create(Args && ... args) {
        pointer value = new value_type(std::forward<Args>(args)...);
        return value;
    }

    inline void destroy(pointer ptr, size_type size) {
        assert(ptr != nullptr);
        assert(size = sizeof(value_type));
        delete ptr;
    }
    
    inline void destroy(pointer ptr, size_type size, const void *) {
        assert(ptr != nullptr);
        assert(size = sizeof(value_type));
        delete[] ptr;
    }

    inline void constructor(pointer ptr) {
        assert(ptr != nullptr);
        void * vptr = static_cast<void *>(ptr);
        ::new (vptr) value_type();
    }

    template <typename ...Args>
    inline void constructor(pointer ptr, Args && ... args) {
        assert(ptr != nullptr);
        void * vptr = static_cast<void *>(ptr);
        ::new (vptr) value_type(std::forward<Args>(args)...);
    }

    template <typename U>
    inline void destructor(U * ptr) {
        assert(ptr != nullptr);
        ptr->~U();
    }

    size_type max_size() const {
        // Estimate maximum array size
        return ((size_type)(-1) / sizeof(value_type));
    }
};

template <typename T, typename Allocator = allocator<T>>
class custom_onetime_ptr : public pure_type<T>::type {
public:
    typedef typename pure_type<T>::type     element_type;
    typedef Allocator                       allocator_type;

    typedef std::function<void(element_type *, size_t)> deleter_type;

protected:
    element_type * value_;
    bool shifted_;
    allocator_type allocator_;

public:
    custom_onetime_ptr(element_type * value = nullptr) : value_(value), shifted_(false) {}
    custom_onetime_ptr(custom_onetime_ptr & src) {
        this->assign(src);
    }
    custom_onetime_ptr(custom_onetime_ptr && src) {
        this->assign(std::forward<custom_onetime_ptr>(src));
    }
    virtual ~custom_onetime_ptr() {
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
            assert(this->deleter_);
            this->allocator_.destroy(this->value_, sizeof(element_type));
            this->value_ = nullptr;
        }
    }

    void destroy() {
        if (!this->shifted_) {
            this->internal_destroy();
            this->shifted_ = true;
        }
    }

    void assign(custom_onetime_ptr && src) {
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

    void assign(element_type * value, const deleter_type & deleter) {
        assert((value == nullptr) || (value != nullptr && value != this->value));
        this->value_ = value;
        this->shifted_ = false;
        this->deleter_ = deleter;
    }

public:
    element_type * value() const { return this->value_; }

    template <typename ...Args>
    JM_FORCEINLINE void create_new(Args && ... args) {
        if (!this->shifted_) {
            if (this->value_) {
                this->allocator_.destroy(this->value_, sizeof(element_type));
            }
        }
        this->value_ = new element_type(std::forward<Args>(args)...);
        this->shifted_ = false;
    }

    element_type & get() const {
        assert(this->value_ != nullptr);
        return *(this->value_);
    }

    void reset(element_type * value = nullptr) {
        if (value != this->value_) {
            this->destroy();
            this->assign(value);
        }
    }

    void reset(element_type * value, const deleter_type & deleter) {
        if (value != this->value_) {
            this->destroy();
            this->assign(value, deleter);
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
