
#ifndef JLANG_LANG_LOGGER_H
#define JLANG_LANG_LOGGER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include <stdarg.h>     // For va_start(), va_end()

#ifndef USE_DEBUG_PRINT
#ifndef NDEBUG
#define USE_DEBUG_PRINT         1
#else
#define USE_DEBUG_PRINT         0
#endif // NDEBUG
#endif // USE_DEBUG_PRINT

namespace jlang {

static const char * sLevelName[] = {
    "Verbo",
    "Debug",
    "Trace",
    "Info",
    "Warn",
    "Error",
    "Fatal",
    "Unknown",
};

static const char * sLevelPrefix[] = {
    "[Verbo]: ",
    "[Debug]: ",
    "[Trace]: ",
    "[Info]:  ",
    "[Warn]:  ",
    "[Error]: ",
    "[Fatal]: ",
    "[Unknown]:",
};

struct LogLevel {
    static const int Verbo = 0;
    static const int Debug = 1;
    static const int Trace = 2;
    static const int Info = 3;
    static const int Warn = 4;
    static const int Error = 5;
    static const int Fatal = 6;
    static const int Unknown = 7;
    static const int Max = Fatal;    
};

template <typename Writer, bool UseCrLf = false>
class Logger {
protected:
    int level_;
    Writer writer_;

private:
    void output_args_impl(int level, const char * fmt, va_list args) {
        if (level >= LogLevel::Max)
            this->print(sLevelPrefix[level]);
        else
            this->print(sLevelPrefix[LogLevel::Unknown]);
        this->print_args(fmt, args);
    }

    void printlog_ln(int level, const char * fmt, va_list args) {
        this->output_args_impl(level, fmt, args);
        this->new_line();
    }

public:
    Logger(const std::string & filename, int logLevel = LogLevel::Info)
        : writer_(filename), level_(logLevel) {}
    virtual ~Logger() {}

    int getLevel() const { return this->level_; }

    void setLevel(int logLevel) {
        this->level_ = logLevel;
    }

    Writer & getWriter() { return this->writer_; }
    const Writer & getWriter() const { return const_cast<const Writer &>(this->writer_); }

    void close() {
        writer_.close();
    }

    void flush() {
        writer_.flush();
    }

    void new_line() {
        writer_.newLine();
    }

    void print(const char * info) {
        writer_.write(info);
    }

    void print_ln(const char * info) {
        writer_.writeLine(info);
    }

    void print_args(const char * fmt, va_list args) {
        writer_.vFormatWrite(fmt, args);
    }

    void print_args_ln(const char * fmt, va_list args) {
        writer_.vFormatWriteLine(fmt, args);
    }

    void printf(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->print_args(fmt, args);
        va_end(args);
    }

    void printf_ln(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->print_args_ln(fmt, args);
        va_end(args);
    }

    void output_args(int level, const char * fmt, va_list args) {
        if (level >= this->level_) {
            this->output_args_impl(level, fmt, args);
        }
    }

    void output_args_ln(int level, const char * fmt, va_list args) {
        if (level >= this->level_) {
            this->output_args_impl(level, fmt, args);
            this->new_line();
        }
    }

    void output(int level, const char * fmt, ...) {
        if (level >= this->level_) {
            va_list args;
            va_start(args, fmt);
            this->output_args_impl(level, fmt, args);
            va_end(args);
        }
    }

    void output_ln(int level, const char * fmt, ...) {
        if (level >= this->level_) {
            va_list args;
            va_start(args, fmt);
            this->output_args_impl(level, fmt, args);
            this->new_line();
            va_end(args);
        }
    }

    void verbo_args(const char * fmt, va_list args) {
        if (this->level_ >= LogLevel::Verbo) {
            this->printlog_ln(LogLevel::Verbo, fmt, args);
        }
    }

    void verbo(const char * fmt, ...) {
        if (this->level_ >= LogLevel::Verbo) {
            va_list args;
            va_start(args, fmt);
            this->printlog_ln(LogLevel::Verbo, fmt, args);
            va_end(args);
        }
    }

    void debug_args(const char * fmt, va_list args) {
        if (this->level_ >= LogLevel::Debug) {
            this->printlog_ln(LogLevel::Debug, fmt, args);
        }
    }

    void debug(const char * fmt, ...) {
        if (this->level_ >= LogLevel::Debug) {
            va_list args;
            va_start(args, fmt);
            this->printlog_ln(LogLevel::Debug, fmt, args);
            va_end(args);
        }
    }

    void trace_args(const char * fmt, va_list args) {
        if (this->level_ >= LogLevel::Trace) {
            this->printlog_ln(LogLevel::Trace, fmt, args);
        }
    }

    void trace(const char * fmt, ...) {
        if (this->level_ >= LogLevel::Trace) {
            va_list args;
            va_start(args, fmt);
            this->printlog_ln(LogLevel::Trace, fmt, args);
            va_end(args);
        }
    }

    void info_args(const char * fmt, va_list args) {
        if (this->level_ >= LogLevel::Info) {
            this->printlog_ln(LogLevel::Info, fmt, args);
        }
    }

    void info(const char * fmt, ...) {
        if (this->level_ >= LogLevel::Info) {
            va_list args;
            va_start(args, fmt);
            this->printlog_ln(LogLevel::Info, fmt, args);
            va_end(args);
        }
    }

    void warn_args(const char * fmt, va_list args) {
        if (this->level_ >= LogLevel::Warn) {
            this->printlog_ln(LogLevel::Warn, fmt, args);
        }
    }

    void warn(const char * fmt, ...) {
        if (this->level_ >= LogLevel::Warn) {
            va_list args;
            va_start(args, fmt);
            this->printlog_ln(LogLevel::Warn, fmt, args);
            va_end(args);
        }
    }

    void error_args(const char * fmt, va_list args) {
        if (this->level_ >= LogLevel::Error) {
            this->printlog_ln(LogLevel::Error, fmt, args);
        }
    }

    void error(const char * fmt, ...) {
        if (this->level_ >= LogLevel::Error) {
            va_list args;
            va_start(args, fmt);
            this->printlog_ln(LogLevel::Error, fmt, args);
            va_end(args);
        }
    }

    void fatal_args(const char * fmt, va_list args) {
        if (this->level_ >= LogLevel::Fatal) {
            this->printlog_ln(LogLevel::Fatal, fmt, args);
        }
    }

    void fatal(const char * fmt, ...) {
        if (this->level_ >= LogLevel::Fatal) {
            va_list args;
            va_start(args, fmt);
            this->printlog_ln(LogLevel::Fatal, fmt, args);
            va_end(args);
        }
    }
};

class LogWriter {
private:
    std::string filename_;

public:
    LogWriter(const std::string & filename) : filename_(filename) {
    }

    ~LogWriter() {}

    std::string & filename() {
        return filename_;
    }

    const std::string & filename() const {
        return filename_;
    }
};

template <bool UseCrLf = false>
class ConsoleLogWriter : public LogWriter {
public:
    ConsoleLogWriter(const std::string & filename) : LogWriter(filename) {}
    ~ConsoleLogWriter() {}

    void close() {
    }

    void flush() {
    }

    bool useCrLf() const {
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
        return UseCrLf;
#else
        return false;
#endif
    }

    void writeChar(char ch) {
        ::putchar((int)ch);
    }

    void write(const char * str, size_t count) {
        ::puts(str);
    }

    void write(const std::string & str) {
        ::puts(str.c_str());
    }

    void newLine() {
        if (this->useCrLf())
            this->write("\r\n");
        else
            this->writeChar('\n');
    }

    void writeLine(const char * str, size_t count) {
        this->write(str, count);
        this->newLine();
    }

    void writeLine(const std::string & str) {
        this->write(str);
        this->newLine();
    }

    void vFormatWrite(const std::string & fmt, va_list args) {
        ::vprintf(fmt.c_str(), args);
    }

    void vFormatWriteLine(const std::string & fmt, va_list args) {
        this->vFormatWrite(fmt, args);
        this->newLine();
    }

    void formatWrite(const std::string & fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vFormatWrite(fmt, args);
        va_end(args);
    }

    void formatWriteLine(const std::string & fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vFormatWriteLine(fmt, args);
        va_end(args);
    }
};

template <bool UseCrLf = false>
class FileLogWriter : public LogWriter {
private:
    FILE * file_;

    static const size_t kMaxBufSize = 8192;

public:
    FileLogWriter(const std::string & filename) : LogWriter(filename), file_(NULL) {
        this->open(filename);
    }

    ~FileLogWriter() {
        this->close();
    }

    void open(const std::string & filename) {
        if (file_ == NULL) {
            file_ = ::fopen(filename.c_str(), "a");
        }
    }

    void close() {
        if (file_ != NULL) {
            ::fclose(file_);
            file_ = NULL;
        }
    }

    void flush() {
        if (file_ != NULL) {
            ::fflush(file_);
        }
    }

    bool useCrLf() const {
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
        return UseCrLf;
#else
        return false;
#endif
    }

    void writeChar(char ch) {
        assert(file_ != NULL);
        ::fputc(ch, file_);
    }

    void write(const char * str, size_t count) {
        assert(file_ != NULL);
        ::fputs(str, file_);
    }

    void write(const std::string & str) {
        assert(file_ != NULL);
        ::fputs(str.c_str(), file_);
    }

    void newLine() {
        if (this->useCrLf())
            this->write("\r\n");
        else
            this->writeChar('\n');
    }

    void writeLine(const char * str, size_t count) {
        this->write(str);
        this->newLine();
    }

    void writeLine(const std::string & str) {
        this->write(str);
        this->newLine();
    }

    void vFormatWrite(const std::string & fmt, va_list args) {
        char buff[kMaxBufSize];
        int actual_size = ::vsnprintf(buff, fmt.c_str(), args);
        if (actual_size >= 0) {
            buff[actual_size] = '\0';
            assert(file_ != NULL);
            ::fputs(buff, file_);
        }
    }

    void vFormatWriteLine(const std::string & fmt, va_list args) {
        this->vFormatWrite(fmt, args);
        this->newLine();
    }

    void formatWrite(const std::string & fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vFormatWrite(fmt, args);
        va_end(args);
    }

    void formatWriteLine(const std::string & fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vFormatWriteLine(fmt, args);
        va_end(args);
    }
};

template <bool UseCrLf = false>
class FileStreamLogWriter : public LogWriter {
private:
    std::fstream fileStream_;

    static const size_t kMaxBufSize = 8192;

public:
    FileStreamLogWriter(const std::string & filename)
        : LogWriter(filename), fileStream_(filename, std::ios::out | std::ios::app) {
    }

    ~FileStreamLogWriter() {
        this->close();
    }

    void open(const std::string & filename) {
    }

    void close() {
        fileStream_.close();
    }

    void flush() {
        assert(fileStream_.is_open());
        fileStream_.flush();
    }

    bool useCrLf() const {
#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_) \
 || defined(_WINDOWS) || defined(WINDOWS) || defined(__WINDOWS__)
        return UseCrLf;
#else
        return false;
#endif
    }

    void writeChar(char ch) {
        assert(fileStream_.is_open());
        fileStream_.put(ch);
    }

    void write(const char * str, size_t count) {
        assert(fileStream_.is_open());
        fileStream_.write(str, count);
    }

    void write(const std::string & str) {
        assert(fileStream_.is_open());
        fileStream_.write(str.c_str(), str.size());
    }

    void newLine() {
        if (this->useCrLf())
            this->write("\r\n");
        else
            this->writeChar('\n');
    }

    void writeLine(const char * str, size_t count) {
        this->write(str, count);
        this->newLine();
    }

    void writeLine(const std::string & str) {
        this->write(str);
        this->newLine();
    }

    void vFormatWrite(const std::string & fmt, va_list args) {
        char buff[kMaxBufSize];
        int actual_size = ::vsnprintf(buff, fmt.c_str(), args);
        if (actual_size >= 0) {
            buff[actual_size] = '\0';
            assert(fileStream_.is_open());
            fileStream_.write(buff, actual_size);
        }
    }

    void vFormatWriteLine(const std::string & fmt, va_list args) {
        this->vFormatWrite(fmt, args);
        this->newLine();
    }

    void formatWrite(const std::string & fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vFormatWrite(fmt, args);
        va_end(args);
    }

    void formatWriteLine(const std::string & fmt, ...) {
        va_list args;
        va_start(args, fmt);
        this->vFormatWriteLine(fmt, args);
        va_end(args);
    }
};

template <bool UseCrLf = false>
class ConsoleLogger : public Logger< ConsoleLogWriter<UseCrLf> > {
public:
    ConsoleLogger(int logLevel = LogLevel::Info) : Logger("Console", logLevel) {}

    virtual ~ConsoleLogger() {
    }
};

template <bool UseCrLf = false>
class FileLogger : public Logger< FileLogWriter<UseCrLf> > {
public:
    FileLogger(const std::string & filename, int logLevel = LogLevel::Info)
        : Logger(filename, logLevel) {
    }

    virtual ~FileLogger() {
    }
};

template <bool UseCrLf = false>
class FileStreamLogger : public Logger< FileStreamLogWriter<UseCrLf> > {
public:
    FileStreamLogger(const std::string & filename, int logLevel = LogLevel::Info)
        : Logger(filename, logLevel) {
    }

    virtual ~FileStreamLogger() {
    }
};

} // namespace jlang

#endif // JLANG_LANG_LOGGER_H
