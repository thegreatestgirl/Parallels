#ifndef SRC_S21_AUNT_ALGORITHM_H_
#define SRC_S21_AUNT_ALGORITHM_H_

#include <algorithm>
#include <chrono>
#include <ctime>
#include <functional>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "../algorithm_result/algorithm_result.h"
#include "../matrix.h"
#include "../timer/timer.h"

namespace s21 {

namespace ant {

constexpr int kIterationsQuantity = 24000;
constexpr int kDefaultPheromoneValue = 1;
constexpr int kDefaultPheromoneChangeValue = 0;
constexpr int kDefaultNextVertexValue = 0;
constexpr int kDefaultCost = 0;
constexpr int kThreadsQuantity = 8;

struct TsmResult {
  std::vector<int> vertices;
  double distance{INFINITY};
};

class AntAlgorithm {
 public:
  using size_type = Matrix::size_type;
  using solution_type = TsmResult;
  using result_type = AlgorithmResult<solution_type>;

 public:
  result_type SolveTravelingSalesmanProblemSequently(
      const Matrix& graph, const int iterations_quantity);
  result_type SolveTravelingSalesmanProblemInParallel(
      const Matrix& graph, const int iterations_quantity);

 private:
  TsmResult tsm_result_;
  result_type result_;
  Matrix pheromones_;
  Matrix change_in_pheromones_;
  std::mutex mtx_;
  s21::Matrix vertices_;

 private:
  void Solve(const Matrix& graph, std::function<void()> solving_method,
             const int iterations_quantity);
  void SolveSequently();
  void SolveInParallel();

  void Setup(const Matrix& graph);
  void AddPheromoneChange();
  bool WasNotVisited(const int vertex_number,
                     const std::vector<int>* visited) const;
  double GetRandomNumberFromZeroToOne() const;
  double GetRandomNumber(int min, int max) const;
  double CalculateAllPossibleWaysCost(const int current_vertex,
                                      const std::vector<int>* visited_);
  int GetNextVertex(const int current_vertex, const std::vector<int>* visited);
  double CalculateTay(const double current_distance);

  void SetPheromoneChanges(const double current_distance,
                           const std::vector<int>* visited);
  void AntPassage();

  bool HasEdge(const int lhs, const int rhs) const;
  bool IsIsolated(const int vertex_number) const;
  bool IsolatedVerticesExists() const;
};

}  // namespace ant

}  // namespace s21

#endif  // SRC_S21_AUNT_ALGORITHM_H_
