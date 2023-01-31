#ifndef ALGORITHM_RESULT_ALGORITHM_RESULT_H_
#define ALGORITHM_RESULT_ALGORITHM_RESULT_H_

#include <vector>

#include "../time_range/time_range.h"

namespace s21 {

template <class T>
class AlgorithmResult {
 public:
  using solution_type = T;

 public:
  // Constructors
  AlgorithmResult() = default;

  // Methods
  void AddTimestamp(const TimeRange& time_range);
  TimeRange CalculateAverageTime() const;
  void Reset();
  void PrintTotalExecutionTime() const;
  void PrintAverageTime() const;

  // Setters
  void SetSolution(const solution_type& solution);
  void SetTotalExecutionTime(const TimeRange& time_range);

  // Getters
  solution_type GetSolution() const;
  TimeRange GetTotalExecutionTime() const;

 private:
  std::vector<TimeRange> timestamps_;
  TimeRange total_execution_time_;
  solution_type solution_;
};

template <class T>
struct AlgorithmResults {
  T sequential_result;
  T parallel_result;
  T conveyor_results;
};

// Constructors

// Methods
template <typename T>
void AlgorithmResult<T>::AddTimestamp(const TimeRange& time_range) {
  timestamps_.push_back(time_range);
}

template <typename T>
TimeRange AlgorithmResult<T>::CalculateAverageTime() const {
  TimeRange time_range;
  int64_t seconds_sum = 0;
  int64_t milliseconds_sum = 0;
  int64_t microseconds_sum = 0;
  for (const TimeRange& time_range : timestamps_) {
    seconds_sum += time_range.GetSeconds();
    milliseconds_sum += time_range.GetMilliseconds();
    microseconds_sum += time_range.GetMicroseconds();
  }
  int64_t average_seconds = seconds_sum / timestamps_.size();
  int64_t average_milliseconds = milliseconds_sum / timestamps_.size();
  int64_t average_microseconds = microseconds_sum / timestamps_.size();
  time_range.SetSeconds(average_seconds);
  time_range.SetMilliseconds(average_milliseconds);
  time_range.SetMicroseconds(average_microseconds);
  return time_range;
}

template <typename T>
void AlgorithmResult<T>::Reset() {
  timestamps_.clear();
}

template <typename T>
void AlgorithmResult<T>::PrintTotalExecutionTime() const {
  std::cout << "Total execution time in microseconds: "
            << total_execution_time_.GetMicroseconds() << std::endl;
  std::cout << "Total execution time in milliseconds: "
            << total_execution_time_.GetMilliseconds() << std::endl;
  std::cout << "Total execution time in seconds     : "
            << total_execution_time_.GetSeconds() << std::endl;
}

template <typename T>
void AlgorithmResult<T>::PrintAverageTime() const {
  TimeRange average_time = CalculateAverageTime();
  std::cout << "Average time in microseconds: "
            << average_time.GetMicroseconds() << std::endl;
  std::cout << "Average time in milliseconds: "
            << average_time.GetMilliseconds() << std::endl;
  std::cout << "Average time in seconds     : " << average_time.GetSeconds()
            << std::endl;
}

// Setters
template <typename T>
void AlgorithmResult<T>::SetSolution(const solution_type& solution) {
  solution_ = solution;
}

template <typename T>
void AlgorithmResult<T>::SetTotalExecutionTime(const TimeRange& time_range) {
  total_execution_time_ = time_range;
}

// Getters
template <typename T>
typename AlgorithmResult<T>::solution_type AlgorithmResult<T>::GetSolution()
    const {
  return solution_;
}

template <typename T>
TimeRange AlgorithmResult<T>::GetTotalExecutionTime() const {
  return total_execution_time_;
}

}  // namespace s21

#endif  // ALGORITHM_RESULT_ALGORITHM_RESULT_H_
