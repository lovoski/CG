#ifndef COMMON_BUFFER_H
#define COMMON_BUFFER_H

#include <raylib.h>

#include <cstdint>
#include <iostream>

class Buffer {
 public:
  Buffer(const int _width, const int _height) : width(_width), height(_height) {
    img = GenImageColor(_width, _height, BLACK);
  }
  ~Buffer() { UnloadImage(img); }
  void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b,
                uint8_t a = 255U) {
    ImageDrawPixel(&img, x, y, {r, g, b, a});
  }
  Color getPixel(int x, int y) { return GetImageColor(img, x, y); }
  Image getImage() const { return img; }
  void clear() { ImageClearBackground(&img, BLACK); }
  void dump(const char *filename) {
    if (ExportImage(img, filename)) {
      printf("save buffer as : %s\n", filename);
    } else {
      printf("fail to save buffer as : %s\n", filename);
    }
  }

 private:
  Image img;
  int width, height;
};

#endif