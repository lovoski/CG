// MPM: material point method
#include <raylib.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>
#include <vector>

using namespace std;
using namespace Eigen;

const int windowWidth = 1000;
const int windowHeight = 800;

int main() {
  InitWindow(windowWidth, windowHeight, "MPM");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}