#ifndef RAYTRACING_CAMERA_HPP
#define RAYTRACING_CAMERA_HPP

#include "global.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"

class camera {
 private:
  vec3 m_pos;
  vec3 m_at;
  vec3 m_vup;

  vec3 viewport_u;
  vec3 viewport_v;

 public:
  double vfov;  // fov of y-axis
  unsigned int width;
  unsigned int height;
  int max_depth = 10;
  int samples_per_pixel = 10;
  double r_tmin = 0.001;
  double r_tmax = infinity;

  camera(unsigned int _w, unsigned int _h, double _vfov)
      : width(_w), height(_h), vfov(_vfov) {
    lookat(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0));
  }
  ~camera() {}

  vec3 ray_trace(ray &r, hittable_list &world, int depth) {
    if (depth <= 0) return vec3(0, 0, 0);
    hit_record rec;
    if (world.hit(r, interval(r_tmin, r_tmax), rec)) {
      ray scattered;
      vec3 attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered))
        return cartesian(attenuation,
                         ray_trace(scattered, world, depth - 1).transpose());
      else
        return vec3(0, 0, 0);
    }
    double a = 0.7 * (r.d.y() + 1.0);
    return (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
  }

  void lookat(vec3 pos, vec3 at, vec3 up) {
    m_pos = pos;
    m_at = at;
    m_vup = up.normalized();
    double viewport_h = 2 * (at - pos).norm() * std::tan(vfov / 2.0);
    double viewport_w = viewport_h * ((double)width / (double)height);
    viewport_u = (at - pos).cross(up).normalized() * viewport_w / width;
    viewport_v = (-up).normalized() * viewport_h / height;
  }

  Buffer render(hittable_list &world) {
    Buffer buffer(width, height);
    #ifdef OPENMP_FOUND
    #pragma omp parallel for
    #endif
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        vec3 pixel_color(0.0, 0.0, 0.0);
        for (int k = 0; k < samples_per_pixel; ++k) {
          vec3 pixel_pos(0, 0, 0);
          pixel_pos += m_at;
          pixel_pos += viewport_u * (x - width / 2.0 + rand_double());
          pixel_pos += viewport_v * (y - height / 2.0 + rand_double());
          ray r(m_pos, (pixel_pos - m_pos).normalized());
          pixel_color += ray_trace(r, world, max_depth);
        }
        // averaging the results
        pixel_color /= samples_per_pixel;
        buffer.setPixel(x, y, (uint8_t)(pixel_color.x() * 255),
                        (uint8_t)(pixel_color.y() * 255),
                        (uint8_t)(pixel_color.z() * 255));
      }
    }
    return buffer;
  }
};

#endif