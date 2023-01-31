#include "time_range.h"

using s21::TimeRange;

// Constructors
TimeRange::TimeRange(const std::chrono::microseconds& duration) {
  duration_ = duration;
  seconds_ = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
  milliseconds_ =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  microseconds_ =
      std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

// Setters
void TimeRange::SetSeconds(const int64_t seconds) { seconds_ = seconds; }

void TimeRange::SetMilliseconds(const int64_t milliseconds) {
  milliseconds_ = milliseconds;
}

void TimeRange::SetMicroseconds(const int64_t microseconds) {
  microseconds_ = microseconds;
}

// Getters
int64_t TimeRange::GetSeconds() const { return seconds_; }

int64_t TimeRange::GetMilliseconds() const { return milliseconds_; }

int64_t TimeRange::GetMicroseconds() const { return microseconds_; }
