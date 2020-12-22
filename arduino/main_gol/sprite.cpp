#include "sprite.h"

Sprite::Sprite(String image, int width, int height, cell_color *palette, int palette_size) {
  this->image = image;
  this->width = width;
  this->height = height;
  if (palette != NULL) {
    this->palette = (cell_color *)malloc(palette_size * sizeof(cell_color));
    for (int i = 0; i < palette_size; ++i) {
      this->palette[i] = {palette[i].red, palette[i].green, palette[i].blue};
    }
  }
  this->palette_size = palette_size;
};

Sprite::~Sprite() {
  delete this->palette;
};

void Sprite::draw(Adafruit_Protomatter *matrix, cell_color *colors, int x, int y) {
  cell_color *pal = this->palette;
  if (colors != NULL) {
    pal = colors;
  }
  for (int cy = 0; cy < this->height; ++cy) {
    int py = y + cy;
    if (py >= matrix->height()) {
      py -= matrix->height();
    }
    for (int cx = 0; cx < this->width; ++cx) {
      int px = x + cx;
      if (px >= matrix->width()) {
        px -= matrix->width();
      }
//      Serial.print("setting pixel at (");
//      Serial.print(cx);
//      Serial.print(", ");
//      Serial.print(cy);
//      Serial.println(").");
      int idx = this->image.charAt(cx + cy * this->width) - '0';
      // Index 0 is transparent.
      if (idx != 0) {
        idx -=1; // move back to zero index
        matrix->drawPixel(px, py, matrix->color565(pal[idx].red, pal[idx].green, pal[idx].blue));
      }
    }
  }
};
