#include "newton.hpp"
#include <cmath>
#include <iostream>

// Functor for f(x)
struct Fun {
  double operator()(double x) const {
    return std::pow(x, 3) + 5 * x + 3;
  }
};

// Functor for f'(x)
struct DFun {
  double operator()(double x) const {
    return 3 * std::pow(x, 2) + 5;
  }
};

int main(int argc, char **argv)
{
  Fun fun;
  DFun dfun;

  NewtonSolver solver(fun, dfun);
  solver.solve(0.0);

  std::cout << "x    = " << solver.get_result() << std::endl;
  std::cout << "r    = " << solver.get_residual() << std::endl;
  std::cout << "iter = " << solver.get_iter() << std::endl;

  return 0;
}
