/**
GOL Clock - A clock that displays simulations.
Copyright (C) 2021 Kevin Ruffin

This file is part of GOL Clock.

GOL Clock is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GOL Clock is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "text.h"

Text::Text() {
  
};

Text::~Text() {
  
};

void Text::draw(int x, int y, Adafruit_Protomatter *matrix, uint16_t textColor, uint16_t bgColor, unsigned int space) {
  for (int i = 0; i < this->value.length(); ++i) {
      unsigned char c = this->value.charAt(i);
      int fontIdx = c - 48;
      if (fontIdx < 0 || fontIdx > Text::numCharacters - 2) {
        fontIdx = Text::numCharacters - 1; // Pick the empty/space to draw. 
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
            matrix->drawPixel(startx + cx, y + cy, textColor); //matrix->color565(255, 0, 0));
          } else {
            matrix->drawPixel(startx + cx, y + cy, bgColor);//matrix->color565(0, 0, 0));
          }
        }
      }
  }
};


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
  // ;
  { 0, 0, 0,
    0, 1, 0,
    0, 1, 0,
    0, 0, 0,
    0, 0, 0,
    0, 1, 0,
    1, 0, 0 },
  // <
  { 0, 0, 0,
    0, 0, 1,
    0, 1, 0,
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    0, 0, 0 },
  // =
  { 0, 0, 0,
    0, 0, 0,
    1, 1, 1,
    0, 0, 0,
    1, 1, 1,
    0, 0, 0,
    0, 0, 0 },
  // >
  { 0, 0, 0,
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    0, 1, 0,
    1, 0, 0,
    0, 0, 0 },
  // ?
  { 0, 1, 0,
    1, 0, 1,
    0, 0, 1,
    0, 1, 0,
    0, 1, 0,
    0, 0, 0,
    0, 1, 0 },
  // @
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 1, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // A
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // B
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // C
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // D
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // E
  { 1, 1, 1,
    1, 0, 0,
    1, 0, 0,
    1, 1, 0,
    1, 0, 0,
    1, 0, 0,
    1, 1, 1 },
  // F
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // G
  { 0, 1, 1,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 1, 1,
    1, 0, 1,
    0, 1, 1 },
  // H
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // I
  { 1, 1, 1,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    1, 1, 1 },
  // J
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // K
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // L
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // M
  { 1, 0, 1,
    1, 0, 1,
    1, 1, 1,
    1, 1, 1,
    1, 1, 1,
    1, 0, 1,
    1, 0, 1 },
  // N
  { 1, 0, 1,
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
    1, 1, 1,
    1, 0, 1,
    1, 0, 1 },
  // O
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // P
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // Q
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // R
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // S
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // T
  { 1, 1, 1,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0 },
  // U
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // V
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // W
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // X
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // Y
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0 },
  // Z
  { 0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
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
