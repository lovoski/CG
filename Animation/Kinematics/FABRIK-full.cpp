#include <kinematics_config.h>
#include <raylib.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace Eigen;

struct Joint {
  float length = -1;
  Vector2f pos;
  Joint *parent = nullptr;
  vector<Joint *> child;
};

struct Goal {
  Vector2f pos;
  Joint *effector = nullptr;
};

class FABRIK {
 public:
  FABRIK(string inputFileName) {
    ifstream inputFile(inputFileName);
    string inputLine;
    getline(inputFile, inputLine);
    // The first line should be the number of joints
    jointNumbers = stoi(inputLine);
    // Initialize joints to destinated size
    joints = new Joint[jointNumbers];
    float xPos, yPos, length;
    int parentId, childId, childNumbers;
    for (int jointInd = 0; jointInd < jointNumbers; ++jointInd) {
      getline(inputFile, inputLine);
      stringstream ss(inputLine);
      ss >> xPos >> yPos >> parentId >> length >> childNumbers;
      joints[jointInd].length = length;
      joints[jointInd].pos.x() = xPos;
      joints[jointInd].pos.y() = yPos;
      if (-1 == parentId) {  // Root node
        root = &joints[jointInd];
        rootPosition = root->pos;
        joints[jointInd].parent = nullptr;
      } else
        joints[jointInd].parent = &joints[parentId];
      if (0 == childNumbers) leafJoints.push_back(&joints[jointInd]);
      for (int i = 0; i < childNumbers; ++i) {
        ss >> childId;
        joints[jointInd].child.push_back(&joints[childId]);
      }
    }
    inputFile.close();
  }
  ~FABRIK() {
    delete[] joints;
    joints = nullptr;
    root = nullptr;
  }
  const vector<Joint *> GetLeafJoints() { return leafJoints; }
  Vector2f GetRootPos() { return root->pos; }
  void MoveRoot(Vector2f delta) {
    rootPosition += delta;
    for (int i = 0; i < jointNumbers; ++i) joints[i].pos += delta;
  }
  void Solve(vector<Goal> goals) {
    m_goals = goals;
    for (int i = 0; i < maxIter; ++i) {
      // Forward : Align end effectors with goals
      Forward();
      // Backward : Align root to start position
      root->pos = rootPosition;
      BackWard(root);
      if (EvaluateError() < tolerance) break;
    }
  }
  void Draw() {
    RecursiveDrawLine(root);
    for (int i = 0; i < jointNumbers; ++i)
      DrawCircle(joints[i].pos.x(), joints[i].pos.y(), 5, BLUE);
    DrawCircle(root->pos.x(), root->pos.y(), 10, BLUE);
    for (auto goal : m_goals) DrawCircle(goal.pos.x(), goal.pos.y(), 10, RED);
  }

 private:
  // Store data in joints, use root for convenience
  Joint *joints, *root;
  // Store leaf joints to create goals
  vector<Joint *> leafJoints;
  vector<Goal> m_goals;
  Vector2f rootPosition;
  float tolerance = 1;
  int jointNumbers, maxIter = 10;
  // This might not be optimal, but it does the job
  map<Joint *, vector<Vector2f>> subbasePositions;

  void RecursiveDrawLine(Joint *parent) {
    if (parent->child.size() == 0) return;
    for (auto child : parent->child) {
      DrawLine(parent->pos.x(), parent->pos.y(), child->pos.x(), child->pos.y(),
               WHITE);
      RecursiveDrawLine(child);
    }
  }

  float EvaluateError() {
    float totalErr = 0;
    for (auto goal : m_goals)
      totalErr += (goal.pos - goal.effector->pos).norm();
    return totalErr;
  }

  void Forward() {
    for (auto goal : m_goals) {
      Joint *cur = goal.effector;
      cur->pos = goal.pos;
    // Loop until current parent is subbase
    _LoopStart:
      while (cur->parent != nullptr && cur->parent->child.size() <= 1) {
        Vector2f d = cur->pos - cur->parent->pos;
        cur->parent->pos = cur->pos - cur->length * d.normalized();
        cur = cur->parent;
      }
      // Current parent is a subbase, store expected parent position in a map
      if (cur->parent != nullptr && cur->parent->child.size() >= 2) {
        Vector2f d = cur->pos - cur->parent->pos;
        subbasePositions[cur->parent].push_back(cur->pos -
                                                cur->length * d.normalized());
        if (subbasePositions[cur->parent].size() == cur->parent->child.size()) {
          // Continue forwarding
          Vector2f centroid = Vector2f::Zero();
          for (auto pos : subbasePositions[cur->parent]) centroid += pos;
          centroid /= subbasePositions[cur->parent].size();
          cur->parent->pos = centroid;
          subbasePositions[cur->parent].clear();
          cur = cur->parent;
          goto _LoopStart;
        }
      }
      // If the root is also a subbase (have multiple children)
      if (cur->parent == nullptr && cur->child.size() > 1) {
        if (subbasePositions[cur].size() == cur->child.size()) {
          Vector2f centroid = Vector2f::Zero();
          for (auto pos : subbasePositions[cur]) centroid += pos;
          centroid /= subbasePositions[cur].size();
          subbasePositions[cur].clear();
          cur->pos = centroid;
        }
      }
    }
  }

  void BackWard(Joint *parent) {
    if (parent->child.size() == 0) return;
    Vector2f d;
    for (auto child : parent->child) {
      d = parent->pos - child->pos;
      child->pos = parent->pos - child->length * d.normalized();
      BackWard(child);
    }
  }

  void PrintJointsLocation(string token) {
    for (int i = 0; i < jointNumbers; ++i)
      printf("%s[j%d=(%.2f, %.2f)] ", token.c_str(), i, joints[i].pos.x(),
             joints[i].pos.y());
    printf("\n");
  }
};

const int windowWidth = 800, windowHeight = 600;

int main() {
  InitWindow(windowWidth, windowHeight, "FABRIK-full");
  SetTargetFPS(60);
  FABRIK solver(RESOURCE_PATH "/fabrik-full-input");

  auto effectors = solver.GetLeafJoints();
  vector<Goal> goals;
  vector<bool> goalMoved(effectors.size(), false);
  for (auto effector : effectors) goals.push_back({effector->pos, effector});

  Vector2f rootPos = solver.GetRootPos();
  bool rootMoved = false;

  while (!WindowShouldClose()) {
    auto mouse = GetMousePosition();
    for (int i = 0; i < goals.size(); ++i) {
      if (CheckCollisionPointCircle(mouse, {goals[i].pos.x(), goals[i].pos.y()},
                                    20) &&
          IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        goalMoved[i] = true;
      if (goalMoved[i]) {
        goals[i].pos.x() = mouse.x;
        goals[i].pos.y() = mouse.y;
        goalMoved[i] = !IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
      }
    }
    if (CheckCollisionPointCircle(mouse, {rootPos.x(), rootPos.y()}, 20) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      rootMoved = true;
    }
    if (rootMoved) {
      Vector2f delta {mouse.x-rootPos.x(), mouse.y-rootPos.y()};
      solver.MoveRoot(delta);
      rootPos.x() = mouse.x;
      rootPos.y() = mouse.y;
      for (auto &goal : goals) {
        goal.pos += delta;
      }
      rootMoved = !IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    }
    solver.Solve(goals);

    BeginDrawing();
    ClearBackground(BLACK);
    solver.Draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}