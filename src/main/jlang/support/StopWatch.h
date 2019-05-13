
#ifndef JLANG_SUPPORT_STOPWATCH_H
#define JLANG_SUPPORT_STOPWATCH_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // WIN32_LEAN_AND_MEAN
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif // _WIN32

#if !defined(_MSC_VER) || (_MSC_VER >= 1800)
#include <chrono>
#endif

#ifndef __COMPILER_BARRIER
#if defined(_MSC_VER) || defined(__ICL) || defined(__INTEL_COMPILER)
#define __COMPILER_BARRIER()        _ReadWriteBarrier()
#else
#define __COMPILER_BARRIER()        asm volatile ("" : : : "memory")
#endif
#endif

#if !defined(_MSC_VER) || (_MSC_VER >= 1800)
using namespace std::chrono;
#endif

namespace jlang {

namespace detail {

template <typename T>
class duration_time {
public:
    typedef T time_float_t;

private:
    time_float_t duration_;

    // 1 second = 1,000 millisec
    static const time_float_t kMillisecCoff = static_cast<time_float_t>(1000.0);
    // 1 second = 1,000,000 microsec
    static const time_float_t kMicrosecCoff = static_cast<time_float_t>(1000000.0);
    // 1 second = 1,000,000,000 nanosec
    static const time_float_t kNanosecCoff = static_cast<time_float_t>(1000000000.0);

public:
    duration_time(time_float_t duration) : duration_(duration) {}
    duration_time(const duration_time<T> & src) : duration_(src.duration_) {}
    ~duration_time() {}

    time_float_t seconds() const {
        return this->duration_;
    }

    time_float_t millisecs() const {
        return (this->seconds() * kMillisecCoff);
    }

    time_float_t microsecs() const {
        return (this->seconds() * kMicrosecCoff);
    }

    time_float_t nanosecs() const {
        return (this->seconds() * kNanosecCoff);
    }
};

} // namespace detail

template <typename T>
class StopWatchBase {
public:
    typedef T                                   implementer;
    typedef typename implementer::time_float_t  time_float_t;
    typedef typename implementer::time_point_t  time_point_t;
    typedef typename implementer::time_stamp_t  time_stamp_t;
    typedef typename implementer::duration_type duration_type;

    typedef StopWatchBase<T> this_type;

private:
    time_point_t start_time_;
    time_point_t stop_time_;

    static time_point_t s_start_time;

    // 1 second = 1000 millisecond
    static time_float_t kMillisecCoff;      // static_cast<time_float_t>(1000.0);

public:
    StopWatchBase() :
        start_time_(implementer::now()), stop_time_(start_time_) {
    }
    StopWatchBase(StopWatchBase<T> const & src) :
        start_time_(src.start_time_), stop_time_(src.stop_time_) {
    }
    ~StopWatchBase() {}

    void reset() {
        __COMPILER_BARRIER();
        start_time_ = implementer::now();
        stop_time_ = start_time_;
        __COMPILER_BARRIER();
    }

    void start() {
        __COMPILER_BARRIER();
        start_time_ = implementer::now();
        __COMPILER_BARRIER();
    }

    void stop() {
        __COMPILER_BARRIER();
        stop_time_ = implementer::now();
        __COMPILER_BARRIER();
    }

    static time_point_t now() {
        __COMPILER_BARRIER();
        time_point_t _now = implementer::now();
        __COMPILER_BARRIER();
        return _now;
    }

    static time_stamp_t timestamp() {
        __COMPILER_BARRIER();
        time_stamp_t _timestamp = implementer::timestamp(this_type::s_start_time);
        __COMPILER_BARRIER();
        return _timestamp;
    }

    template <typename U>
    static detail::duration_time<time_float_t> duration(U now, U old) {
        __COMPILER_BARRIER();
        detail::duration_time<time_float_t> _duration(implementer::duration_cast(now, old));
        __COMPILER_BARRIER();
        return _duration;
    }

    time_float_t peekElapsedTime() const {
        __COMPILER_BARRIER();
        time_float_t elapsed_time = implementer::duration_cast(implementer::now(), start_time_);
        __COMPILER_BARRIER();
        return elapsed_time;
    }

    time_float_t peekElapsedMillisec() const {
        return (this->peekElapsedTime() * kMillisecCoff);
    }

    time_float_t getElapsedTime() {
        __COMPILER_BARRIER();
        time_float_t elapsed_time = implementer::duration_cast(stop_time_, start_time_);
        __COMPILER_BARRIER();
        return elapsed_time;
    }

    time_float_t getElapsedMillisec() {
        return (this->getElapsedTime() * kMillisecCoff);
    }
};

template <typename T>
typename StopWatchBase<T>::time_point_t
StopWatchBase<T>::s_start_time = StopWatchBase<T>::implementer::now();

// 1 second = 1000 millisec
template <typename T>
typename StopWatchBase<T>::time_float_t
StopWatchBase<T>::kMillisecCoff = static_cast<typename StopWatchBase<T>::time_float_t>(1000.0);

template <typename T>
class StopWatchExBase {
public:
    typedef T                                   implementer;
    typedef StopWatchExBase<T>                  this_type;
    typedef typename implementer::time_float_t  time_float_t;
    typedef typename implementer::time_point_t  time_point_t;
    typedef typename implementer::time_stamp_t  time_stamp_t;
    typedef typename implementer::duration_type duration_type;

private:
    time_point_t start_time_;
    time_point_t stop_time_;
    time_float_t elapsed_time_;
    time_float_t total_elapsed_time_;
    bool         running_;

    static time_point_t s_start_time;

    // The zero value time.
    static time_float_t kTimeZero;      // static_cast<time_float_t>(0.0);
    // 1 second = 1000 millisec
    static time_float_t kMillisecCoff;  // static_cast<time_float_t>(1000.0);

public:
    StopWatchExBase() : elapsed_time_(kTimeZero),
        total_elapsed_time_(kTimeZero), running_(false) {
        start_time_ = implementer::now();
    }
    StopWatchExBase(StopWatchExBase<T> const & src) :
        start_time_(src.start_time_), stop_time_(src.stop_time_),
        elapsed_time_(src.elapsed_time_), total_elapsed_time_(src.total_elapsed_time_),
        running_(src.running_) {
    }
    ~StopWatchExBase() {}

    void reset() {
        __COMPILER_BARRIER();
        elapsed_time_ = kTimeZero;
        total_elapsed_time_ = kTimeZero;
        start_time_ = implementer::now();
        running_ = false;
        __COMPILER_BARRIER();
    }

    void restart() {
        __COMPILER_BARRIER();
        running_ = false;
        elapsed_time_ = kTimeZero;
        total_elapsed_time_ = kTimeZero;
        start_time_ = implementer::now();
        running_ = true;
        __COMPILER_BARRIER();
    }

    void start() {
        if (!running_) {
            elapsed_time_ = kTimeZero;
            start_time_ = implementer::now();
            running_ = true;
        }
        __COMPILER_BARRIER();
    }

    void stop() {
        __COMPILER_BARRIER();
        if (running_) {
            stop_time_ = implementer::now();
            running_ = false;
            __COMPILER_BARRIER();
            elapsed_time_ = this->getDurationTime();
        }
    }

    void mark_start() {
        start_time_ = implementer::now();
        running_ = true;
        __COMPILER_BARRIER();
    }

    void mark_stop() {
        __COMPILER_BARRIER();
        stop_time_ = implementer::now();
        running_ = false;
    }

    void resume() {
        this->start();
    }

    void pause() {
        __COMPILER_BARRIER();
        if (running_) {
            stop_time_ = implementer::now();
            running_ = false;
            __COMPILER_BARRIER();
            elapsed_time_ = this->getDurationTime();
            total_elapsed_time_ += elapsed_time_;
        }
        __COMPILER_BARRIER();
    }

    void again() {
        __COMPILER_BARRIER();
        stop();
        __COMPILER_BARRIER();
        if (elapsed_time_ != kTimeZero) {
            total_elapsed_time_ += elapsed_time_;
            elapsed_time_ = kTimeZero;
        }
    }

    static time_point_t now() {
        __COMPILER_BARRIER();
        time_point_t _now = implementer::now();
        __COMPILER_BARRIER();
        return _now;
    }

    static time_stamp_t timestamp() {
        __COMPILER_BARRIER();
        time_stamp_t _timestamp = implementer::timestamp(this_type::s_start_time);
        __COMPILER_BARRIER();
        return _timestamp;
    }

    template <typename U>
    static detail::duration_time<time_float_t> duration(U now, U old) {
        __COMPILER_BARRIER();
        detail::duration_time<time_float_t> _duration(implementer::duration_cast(now, old));
        __COMPILER_BARRIER();
        return _duration;
    }

    time_float_t getDurationTime() const {
        __COMPILER_BARRIER();
        detail::duration_time<time_float_t> _duration_time = implementer::duration_cast(stop_time_, start_time_);
        __COMPILER_BARRIER();
        return _duration_time;
    }

    time_float_t peekElapsedTime() const {
        __COMPILER_BARRIER();
        time_float_t elapsed_time = implementer::duration_cast(implementer::now(), start_time_);
        __COMPILER_BARRIER();
        return elapsed_time;
    }

    time_float_t peekElapsedMillisec() const {
        return (this->peekElapsedTime() * kMillisecCoff);
    }

    time_float_t getElapsedTime() {
        __COMPILER_BARRIER();
        stop();
        __COMPILER_BARRIER();
        return this->elapsed_time_;
    }

    time_float_t getElapsedMillisec() {
        return (this->getElapsedTime() * kMillisecCoff);
    }

    time_float_t getTotalTime() const {
        assert(running_ == false);
        __COMPILER_BARRIER();
        return this->total_elapsed_time_;
    }

    time_float_t getTotalMillisec() const {
        __COMPILER_BARRIER();
        return (this->getTotalTime() * kMillisecCoff);
    }
};

template <typename T>
typename StopWatchExBase<T>::time_point_t
StopWatchExBase<T>::s_start_time = StopWatchExBase<T>::implementer::now();

// The zero value time.
template <typename T>
typename StopWatchExBase<T>::time_float_t
StopWatchExBase<T>::kTimeZero = static_cast<typename StopWatchExBase<T>::time_float_t>(0.0);

// 1 second = 1000 millisec
template <typename T>
typename StopWatchExBase<T>::time_float_t
StopWatchExBase<T>::kMillisecCoff = static_cast<typename StopWatchExBase<T>::time_float_t>(1000.0);

#if !defined(_MSC_VER) || (_MSC_VER >= 1800)

template <typename T>
class StdStopWatchImpl {
public:
    typedef T                                               time_float_t;
    typedef std::chrono::time_point<high_resolution_clock>  time_point_t;
    typedef double                                          time_stamp_t;
    typedef std::chrono::duration<time_float_t>             duration_type;

public:
    StdStopWatchImpl() {}
    ~StdStopWatchImpl() {}

    static time_point_t now() {
        return std::chrono::high_resolution_clock::now();
    }

    static time_float_t duration_cast(time_stamp_t now_time, time_stamp_t old_time) {
        return static_cast<time_float_t>(now_time - old_time);
    }

    static time_float_t duration_cast(time_point_t now_time, time_point_t old_time) {
        duration_type duration_time = std::chrono::duration_cast<duration_type>(now_time - old_time);
        return duration_time.count();
    }

    static time_stamp_t timestamp(time_point_t start_time) {
        return static_cast<time_stamp_t>(duration_cast(StdStopWatchImpl<T>::now(), start_time));
    }
};

typedef StopWatchBase< StdStopWatchImpl<double> >       StopWatch;
typedef StopWatchExBase< StdStopWatchImpl<double> >     StopWatchEx;

#endif // (_MSC_VER >= 1800)

#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_)

template <typename T>
class timeGetTimeImpl {
public:
    typedef T       time_float_t;
    typedef DWORD   time_point_t;
    typedef DWORD   time_stamp_t;
    typedef T       duration_type;

public:
    timeGetTimeImpl() {}
    ~timeGetTimeImpl() {}

    static time_point_t now() {
        return ::timeGetTime();
    }

    static time_float_t duration_cast(time_point_t now_time, time_point_t old_time) {
        return (static_cast<time_float_t>(now_time - old_time) / static_cast<time_float_t>(1000.0));
    }

    static time_stamp_t timestamp(time_point_t start_time) {
        return static_cast<time_stamp_t>(timeGetTimeImpl<T>::now());
    }
};

typedef StopWatchBase< timeGetTimeImpl<double> >    timeGetTimeStopWatch;
typedef StopWatchExBase< timeGetTimeImpl<double> >  timeGetTimeStopWatchEx;

#else

typedef StopWatchBase< StdStopWatchImpl<double> >   timeGetTimeStopWatch;
typedef StopWatchExBase< StdStopWatchImpl<double> > timeGetTimeStopWatchEx;

#endif // _WIN32

#if defined(_WIN32) || defined(WIN32) || defined(OS_WINDOWS) || defined(_WINDOWS_)

template <typename T>
class getTickCountImpl {
public:
    typedef T       time_float_t;
    typedef DWORD   time_point_t;
    typedef DWORD   time_stamp_t;
    typedef T       duration_type;

public:
    getTickCountImpl() {}
    ~getTickCountImpl() {}

    static time_point_t now() {
        return ::GetTickCount();
    }

    static time_float_t duration_cast(time_point_t now_time, time_point_t old_time) {
        return (static_cast<time_float_t>(now_time - old_time) / static_cast<time_float_t>(1000.0));
    }

    static time_stamp_t timestamp(time_point_t start_time) {
        return static_cast<time_stamp_t>(getTickCountImpl<T>::now());
    }
};

typedef StopWatchBase< getTickCountImpl<double> >   getTickCountStopWatch;
typedef StopWatchExBase< getTickCountImpl<double> > getTickCountStopWatchEx;

#else

typedef StopWatchBase< StdStopWatchImpl<double> >   getTickCountStopWatch;
typedef StopWatchExBase< StdStopWatchImpl<double> > getTickCountStopWatchEx;

#endif // _WIN32

#if defined(_MSC_VER) && (_MSC_VER < 1800)

typedef StopWatchBase< timeGetTimeImpl<double> >    StopWatch;
typedef StopWatchExBase< timeGetTimeImpl<double> >  StopWatchEx;

#endif

} // namespace jlang

#undef __COMPILER_BARRIER

#endif // JLANG_SUPPORT_STOPWATCH_H
