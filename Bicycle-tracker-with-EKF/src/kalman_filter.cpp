#include "kalman_filter.h"
#include <math.h>

using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
  x_ = F_ * x_;
  MatrixXd Ft = F_.transpose();
  P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
  VectorXd z_pred = H_ * x_;
  VectorXd y = z - z_pred;
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd PHt = P_ * Ht;
  MatrixXd K = PHt * Si;

  //new estimate
  x_ = x_ + (K * y);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */
    double px = x_[0];
  double py = x_[1];
  double vx = x_[2];
  double vy = x_[3];

  double px2 = px * px;
  double py2 = py * py;

  double rho = sqrt(px2 + py2);
  double n_rho = rho;
  if (n_rho < 0.00001) n_rho = 0.00001;
  double phi = atan2(py, px);

  VectorXd z_pred = VectorXd(3);
  z_pred << 0,0,0;
  if(px != 0 && py != 0) {
    z_pred << rho, phi, (px * vx + py * vy) / n_rho;
  }

  VectorXd y = z - z_pred; // Converted outside

  while (abs(y[1]) > 3.14) {
      if (y[1] > 0) {
          y[1] -= 2*3.14;
      }
      else {
          y[1] += 2*3.14;
      }
  }

  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd PHt = P_ * Ht;
  MatrixXd K = PHt * Si;

  //new estimate
  x_ = x_ + (K * y);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;
}
