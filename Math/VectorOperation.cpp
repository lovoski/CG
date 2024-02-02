#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>

#include "Quaternion.hpp"

using namespace std;
using namespace Eigen;

int main() {
  Vector3d x, y, z, r;
  z << 1, 1, 1;
  z.normalize();
  r << 2, 3, 1;
  x = z.cross(r).normalized();
  y = z.cross(x).normalized();
  Matrix3d M;
  M.row(0) = x;
  M.row(1) = y;
  M.row(2) = z;
  cout << M << endl;
  cout << "\n" << M.inverse() << endl;
  cout << "\n" << M.transpose() << endl;

  lvk::Quaternion q1 = lvk::FromEulerAngles(Vector3d(180, 0, 0));
  lvk::Quaternion q2 = lvk::FromEulerAngles(Vector3d(0, 180, 180));
  q1.Display();
  q2.Display();
  return 0;
}