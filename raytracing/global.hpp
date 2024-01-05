#ifndef RAYTRACING_GLOBAL_HPP
#define RAYTRACING_GLOBAL_HPP

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <vector>


// Common headers
#include <raylib.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#ifdef OPENMP_FOUND
#include <omp.h>
#endif

#include "Buffer.h"
#include "interval.hpp"
#include "ray.hpp"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using vec2 = Eigen::Vector2d;
using vec3 = Eigen::Vector3d;
using vec4 = Eigen::Vector4d;
using mat3 = Eigen::Matrix3d;
using mat4 = Eigen::Matrix4d;

const double eps = 0.00001;
const double pi = 3.1415926535;
const double infinity = std::numeric_limits<double>::infinity();

inline std::mt19937 &rand_generator() {
  static std::random_device rd;
  static std::mt19937 rng(rd());
  return rng;
}
double deg2rad(double deg) { return pi * deg / 180.0; }
double rand_double() {
  std::uniform_real_distribution<double> dist(0, 1);
  return dist(rand_generator());
}
double rand_double(double min, double max) {
  std::uniform_real_distribution<double> dist(min, max);
  return dist(rand_generator());
}
vec3 rand_vec3_on_unit_sphere() {
  vec3 p;
  static std::default_random_engine generator{std::random_device{}()};
  std::uniform_real_distribution<float> distribution(0.0, 1.0);
  do {
    p = 2.0 * vec3(distribution(generator), distribution(generator),
                   distribution(generator)) -
        vec3(1, 1, 1);
  } while (p.norm() >= 1.0);
  return p.normalized();
}
vec3 rand_vec3_on_hemisphere(vec3 &normal) {
  vec3 ret = rand_vec3_on_unit_sphere();
  if (ret.dot(normal) > 0)
    return ret;
  else
    return -ret;
}

vec3 reflect(vec3 &v, vec3 &n) { return v - 2 * v.dot(n) * n; }

// v -> view direction
// n -> normal
// r -> eta_{from} / eta_{to}
vec3 refract(vec3 &v, vec3 &n, double r) {
  double cos_theta = std::min((-v).dot(n), 1.0);
  vec3 R_perpendicular = r * (v + cos_theta * n);
  vec3 R_parallel =
      -sqrt(abs(1.0 - R_perpendicular.norm() * R_perpendicular.norm())) * n;
  return R_perpendicular + R_parallel;
}

vec3 cartesian(vec3 v, vec3 u) {
  return vec3(v.x() * u.x(), v.y() * u.y(), v.z() * u.z());
}

#endif