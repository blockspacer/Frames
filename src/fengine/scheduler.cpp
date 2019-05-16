#include "scheduler.h"

#include <thread>

#include <easylogging++/easylogging++.h>

#include "utils/timing.h"

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
    for (size_t i = 0; i < m_tasks.size(); ++i) {
        auto&& ticker = m_tickers.at(i);
        if (ticker.update()) {
            m_tasks[i](ticker.getDelta());
        }
    }
}

} // namespace frames
