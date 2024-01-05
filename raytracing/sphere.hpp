#ifndef RAYTRACING_SPHERE_HPP
#define RAYTRACING_SPHERE_HPP

#include "global.hpp"
#include "hittable.hpp"

class material;

class sphere : public hittable {
 private:
  vec3 o;
  double radius;
  shared_ptr<material> mat;

 public:
  sphere(const double _r, const vec3 _o, shared_ptr<material> _mat)
      : radius(_r), o(_o), mat(_mat) {}
  ~sphere() {}

  double sdf(vec3 p) { return (p - o).norm(); }
  bool hit(ray &r, interval r_interval, hit_record &rec) const {
    double a = r.d.dot(r.d);
    double half_b = r.d.dot(r.o - o);
    double c = (r.o - o).dot(r.o - o) - radius * radius;
    double discriminant = half_b * half_b - a * c;
    double sqrt_d = sqrt(discriminant);
    if (discriminant < 0.0) return false;
    // returns the first hit point
    double root = (-half_b - sqrt_d) / a;
    if (r_interval.out(root)) {
      root = (-half_b + sqrt_d) / a;
      if (r_interval.out(root)) return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    rec.mat = mat;
    rec.set_face_normal(r, (rec.p - o) / radius);
    return true;
  }
};

#endif