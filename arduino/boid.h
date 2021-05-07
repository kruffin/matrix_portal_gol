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
#include "viz.h"

#define TYPE_BOID 1

#define BOID_NEAR 5
#define BOID_APPROACH 0.05
#define BOID_TOO_CLOSE 1
#define BOID_FLEE 0.05
#define BOID_CENTER 0.005
#define EDGE_MARGIN 4
#define BOID_TURN 0.6

struct boid {
  double x;
  double y;
  double vx;
  double vy;
  // separation
  double closeDx;
  double closeDy;
  // alignment
  double alignVx;
  double alignVy;
  // cohesion
  double cohesionX;
  double cohesionY;

  int neighborBoids;
  
  double speed;
  double minSpeed;
  unsigned char colorIndex;
  bool attractive;
};

class BoidViz : public Viz {
public:
  BoidViz(int width, int height, double maxSpeed, unsigned char maxColorIndex, unsigned long run_time);
  ~BoidViz();

  void update(unsigned long dt);
  void draw(Adafruit_Protomatter *matrix, cell_color *col);
  void randomize();
  bool isFinished();
  int getType() { return TYPE_BOID; };

  static const unsigned char MAX_BOIDS = 75;
protected:
  int width;
  int height;
  unsigned char maxColorIndex;
  double maxBoidSpeed;
  unsigned char numBoids;
  boid *boidList;
  unsigned long runTime;
  unsigned long sim_time;

  double magnitude(double x, double y);
  void normalize(double &x, double &y);
};
