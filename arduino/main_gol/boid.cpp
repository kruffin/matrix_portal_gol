#include <cstdlib>
#include "boid.h"

BoidViz::BoidViz(int width, int height, double maxSpeed, unsigned char maxColorIndex) {
   this->width = width;
   this->height = height;
   this->maxBoidSpeed = maxSpeed;
   this->maxColorIndex = maxColorIndex;
   this->boidList = new boid[BoidViz::MAX_BOIDS];
};

BoidViz::~BoidViz() {
  if (NULL != this->boidList) {
    delete[] this->boidList;
    this->boidList = NULL;
  }
};

void BoidViz::update(unsigned long dt) {
  //Serial.print("dt: ");
  //Serial.println(dt);
  this->runTime += dt;

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
      } else if (dist <= BOID_NEAR) {
        // apply a force to a boid (if other is attractive) towards it
        if (b->attractive) {
          a->vx += -dirx / dist * (BOID_NEAR - dist) * BOID_APPROACH;
          a->vy += -diry / dist * (BOID_NEAR - dist) * BOID_APPROACH;
        }

        // opposite force to the other
        if (a->attractive) {
          b->vx += +dirx / dist * (BOID_NEAR - dist) * BOID_APPROACH;
          b->vy += +diry / dist * (BOID_NEAR - dist) * BOID_APPROACH;
        }
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

void BoidViz::draw(Adafruit_Protomatter *matrix, cell_color *col) {
  matrix->fillScreen(0x0);
  
  for (int i = 0; i < this->numBoids; ++i) {
    boid b = this->boidList[i];
    cell_color c = col[b.colorIndex];
    int x = int(b.x);
    int y = int(b.y);
    if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
      matrix->drawPixel(x, y, matrix->color565(c.red, c.green, c.blue));
    }
  };
};

void BoidViz::randomize() {
  this->runTime = 0;
  
  // randomly pick a number of boids (with at least 5).
  this->numBoids = random(20, BoidViz::MAX_BOIDS + 1);

  for (int i = 0; i < this->numBoids; ++i) {
    // start them at random positions with random directions.
    this->boidList[i].x = random(0, this->width);
    this->boidList[i].y = random(0, this->height);

    // whether other boids like being around this one
    this->boidList[i].attractive = (random(11) >= 5);

    // to make this easy will have a set of starting directions.
    unsigned char dir = random(8);
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

    // get a random color, don't pick the zeroth color.
    this->boidList[i].colorIndex = 1 + random(this->maxColorIndex + 1);

    this->boidList[i].speed = this->maxBoidSpeed;
  }
};

bool BoidViz::isFinished() {
  return this->runTime > 30 * 1000; // stop after 30 seconds.
};

double BoidViz::magnitude(double x, double y) {
  return sqrt(sq(x) + sq(y));
};
void BoidViz::normalize(double &x, double &y) {
  double mag = this->magnitude(x, y);
  x = x / mag;
  y = y / mag;
};
