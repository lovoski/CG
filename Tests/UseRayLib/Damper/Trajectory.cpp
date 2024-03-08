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

float halflife_to_damping(float halflife, float eps = 1e-5f) {
  return (4.0f * 0.69314718056f) / (halflife + eps);
}

float fast_negexp(float x) {
  return 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
}

// Critical exact spring damper
void spring_character_update(float& x, float& v, float& a, float v_goal,
                             float halflife, float dt) {
  float y = halflife_to_damping(halflife) / 2.0f;
  float j0 = v - v_goal;
  float j1 = a + j0 * y;
  float eydt = fast_negexp(y * dt);

  x = eydt * (((-j1) / (y * y)) + ((-j0 - j1 * dt) / y)) + (j1 / (y * y)) +
      j0 / y + v_goal * dt + x;
  v = eydt * (j0 + j1 * dt) + v_goal;
  a = eydt * (a - j1 * y * dt);
}

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
    if (CheckCollisionPointCircle(mousePos, {inputPos.x(), inputPos.y()}, 10) &&
        IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      dragInputPoint = true;
    if (dragInputPoint) {
      // Make sure input pos is inside the circle
      Evec2 mPos{mousePos.x, mousePos.y};
      float length = min(r, (mPos - center).norm());
      inputPos = length * (mPos - center).normalized() + center;
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

  Evec2 GetInputVector() { return inputPos - center; }

 private:
  int x, y;
  float r;
  Evec2 inputPos, center;
  bool dragInputPoint = false;
};

int main() {
  const int windowWidth = 800, windowHeight = 600;
  InitWindow(windowWidth, windowHeight, "Trajectory");
  SetTargetFPS(60);
  rlImGuiSetup(false);

  InputVisualizer iv(100, 500, 50);

  float dt = 1.0 / 60, halfLife = 0.15, velTerm = 4;
  Evec2 playerPos{400, 300}, playerVel{0, 0}, playerAcc{0, 0};

  vector<Evec2> predPos(3, Evec2::Zero()), predVel(3, Evec2::Zero()), predAcc(3, Evec2::Zero());

  while (!WindowShouldClose()) {
    SetTargetFPS(1 / dt);
    iv.StandBy();

    auto inputVec = iv.GetInputVector();
    inputVec *= velTerm;
    // printf("Input vector : x=%f, y=%f\n", inputVec.x(), inputVec.y());

    for (int i = 0; i < 3; ++i) {
      predPos[i] = playerPos;
      predVel[i] = playerVel;
      predAcc[i] = playerAcc;
    }
    spring_character_update(playerPos.x(), playerVel.x(), playerAcc.x(),
                            inputVec.x(), halfLife, dt);
    spring_character_update(playerPos.y(), playerVel.y(), playerAcc.y(),
                            inputVec.y(), halfLife, dt);
    for (int i = 0; i < 3; ++i) {
      spring_character_update(predPos[i].x(), predVel[i].x(), predAcc[i].x(),
                              inputVec.x(), halfLife, dt * (i + 1) * 20);
      spring_character_update(predPos[i].y(), predVel[i].y(), predAcc[i].y(),
                              inputVec.y(), halfLife, dt * (i + 1) * 20);
    }

    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(WHITE);

    iv.Draw();

    // DrawLine(playerPos.x(), playerPos.y(), playerPos.x() + playerVel.x(),
    //          playerPos.y() + playerVel.y(), RED);
    DrawLine(playerPos.x(), playerPos.y(), predPos[0].x(), predPos[0].y(), BLUE);
    for (int i = 1; i < 3; ++i)
      DrawLine(predPos[i-1].x(), predPos[i-1].y(), predPos[i].x(), predPos[i].y(), BLUE);
    for (int i = 0; i < 3; ++i)
      DrawCircle(predPos[i].x(), predPos[i].y(), 3, RED);
    DrawCircle(playerPos.x(), playerPos.y(), 6, RED);

    ImGui::Begin("Parameters");
    ImGui::SliderFloat("dt", &dt, 0, 0.1, "%.2f");
    ImGui::SliderFloat("halflife", &halfLife, 0, 1, "%.2f");
    ImGui::SliderFloat("velocity term", &velTerm, 1, 10, "%.2f");
    ImGui::End();
    rlImGuiEnd();
    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();
  return 0;
}