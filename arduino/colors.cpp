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

#include "colors.h"

const cell_color Colors::colors[20] = {
  {0,   0,    0},
  {100, 0,    0},
  {0,   100,  0},
  {0,   0,    100},
  {100, 0,    100},
  {0,   100,  100},
  {0,   100,  100},
  {100, 100,  100},
  {89,  152,  26},
  {219, 164,  14},
  {33,  182,  168},
  {163, 235,  177},
  {24,  165,  88},
  {255, 39,   104},
  {251, 96,   144},
  {255, 197,  208},
  {174, 56,   139},
  {241, 192,  185},
  {53,  66,   124},
  {37,  48,   170}
};

const cell_color Colors::night_colors[20] = {
  {0,   0,    0},
  {20, 0,    0},
  {30,   0,  0},
  {40, 0,  0},
  {50, 0,    0},
  {60,   0,  0},
  {70, 0,   0},
  {80, 0,  0},
  {20,  0,  20},
  {20, 0,  30},
  {20,  0,  40},
  {20, 0,  50},
  {20,  0,  60},
  {20, 20,   0},
  {20, 30,   0},
  {20, 40,  0},
  {20, 50,   0},
  {20, 60,  0},
  {20, 70,   0},
  {20,  80,   0}
};
