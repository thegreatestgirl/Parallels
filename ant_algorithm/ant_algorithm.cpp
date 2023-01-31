#include "ant_algorithm.h"

using s21::ant::AntAlgorithm;

typename AntAlgorithm::result_type
AntAlgorithm::SolveTravelingSalesmanProblemSequently(
    const s21::Matrix& graph, const int iterations_quantity) {
  std::function<void()> solving_method =
      std::bind(&AntAlgorithm::SolveSequently, this);
  Solve(graph, solving_method, iterations_quantity);
  return result_;
}

typename AntAlgorithm::result_type
AntAlgorithm::SolveTravelingSalesmanProblemInParallel(
    const Matrix& graph, const int iterations_quantity) {
  std::function<void()> solving_method =
      std::bind(&AntAlgorithm::SolveInParallel, this);
  Solve(graph, solving_method, iterations_quantity);
  return result_;
}

void AntAlgorithm::Solve(const Matrix& graph,
                         std::function<void()> solving_method,
                         const int iterations_quantity) {
  Timer total_time_timer;
  Timer average_time_timer;
  total_time_timer.Begin();
  for (int i = 0; i < iterations_quantity; ++i) {
    average_time_timer.Begin();
    Setup(graph);
    solving_method();
    s21::TimeRange timestamp = average_time_timer.Timestamp();
    result_.AddTimestamp(timestamp);
  }
  s21::TimeRange timestamp = total_time_timer.Timestamp();
  result_.SetTotalExecutionTime(timestamp);
  result_.SetSolution(tsm_result_);
}

void AntAlgorithm::SolveSequently() {
  int iterations_quantity = kIterationsQuantity;
  while (iterations_quantity--) {
    AntPassage();
  }
}

void AntAlgorithm::SolveInParallel() {
  int iterations_quantity = kIterationsQuantity;
  while (iterations_quantity > 0) {
    std::thread t0(&AntAlgorithm::AntPassage, this);
    std::thread t1(&AntAlgorithm::AntPassage, this);
    std::thread t2(&AntAlgorithm::AntPassage, this);
    std::thread t3(&AntAlgorithm::AntPassage, this);
    std::thread t4(&AntAlgorithm::AntPassage, this);
    std::thread t5(&AntAlgorithm::AntPassage, this);
    std::thread t6(&AntAlgorithm::AntPassage, this);
    std::thread t7(&AntAlgorithm::AntPassage, this);
    t0.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    mtx_.lock();
    AddPheromoneChange();
    mtx_.unlock();
    iterations_quantity -= kThreadsQuantity;
  }
}

void AntAlgorithm::AntPassage() {
  std::vector<int>* visited_vertices = new std::vector<int>;
  double current_dictance = 0;
  int current_vertex = 0, first_vertex = 0, next_vertex = 0;

  visited_vertices->push_back(current_vertex);
  next_vertex = GetNextVertex(current_vertex, visited_vertices);

  while (visited_vertices->size() != vertices_.GetRows()) {
    if (WasNotVisited(next_vertex, visited_vertices)) {
      visited_vertices->push_back(next_vertex);
      current_dictance += vertices_(current_vertex, next_vertex);
    }
    int tmp_next = next_vertex;
    current_vertex = next_vertex;
    next_vertex = GetNextVertex(current_vertex, visited_vertices);
    if (next_vertex == kDefaultNextVertexValue) {
      next_vertex = tmp_next;
    }
  }

  if (HasEdge(current_vertex, first_vertex)) {
    visited_vertices->push_back(first_vertex);
    current_dictance += vertices_(current_vertex, first_vertex);
  }

  SetPheromoneChanges(current_dictance, visited_vertices);
  mtx_.lock();
  if (current_dictance < tsm_result_.distance) {
    tsm_result_.distance = current_dictance;
    tsm_result_.vertices = *visited_vertices;
  }
  mtx_.unlock();
  delete visited_vertices;
}

void AntAlgorithm::Setup(const Matrix& graph) {
  vertices_ = graph;
  if (IsolatedVerticesExists()) {
    vertices_.Clear();
    throw s21::DisconnectedGraph();
  }
  pheromones_.Resize(vertices_.GetRows(), vertices_.GetColumns());
  change_in_pheromones_.Resize(vertices_.GetRows(), vertices_.GetColumns());
  for (s21::Matrix::size_type i = 0; i < pheromones_.GetRows(); ++i) {
    for (s21::Matrix::size_type j = 0; j < pheromones_.GetColumns(); ++j) {
      pheromones_(i, j) = kDefaultPheromoneValue;
      change_in_pheromones_(i, j) = kDefaultPheromoneChangeValue;
    }
  }
}

double AntAlgorithm::CalculateTay(const double current_distance) {
  double Q = 0, tay = 0;
  for (size_type i = 0; i < vertices_.GetRows(); i++) {
    for (size_type j = 0; j < vertices_.GetColumns(); j++) {
      Q += vertices_(i, j);
    }
  }
  Q /= vertices_.GetRows() * vertices_.GetColumns();
  tay = Q / current_distance;
  return tay;
}

double AntAlgorithm::GetRandomNumber(const int min, const int max) const {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
  return static_cast<double>(dist6(rng));
}

double AntAlgorithm::GetRandomNumberFromZeroToOne() const {
  return GetRandomNumber(0, 100) / 100.0;
}

void AntAlgorithm::SetPheromoneChanges(const double current_dictance,
                                       const std::vector<int>* visited) {
  double tay = CalculateTay(current_dictance);
  double p = 0;
  Matrix pheromones_tmp(pheromones_);
  for (size_type i = 0; i < pheromones_tmp.GetRows(); i++) {
    for (size_type j = 0; j < pheromones_tmp.GetColumns(); j++) {
      pheromones_tmp(i, j) = (1 - p) * pheromones_tmp(i, j);
    }
  }

  std::vector<int>::const_iterator next = visited->cbegin() + 1,
                                   cur = visited->cbegin();
  while (next != visited->cend()) {
    pheromones_tmp(*cur, *next) = (1 - p) * pheromones_(*cur, *next) + tay;
    pheromones_tmp(*next, *cur) = (1 - p) * pheromones_(*next, *cur) + tay;
    cur++;
    next++;
  }

  for (size_type i = 0; i < pheromones_tmp.GetRows(); i++) {
    for (size_type j = 0; j < pheromones_tmp.GetColumns(); j++) {
      change_in_pheromones_(i, j) += pheromones_tmp(i, j) - pheromones_(i, j);
    }
  }
}

void AntAlgorithm::AddPheromoneChange() {
  for (int i = 0; i < change_in_pheromones_.GetRows(); ++i) {
    for (int j = 0; j < change_in_pheromones_.GetColumns(); ++j) {
      pheromones_(i, j) += change_in_pheromones_(i, j);
      change_in_pheromones_(i, j) = kDefaultPheromoneChangeValue;
    }
  }
}

double AntAlgorithm::CalculateAllPossibleWaysCost(
    const int current_vertex, const std::vector<int>* visited) {
  double cost = 0;
  for (int i = 0; i < vertices_.GetRows(); ++i) {
    if (vertices_(current_vertex, i) != kDefaultCost &&
        WasNotVisited(i, visited)) {
      cost +=
          (1 / vertices_(current_vertex, i)) * pheromones_(current_vertex, i);
    }
  }
  return cost;
}

int AntAlgorithm::GetNextVertex(const int current_vertex,
                                const std::vector<int>* visited) {
  double denominator = CalculateAllPossibleWaysCost(current_vertex, visited);
  double random_number = GetRandomNumberFromZeroToOne(), p_ij = 0,
         p_ij_prev = 0;
  int next_vertex = 0;
  for (int i = 0; i < vertices_.GetRows(); ++i) {
    if (vertices_(current_vertex, i) != kDefaultCost &&
        WasNotVisited(i, visited)) {
      p_ij_prev = p_ij;
      p_ij += ((1 / vertices_(current_vertex, i)) *
               pheromones_(current_vertex, i)) /
              denominator;
      if (random_number > p_ij_prev && random_number <= p_ij) {
        next_vertex = i;
      }
    }
  }
  return next_vertex;
}

bool AntAlgorithm::HasEdge(const int lhs, const int rhs) const {
  return vertices_(lhs, rhs) != kDefaultCost;
}

bool AntAlgorithm::IsolatedVerticesExists() const {
  for (Matrix::size_type vertex = 0; vertex < vertices_.GetRows(); ++vertex) {
    if (IsIsolated(vertex)) {
      return true;
    }
  }
  return false;
}

bool AntAlgorithm::IsIsolated(const int vertex_number) const {
  bool is_isolated = true;
  for (Matrix::size_type vertex = 0; vertex < vertices_.GetRows(); ++vertex) {
    if (HasEdge(vertex_number, vertex) &&
        static_cast<Matrix::size_type>(vertex_number) != vertex) {
      is_isolated = false;
    }
  }
  for (Matrix::size_type vertex = 0; vertex < vertices_.GetColumns();
       ++vertex) {
    if (HasEdge(vertex, vertex_number) &&
        static_cast<Matrix::size_type>(vertex_number) != vertex) {
      is_isolated = false;
    }
  }
  return is_isolated;
}

bool AntAlgorithm::WasNotVisited(const int vertex_number,
                                 const std::vector<int>* visited) const {
  return std::find(visited->cbegin(), visited->cend(), vertex_number) ==
         visited->cend();
}
