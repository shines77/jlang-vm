
#ifndef JLANG_LANG_NONCOPYABLE_H
#define JLANG_LANG_NONCOPYABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace jlang {

///////////////////////////////////////////////////
// class NonCopyable
///////////////////////////////////////////////////

class NonCopyable {
public:
    NonCopyable() = default;
    ~NonCopyable() = default;

    // NonCopyable
    NonCopyable(const NonCopyable & rhs) = delete;
    NonCopyable(NonCopyable && rhs) = delete;
    NonCopyable & operator = (const NonCopyable & rhs) = delete;
};

} // namespace jlang

#endif // JLANG_LANG_NONCOPYABLE_H
