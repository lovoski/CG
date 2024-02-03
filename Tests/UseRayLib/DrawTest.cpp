#include <raylib.h>

int main() {
  const int windowWidth = 800;
  const int windowHeight = 600;
  InitWindow(windowWidth, windowHeight, "Draw Test");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(WHITE);
    DrawCircle(100, 100, 10, BLUE);
    DrawCircleLines(100, 150, 10, BLUE);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}