#ifndef RAYTRACING_RAY_HPP
#define RAYTRACING_RAY_HPP

#include <Eigen/Core>

struct ray {
  Eigen::Vector3d o;
  Eigen::Vector3d d;

  ray() {}
  ray(const Eigen::Vector3d _o, const Eigen::Vector3d _d) : o(_o) {
    // Ensure d is a normalized vector
    d = _d.normalized();
  }
  ~ray() {}
  Eigen::Vector3d at(double t) { return o + t * d; }
};

#endif