#ifndef TIME_RANGE_TIME_RANGE_H_
#define TIME_RANGE_TIME_RANGE_H_

#include <chrono>
#include <iostream>

namespace s21 {

class TimeRange {
 public:
  // Constructors
  TimeRange() = default;
  TimeRange(const std::chrono::microseconds& duration);

  // Setters
  void SetSeconds(const int64_t seconds);
  void SetMilliseconds(const int64_t milliseconds);
  void SetMicroseconds(const int64_t microseconds);

  // Getters
  int64_t GetSeconds() const;
  int64_t GetMilliseconds() const;
  int64_t GetMicroseconds() const;

 private:
  std::chrono::microseconds duration_;
  int64_t seconds_;
  int64_t milliseconds_;
  int64_t microseconds_;
};

}  // namespace s21

#endif  // TIME_RANGE_TIME_RANGE_H_
