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

#pragma once

#include <Adafruit_Protomatter.h>

class Text {
public:
  Text();
  ~Text();

  void draw(int x, int y, Adafruit_Protomatter *matrix, uint16_t textColor, uint16_t bgColor=0, unsigned int space = 1);

  String value = "6:27";

  static const unsigned char textHeight = 7;
  static const unsigned char textWidth = 3;
  static const unsigned char numCharacters = 44;
private:
  // DEBT: Only care about 11 characters plus empty/space. Characters are 0-9 and :
  static const bool font[][Text::textHeight * Text::textWidth];
};
