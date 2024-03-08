#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <list>
#include <vector>

using namespace std;
using Evec2 = Eigen::Vector2f;

class InputVisualizer {
 public:
  InputVisualizer(int _x, int _y, float _radius) {
    x = _x;
    y = _y;
    r = _radius;
    center << x, y;
    inputPos << x, y;
  }
  ~InputVisualizer() {}

  // Handle user input
  void StandBy() {
    auto mousePos = GetMousePosition();
    if (CheckCollisionPointCircle(mousePos, {inputPos.x(), inputPos.y()}, 10) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      dragInputPoint = true;
    if (dragInputPoint) {
      // Make sure input pos is inside the circle
      Evec2 mPos {mousePos.x, mousePos.y};
      float length = min(r, (mPos-center).norm());
      inputPos = length * (mPos-center).normalized() + center;
      dragInputPoint = !IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    } else
      inputPos = center;
  }
  // Do the visualization
  void Draw() {
    DrawLine(x, y, inputPos.x(), inputPos.y(), BLACK);
    DrawCircle(inputPos.x(), inputPos.y(), 5, BLACK);
    DrawCircleLines(x, y, r, BLACK);
    DrawCircle(x, y, 5, BLACK);
  }
 private:
  int x, y;
  float r;
  Evec2 inputPos, center;
  bool dragInputPoint = false;
};

int main() {
  const int windowWidth = 800, windowHeight = 600;
  InitWindow(windowWidth, windowHeight, "Trajectory");
  rlImGuiSetup(false);

  InputVisualizer iv(100, 500, 50);

  while (!WindowShouldClose()) {

    iv.StandBy();

    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(WHITE);

    iv.Draw();

    ImGui::Begin("Parameters");
    ImGui::End();
    rlImGuiEnd();
    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();
  return 0;
}