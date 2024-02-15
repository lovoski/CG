#include <raylib.h>
#include <iostream>

const unsigned int windowWidth = 800;
const unsigned int windowHeight = 600;

int main() {
  InitWindow(windowWidth, windowHeight, "Interaction Test");
  SetTargetFPS(60);

  bool moveCircle = false;

  Vector2 circlePos {400, 300};

  while (!WindowShouldClose()) {
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointCircle(mouse, circlePos, 20) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      moveCircle = true;
    if (moveCircle) {
      circlePos = mouse;
      moveCircle = !IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    }

    BeginDrawing();

    ClearBackground(BLACK);
    if (moveCircle)
      DrawCircle(circlePos.x, circlePos.y, 25, WHITE);
    else DrawCircle(circlePos.x, circlePos.y, 20, WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}