#pragma once
#include <Adafruit_Protomatter.h>
#include "colors.h"

class Sprite {
public:
  Sprite(String image, int width, int height, cell_color *palette, int palette_size);
  ~Sprite();

  void draw(Adafruit_Protomatter *matrix, cell_color *colors, int x, int y);

  int palette_size;
private:
  int width;
  int height;
  String image;
  cell_color *palette;
};
