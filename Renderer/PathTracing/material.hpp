#ifndef RAYTRACING_MATERIAL_HPP
#define RAYTRACING_MATERIAL_HPP

#include "global.hpp"
#include "hittable.hpp"

// Abstract behavior for a material
class material {
 public:
  virtual ~material() = default;
  virtual bool scatter(ray &r, hit_record &rec, vec3 &attennuation,
                       ray &scattered) const = 0;
};

class lambertian : public material {
 private:
  vec3 albedo;

 public:
  lambertian(vec3 color) : albedo(color) {}
  bool scatter(ray &r, hit_record &rec, vec3 &attenuation,
               ray &scattered) const override {
    vec3 reflected_dir = rec.normal + rand_vec3_on_unit_sphere();
    if (reflected_dir.norm() < eps)
      scattered = ray(rec.p, rec.normal);
    else
      scattered = ray(rec.p, reflected_dir.normalized());
    attenuation = albedo;
    return true;
  }
};

class metal : public material {
 private:
  vec3 albedo;
  double fuzziness;

 public:
  metal(vec3 color, double _fuzziness) : albedo(color), fuzziness(_fuzziness) {}
  bool scatter(ray &r, hit_record &rec, vec3 &attenuation,
               ray &scattered) const override {
    vec3 reflect_dir = reflect(r.d, rec.normal);
    scattered =
        ray(rec.p, reflect_dir + fuzziness * rand_vec3_on_unit_sphere());
    attenuation = albedo;
    return scattered.d.dot(rec.normal) > 0.0;
  }
};

class transparent : public material {
 private:
  double eta;
  static double reflectance(double cosine, double ref_idx) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }

 public:
  // Rate of refraction
  transparent(double _eta) : eta(_eta) {}
  bool scatter(ray &r, hit_record &rec, vec3 &attenuation,
               ray &scattered) const override {
    attenuation = vec3(1, 1, 1);
    double refract_ratio = rec.front_face ? 1.0 / eta : eta;
    double cos_theta = std::min((-r.d).dot(rec.normal), 1.0);
    double sin_theta = sqrt(abs(1.0 - cos_theta * cos_theta));
    vec3 scattered_dir;
    if (refract_ratio * sin_theta > 1.0 ||
        reflectance(cos_theta, refract_ratio) >
            rand_double()) {  // reflect only
      scattered_dir = reflect(r.d, rec.normal);
    } else {
      scattered_dir = refract(r.d, rec.normal, refract_ratio);
    }
    scattered = ray(rec.p, scattered_dir);
    return true;
  }
};

#endif