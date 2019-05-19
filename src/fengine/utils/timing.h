#ifndef FRAMES_TIMING_H
#define FRAMES_TIMING_H

#include <easylogging++/easylogging++.h>

#include <chrono>
#include <functional>
#include <thread>

#define SCOPED_MEASURE(cb) frames::timing::ScopedMeasure timer__COUNTER__(cb)

/*
// Using rdtscp is disabled because it doesn't measure time anyways
#if defined _WIN32 || defined __CYGWIN__ || ((defined __i386 || defined _M_IX86 || defined __x86_64__ || defined _M_X64) && !defined __ANDROID__) || __ARM_ARCH >= 6
#define HAVE_RDTSCP
#endif
*/

#ifdef HAVE_RDTSCP
#include <thread>
#elif _WIN32
#include <windows.h>
#endif

namespace frames {

namespace timing {
#ifdef _WIN32
    namespace {
        const int64_t g_Frequency = []() -> int64_t {
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            return frequency.QuadPart;
        }();
    }
#endif

    // Conversion utilities
    typedef std::chrono::duration<double> dsec;
    typedef std::chrono::duration<double, std::milli> dmilli;
    typedef std::chrono::duration<double, std::micro> dmicro;

    /**
     * @brief High resolution clock
     * @note This has the same or a better resolution than
     * std::chrono::high_resolution::clock especially on Windows.
     */
    struct Clock {
        typedef int64_t rep;
        typedef std::nano period;
        typedef std::chrono::duration<rep, period> duration;
        typedef std::chrono::time_point<Clock> time_point;
        static const bool is_steady = true;
#ifdef HAVE_RDTSCP
        static double multiplier;
#endif

        static inline time_point now()
        {
#ifdef HAVE_RDTSCP
            return time_point(duration(uint64_t(multiplier * readTSCp())));
#elif _WIN32
            LARGE_INTEGER count;
            QueryPerformanceCounter(&count);
            return time_point(
                duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
#else
            return time_point(std::chrono::high_resolution_clock::now().time_since_epoch());
#endif
        }

        /**
         * @brief Estimates the clock resolution
         * @return Resolution in ns
         */
        static uint64_t resolution();

        /**
         * @brief Calibrates the clock
         * @note Only necessary if the rdtscp instruction is available.
         * The compiler will optimize it away.
         */
        static void calibrate();
    };

    class Ticker {
    public:
        /**
         * @brief Setups a ticker with an update rate in updates/second
         * @param updateRate The update rate in updates/s
         */
        Ticker(const unsigned int updateRate = 0);

        /**
         * @brief Updates the ticker
         * @return True if it needs to be updated
         */
        bool update();

        /**
         * @brief Returns the next update time
         * @return The next update time
         */
        Clock::duration next();

        /**
         * @brief Returns the time between the last two updates
         * @return The time between the last two updates
         */
        Clock::duration getDelta();

    private:
        bool m_instant = true;

        Clock::duration m_accumulator = Clock::duration::zero();
        Clock::duration m_timestep    = Clock::duration::zero();
        Clock::duration m_frametime   = Clock::duration::zero();

        Clock::time_point m_lastUpdate;
    };

    class Event {
        typedef std::function<void()> Callback_type;

    public:
        void in(Clock::duration when, Callback_type what);

        void update();

    private:
        Clock::time_point m_timeout;
        Callback_type m_callback;
        bool m_started = false;
    };

    struct ScopedMeasure {
        typedef std::function<void(Clock::duration)> Callback_type;

        inline ScopedMeasure(const Callback_type& cb)
            : callback(cb)
        {
            start = Clock::now();
        }

        inline ~ScopedMeasure()
        {
            callback(Clock::now() - start);
        }

    private:
        Clock::time_point start;
        Callback_type callback;
    };

} // namespace timing

} // namespace Frames

#endif // FRAMES_TIMING_H
