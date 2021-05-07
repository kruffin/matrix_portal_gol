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

#include <cstdlib>
#include "boid.h"

BoidViz::BoidViz(int width, int height, double maxSpeed, unsigned char maxColorIndex, unsigned long run_time) {
   this->width = width;
   this->height = height;
   this->maxBoidSpeed = maxSpeed;
   this->maxColorIndex = maxColorIndex;
   this->boidList = new boid[BoidViz::MAX_BOIDS];
   this->sim_time = run_time;
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

  for (int k = 0; k < this->numBoids; ++k) {
    this->boidList[k].closeDx = 0.0;
    this->boidList[k].closeDy = 0.0;

    this->boidList[k].alignVx = 0.0;
    this->boidList[k].alignVy = 0.0;

    this->boidList[k].cohesionX = 0.0;
    this->boidList[k].cohesionY = 0.0;

    this->boidList[k].neighborBoids = 0;
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
        a->closeDx += dirx;
        a->closeDy += diry;
        b->closeDx += -dirx;
        b->closeDy += -diry;
        //a->vx += dirx / dist * (BOID_TOO_CLOSE - dist) * BOID_FLEE;
        
      } else if (dist <= BOID_NEAR) {
        // apply a force to a boid (if other is attractive) towards it
        if (b->attractive) {
          a->alignVx += b->vx;
          a->alignVy += b->vy;

          a->cohesionX += b->x;
          a->cohesionY += b->y;
          
          a->neighborBoids++;
          
          //a->vy += -diry / dist * (BOID_NEAR - dist) * BOID_APPROACH;
        }

        // opposite force to the other
        if (a->attractive) {
          b->alignVx += a->vx;
          b->alignVy += a->vy;

          b->cohesionX += a->x;
          b->cohesionY += a->y;
          
          b->neighborBoids++;
        }
      }
    }
  }

  // Update positions and clamp speeds
  for (int i = 0; i < this->numBoids; ++i) {
    boid *b = this->boidList + i;

    b->vx += b->closeDx * BOID_FLEE;
    b->vy += b->closeDy * BOID_FLEE;

    if (b->neighborBoids > 0) {
      b->alignVx /= b->neighborBoids;
      b->alignVy /= b->neighborBoids;
  
      b->vx += (b->alignVx - b->vx) * BOID_APPROACH;
      b->vy += (b->alignVy - b->vy) * BOID_APPROACH;
  
      b->cohesionX /= b->neighborBoids;
      b->cohesionY /= b->neighborBoids;
  
      b->vx += (b->cohesionX - b->x) * BOID_CENTER;
      b->vy += (b->cohesionY - b->y) * BOID_CENTER;
    }

    if (b->x <= EDGE_MARGIN) {
      b->vx += BOID_TURN;
    }
    if (b->x >= this->width - EDGE_MARGIN) {
      b->vx -= BOID_TURN;
    }
    if (b->y <= EDGE_MARGIN) {
      b->vy += BOID_TURN;
    }
    if (b->y >= this->height - EDGE_MARGIN) {
      b->vy -= BOID_TURN;
    }
    
    double mag = this->magnitude(b->vx, b->vy);
    if (mag > b->speed) {
      this->normalize(b->vx, b->vy);
      b->vx *= b->speed;
      b->vy *= b->speed;
    } else if (mag < b->minSpeed) {
      this->normalize(b->vx, b->vy);
      b->vx *= b->minSpeed;
      b->vy *= b->minSpeed;
    } else {
      b->vx += b->vx / mag * b->speed * double(dt) / double(1000) / 2;
      b->vy += b->vy / mag * b->speed * double(dt) / double(1000) / 2;
    }

    double dx = b->vx * double(dt) / double(1000);
    double dy = b->vy * double(dt) / double(1000);
    
    b->x += dx;
//    if (b->x >= this->width) {
//      b->x -= this->width; // wrap around
//    } else if (b->x < 0) {
//      b->x += (this->width - 1);
//    }
    b->y += dy;
//    if (b->y >= this->height) {
//      b->y -= this->height;
//    } else if (b->y < 0) {
//      b->y += (this->height - 1);
//    }
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
  this->numBoids = random(BoidViz::MAX_BOIDS/2, BoidViz::MAX_BOIDS + 1);

  for (int i = 0; i < this->numBoids; ++i) {
    // start them at random positions with random directions.
    this->boidList[i].x = random(0, this->width);
    this->boidList[i].y = random(0, this->height);

    // whether other boids like being around this one
    this->boidList[i].attractive = true;//(random(11) >= 5);

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
    this->boidList[i].colorIndex = 1 + random(this->maxColorIndex);

    this->boidList[i].speed = random(this->maxBoidSpeed/2, this->maxBoidSpeed);
    this->boidList[i].minSpeed = this->maxBoidSpeed/2;
  }
};

bool BoidViz::isFinished() {
  return this->runTime > this->sim_time; // stop after 30 seconds.
};

double BoidViz::magnitude(double x, double y) {
  return sqrt(sq(x) + sq(y));
};
void BoidViz::normalize(double &x, double &y) {
  double mag = this->magnitude(x, y);
  x = x / mag;
  y = y / mag;
};
