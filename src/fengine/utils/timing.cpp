#include "timing.h"

#include <easylogging++/easylogging++.h>

#include <cstdint>
#include <numeric>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#include <Tracy.hpp>

namespace frames {

namespace timing {
#ifdef HAVE_RDTSCP
    double Clock::multiplier = 1.0;

    static inline uint64_t rdtscp()
    {
        unsigned dummy;
        return __rdtscp(&dummy); // waits for earlier insns to retire, but allows later to start
    }
#endif

    uint64_t Clock::resolution()
    {
        int64_t resolution = std::numeric_limits<int64_t>::max();
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
        // Calibrate for real time
        LOG(DEBUG) << "Calibrating clock";
        const auto t0 = std::chrono::high_resolution_clock::now();
        const auto r0 = readTSCp();

        // We want at least 5% precision
        // Since high_resolution_clock precision is at maximum 15ms, we solve x
        // in (15 ms) / (x ms) < 5%
        // Which gives us x = 300 ms

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        const auto t1 = std::chrono::high_resolution_clock::now();
        const auto r1 = readTSCp();

        const auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        const auto dr = r1 - r0;

        multiplier = double(dt) / double(dr);
#endif
    }

    Ticker::Ticker(const unsigned int updateRate)
    {
        if (updateRate != 0) {
            m_instant    = false;
            m_timestep   = std::chrono::nanoseconds(1000000000 / updateRate);
            m_lastUpdate = Clock::now();
        }
    }

    bool Ticker::update()
    {
        // Always update
        if (m_instant)
            return true;

        auto timeNow = Clock::now();

        m_accumulator = timeNow - m_lastUpdate;

        // Update if enough time has passed since lastUpdate;
        if (m_accumulator >= m_timestep) {
            m_frametime   = m_accumulator;
            m_accumulator = Clock::duration::zero();
            //while (m_accumulator >= m_timestep)
            //    m_accumulator -= m_timestep;

            m_lastUpdate = timeNow;
            return true;
        }

        return false;
    }

    Clock::duration Ticker::next()
    {
        return m_timestep - m_accumulator;
    }

    Clock::duration Ticker::getDelta()
    {
        return m_frametime;
    }

    void Event::in(Clock::duration when, Event::Callback_type what)
    {
        m_callback = what;
        m_timeout  = Clock::now() + when;
        m_started  = true;
    }

    void Event::update()
    {
        if (m_started && Clock::now() >= m_timeout) {
            m_started = false;
            m_callback();
        }
    }

} // namespace timing

} // namespace Frames
