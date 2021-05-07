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
#include "sprite.h"

#define TYPE_SPRITE 5

struct boid_attr {
  unsigned char sprite_idx;
  unsigned char palette_start;
};

class SpriteViz : public BoidViz {
public:
  SpriteViz(int width, int height, double maxSpeed, unsigned char maxColorIndex, Sprite *sprites, unsigned char num_sprites, unsigned char min_boids, unsigned long run_time);
  ~SpriteViz();
  
  void update(unsigned long dt);
  void randomize();
  void draw(Adafruit_Protomatter *matrix, cell_color *col);
  int getType() { return TYPE_SPRITE; };
protected:
  boid_attr boidAttributes[BoidViz::MAX_BOIDS];
  Sprite *sprites;
  unsigned char num_sprites;
  unsigned char min_boids;
};
