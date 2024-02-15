#include "../GUI.h"

using namespace std;
using namespace Eigen;

const int windowWidth = 800;
const int windowHeight = 600;

class Incompressible {
 public:
  Incompressible(const int nodes, const int spacing, const int startX,
                 const int y) {
    n = nodes;
    l = spacing;
    x.resize(nodes);
    v.resize(nodes + 1, Vector2f::Zero());
    f.resize(nodes + 1, Vector2f::Zero());
    len.resize(nodes, spacing);
    for (int i = 0; i < nodes; ++i) {
      x[i].x() += i * spacing + startX;
      x[i].y() = y;
    }
  }
  ~Incompressible() {}

  // Update the left most node
  void ManualUpdatePosition() {
    auto mousePos = GetMousePosition();
    if (CheckCollisionPointCircle(mousePos, {x[0].x(), x[0].y()},
                                  boundRadius) &&
        IsMouseButtonDown(MOUSE_LEFT_BUTTON))
      moveLeftMostNode = true;
    if (moveLeftMostNode) {
      x[0].x() = mousePos.x;
      x[0].y() = mousePos.y;
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) moveLeftMostNode = false;
    }
  }

  void Update(float dt) {
    ManualUpdatePosition();

    for (int i = 0; i < x.size() - 1; ++i) {
      len[i] = (x[i] - x[i + 1]).norm();
      f[i] = (x[i] - x[i + 1]) * (1 - l / len[i]) * k;
    }

    for (int i = 1; i < x.size(); ++i) {
      x[i] += v[i] * dt;
      v[i] += (f[i - 1] - f[i] - lambda * v[i]) / m * dt;
    }

    // Boundary condition
    for (int i = 0; i < x.size(); ++i) {
      if (x[i].x() < 0) {
        x[i].x() = 0;
        v[i].x() *= -1;
      }
      if (x[i].y() < 0) {
        x[i].y() = 0;
        v[i].y() *= -1;
      }
      if (x[i].x() > windowWidth) {
        x[i].x() = windowWidth;
        v[i].x() *= -1;
      }
      if (x[i].y() > windowHeight) {
        x[i].y() = windowHeight;
        v[i].y() *= -1;
      }
    }
  }

  void Draw() {
    for (int i = 1; i < n; ++i) {
      DrawLine(x[i - 1].x(), x[i - 1].y(), x[i].x(), x[i].y(), WHITE);
      DrawCircle(x[i].x(), x[i].y(), boundRadius, WHITE);
    }
    DrawCircle(x[0].x(), x[0].y(), boundRadius, RED);
  }

 private:
  vector<Vector2f> x;
  vector<Vector2f> v;
  vector<Vector2f> f;
  vector<float> len;

  int l, n;

  float A = 50, w = 1;
  float k = 1, m = 1, lambda = 0;
  float boundRadius = 10;

  bool moveLeftMostNode = false;
};

int main() {
  GUI gui(windowWidth, windowHeight, "Incompressible");
  Incompressible inc(4, 75, 20, 100);

  float time = 0;

  gui.RenderLoop([&]() {
    ClearBackground(BLACK);
    float dt = GetFrameTime();
    time += dt;
    inc.Update(dt);
    inc.Draw();
  });

  return 0;
}