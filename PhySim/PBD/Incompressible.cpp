#include "../GUI.h"

using namespace std;
using namespace Eigen;

const int windowWidth = 800;
const int windowHeight = 600;

class Incompressible {
 public:
  Incompressible(vector<Vector2f> _initPositions, vector<float> _length) {
    n = _initPositions.size();
    x.resize(n);
    p.resize(n);
    v.resize(n, Vector2f::Zero());
    l = _length;
    x[0] = _initPositions[0];
    for (int i = 1; i < n; ++i) x[i] = _initPositions[i];
    rest = x;
  }
  ~Incompressible() {}

  void Simulate(float dt, Vector2f g) {
    if (dt == 0) dt = 0.01f;
    ManualUpdatePosition();
    float _dt = simulationSpeed * dt / maxSubsteps;
    for (int i = 0; i < maxSubsteps; ++i) Update(_dt, g);
  }

  void SetParameters(float _stiffness = 100, float _damping = 0.2,
                     int _substeps = 5, int _simulationSpeed = 1) {
    stiffness = _stiffness;
    damping = _damping;
    maxSubsteps = _substeps;
    simulationSpeed = _simulationSpeed;
  }

  void Draw() {
    for (int i = 1; i < n; ++i)
      DrawLine(x[i - 1].x(), x[i - 1].y(), x[i].x(), x[i].y(), WHITE);
    for (int i = 1; i < n; ++i) DrawCircle(x[i].x(), x[i].y(), 5, BLUE);
    DrawCircle(x[0].x(), x[0].y(), 10, RED);
    for (int i = 1; i < n; ++i) DrawCircle(rest[i].x(), rest[i].y(), 5, GREEN);
  }

 private:
  vector<Vector2f> x, v, p, rest;
  vector<float> l;

  int n, maxSubsteps = 5, simulationSpeed = 1;

  float stiffness = 100, damping = 0.2;

  void Update(float dt, Vector2f g) {
    UpdateVelocity(dt, g);
    for (int i = 1; i < n; ++i) p[i] = x[i] + dt * v[i];
    ApplyConstraints();
    for (int i = 1; i < n; ++i) {
      v[i] = (p[i] - x[i]) / dt;
      x[i] = p[i];
    }
  }

  void UpdateVelocity(float dt, Vector2f g) {
    rest[1] = x[0] + l[0] * Vector2f(1, 0);
    for (int i = 2; i < n; ++i) {
      rest[i] = l[i - 1] * (x[i - 1] - x[i - 2]).normalized() + x[i - 1];
    }
    for (int i = 1; i < n; ++i) {
      v[i] += dt * (g + stiffness * (rest[i] - x[i]));
      v[i] *= (1 - damping);
    }
  }
  void ApplyConstraints() {
    // Make sure the distance between nodes are constant
    Vector2f d;
    p[0] = x[0];
    for (int i = 1; i < n; ++i) {
      d = p[i - 1] - p[i];
      p[i] += (1 - l[i - 1] / d.norm()) * d;
    }
  }

  bool moveLeftMostNode = false;
  // Update the left most node by hand
  // Move the rest positions as well
  void ManualUpdatePosition() {
    auto mousePos = GetMousePosition();
    if (CheckCollisionPointCircle(mousePos, {x[0].x(), x[0].y()}, 10) &&
        IsMouseButtonDown(MOUSE_LEFT_BUTTON))
      moveLeftMostNode = true;
    if (moveLeftMostNode) {
      x[0].x() = mousePos.x;
      x[0].y() = mousePos.y;
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) moveLeftMostNode = false;
    }
  }

  void PrintPositions() {
    for (int i = 0; i < n; ++i)
      printf("[x%d : (%f, %f)] ", i, x[i].x(), x[i].y());
    printf("\n");
  }
  void PrintPredictPositions() {
    for (int i = 1; i < n; ++i)
      printf("[p%d : (%f, %f)] ", i, p[i].x(), p[i].y());
    printf("\n");
  }
  void PrintVelocity() {
    for (int i = 1; i < n; ++i)
      printf("[v%d : (%f, %f)] ", i, v[i].x(), v[i].y());
    printf("\n");
  }
};

int main() {
  GUI gui(windowWidth, windowHeight, "Incompressible");

  vector<Vector2f> initPositions{
      {100, 300}, {200, 300}, {300, 300}, {400, 300}, {500, 300},
  };
  vector<float> length{100, 100, 100, 100};

  Incompressible simulator(initPositions, length);

  Vector2f gravity{0, 0};

  int simulationSpeed = 5, simulationSubsteps = 5;
  float stiffness = 100, damping = 0.2;

  gui.RenderLoop([&]() {
    ImGui::Begin("Parameters");
    ImGui::SliderInt("Speed", &simulationSpeed, 1, 10);
    ImGui::SliderInt("Substeps", &simulationSubsteps, 1, 10);
    ImGui::SliderFloat("Stiffness", &stiffness, 1, 200);
    ImGui::SliderFloat("Damping", &damping, 0, 1);
    ImGui::End();
    simulator.SetParameters(stiffness, damping, simulationSubsteps, simulationSpeed);
    ClearBackground(BLACK);
    float dt = GetFrameTime();
    simulator.Simulate(dt, gravity);
    simulator.Draw();
  });

  return 0;
}