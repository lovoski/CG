#include "camera.hpp"
#include "global.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

const int windowWidth = 800;
const int windowHeight = 600;

int main() {
  InitWindow(windowWidth, windowHeight, "raytracing");
  SetTraceLogLevel(LOG_WARNING);
  SetTargetFPS(60);

  // materials
  auto mat_ground = make_shared<lambertian>(vec3(0.8, 0.8, 0));
  auto mat_sphere = make_shared<lambertian>(vec3(0.9, 0.3, 0.3));
  auto mat_left = make_shared<transparent>(1.5);
  auto mat_right = make_shared<metal>(vec3(0.8, 0.6, 0.2), 0);

  // objects
  hittable_list world;
  world.add(make_shared<sphere>(sphere(0.5, vec3(0.0, 0.0, -1.0), mat_sphere)));
  world.add(
      make_shared<sphere>(sphere(100.0, vec3(0.0, -100.5, -1.0), mat_ground)));
  world.add(make_shared<sphere>(sphere(0.5, vec3(-1.0, 0.0, -1.0), mat_left)));
  world.add(make_shared<sphere>(sphere(-0.4, vec3(-1.0, 0.0, -1.0), mat_left)));
  world.add(make_shared<sphere>(sphere(0.5, vec3(1.0, 0.0, -1.0), mat_right)));

  // camera
  camera cam(windowWidth, windowHeight, deg2rad(40.0));

  cam.lookat(vec3(0, 0, 3), vec3(0, 0, -1), vec3(0, 1.0, 0));
  Texture texture = LoadTextureFromImage(cam.render(world).getImage());
  while (!WindowShouldClose()) {
    BeginDrawing();
    DrawTexture(texture, 0, 0, WHITE);
    EndDrawing();
  }
  UnloadTexture(texture);
  CloseWindow();
  return 0;
}