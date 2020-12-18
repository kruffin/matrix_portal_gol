#pragma once
#include "boid.h"

#define TYPE_RAIN 3

class RainViz : public BoidViz {
public:
  RainViz(int width, int height, double maxSpeed, unsigned char maxColorIndex);
  ~RainViz();
  
  void update(unsigned long dt);
  void randomize();
  int getType() { return TYPE_RAIN; };
private:

};
