
#ifndef JLANG_LANG_NONCOPYABLE_H
#define JLANG_LANG_NONCOPYABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace jlang {
namespace lang {

///////////////////////////////////////////////////
// class NonCopyable
///////////////////////////////////////////////////

class NonCopyable {
public:
    NonCopyable() {}
    ~NonCopyable() {}
};

} // namespace lang
} // namespace jlang

#endif // JLANG_LANG_NONCOPYABLE_H
