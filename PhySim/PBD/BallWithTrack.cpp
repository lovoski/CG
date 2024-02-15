#include "../GUI.h"

using namespace std;
using namespace Eigen;

class BallWithTrack {
 public:
  BallWithTrack(Vector2f _trackCenter, float _trackRadius) {
    trackCenter = _trackCenter;
    trackRadius = _trackRadius;
    x = trackCenter + Vector2f(trackRadius, 0);
    v = Vector2f::Zero();
  }
  ~BallWithTrack() {}

  void Update(float dt, Vector2f g) {
    if (dt == 0) dt = 0.0001f;
    UpdateVelocity(dt, g);
    p = x + dt * v;
    ApplyConstraints(dt);
    v = (p-x) / dt;
    x = p;
  }
  void Draw() {
    DrawLine(x.x(), x.y(), x.x()+v.x(), x.y()+v.y(), BLUE);
    DrawCircleLines(trackCenter.x(), trackCenter.y(), trackRadius, WHITE);
    DrawCircle(trackCenter.x(), trackCenter.y(), 5, WHITE);

    DrawCircle(x.x(), x.y(), 10, RED);
  }
 private:
  float trackRadius;
  Vector2f trackCenter;

  Vector2f x, v, p;

  void UpdateVelocity(float dt, Vector2f g) {
    v += dt * g;
  }
  // Keep the ball on the closest position on track
  void ApplyConstraints(float dt) {
    auto dir = trackCenter-p;
    p = p + (1-trackRadius/dir.norm()) * dir;
  }
};

int main() {
  GUI gui(800, 600, "Ball With Track");

  BallWithTrack simulator({400, 300}, 150);

  Vector2f gravity {0, 9.8};

  bool pause = false;

  float dt;
  gui.RenderLoop([&]() {
    dt = GetFrameTime();
    ClearBackground(BLACK);
    if (IsKeyPressed(KEY_SPACE))
      pause = !pause;
    if (!pause)
      simulator.Update(dt, 10 * gravity);
    simulator.Draw();
  });
  return 0;
}