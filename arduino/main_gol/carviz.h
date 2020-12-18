#pragma once
#include "boid.h"

#define TYPE_CAR 2

struct boid_dir {
  float x;
  float y;
};

class CarViz : public BoidViz {
public:
  CarViz(int width, int height, double maxSpeed, unsigned char maxColorIndex);
  ~CarViz();
  
  void update(unsigned long dt);
  void randomize();
  int getType() { return TYPE_CAR; };
protected:
  boid_dir boidDirections[BoidViz::MAX_BOIDS];
};
