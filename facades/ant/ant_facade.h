#ifndef FACADES_ANT_INT_FACADE_H_
#define FACADES_ANT_INT_FACADE_H_

#include "../../algorithm_result/algorithm_result.h"
#include "../../ant_algorithm/ant_algorithm.h"

namespace s21 {

namespace ant {

class Facade {
 public:
  using result_type = AntAlgorithm::result_type;

 public:
  Facade();
  Facade(const Facade&) = delete;
  Facade(Facade&&) = delete;
  Facade& operator=(const Facade&) = delete;
  Facade& operator=(Facade&&) = delete;
  ~Facade();

  AlgorithmResults<result_type> GetSolution(const s21::Matrix& matrix,
                                            const int iterations_quantity);

 private:
  AntAlgorithm* ant_algorithm_;
};

}  // namespace ant

}  // namespace s21

#endif  // FACADES_ANT_INT_FACADE_H_
