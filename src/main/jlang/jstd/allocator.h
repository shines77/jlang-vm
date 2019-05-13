
#ifndef JSTD_ALLOCATOR_H
#define JSTD_ALLOCATOR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/basic/stddef.h"

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <memory.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <memory>
#include <functional>

#define MINIMUM_ALIGNMENT   4
#define DEFAULT_ALIGNMENT   8

namespace jstd {

// TODO: _Allocate()

inline
void * _Allocate(std::size_t size) {
    void * ptr = nullptr;
    if (likely(size != 0)) {
        ptr = ::malloc(size);
    }
    return ptr;
}

// TODO: _Deallocate()

inline
void _Deallocate(void * p, std::size_t element_size = 0) {
    ::free(p);
}

inline
void _Deallocate(void * p, std::size_t count, std::size_t element_size) {
    _Deallocate(p, count * element_size);
}

// TODO: _AlignedAllocate()

inline
void * _AlignedAllocate(std::size_t size,
                 std::size_t alignment = 8) {
    void * ptr = nullptr;
    if (likely(size != 0)) {
#ifdef _WIN32
        ptr = ::_aligned_malloc(size, alignment);
#else
        ptr = ::memalign(alignment, size);
#endif
    }
    return ptr;
}

// TODO: _AlignedDeallocate()

inline
void _AlignedDeallocate(void * p, std::size_t element_size = 0) {
#ifdef _WIN32
    ::_aligned_free(p);
#else
    ::free(p);
#endif
}

inline
void _AlignedDeallocate(void * p, std::size_t count, std::size_t element_size) {
    _AlignedDeallocate(p, count * element_size);
}

template <typename T>
struct minimum_alignment {
    static const std::size_t value = ((alignof(T) >= MINIMUM_ALIGNMENT) ? alignof(T) : MINIMUM_ALIGNMENT);
};

template <typename T, std::size_t Alignment = minimum_alignment<T>::value>
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

    typedef allocator<T, Alignment> this_type;

    // TODO: kAlignment = round_to_power2(Alignment);
    static const size_type kAlignment = Alignment;

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

    size_type alignment() const { return kAlignment; }

    pointer address(reference value) const {
        return std::addressof(value);
    }

    const_pointer address(const_reference value) const {
        return std::addressof(value);
    }

    inline pointer allocate() {
        if (kAlignment != 0)
            return static_cast<pointer>(_AlignedAllocate(sizeof(value_type), kAlignment));
        else
            return static_cast<pointer>(_Allocate(sizeof(value_type)));
    }

    inline pointer allocate(size_type count) {
        if (kAlignment != 0)
            return static_cast<pointer>(_AlignedAllocate(count * sizeof(value_type), kAlignment));
        else
            return static_cast<pointer>(_Allocate(count * sizeof(value_type)));
    }

    inline pointer allocate(size_type count, const void *) {
        return this->allocate(count, kAlignment);
    }

    inline void deallocate(pointer ptr) {
        if (kAlignment != 0)
            _AlignedDeallocate(ptr, sizeof(value_type));
        else
            _Deallocate(ptr, sizeof(value_type));
    }

    inline void deallocate(pointer ptr, size_type count) {
        if (kAlignment != 0)
            _AlignedDeallocate(ptr, count, sizeof(value_type));
        else
            _Deallocate(ptr, count, sizeof(value_type));
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

    inline pointer create() {
        pointer ptr = this->allocate();
        this->constructor(ptr);
        return ptr;
    }

    template <typename ...Args>
    inline pointer create(Args && ... args) {
        pointer ptr = this->allocate(std::forward<Args>(args)...);
        this->constructor(ptr);
        return ptr;
    }

    inline void destroy(pointer ptr) {
        assert(ptr != nullptr);
        this->destructor(ptr);
        this->deallocate(ptr);
    }
    
    inline void destroy(pointer ptr, size_type count) {
        assert(ptr != nullptr);
        for (size_type i = count; i != 0; --i) {
            this->destructor(ptr);
            ptr++;
        }
        this->deallocate(ptr, count);
    }

    size_type max_size() const {
        // Estimate maximum array size
        return ((size_type)(-1) / sizeof(value_type));
    }
};

template <typename T, std::size_t Alignment = minimum_alignment<T>::value>
class generic_allocator {
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

    typedef generic_allocator<T, Alignment> this_type;

    // TODO: kAlignment = round_to_power2(Alignment);
    static const size_type kAlignment = Alignment;

public:
    generic_allocator() {}
    generic_allocator(const this_type &) {}

    template <typename Other>
    generic_allocator(const generic_allocator<Other, Alignment> &) {}

    template <typename Other>
    this_type & operator = (const generic_allocator<Other, Alignment> &) {
        return *this;
    }

    ~generic_allocator() {}

    size_type alignment() const { return kAlignment; }

    pointer address(reference value) const {
        return std::addressof(value);
    }

    const_pointer address(const_reference value) const {
        return std::addressof(value);
    }

    inline pointer create() {
        pointer ptr = ::new value_type();
        return ptr;
    }

    template <typename ...Args>
    inline pointer create(Args && ... args) {
        pointer ptr = ::new value_type(std::forward<Args>(args)...);
        return ptr;
    }

    inline void destroy(pointer ptr) {
        assert(ptr != nullptr);
        delete ptr;
    }
    
    inline void destroy(pointer ptr, size_type count) {
        assert(ptr != nullptr);
        delete[] ptr;
    }

    size_type max_size() const {
        // Estimate maximum array size
        return ((size_type)(-1) / sizeof(value_type));
    }
};

} // namespace jstd

#endif // JSTD_ALLOCATOR_H
