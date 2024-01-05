#ifndef RAYTRACING_INTERVAL_HPP
#define RAYTRACING_INTERVAL_HPP

#include <limits>

class interval {
 public:
  double min, max;
  interval() {
    min = std::numeric_limits<double>::infinity();
    max = -min;
  }
  interval(double _min, double _max) : min(_min), max(_max) {}
  ~interval() {}

  bool in(double x) const { return x >= min && x <= max; }
  bool out(double x) const { return x < min || x > max; }
  double clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
  }
};

#endif