#ifndef PHYSIM_GUI_H
#define PHYSIM_GUI_H

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <functional>
#include <iostream>
#include <vector>

class GUI {
 public:
  GUI(const int windowWidth, const int windowHeight, const char *title) {
    InitWindow(windowWidth, windowHeight, title);
    SetTargetFPS(60);
    rlImGuiSetup(false);
  }
  ~GUI() {
    rlImGuiShutdown();
    CloseWindow();
  }

  void RenderLoop(std::function<void(void)> &&f) {
    while (!WindowShouldClose()) {
      BeginDrawing();
      rlImGuiBegin();
      f();
      rlImGuiEnd();
      EndDrawing();
    }
  }
};

#endif
