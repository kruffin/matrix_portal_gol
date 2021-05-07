#include "spriteviz.h"

SpriteViz::SpriteViz(int width, int height, double maxSpeed, unsigned char maxColorIndex, Sprite *sprites, unsigned char num_sprites, unsigned char min_boids, unsigned long run_time) : BoidViz(width, height, maxSpeed, maxColorIndex, run_time) {
  this->sprites = sprites;
  this->num_sprites = num_sprites;
  this->min_boids = min_boids;
};

SpriteViz::~SpriteViz() {
  
};

void SpriteViz::update(unsigned long dt) {
  this->runTime += dt;

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

void SpriteViz::randomize() {
  this->runTime = 0;
  
  // randomly pick a number of boids (with at least 5).
  this->numBoids = random(this->min_boids, BoidViz::MAX_BOIDS + 1);

  unsigned char dir = random(2);
  
  for (int i = 0; i < this->numBoids; ++i) {
    // start them at random positions with random directions.
    this->boidList[i].x = random(0, this->width);
    this->boidList[i].y = random(0, this->height);

    // whether other boids like being around this one
    this->boidList[i].attractive = (random(11) >= 5);

    // to make this easy will have a set of starting directions.
    switch(dir) {
      //south-west
      case 0:
        this->boidList[i].vx = -0.5 * this->maxBoidSpeed;
        this->boidList[i].vy = 0.5 * this->maxBoidSpeed;
        break;
      //south-east
      case 1:
        this->boidList[i].vx = 0.5 * this->maxBoidSpeed;
        this->boidList[i].vy = 0.5 * this->maxBoidSpeed;
        break;
    }

    // pick a random sprite
    int spriteIdx = random(this->num_sprites);
    this->boidAttributes[i].sprite_idx = spriteIdx;
    // get a random color, don't pick the zeroth color.
    this->boidList[i].colorIndex = 1 + random(this->maxColorIndex - this->sprites[spriteIdx].palette_size);

    this->boidList[i].speed = random(1, this->maxBoidSpeed+1);
  }
};

void SpriteViz::draw(Adafruit_Protomatter *matrix, cell_color *col) {
  matrix->fillScreen(0x0);
  
  for (int i = 0; i < this->numBoids; ++i) {
    boid b = this->boidList[i];
    int x = int(b.x);
    int y = int(b.y);
    if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
      this->sprites[this->boidAttributes[i].sprite_idx].draw(matrix, col + b.colorIndex, x, y);
    }
  };
};
