#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

#include "../time_range/time_range.h"

namespace s21 {

class Timer {
 public:
  // Functions
  void Begin();
  TimeRange Timestamp();

 private:
  std::chrono::_V2::system_clock::time_point begin_time_;
};

}  // namespace s21

#endif  // TIMER_TIMER_H_
