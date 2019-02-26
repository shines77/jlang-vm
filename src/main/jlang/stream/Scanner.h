
#ifndef JLANG_STREAM_SCANNER_H
#define JLANG_STREAM_SCANNER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/stream/StringStream.h"
#include "jlang/stream/InputStringStream.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#include <string>

namespace jlang {

class InputStream;

///////////////////////////////////////////////////
// class Scanner
///////////////////////////////////////////////////

template <typename T>
class Scanner {
private:
    InputStringStream & stream_;
    std::string delimier_;

public:
    Scanner(InputStringStream & stream) : stream_(stream) {}
    ~Scanner() {}

    void close() {
        //
    }

    std::string & delimiter() { return delimier_; }
    const std::string & delimiter() const { return delimier_; }

    void setDelimiter(const std::string & delimiter) {
        delimier_ = delimiter;
    }

    bool hasBefore() {
        return stream_.has_before();
    }

    bool hasBefore(char ch) {
        return stream_.has_before(ch);
    }

    bool hasNext() {
        return stream_.has_next();
    }

    bool hasNext(char ch) {
        return stream_.has_next(ch);
    }

    bool hasNext(const std::string & pattern) {
        return stream_.has_next(pattern);
    }
};

} // namespace jlang

#endif // JLANG_STREAM_SCANNER_H
