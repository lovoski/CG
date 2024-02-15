#include "../GUI.h"

using namespace std;
using namespace Eigen;

class Pendulums {
 public:
  Pendulums(int nodes, vector<float> mass, vector<float> length, Vector2f x0) {
    n = nodes;
    x.resize(nodes);
    p.resize(nodes);
    v.resize(nodes, Vector2f::Zero());
    m = mass;
    l = length;
    x[0] = x0;
    Vector2f dir{2, -1};
    dir.normalize();
    for (int i = 1; i < n; ++i) {
      x[i] = x[i - 1] + l[i - 1] * dir;
    }
  }
  ~Pendulums() {}
  void Update(float dt, Vector2f g) {
    if (dt == 0) dt = 0.0001f;
    UpdateVelocity(dt, g);
    for (int i = 0; i < n; ++i) p[i] = x[i] + dt * v[i];
    ApplyConstraints();
    for (int i = 0; i < n; ++i) {
      v[i] = (p[i] - x[i]) / dt;
      x[i] = p[i];
    }
  }
  void Simulate(float dt, Vector2f g) {
    float _dt = dt / numSubsteps;
    for (int i = 0; i < numSubsteps; ++i)
      Update(_dt, g);
  }
  void Draw() {
    for (auto i = 0; i < n - 1; ++i)
      DrawLine(x[i].x(), x[i].y(), x[i + 1].x(), x[i + 1].y(), RED);
    for (auto i = 0; i < n; ++i) DrawCircle(x[i].x(), x[i].y(), 10, WHITE);
  }

 private:
  int n, numSubsteps = 5;
  vector<float> m, l;
  vector<Vector2f> x, v, p;
  void UpdateVelocity(float dt, Vector2f g) {
    for (int i = 0; i < n; ++i)
      if (m[i] > 0) v[i] += dt * g;
  }
  void ApplyConstraints() {
    // Make the distance between nodes remain the same
    Vector2f dir;
    float w1, w2, d;
    for (int i = 1; i < n; ++i) {
      dir = p[i - 1] - p[i];
      d = dir.norm();
      if (d == 0) d = 0.0001f;
      w1 = m[i - 1] > 0 ? 1 / m[i - 1] : 0;
      w2 = m[i] > 0 ? 1 / m[i] : 0;
      p[i - 1] -= w1 / (w1 + w2) * (1 - l[i - 1] / d) * dir;
      p[i] += w2 / (w1 + w2) * (1 - l[i - 1] / d) * dir;
    }
  }
};

int main() {
  GUI gui(800, 600, "Pendulums");

  vector<float> mass{0, 1, 1, 1};
  vector<float> length{100, 100, 100};

  Vector2f gravity{0, 9.8};
  Vector2f x0{350, 200};

  Pendulums simulator(4, mass, length, x0);

  float dt;
  bool pause = false;

  gui.RenderLoop([&]() {
    dt = GetFrameTime();
    ClearBackground(BLACK);
    if (IsKeyPressed(KEY_SPACE)) pause = !pause;
    if (!pause) simulator.Simulate(dt, 10 * gravity);
    simulator.Draw();
  });
  return 0;
}