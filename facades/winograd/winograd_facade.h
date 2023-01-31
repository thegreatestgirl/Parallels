#ifndef FACADES_WINOGRAD_WINOGRAD_FACADES_H_
#define FACADES_WINOGRAD_WINOGRAD_FACADES_H_

#include "../../algorithm_result/algorithm_result.h"
#include "../../winograd_algorithm/winograd.h"

namespace s21 {

namespace winograd {

struct WinogradResults {
  Matrix first_src_matrix;
  Matrix second_src_matrix;
  Matrix result_matrix;
};

class Facade {
 public:
  using solution_type = Winograd::solution_type;
  using result_type = AlgorithmResult<solution_type>;

 public:
  Facade();
  Facade(const Facade&) = delete;
  Facade(Facade&&) = delete;
  Facade& operator=(const Facade&) = delete;
  Facade& operator=(Facade&&) = delete;
  ~Facade();

  AlgorithmResults<result_type> GetSolution(const Matrix& lhs,
                                            const Matrix& rhs,
                                            const int iterations_quantity,
                                            const int threads_quantity);

 private:
  Winograd* winograd_;
};

}  // namespace winograd

}  // namespace s21

#endif  // FACADES_WINOGRAD_WINOGRAD_FACADES_H_
