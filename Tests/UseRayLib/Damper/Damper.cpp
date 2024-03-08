#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <list>
#include <vector>

using namespace std;
using Evec2 = Eigen::Vector2f;

#define M_PI 3.1415926

inline float Lerp(float a, float b, float t) { return (1 - t) * a + t * b; }

inline float Damper(float x, float goal, float factor) {
  return Lerp(x, goal, factor);
}

inline float DamperExp(float x, float goal, float halflife, float dt) {
  return Lerp(x, goal, 1- pow(2, -dt/halflife));
}

float fast_negexp(float x)
{
    return 1.0f / (1.0f + x + 0.48f*x*x + 0.235f*x*x*x);
}

float fast_atan(float x)
{
    float z = fabs(x);
    float w = z > 1.0f ? 1.0f / z : z;
    float y = (M_PI / 4.0f)*w - w*(w - 1)*(0.2447f + 0.0663f*w);
    return copysign(z > 1.0f ? M_PI / 2.0 - y : y, x);
}

float squaref(float x)
{
    return x*x;
}

void spring_damper_exact(
    float& x, 
    float& v, 
    float x_goal, 
    float v_goal, 
    float stiffness, 
    float damping, 
    float dt, 
    float eps = 1e-5f)
{
    float g = x_goal;
    float q = v_goal;
    float s = stiffness;
    float d = damping;
    float c = g + (d*q) / (s + eps);
    float y = d / 2.0f; 
    
    if (fabs(s - (d*d) / 4.0f) < eps) // Critically Damped
    {
        float j0 = x - c;
        float j1 = v + j0*y;
        
        float eydt = fast_negexp(y*dt);
        
        x = j0*eydt + dt*j1*eydt + c;
        v = -y*j0*eydt - y*dt*j1*eydt + j1*eydt;
    }
    else if (s - (d*d) / 4.0f > 0.0) // Under Damped
    {
        float w = sqrtf(s - (d*d)/4.0f);
        float j = sqrtf(squaref(v + y*(x - c)) / (w*w + eps) + squaref(x - c));
        float p = fast_atan((v + (x - c) * y) / (-(x - c)*w + eps));
        
        j = (x - c) > 0.0f ? j : -j;
        
        float eydt = fast_negexp(y*dt);
        
        x = j*eydt*cosf(w*dt + p) + c;
        v = -y*j*eydt*cosf(w*dt + p) - w*j*eydt*sinf(w*dt + p);
    }
    else if (s - (d*d) / 4.0f < 0.0) // Over Damped
    {
        float y0 = (d + sqrtf(d*d - 4*s)) / 2.0f;
        float y1 = (d - sqrtf(d*d - 4*s)) / 2.0f;
        float j1 = (c*y0 - x*y0 - v) / (y1 - y0);
        float j0 = x - j1 - c;
        
        float ey0dt = fast_negexp(y0*dt);
        float ey1dt = fast_negexp(y1*dt);

        x = j0*ey0dt + j1*ey1dt + c;
        v = -y0*j0*ey0dt - y1*j1*ey1dt;
    }
}

int main() {
  const int windowWidth = 800, windowHeight = 600;
  InitWindow(windowWidth, windowHeight, "Damper");
  SetTargetFPS(60);
  rlImGuiSetup(false);

  float dt = 1.0/60;
  float halfLife = 0.15;
  float addTimer = 0, speed = 400;
  float stiffness = 10, damping = 2;

  Evec2 header{700, 300}, lastPos;
  Evec2 goal{700, 300};
  bool moveGoal = false;
  list<Evec2> positions;

  float vy = 0;

  while (!WindowShouldClose()) {
    SetTargetFPS(1 / dt);
    auto mousePos = GetMousePosition();
    if (CheckCollisionPointCircle(mousePos, {goal.x(), goal.y()}, 10) &&
        IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      moveGoal = true;
    }
    if (moveGoal) {
      goal.y() = mousePos.y;
      moveGoal = !IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    }
    // Update header
    // header.y() = DamperExp(header.y(), goal.y(), halfLife, dt);
    spring_damper_exact(header.y(), vy, goal.y(), 0, stiffness, damping, dt);

    // Update the rest positions
    for (auto it = positions.begin(); it != positions.end();) {
      (*it).x() -= dt * speed;
      if ((*it).x() < -speed * dt)
        it = positions.erase(it);
      else
        it++;
    }
    // Record new position
    positions.push_back(header);

    BeginDrawing();
    rlImGuiBegin();
    // Visualization
    ClearBackground(WHITE);

    if (!positions.empty()) lastPos = positions.front();
    for (auto pos : positions) {
      DrawLine(lastPos.x(), lastPos.y(), pos.x(), pos.y(), BLUE);
      DrawCircle(pos.x(), pos.y(), 5, BLUE);
      lastPos = pos;
    }
    if (!positions.empty())
      DrawLine(lastPos.x(), lastPos.y(), header.x(), header.y(), BLUE);
    DrawCircle(goal.x(), goal.y(), 5, RED);
    DrawCircle(header.x(), header.y(), 5, BLUE);

    ImGui::Begin("Parameters");
    ImGui::SliderFloat("dt", &dt, 0, 0.1, "%.2f");
    ImGui::SliderFloat("damping", &damping, 0, 20, "%.2f");
    ImGui::SliderFloat("stiffness", &stiffness, 0, 30, "%.2f");
    // ImGui::SliderFloat("Halflife", &halfLife, 0, 0.5, "%.2f");
    ImGui::End();

    rlImGuiEnd();
    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();
  return 0;
}