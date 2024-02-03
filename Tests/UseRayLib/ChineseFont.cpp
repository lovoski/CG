#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "ResourceConfig.h"

const int windowWidth = 1000;
const int windowHeight = 800;

int main() {
  InitWindow(windowWidth, windowHeight, "中文字体测试");
  SetTargetFPS(60);
  SetTraceLogLevel(LOG_WARNING);

  int fileSize;
  unsigned char *fontFileData =
      LoadFileData(RESOURCES_PATH "/fonts/NotoSansSC-Regular.ttf", &fileSize);

  char text[100] = "这是一段中文，带有标点符号。";

  int fontSize = 70;
  int fontSpacing = 5;

  while (!WindowShouldClose()) {
    int codePointsCount;
    int *codePoints = LoadCodepoints(text, &codePointsCount);
    Font font = LoadFontFromMemory(".ttf", fontFileData, fileSize, fontSize,
                                   codePoints, codePointsCount);
    UnloadCodepoints(codePoints);
    BeginDrawing();
    ClearBackground(WHITE);
    DrawTextEx(font, text, (Vector2){50, 300}, fontSize, fontSpacing, BLACK);
    EndDrawing();
    UnloadFont(font);
  }

  CloseWindow();
  return 0;
}