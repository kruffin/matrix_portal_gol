#pragma once
#include "viz.h"

#define TYPE_BOID 1

#define BOID_NEAR 10
#define BOID_APPROACH 0.05
#define BOID_TOO_CLOSE 4
#define BOID_FLEE 0.06

struct boid {
  double x;
  double y;
  double vx;
  double vy;
  double speed;
  unsigned char colorIndex;
  bool attractive;
};

class BoidViz : public Viz {
public:
  BoidViz(int width, int height, double maxSpeed, unsigned char maxColorIndex);
  ~BoidViz();

  void update(unsigned long dt);
  void draw(Adafruit_Protomatter *matrix, cell_color *col);
  void randomize();
  bool isFinished();
  int getType() { return TYPE_BOID; };

  static const unsigned char MAX_BOIDS = 50;
protected:
  int width;
  int height;
  unsigned char maxColorIndex;
  double maxBoidSpeed;
  unsigned char numBoids;
  boid *boidList;
  unsigned long runTime;

  double magnitude(double x, double y);
  void normalize(double &x, double &y);
};
