#include "timing.h"

#include <easylogging++/easylogging++.h>

#ifdef HAVE_RDTSCP
#include <thread>
#elif _WIN32
#include <windows.h>
#endif

namespace frames {

namespace timing {

#ifdef HAVE_RDTSCP
    double Clock::multiplier = 1.0;

    static inline uint64_t rdtscp(void)
    {
        uint32_t eax, edx;

        __asm__ __volatile__("rdtscp"
                             : "+a"(eax), "=d"(edx)
                             :
                             : "%ecx", "memory");

        return (((uint64_t)edx << 32) | eax);
    }
#elif _WIN32
    namespace {
        const long long g_Frequency = []() -> long long {
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            return frequency.QuadPart;
        }();
    }
#endif

    Clock::time_point Clock::now()
    {
#ifdef HAVE_RDTSCP
        return time_point(duration(uint64_t(multiplier * rdtscp())));
#elif _WIN32
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        return time_point(
            duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
#else
        return time_point(std::chrono::high_resolution_clock::now().time_since_epoch());
#endif
    }

    uint64_t Clock::resolution()
    {
        uint64_t resolution = std::numeric_limits<int64_t>::max();
        for (int i = 0; i < 50000 * 10; i++) {
            const auto t0i = now();
            const auto t1i = now();
            const auto dti = (t1i - t0i).count();
            if (dti > 0 && dti < resolution)
                resolution = dti;
        }
        return resolution;
    }

    void Clock::calibrate()
    {
#ifdef HAVE_RDTSCP
        LOG(DEBUG) << "Calibrating";
        const auto t0 = std::chrono::high_resolution_clock::now();
        const auto r0 = rdtscp();

        // We want at least 5% precision
        // Since high_resolution_clock precision is at maximum 15ms, we solve x
        // in (15 ms) / (x ms) < 5%
        // Which gives us x = 300 ms

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        const auto t1 = std::chrono::high_resolution_clock::now();
        const auto r1 = rdtscp();

        const auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        const auto dr = r1 - r0;

        multiplier = double(dt) / double(dr);
#endif
    }

} // namespace timing

} // namespace Frames
