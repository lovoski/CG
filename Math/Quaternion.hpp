#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <Eigen/Core>
#include <Eigen/Dense>
#include <cmath>

namespace lvk {

class Quaternion {
 public:
  double x, y, z, w, norm;
  Quaternion() {}
  Quaternion(double _w, double _x, double _y, double _z)
      : x(_x), y(_y), z(_z), w(_w) {
    norm = sqrt(x * x + y * y + z * z + w * w);
  }
  // construct form a rotation axis and an angle
  Quaternion(double angle, Eigen::Vector3d axis) {
    axis = axis.normalized();
    w = std::cos(angle / 2);
    x = axis.x() * sin(angle / 2);
    y = axis.y() * sin(angle / 2);
    z = axis.z() * sin(angle / 2);
    norm = sqrt(x * x + y * y + z * z + w * w);
  }
  Quaternion(Quaternion &&quat) {
    x = std::move(quat.x);
    y = std::move(quat.y);
    z = std::move(quat.z);
    w = std::move(quat.w);
    norm = std::move(quat.norm);
  }
  Quaternion(Quaternion &quat) {
    x = quat.x;
    y = quat.y;
    z = quat.z;
    w = quat.w;
    norm = quat.norm;
  }
  Quaternion &operator=(Quaternion &&quat) {
    x = std::move(quat.x);
    y = std::move(quat.y);
    z = std::move(quat.z);
    w = std::move(quat.w);
    norm = std::move(quat.norm);
    return *this;
  }
  Quaternion &operator=(Quaternion &quat) {
    x = quat.x;
    y = quat.y;
    z = quat.z;
    w = quat.w;
    norm = quat.norm;
    return *this;
  }

  void Normalize() {
    if (norm == 0) return;
    x /= norm;
    y /= norm;
    z /= norm;
    w /= norm;
    norm = 1.0;
  }
  Quaternion Normalized() {
    if (norm == 0) return Quaternion(0.0, 0.0, 0.0, 0.0);
    return Quaternion(w / norm, x / norm, y / norm, z / norm);
  }
  Quaternion Conjugate() { return Quaternion(w, -x, -y, -z); }
  Quaternion inverse() {
    if (norm == 0) return Quaternion(0.0, 0.0, 0.0, 0.0);
    return Quaternion(w / norm, -x / norm, -y / norm, -z / norm);
  }

  void Display() { printf("w:%lf,x:%lf,y:%lf,z:%lf\n", w, x, y, z); }

  Eigen::Matrix3d ToMat3() {
    if (norm == 0) return Eigen::Matrix3d::Identity();
    double a = w / norm;
    double b = x / norm;
    double c = y / norm;
    double d = z / norm;
    Eigen::Matrix3d ret;
    ret << 1 - 2 * c * c - 2 * d * d, 2 * b * c - 2 * a * d,
        2 * a * c + 2 * b * d, 2 * b * c + 2 * a * d, 1 - 2 * b * b - 2 * d * d,
        2 * c * d - 2 * a * b, 2 * b * d - 2 * a * c, 2 * a * b + 2 * c * d,
        1 - 2 * b * b - 2 * c * c;
    return ret;
  }
  Eigen::Matrix4d ToMat4() {
    if (norm == 0) return Eigen::Matrix4d::Identity();
    double a = w / norm;
    double b = x / norm;
    double c = y / norm;
    double d = z / norm;
    Eigen::Matrix4d ret;
    ret << 1 - 2 * c * c - 2 * d * d, 2 * b * c - 2 * a * d,
        2 * a * c + 2 * b * d, 0.0, 2 * b * c + 2 * a * d,
        1 - 2 * b * b - 2 * d * d, 2 * c * d - 2 * a * b, 0.0,
        2 * b * d - 2 * a * c, 2 * a * b + 2 * c * d, 1 - 2 * b * b - 2 * c * c,
        0.0, 0.0, 0.0, 0.0, 1.0;
    return ret;
  }
};

// grabmann product
Quaternion operator*(Quaternion p, Quaternion q) {
  Eigen::Vector3d _v(p.x, p.y, p.z);
  Eigen::Vector3d _u(q.x, q.y, q.z);
  double s = p.w;
  double t = q.w;
  auto vec = s * _u + t * _v + _v.cross(_u);
  return Quaternion(s * t - _v.dot(_u), vec.x(), vec.y(), vec.z());
}
Quaternion operator+(Quaternion p, Quaternion q) {
  return Quaternion(p.w + q.w, p.x + q.x, p.y + q.y, p.z + q.z);
}
Quaternion operator-(Quaternion p, Quaternion q) {
  return Quaternion(p.w - q.w, p.x - q.x, p.y - q.y, p.z - q.z);
}
Quaternion operator-(Quaternion q) {
  return Quaternion(-q.w, -q.x, -q.y, -q.z);
}
Eigen::Vector3d operator*(Quaternion &q, Eigen::Vector3d &vec) {
  Quaternion v(0, vec.x(), vec.y(), vec.z());
  q.Normalize();
  auto tmp = q * v * q.inverse();
  return Eigen::Vector3d(tmp.x, tmp.y, tmp.z);
}
Quaternion operator*(double k, Quaternion &q) {
  return Quaternion(k * q.w, k * q.x, k * q.y, k * q.z);
}
Quaternion Lerp(Quaternion q0, Quaternion q1, const double t) {
  q0.Normalize();
  q1.Normalize();
  return (1.0f - t) * q0 + t * q1;
}
Quaternion Slerp(Quaternion q0, Quaternion q1, const double t) {
  q0.Normalize();
  q1.Normalize();
  Eigen::Vector3d v0(q0.x, q0.y, q0.z);
  Eigen::Vector3d v1(q1.x, q1.y, q1.z);
  double dot_val = v0.dot(v1);
  if (dot_val < 0) {
    dot_val = -dot_val;
    q0 = -q0;
  }
  double theta = std::acos(dot_val);
  return std::sin((1.0f - t) * theta) / std::sin(theta) * q0 +
         std::sin(t * theta) / std::sin(theta) * q1;
}
Quaternion FromEulerAngles(double phi_x, double phi_y, double phi_z) {
  Quaternion qx(phi_x, Eigen::Vector3d(1, 0, 0));
  Quaternion qy(phi_y, Eigen::Vector3d(0, 1, 0));
  Quaternion qz(phi_z, Eigen::Vector3d(0, 0, 1));
  return qz * qy * qx;
}
Quaternion FromEulerAngles(Eigen::Vector3d euler_angles) {
  Quaternion qx(euler_angles.x(), Eigen::Vector3d(1, 0, 0));
  Quaternion qy(euler_angles.y(), Eigen::Vector3d(0, 1, 0));
  Quaternion qz(euler_angles.z(), Eigen::Vector3d(0, 0, 1));
  return qz * qy * qx;
}

};  // namespace lvk

#endif