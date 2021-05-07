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
#include "boid.h"

#define TYPE_CAR 2

struct boid_dir {
  float x;
  float y;
};

class CarViz : public BoidViz {
public:
  CarViz(int width, int height, double maxSpeed, unsigned char maxColorIndex, unsigned long run_time);
  ~CarViz();
  
  void update(unsigned long dt);
  void randomize();
  int getType() { return TYPE_CAR; };
protected:
  boid_dir boidDirections[BoidViz::MAX_BOIDS];
};
