#ifndef RAYTRACING_HITTABLE_HPP
#define RAYTRACING_HITTABLE_HPP

#include "global.hpp"

class material;

struct hit_record {
  double t;
  vec3 p;
  vec3 normal;
  bool front_face;
  shared_ptr<material> mat;
  void set_face_normal(ray &r, vec3 n) {
    front_face = r.d.dot(n) < 0.0;
    normal = front_face ? n : -n;
  }
};

class hittable {
 public:
  virtual ~hittable() = default;
  // One important reason for setting up the r_interval is to
  // avoid the reflected ray from intersection the hit plane
  // at a very little t (0.001)
  virtual bool hit(ray &r, interval r_interval, hit_record &rec) const = 0;
};

#endif