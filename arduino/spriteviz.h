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
