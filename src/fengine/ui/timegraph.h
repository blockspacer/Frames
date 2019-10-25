#ifndef TIMEGRAPH_H
#define TIMEGRAPH_H

#include <cstdint>

#include "../utils/timing.h"
#include "graph.h"

#include <Tracy.hpp>

namespace frames {
namespace ui {
    template <int n>
    class TimeGraph : public Graph<n> {
    public:
        TimeGraph(const std::string& name)
            : Graph<n>(name)
        {
        }

        void update(const timing::Clock::duration& time)
        {
            ZoneScoped;
            const double timems =
                std::chrono::duration_cast<timing::dmilli>(time).count();
            // const double timems = timing::dmilli(time).count();

            // Append the new value to the end
            Graph<200>::push(timems);

            constexpr double alpha = 0.95;

            this->m_avg = alpha * this->m_avg + (1. - alpha) * timems;
        }
    };

} // namespace ui
} // namespace frames

#endif // TIMEGRAPH_H
