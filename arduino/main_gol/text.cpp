#include "text.h"

const bool Text::font[][Text::textHeight * Text::textWidth] = {
  // 0
  { 1, 1, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 1 },
  // 1
  { 0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0 },
  // 2
  { 1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    1, 1, 1,
    1, 0, 0,
    1, 0, 0,
    1, 1, 1 },
  // 3
  { 1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    1, 1, 1 },
  // 4
  { 1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1 },
  // 5
  { 1, 1, 1,
    1, 0, 0,
    1, 0, 0,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    1, 1, 1 },
  // 6
  { 0, 1, 1,
    1, 0, 0,
    1, 0, 0,
    1, 1, 0,
    1, 0, 1,
    1, 0, 1,
    0, 1, 1 },
  // 7
  { 1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    0, 1, 0,
    0, 1, 0,
    1, 0, 0,
    1, 0, 0 },
  // 8
  { 0, 1, 0,
    1, 0, 1,
    1, 0, 1,
    0, 1, 0,
    1, 0, 1,
    1, 0, 1,
    0, 1, 0 },
  // 9
  { 0, 1, 0,
    1, 0, 1,
    1, 0, 1,
    0, 1, 1,
    0, 0, 1,
    0, 0, 1,
    1, 1, 0 },
  // :
  { 0, 0, 0,
    0, 1, 0,
    0, 1, 0,
    0, 0, 0,
    0, 1, 0,
    0, 1, 0,
    0, 0, 0 },
  // space/empty
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 }
};

Text::Text() {
  
};

Text::~Text() {
  
};

void Text::draw(int x, int y, Adafruit_Protomatter *matrix, unsigned int space) {
  for (int i = 0; i < this->value.length(); ++i) {
      unsigned char c = this->value.charAt(i);
      int fontIdx = c - 48;
      if (fontIdx < 0 || fontIdx > 10) {
        fontIdx = 11; // Pick the empty/space to draw. 
      }

//      Serial.print("char: ");
//      Serial.print(c);
//      Serial.print(" num: ");
//      Serial.print(int(c));
//      Serial.print(" index: ");
//      Serial.println(fontIdx);

      const bool *charForm = Text::font[fontIdx];
      int startx = x + i * (Text::textWidth + space);
      for (int cy = 0; cy < Text::textHeight; ++cy) {
        for (int cx = 0; cx < Text::textWidth; ++cx) {
          if (charForm[cx + cy * Text::textWidth]) {
            matrix->drawPixel(startx + cx, y + cy, matrix->color565(255, 0, 0));
          } else {
            matrix->drawPixel(startx + cx, y + cy, matrix->color565(0, 0, 0));
          }
        }
      }
  }
};
