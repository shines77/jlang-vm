
#ifndef JLANG_FS_FILEINPUTSTREAM_H
#define JLANG_FS_FILEINPUTSTREAM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/stream/StringStream.h"
#include "jlang/fs/FileName.h"

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
// class FileStringStream
///////////////////////////////////////////////////

class FileStringStream {
private:
    StringStream stream_;
    bool loaded_;
    fs::FileName filename_;

    static const std::ios_base::openmode default_mode = std::ios::in | std::ios::binary;
    static const int default_prot = 64;

public:
    FileStringStream() : loaded_(false) {
    }
    FileStringStream(const std::string & filename,
               std::ios_base::openmode mode = default_mode, int prot = default_prot)
        : loaded_(false), filename_(filename) {
        loadFile(filename, mode, prot);
    }
    FileStringStream(const std::string & base_dir, const std::string & filename,
               std::ios_base::openmode mode = default_mode, int prot = default_prot)
        : loaded_(false), filename_(base_dir, filename) {
        loadFile(base_dir, filename, mode, prot);
    }
    virtual ~FileStringStream() {}

    StringStream & getStream() {
        return this->stream_;
    }

    const StringStream & getStream() const {
        return this->stream_;
    }

    std::string & filename() {
        return this->filename_.filename();
    }

    const std::string & filename() const {
        return this->filename_.filename();
    }

    std::string & path() {
        return this->filename();
    }

    const std::string & path() const {
        return this->filename();
    }

    bool isLoaded() const { return this->loaded_; }

    bool loadFile(const std::string & filename, std::ios_base::openmode mode = default_mode, int prot = 64) {
        this->filename_.setFileName(filename);
        return this->loadFileInternal(filename_.filename(), mode, prot);
    }

    bool loadFile(const std::string & base_dir, const std::string & filename,
                  std::ios_base::openmode mode = default_mode, int prot = 64) {
        this->filename_.setFileName(base_dir, filename);
        return this->loadFileInternal(filename_.filename(), mode, prot);
    }

private:
    bool loadFileInternal(const std::string & absolute_filename,
                          std::ios_base::openmode mode = std::ios::in | std::ios::binary, int prot = 64) {
        static const std::streamsize kReadBufSize = 8192;
        bool loaded = false;

        std::ifstream ifs;
        try {
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__) 
            ifs.open(absolute_filename.c_str(), mode, prot);
#else
            ifs.open(absolute_filename.c_str(), mode);
#endif
            if (ifs.is_open()) {
                ifs.seekg(0, std::ios::end);
                std::ifstream::pos_type file_size = ifs.tellg();
                if (this->stream_.sizes() != 0)
                    this->stream_.clear();
                this->stream_.seek(SeekType::Begin);
                this->stream_.reserve(static_cast<size_t>(file_size));

                ifs.seekg(0, std::ios::beg);
                char buffer[8192];

                while (!ifs.eof()) {
                    buffer[0] = '\0';
                    ifs.read(buffer, kReadBufSize);
                    size_t readBytes = static_cast<size_t>(ifs.gcount());
                    buffer[readBytes] = '\0';
                    if (readBytes > 0) {
                        this->stream_.write(buffer, readBytes);
                    }
                }
                ifs.close();
                this->stream_.put_null();
                this->stream_.reset();
                loaded = true;
            }
        }
        catch (const std::exception & ex) {
            std::cout << "Exception: class FileInputStream() - " << ex.what() << std::endl;
        }
        this->loaded_ = loaded;
        return loaded;
    }

private:
    // NonCopyable
    FileStringStream(const FileStringStream & src) = delete;
    FileStringStream & operator = (const FileStringStream & rhs) = delete;
};

} // namespace jlang

#endif // JLANG_FS_FILEINPUTSTREAM_H
