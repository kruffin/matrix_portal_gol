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
#include "colors.h"

class Viz {
public:
  Viz() {};
  virtual ~Viz() {};
  virtual void update(unsigned long dt) {};
  virtual void draw(Adafruit_Protomatter *matrix, cell_color *col) {};
  virtual void randomize() {};
  virtual bool isFinished() {};
  virtual int getType() {};
};
