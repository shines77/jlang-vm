
#ifndef JLANG_FS_FILENAME_H
#define JLANG_FS_FILENAME_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/fs/FileSystem.h"

#include <string>

namespace jlang {
namespace fs {

class FilePath;

class FileName {
private:
    std::string filename_;

public:
    FileName() {}
    FileName(const char * filename)
        : filename_(fs::completePath(filename)) {
    }
    FileName(const std::string & filename)
        : filename_(fs::completePath(filename)) {
    }
    FileName(const char * base_dir, const char * filename)
        : filename_(fs::completePath(base_dir, filename)) {
    }
    FileName(const std::string & base_dir, const std::string & filename)
        : filename_(fs::completePath(base_dir, filename)) {
    }
    FileName(const FileName & src)
        : filename_(src.filename_) {
    }
    // The implement code see below
    FileName(const FilePath & src);

    virtual ~FileName() {}

    std::string & filename() { return filename_; }
    const std::string & filename() const { return filename_; }

    std::string & path() { return filename_; }
    const std::string & path() const { return filename_; }

    void setFileName(const char * filename) {
        this->setFileName(filename);
    }

    void setFileName(const char * base_dir, const char * filename) {
        this->setFileName(base_dir, filename);
    }

    void setFileName(const std::string & filename) {
        this->filename_ = fs::completePath(filename);
    }

    void setFileName(const std::string & base_dir, const std::string & filename) {
        this->filename_ = fs::completePath(base_dir, filename);
    }

    void setFileName(const fs::FileName & filename) {
        this->filename_ = filename.filename_;
    }

    // The implement code see below
    void setPath(const fs::FilePath & filename);
};

class SourceFileName {
private:
    std::string filename_;
    std::string orginal_filename_;
    bool is_same_;

public:
    SourceFileName() : is_same_(false) {}
    SourceFileName(const char * filename) :
        filename_(fs::completePath(filename)),
        orginal_filename_(filename),
        is_same_(fs::isSameFile(filename_, orginal_filename_)) {}
    SourceFileName(const std::string & filename) :
        filename_(fs::completePath(filename)),
        orginal_filename_(filename),
        is_same_(fs::isSameFile(filename_, orginal_filename_)) {}
    SourceFileName(const char * base_dir, const char * filename) :
        filename_(fs::completePath(base_dir, filename)),
        orginal_filename_(filename),
        is_same_(fs::isSameFile(filename_, orginal_filename_)) {}
    SourceFileName(const std::string & base_dir, const std::string & filename) :
        filename_(fs::completePath(base_dir, filename)),
        orginal_filename_(filename),
        is_same_(fs::isSameFile(filename_, orginal_filename_)) {}
    SourceFileName(const SourceFileName & src) :
        filename_(src.filename_), orginal_filename_(src.orginal_filename_),
        is_same_(src.is_same_) {}

    // The implement code see below
    SourceFileName(const FilePath & src);

    virtual ~SourceFileName() {}

    bool isSame() const { return is_same_; }

    std::string & absoluteFile() { return filename_; }
    std::string & originalFile() { return orginal_filename_; }

    const std::string & absoluteFile() const { return filename_; }
    const std::string & originalFile() const { return orginal_filename_; }

    std::string & absolutePath() { return filename_; }
    std::string & originalPath() { return orginal_filename_; }

    const std::string & absolutePath() const { return filename_; }
    const std::string & originalPath() const { return orginal_filename_; }

    void setFileName(const char * filename) {
        this->setFileName(filename);
    }

    void setFileName(const char * base_dir, const char * filename) {
        this->setFileName(base_dir, filename);
    }

    void setFileName(const std::string & filename) {
        this->filename_ = fs::completePath(filename);
        this->orginal_filename_ = filename;
        // Check the absolute filename is same to original filename?
        this->is_same_ = fs::isSameFile(this->filename_, this->orginal_filename_);
    }

    void setFileName(const std::string & base_dir, const std::string & filename) {
        this->filename_ = fs::completePath(base_dir, filename);
        this->orginal_filename_ = filename;
        // Check the absolute filename is same to original filename?
        this->is_same_ = fs::isSameFile(this->filename_, this->orginal_filename_);
    }

    void setFileName(const fs::SourceFileName & filename) {
        this->filename_ = filename.absoluteFile();
        this->orginal_filename_ = filename.originalFile();
        this->is_same_ = filename.isSame();
    }

    // The implement code see below
    void setPath(const fs::FilePath & filename);
};

class FilePath {
private:
    std::string path_;

public:
    FilePath(const char * path) : path_(fs::completePath(path)) {}
    FilePath(const std::string & path) : path_(fs::completePath(path)) {}
    FilePath(const FilePath & src) : path_(src.path_) {}
    FilePath(const FileName & src) : path_(src.path()) {}
    FilePath(const SourceFileName & src) : path_(src.absolutePath()) {}
    virtual ~FilePath() {}

    const std::string & path() const {
        return path_;
    }
};

} // namespace fs
} // namespace jlang

#endif // JLANG_FS_FILENAME_H
