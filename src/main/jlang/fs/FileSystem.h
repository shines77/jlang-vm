
#ifndef JLANG_FS_FILESYSTEM_H
#define JLANG_FS_FILESYSTEM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jlang/lang/Global.h"
#include "jlang/lang/CharInfo.h"
#include "jlang/lang/Error.h"
#include "jlang/support/StringUtils.h"

#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
#ifndef _WINDOWS_LEAN
#define _WINDOWS_LEAN
#endif // _WINDOWS_LEAN
#include <windows.h>    // For Sleep(), GetModuleFileNameA()
#else
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>     // For usleep(), readlink()
#endif // WIN32

#include <string>
#include <vector>
#include <assert.h>

namespace jlang {

///////////////////////////////////////////////////
// namespace fs
///////////////////////////////////////////////////

namespace fs {

enum FileType {
    Unknown = -1,
    Error,
    Normal,
    ParentDir,
    CurrentDir,
    Directory,
    File,
    SymLink,
    Last
};

#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)

enum {
    PATH_SEPARATOR = '\\'
};

static inline bool isPathSeparator(char ch) {
    return (ch == '\\' || ch == '/');
}

static inline bool isAbsolutePath(const char * path) {
    char first_char = path[0];
    if (fs::isPathSeparator(first_char)) {
        return true;
    }
    else {
        if (CharInfo::isAlphabet(first_char)) {
            char second_char = path[1];
            if (second_char == ':') {
                return true;
            }
        }
    }
    return false;
}

static std::string getExeName() {
    std::string exe_filename;
    char filename[1024];
    DWORD dwSize = GetModuleFileNameA(NULL, filename, sizeof(filename) / sizeof(char));
    if (dwSize > 0) {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
            filename[dwSize] = '\0';
            Global::lastError().setLastError(0);
        }
        else {
            Global::lastError().setLastError(Error::ErrorInsufficientBuffer);
        }
        exe_filename = filename;
    }
    else {
        Global::lastError().setLastError(GetLastError());
    }
    return exe_filename;
}

static std::string getAppPath(bool including_slash = true) {
    std::string app_path;
    char filename[1024];
    DWORD dwSize = GetModuleFileNameA(NULL, filename, sizeof(filename) / sizeof(char));
    if (dwSize > 0) {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
            if (including_slash) {
                // The tail char is including the separator.
                for (int i = (int)(dwSize - 1); i >= 0; --i) {
                    if (filename[i] == fs::PATH_SEPARATOR) {
                        assert(i < (int)sizeof(filename));
                        filename[i + 1] = '\0';
                        break;
                    }
                }
            }
            else {
                // The tail char is not including the separator.
                for (int i = (int)(dwSize - 1); i >= 0; --i) {
                    if (filename[i] == fs::PATH_SEPARATOR) {
                        filename[i] = '\0';
                        break;
                    }
                }
            }
            Global::lastError().setLastError(0);
        }
        else {
            Global::lastError().setLastError(Error::ErrorInsufficientBuffer);
        }
        app_path = filename;
    }
    else {
        Global::lastError().setLastError(GetLastError());
    }
    return app_path;
}

#else // !_WIN32

enum {
    PATH_SEPARATOR = '/'
};

static inline bool isPathSeparator(char ch) {
    return (ch == '/' || ch == '\\');
}

static inline bool isAbsolutePath(const char * path) {
    char first_char = path[0];
    return fs::isPathSeparator(first_char);
}

#if 1

static std::string getExeName() {
    std::string exe_filename;
    char filename[1024];
    const size_t max_cnt = sizeof(filename) / sizeof(char);
    int cnt = readlink("/proc/self/exe", filename, max_cnt - 1);
    if (cnt >= 0 && cnt < (int)max_cnt) {
        filename[cnt] = '\0';
        exe_filename = filename;
        Global::lastError().setLastError(0);
    }
    else if (cnt >= (int)max_cnt) {
        Global::lastError().setLastError(Error::ErrorInsufficientBuffer);
    }
    else {
        Global::lastError().setLastError(errno);
    }
    return exe_filename;
}

#else

static std::string getExeName() {
    std::string exe_filename;
    ptrdiff_t cnt;
    char filename[1024];
    const size_t max_cnt = sizeof(filename) / sizeof(char);
    char * filename_end = getcwd(filename, max_cnt - 1);
    if (filename_end != NULL && filename_end >= filename)
        cnt = filename_end - filename;
    else
        cnt = -1;
    if (cnt >= 0 && cnt < (ptrdiff_t)max_cnt) {
        filename[cnt] = '\0';
        exe_filename = filename;
        Global::lastError().setLastError(0);
    }
    else if (cnt >= (ptrdiff_t)max_cnt) {
        Global::lastError().setLastError(Error::ErrorInsufficientBuffer);
    }
    else {
        Global::lastError().setLastError(errno);
    }
    return exe_filename;
}

#endif

static std::string getAppPath(bool including_slash = true) {
    std::string app_path;
    char filename[1024];
    const size_t max_cnt = sizeof(filename) / sizeof(char);
    int cnt = readlink("/proc/self/exe", filename, max_cnt - 1);
    if (cnt >= 0 && cnt < (int)max_cnt) {
        filename[cnt] = '\0';
        if (including_slash) {
            // The tail char is including the separator.
            for (int i = cnt; i >= 0; --i) {
                if (filename[i] == fs::PATH_SEPARATOR) {
                    assert(i < (int)sizeof(filename));
                    filename[i + 1] = '\0';
                    break;
                }
            }
        }
        else {
            // The tail char is not including the separator.
            for (int i = cnt; i >= 0; --i) {
                if (filename[i] == fs::PATH_SEPARATOR) {
                    filename[i] = '\0';
                    break;
                }
            }
        }
        app_path = filename;
        Global::lastError().setLastError(0);
    }
    else if (cnt >= (int)max_cnt) {
        Global::lastError().setLastError(Error::ErrorInsufficientBuffer);
    }
    else {
        Global::lastError().setLastError(errno);
    }
    return app_path;
}

#endif // _WIN32

static std::string appendPath(const std::string & base, const std::string & filename) {
    std::string new_path;
    std::vector<int> index_list;
    int i, index;
    int base_left = -1, pos;

    Global::lastError().setLastError(0);

    // The first step: Scan the [base] path.

    // TrimLeft(base)
    for (i = 0; i < (int)base.size(); ++i) {
        if (base[i] != ' ') {
            index_list.push_back(0);
            base_left = i;
            break;
        }
    }
    pos = base_left;

    // If base is empty.
    if ((pos + 1) >= (int)base.size()) {
        return filename;
    }

    index = 0;
    new_path.reserve(512);

#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)

    // "E:\abc"
    char first_char = base[pos];
    if (CharInfo::isAlphabet(first_char)) {
        if ((pos + 2) < (int)base.size()) {
            char second_char = base[pos + 1];
            if (second_char == ':') {
                char third_char = base[pos + 2];
                if (fs::isPathSeparator(third_char)) {
                    // "E:\xxxx"
                    pos += 3;
                }
                else if ((pos + 2) == (int)base.size()) {
                    // "E:"
                    pos += 2;
                }
                index = 3;
                index_list.push_back(3);
                new_path.push_back(first_char);
                new_path.push_back(second_char);
                new_path.push_back(fs::PATH_SEPARATOR);
            }
            else {
                // "E?"
                new_path.push_back(first_char);
                new_path.push_back(second_char);
                Global::lastError().setLastError(Error::IllegalPathOrFilename);
                return new_path;
            }
        }
        else {
            // "E"
            new_path.push_back(first_char);
            Global::lastError().setLastError(Error::IllegalPathOrFilename);
            return new_path;
        }
    }
    // "\\192.168.1.100\abc"
    else if (first_char == fs::PATH_SEPARATOR) {
        if ((pos + 2) < (int)base.size()) {
            char second_char = base[pos + 1];
            if (second_char == fs::PATH_SEPARATOR) {
                // "\\xxxx"
                new_path.push_back(fs::PATH_SEPARATOR);
                new_path.push_back(fs::PATH_SEPARATOR);
                index = 2;
                pos += 2;
                bool found = false;
                for (i = pos; i < (int)base.size(); ++i) {
                    index++;
                    if (fs::isPathSeparator(base[i])) {
                        new_path.push_back(fs::PATH_SEPARATOR);
                        index_list.push_back(index);
                        pos = i + 1;
                        found = true;
                        break;
                    }
                    else {
                        new_path.push_back(base[i]);
                    }
                }
                if (!found) {
                    index_list.push_back((int)base.size());
                    pos = (int)base.size();
                }
            }
        }
        else {
            // "\"
            new_path.push_back(fs::PATH_SEPARATOR);
            index_list.push_back(1);
            index = 1;
            pos += 1;
        }
    }
    // "\" or "/"
    else if (first_char == '/') {
        new_path.push_back(fs::PATH_SEPARATOR);
        index_list.push_back(1);
        index = 1;
        pos += 1;
    }

#endif // _WIN32

    // Scan the [base] path list.
    while (pos < (int)base.size()) {
        bool found = false;
        for (i = pos; i < (int)base.size(); ++i) {
            index++;
            if (fs::isPathSeparator(base[i])) {
                new_path.push_back(fs::PATH_SEPARATOR);
                index_list.push_back(index);
                pos = i + 1;
                found = true;
                break;
            }
            else {
                new_path.push_back(base[i]);
            }
        }
        if (!found) {
            index_list.push_back((int)base.size());
            pos = (int)base.size();
            break;
        }
    }

    // The second step: Scan the [filename] path.
    int path_depth = (int)index_list.size() - 1;

    // TrimLeft(filename)
    int file_left = -1;
    for (i = 0; i < (int)filename.size(); ++i) {
        if (filename[i] != ' ') {
            file_left = i;
            break;
        }
    }
    pos = file_left;

    // Scan the [filename] path body.
    while (pos < (int)filename.size()) {
Next_Searching:
        if (filename[pos] == '.') {
            if (filename[pos + 1] == '.') {
                if (fs::isPathSeparator(filename[pos + 2])) {
                    // "..\" or "../"
                    pos += 3;
                }
                else if ((pos + 2) >= (int)filename.size()) {
                    // ".." and it reach the end of the string.
                    pos += 2;
                }
                else {
                    // "..???"
                    Global::lastError().setLastError(Error::IllegalPathOrFilename);
                    return new_path;
                }
                if (path_depth > 0) {
                    int last = index_list[path_depth];
                    int first = index_list[path_depth - 1];
                    assert(last > 0);
                    assert(first >= 0);
                    if (first > 0) {
                        for (i = last; i > first; --i) {
                            new_path.pop_back();
                        }
                        index_list.pop_back();
                        path_depth--;
                        index = first;
                    }
                    else {
                        /* Do nothing !! */
                    }
                }
                else {
                    /* Do nothing !! */
                }
                goto Next_Searching;
            }
            else if (fs::isPathSeparator(filename[pos + 1])) {
                // ".\" or "./"
                pos += 2;
                goto Next_Searching;
            }
        }
        bool found = false;
        for (i = pos; i < (int)filename.size(); ++i) {
            index++;
            if (fs::isPathSeparator(filename[i])) {
                new_path.push_back(fs::PATH_SEPARATOR);
                index_list.push_back(index);
                path_depth++;
                pos = i + 1;
                found = true;
                break;
            }
            else {
                new_path.push_back(filename[i]);
            }
        }
        if (!found) {
            index_list.push_back((int)filename.size());
            pos = (int)base.size();
            break;
        }
    }

    return new_path;
}

static std::string completePath(const std::string & filename) {
    std::string abs_filename;
    // The base_dir is empty.
    if (!fs::isAbsolutePath(filename.c_str())) {
        // The base_dir is empty, filename is not a absolute path.
        abs_filename = fs::getAppPath(true);
        assert(Global::lastError().isSuccess());
        //abs_filename += filename;
        abs_filename = fs::appendPath(abs_filename, filename);
    }
    else {
        // The base_dir is empty, filename is a absolute path.
        abs_filename = filename;
    }
    return abs_filename;
}

static std::string completePath(const std::string & base_dir, const std::string & filename)
{
    std::string abs_filename;
    if (!StringUtils::isEmpty(base_dir)) {
        // The base_dir is not empty.
        if (!fs::isAbsolutePath(base_dir.c_str())) {
            // The base_dir is not a absolute path and is not empty.
            abs_filename = fs::getAppPath(true);
            assert(Global::lastError().isSuccess());
            assert(abs_filename.size() > 0);

            //abs_filename += base_dir;
            abs_filename = fs::appendPath(abs_filename, base_dir);
        }
        else {
            // The base_dir is a absolute path and is not empty.
            abs_filename = base_dir;
        }
        assert(base_dir.size() > 0);
        char base_last = base_dir[base_dir.size() - 1];
        char file_first = filename[0];
        bool base_has_tail = fs::isPathSeparator(base_last);
        bool file_has_head = fs::isPathSeparator(file_first);
            
        if (!base_has_tail && !file_has_head) {
            abs_filename += fs::PATH_SEPARATOR;
        }
        else if (base_has_tail && file_has_head) {
            if (abs_filename.size() > 0) {
                abs_filename[abs_filename.size() - 1] = '\0';
                abs_filename.pop_back();
            }
        }
        //abs_filename += filename;
        abs_filename = fs::appendPath(abs_filename, filename);
        return abs_filename;
    }
    else {
        return fs::completePath(filename);
    }
}

static bool isSameFile(const std::string & file1, const std::string & file2)
{
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
    if (file1.size() != file2.size())
        return false;

    for (size_t i = 0; i < file1.size(); ++i) {
        unsigned char ch1 = StringUtils::toLowerChar(file1[i]);
        unsigned char ch2 = StringUtils::toLowerChar(file2[i]);
        if (ch1 != ch2)
            return false;
    }

    return true;
#else
    return (file1 == file2);
#endif   
}

static bool isSameFileReversi(const std::string & file1, const std::string & file2)
{
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
    if (file1.size() != file2.size())
        return false;

    for (int i = (int)(file1.size()) - 1; i >= 0; --i) {
        unsigned char ch1 = StringUtils::toLowerChar(file1[i]);
        unsigned char ch2 = StringUtils::toLowerChar(file2[i]);
        if (ch1 != ch2)
            return false;
    }

    return true;
#else
    return (file1 == file2);
#endif   
}

} // namespace fs
} // namespace jlang

#endif // JLANG_FS_FILESYSTEM_H
