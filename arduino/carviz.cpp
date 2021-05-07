#include "carviz.h"

CarViz::CarViz(int width, int height, double maxSpeed, unsigned char maxColorIndex, unsigned long run_time) : BoidViz(width, height, maxSpeed, maxColorIndex, run_time) {
  
};

CarViz::~CarViz() {
  
};

void CarViz::update(unsigned long dt) {
  this->runTime += dt;
  for (int i = 0; i < this->numBoids; ++i) {
    this->boidList[i].vx = this->boidDirections[i].x;
    this->boidList[i].vy = this->boidDirections[i].y;
  }

  for (int i = 0; i < this->numBoids; ++i) {
    boid *a = this->boidList + i;
    for (int j = i+1; j < this->numBoids; ++j) {
      boid *b = this->boidList + j;

      double dirx = a->x - b->x;
      double diry = a->y - b->y;
      double dist = this->magnitude(dirx, diry);
      if (dist <= BOID_TOO_CLOSE) {
        // apply a force to both boids away from each other
        a->vx += dirx / dist * (BOID_TOO_CLOSE - dist) * BOID_FLEE;
        a->vy += diry / dist * (BOID_TOO_CLOSE - dist) * BOID_FLEE;

        // opposite force to the other
        b->vx += -dirx / dist * (BOID_TOO_CLOSE - dist) * BOID_FLEE;
        b->vy += -diry / dist * (BOID_TOO_CLOSE - dist) * BOID_FLEE;
      }
    }
  }

  // Update positions and clamp speeds
  for (int i = 0; i < this->numBoids; ++i) {
    boid *b = this->boidList + i;
    double mag = this->magnitude(b->vx, b->vy);
    if (mag > b->speed) {
      this->normalize(b->vx, b->vy);
      b->vx *= b->speed;
      b->vy *= b->speed;
    } else {
      b->vx += b->vx / mag * b->speed * double(dt) / double(1000) / 2;
      b->vy += b->vy / mag * b->speed * double(dt) / double(1000) / 2;
    }

    double dx = b->vx * double(dt) / double(1000);
    double dy = b->vy * double(dt) / double(1000);

    b->x += dx;
    if (b->x >= this->width) {
      b->x -= this->width; // wrap around
    } else if (b->x < 0) {
      b->x += (this->width - 1);
    }
    b->y += dy;
    if (b->y >= this->height) {
      b->y -= this->height;
    } else if (b->y < 0) {
      b->y += (this->height - 1);
    }
  }
};

void CarViz::randomize() {
  this->runTime = 0;
  
  // randomly pick a number of boids (with at least 5).
  this->numBoids = random(BoidViz::MAX_BOIDS/2, BoidViz::MAX_BOIDS + 1);

  for (int i = 0; i < this->numBoids; ++i) {
    // start them at random positions with random directions.
    this->boidList[i].x = random(0, this->width);
    this->boidList[i].y = random(0, this->height);

    // whether other boids like being around this one
    this->boidList[i].attractive = (random(11) >= 5);

    // to make this easy will have a set of starting directions.
    unsigned char dir = random(4);
    switch(dir) {
      //north
      case 0:
        this->boidList[i].vx = 0.0;
        this->boidList[i].vy = -1.0 * this->maxBoidSpeed;
        break;
      //south
      case 1:
        this->boidList[i].vx = 0.0;
        this->boidList[i].vy = 1.0 * this->maxBoidSpeed;
        break;
      //east
      case 2:
        this->boidList[i].vx = 1.0 * this->maxBoidSpeed;
        this->boidList[i].vy = 0.0;
        break;
      //west
      case 3:
        this->boidList[i].vx = -1.0 * this->maxBoidSpeed;
        this->boidList[i].vy = 0.0;
        break;
      //north-west
      case 4:
        this->boidList[i].vx = -0.5 * this->maxBoidSpeed;
        this->boidList[i].vy = -0.5 * this->maxBoidSpeed;
        break;
      //north-east
      case 5:
        this->boidList[i].vx = 0.5 * this->maxBoidSpeed;
        this->boidList[i].vy = -0.5 * this->maxBoidSpeed;
        break;
      //south-west
      case 6:
        this->boidList[i].vx = -0.5 * this->maxBoidSpeed;
        this->boidList[i].vy = 0.5 * this->maxBoidSpeed;
        break;
      //south-east
      case 7:
        this->boidList[i].vx = 0.5 * this->maxBoidSpeed;
        this->boidList[i].vy = 0.5 * this->maxBoidSpeed;
        break;
    }

    this->boidDirections[i].x = this->boidList[i].vx;
    this->boidDirections[i].y = this->boidList[i].vy;

    // get a random color, don't pick the zeroth color.
    this->boidList[i].colorIndex = 1 + random(this->maxColorIndex);

    this->boidList[i].speed = random(1, this->maxBoidSpeed+1);
  }
};
