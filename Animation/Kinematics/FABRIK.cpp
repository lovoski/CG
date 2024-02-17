// FABRIK : Heuristic iterative solver
#include <raylib.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <vector>


using namespace std;
using namespace Eigen;

const int windowWidth = 800;
const int windowHeight = 600;

class FABRIK {
 public:
  FABRIK(vector<Vector2f> _initPositions, vector<float> _armLength) {
    if (_initPositions.size() < 3) {
      printf("at least three pivots are required\n");
      return;
    }
    start = _initPositions[0];
    n = _initPositions.size();
    l = _armLength;
    p = _initPositions;
  }
  ~FABRIK() {}
  void Solve(Vector2f _target) {
    target = _target;
    for (int i = 0; i < maxIter; ++i) {
      Backward();
      Forward();
      if ((target-p[n-1]).norm() < tol) break;
    }
  }
  void Draw() {
    for (int i = 0; i < n - 1; ++i)
      DrawLine(p[i].x(), p[i].y(), p[i + 1].x(), p[i + 1].y(), WHITE);
    for (int i = 1; i < n; ++i) DrawCircle(p[i].x(), p[i].y(), 5, BLUE);
    DrawCircle(p[0].x(), p[0].y(), 10, BLUE);
    DrawCircle(target.x(), target.y(), 10, RED);
  }

 private:
  float tol = 0.001f;
  int n, maxIter = 10;
  Vector2f target, start;
  vector<float> l;
  vector<Vector2f> p;

  void Forward() {
    // Align start pivot to start
    p[0] = start;
    Vector2f d;
    for (int i = 1; i <= n-1; ++i) {
      d = (p[i-1]-p[i]).normalized();
      p[i] = p[i-1]-l[i-1]*d;
    }
  }
  void Backward() {
    // Align end pivot to target
    p[n-1] = target;
    Vector2f d;
    for (int i = n-2; i >= 0; --i) {
      d = (p[i+1]-p[i]).normalized();
      p[i] = p[i+1]-l[i]*d;
    }
  }
};

int main() {
  InitWindow(windowWidth, windowHeight, "FABRIK");
  SetTargetFPS(60);

  vector<Vector2f> initPositions{
      {100, 300}, {200, 300}, {300, 300}, {400, 300}, {500, 300}, {600, 300},
  };
  vector<float> armLength{100, 100, 100, 100, 100};
  Vector2f target{600, 300};
  bool moveTarget = false;

  FABRIK solver(initPositions, armLength);

  while (!WindowShouldClose()) {
    auto mouse = GetMousePosition();
    if (CheckCollisionPointCircle(mouse, {target.x(), target.y()}, 20) &&
        IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      moveTarget = true;
    if (moveTarget) {
      target.x() = mouse.x;
      target.y() = mouse.y;
      moveTarget = !IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    }

    solver.Solve(target);

    BeginDrawing();
    ClearBackground(BLACK);
    solver.Draw();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}