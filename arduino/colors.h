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
#define MAX_COLORS 20
#define MIN_COLORS 2

struct cell_color {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

class Colors {
public:
  static const cell_color *get_colors(bool isNight) {
    if (isNight) {
      return Colors::night_colors;
    } else {
      return Colors::colors;
    }
  };
private:
  static const cell_color colors[20];
  static const cell_color night_colors[20];
};
