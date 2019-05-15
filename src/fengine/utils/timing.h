#ifndef FRAMES_TIMING_H
#define FRAMES_TIMING_H

#include <chrono>

#if defined _WIN32 || defined __CYGWIN__ || ((defined __i386 || defined _M_IX86 || defined __x86_64__ || defined _M_X64) && !defined __ANDROID__) || __ARM_ARCH >= 6
#define HAVE_RDTSCP
#endif

namespace frames {

namespace timing {
    // Conversion utilities
    typedef std::chrono::duration<double> dsec;
    typedef std::chrono::duration<double, std::milli> dmilli;
    typedef std::chrono::duration<double, std::micro> dmicro;

    /**
     * @brief High resolution clock
     * @note This has a better resolution than
     * std::chrono::high_resolution::clock on platforms with rdtscp support or
     * on Windows.
     */
    struct Clock {
        typedef long long rep;
        typedef std::nano period;
        typedef std::chrono::duration<rep, period> duration;
        typedef std::chrono::time_point<Clock> time_point;
        static const bool is_steady = true;
#ifdef HAVE_RDTSCP
        static double multiplier;
#endif

        static inline time_point now();

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

} // namespace timing

} // namespace Frames

#endif // FRAMES_TIMING_H
