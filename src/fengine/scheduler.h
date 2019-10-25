#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <functional>

#include "utils/timing.h"

namespace frames {

/**
 * @brief The system scheduler manages update timing of functions
 */
class Scheduler {
 public:
  void init();
  void cleanup();

  /**
   * @brief Adds a task to the scheduler
   * @param func Task to add
   */
  void add(std::function<void(timing::Clock::duration)> task,
           unsigned int updateRate = 0);

  void setRate(uint32_t index, unsigned int updateRate);

  void update();

 private:
  std::vector<std::function<void(timing::Clock::duration)>> m_tasks;
  std::vector<timing::Ticker> m_tickers;
};

} // namespace frames

#endif // SCHEDULER_H
