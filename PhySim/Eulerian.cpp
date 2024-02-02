// Eulerian
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>
#include <vector>

using namespace std;
using namespace Eigen;

const int windowWidth = 1000;
const int windowHeight = 800;

class Simulator2D {
 public:
  Simulator2D(const int gridW, const int gridH) {
    int totalGridCells = gridW*gridH;
    u = MatrixXd::Zero(totalGridCells, 2);
    v = MatrixXd::Zero(totalGridCells, 2);
  }
  ~Simulator2D() {}
  void Update(double dt, Vector2d gravity) {
    // Add external forces: gravity, for example
    // Projection: make the fluid incompressible
    // Advection: move the velocity field
  }
 private:
  MatrixXd u, v;
};

int main() {
  InitWindow(windowWidth, windowHeight, "Eulerian");
  SetTargetFPS(60);
  rlImGuiSetup(false);

  float val = 0.0f;

  while (!WindowShouldClose()) {
    BeginDrawing();
    rlImGuiBegin();

    ClearBackground(WHITE);
    DrawCircle(300, 300, 100, BLACK);
    ImGui::ShowDemoWindow();

    rlImGuiEnd();
    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();
  return 0;
}