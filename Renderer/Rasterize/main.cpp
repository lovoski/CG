// Software rasterizer
#include <imgui.h>
#include <omp.h>
#include <raylib.h>
#include <rlImGui.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>


using namespace std;
using namespace Eigen;

const int windowWidth = 1000;
const int windowHeight = 800;

void Vertex() {}

void Fragment() {}

class Renderer {
 public:
  Renderer(const int width, const int height) : _width(width), _height(height) {
    pixelNumbers = width * height;
    depth.resize(pixelNumbers);
    buf = GenImageColor(width, height, BLACK);
  }
  ~Renderer() { UnloadImage(buf); }
  Image &Render() {
    int x, y;
#pragma parallel for
    for (int i = 0; i < pixelNumbers; ++i) {
      y = i % _width;
      x = i - y * _width;
    }
  }

 private:
  int pixelNumbers;
  int _width, _height;
  Image buf;
  vector<double> depth;
};

int main() {
  InitWindow(windowWidth, windowHeight, "Rasterize Renderer");
  SetTargetFPS(60);
  rlImGuiSetup(false);

  while (!WindowShouldClose()) {
    BeginDrawing();
    rlImGuiBegin();

    ClearBackground(BLACK);

    rlImGuiEnd();
    EndDrawing();
  }

  rlImGuiShutdown();
  CloseWindow();

  return 0;
}