#ifndef MPC_H
#define MPC_H

#include <vector>
#include "Eigen-3.3/Eigen/Core"

using namespace std;

class MPC {
 public:
  MPC();

  virtual ~MPC();

  // Solve the model given an initial state and polynomial coefficients.
  // Return the first actuatotions.
  const double Lf = 2.67;
  vector<double> Solve(Eigen::VectorXd state, Eigen::VectorXd coeffs, vector<double> &mpc_x_vals, vector<double> &mpc_y_vals);
};

#endif /* MPC_H */
