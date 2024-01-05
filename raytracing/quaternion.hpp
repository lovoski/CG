#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "global.hpp"

class quaternion {
 public:
  double x, y, z, w, norm;
  quaternion() {}
  quaternion(double _w, double _x, double _y, double _z)
      : x(_x), y(_y), z(_z), w(_w) {
    norm = sqrt(x * x + y * y + z * z + w * w);
  }
  // construct form a rotation axis and an angle
  quaternion(double angle, Eigen::Vector3d axis) {
    axis = axis.normalized();
    w = std::cos(angle / 2);
    x = axis.x() * sin(angle / 2);
    y = axis.y() * sin(angle / 2);
    z = axis.z() * sin(angle / 2);
    norm = sqrt(x * x + y * y + z * z + w * w);
  }
  quaternion(quaternion &&quat) {
    x = std::move(quat.x);
    y = std::move(quat.y);
    z = std::move(quat.z);
    w = std::move(quat.w);
    norm = std::move(quat.norm);
  }
  quaternion(quaternion &quat) {
    x = quat.x;
    y = quat.y;
    z = quat.z;
    w = quat.w;
    norm = quat.norm;
  }
  quaternion &operator=(quaternion &&quat) {
    x = std::move(quat.x);
    y = std::move(quat.y);
    z = std::move(quat.z);
    w = std::move(quat.w);
    norm = std::move(quat.norm);
    return *this;
  }
  quaternion &operator=(quaternion &quat) {
    x = quat.x;
    y = quat.y;
    z = quat.z;
    w = quat.w;
    norm = quat.norm;
    return *this;
  }

  void normalize() {
    if (norm == 0) return;
    x /= norm;
    y /= norm;
    z /= norm;
    w /= norm;
    norm = 1.0;
  }
  quaternion normalized() {
    if (norm == 0) return quaternion(0.0, 0.0, 0.0, 0.0);
    return quaternion(w / norm, x / norm, y / norm, z / norm);
  }
  quaternion conjugate() { return quaternion(w, -x, -y, -z); }
  quaternion inverse() {
    if (norm == 0) return quaternion(0.0, 0.0, 0.0, 0.0);
    return quaternion(w / norm, -x / norm, -y / norm, -z / norm);
  }

  void display() { printf("w:%lf,x:%lf,y:%lf,z:%lf", w, x, y, z); }

  Eigen::Matrix3d to_mat3() {
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
  Eigen::Matrix4d to_mat4() {
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
quaternion operator*(quaternion p, quaternion q) {
  Eigen::Vector3d _v(p.x, p.y, p.z);
  Eigen::Vector3d _u(q.x, q.y, q.z);
  double s = p.w;
  double t = q.w;
  auto vec = s * _u + t * _v + _v.cross(_u);
  return quaternion(s * t - _v.dot(_u), vec.x(), vec.y(), vec.z());
}
quaternion operator+(quaternion p, quaternion q) {
  return quaternion(p.w + q.w, p.x + q.x, p.y + q.y, p.z + q.z);
}
quaternion operator-(quaternion p, quaternion q) {
  return quaternion(p.w - q.w, p.x - q.x, p.y - q.y, p.z - q.z);
}
quaternion operator-(quaternion q) {
  return quaternion(-q.w, -q.x, -q.y, -q.z);
}
Eigen::Vector3d operator*(quaternion &q, Eigen::Vector3d &vec) {
  quaternion v(0, vec.x(), vec.y(), vec.z());
  q.normalize();
  auto tmp = q * v * q.inverse();
  return Eigen::Vector3d(tmp.x, tmp.y, tmp.z);
}
quaternion operator*(double k, quaternion &q) {
  return quaternion(k * q.w, k * q.x, k * q.y, k * q.z);
}
quaternion lerp(quaternion q0, quaternion q1, const double t) {
  q0.normalize();
  q1.normalize();
  return (1.0f - t) * q0 + t * q1;
}
quaternion slerp(quaternion q0, quaternion q1, const double t) {
  q0.normalize();
  q1.normalize();
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
quaternion from_euler_angles(double phi_x, double phi_y, double phi_z) {
  quaternion qx(phi_x, Eigen::Vector3d(1, 0, 0));
  quaternion qy(phi_y, Eigen::Vector3d(0, 1, 0));
  quaternion qz(phi_z, Eigen::Vector3d(0, 0, 1));
  return qz * qy * qx;
}
quaternion from_euler_angles(Eigen::Vector3d euler_angles) {
  quaternion qx(euler_angles.x(), Eigen::Vector3d(1, 0, 0));
  quaternion qy(euler_angles.y(), Eigen::Vector3d(0, 1, 0));
  quaternion qz(euler_angles.z(), Eigen::Vector3d(0, 0, 1));
  return qz * qy * qx;
}

#endif