#include "timer.h"

using s21::Timer;

void Timer::Begin() { begin_time_ = std::chrono::high_resolution_clock::now(); }

s21::TimeRange Timer::Timestamp() {
  std::chrono::_V2::system_clock::time_point current_time =
      std::chrono::high_resolution_clock::now();
  std::chrono::microseconds duration =
      std::chrono::duration_cast<std::chrono::microseconds>(current_time -
                                                            begin_time_);
  return TimeRange(duration);
}
