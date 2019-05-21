#include "scheduler.h"

#include <thread>

#include <easylogging++/easylogging++.h>

#include "utils/timing.h"

#include "libs/tracy/Tracy.hpp"

namespace frames {

void Scheduler::init()
{
}

void Scheduler::cleanup()
{
}

void Scheduler::add(std::function<void(timing::Clock::duration)> task, unsigned int updateRate)
{
    m_tasks.push_back(task);
    m_tickers.emplace_back(updateRate);
}

void Scheduler::update()
{
    // Update if needed
    for (size_t i = 0; i < m_tasks.size(); ++i) {
        auto&& ticker = m_tickers.at(i);
        if (ticker.update()) {
            m_tasks[i](ticker.getDelta());
        }
    }

    // Sleep until the next update is needed
    timing::Clock::duration timeToWait = timing::Clock::duration::max();
    const auto timeNow                 = timing::Clock::now();

    for (size_t i = 0; i < m_tasks.size(); i++) {
        auto&& ticker = m_tickers.at(i);

        // Find the closest update
        timeToWait = std::min(timeToWait, ticker.next());
    }

    if (timeToWait > std::chrono::milliseconds(3))
        std::this_thread::sleep_for(timeToWait - std::chrono::milliseconds(3));
}

} // namespace frames
