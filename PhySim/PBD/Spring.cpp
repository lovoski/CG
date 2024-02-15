#include "../GUI.h"

using namespace std;
using namespace Eigen;

const int windowWidth = 400;
const int windowHeight = 300;

class Spring {
 public:
  Spring(const int nodes, const int spacing, const int startX, const int y) {
    n = nodes;
    l = spacing;
    x.resize(nodes);
    v.resize(nodes+1, Vector2f::Zero());
    f.resize(nodes+1, Vector2f::Zero());
    len.resize(nodes, spacing);
    for (int i = 0; i < nodes; ++i) {
      x[i].x() += i*spacing+startX;
      x[i].y() = y;
    }
  }
  ~Spring() {}

  void Update(float dt) {
    // Move the left most node
    accuTime += dt;
    x[0].y() += A * sin(w * accuTime) * dt;

    for (int i = 0; i < x.size()-1; ++i) {
      len[i] = (x[i]-x[i+1]).norm();
      f[i] = (x[i]-x[i+1])*(1-l/len[i])*k;
    }

    for (int i = 1; i < x.size(); ++i) {
      x[i] += v[i]*dt;
      v[i] += (f[i-1]-f[i]-lambda*v[i])/m*dt;
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

  Color GetSpringColor(int ind) {
    unsigned char r = static_cast<unsigned char>(255*min(maxF, f[ind].norm())/maxF);
    return {r, 0, 100, 255};
  }

  void Draw() {
    for (int i = 0; i < n; ++i) {
      if (i != n-1)
        DrawLine(x[i].x(), x[i].y(), x[i+1].x(), x[i+1].y(), GetSpringColor(i));
      DrawRectangle(x[i].x()-5, x[i].y()-5, 10, 10, WHITE);
    }
  }
 private:
  vector<Vector2f> x;
  vector<Vector2f> v;
  vector<Vector2f> f;
  vector<float> len;

  int l, n;

  float A = 50, w = 1;
  float k = 1, m = 1, lambda = 0;

  float maxF = 100;

  float accuTime = 0;
};

int main() {
  GUI gui(windowWidth, windowHeight, "Spring");

  Spring spring(4, 75, 20, 100);

  float time = 0;

  gui.RenderLoop([&]() {
    float dt = GetFrameTime();
    time += dt;

    ClearBackground(BLACK);

    spring.Update(dt);
    spring.Draw();
  });
  return 0;
}