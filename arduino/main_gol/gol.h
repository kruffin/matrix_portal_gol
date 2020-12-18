#pragma once
#include <Adafruit_Protomatter.h>
#include "viz.h"

#define TYPE_GOL 0

struct gol_cell {
  unsigned char value; // 0 is dead and anything else is alive.
};

class Gol : public Viz {
public:
  Gol(int width, int height, unsigned char max_val);
  ~Gol();

  // Draws from the old_board buffer; assumes life() has been called prior.
  void draw(Adafruit_Protomatter *matrix, cell_color *col);
  // Ticks the simulation one round and then flips the buffers.
  void update(unsigned long dt);

  void randomize();
  bool isFinished();
  int getType() { return TYPE_GOL; };
private:
  int width;
  int height;
  unsigned char max_val;
  unsigned int iterations;
  
  gol_cell *board;
  gol_cell *old_board;

  void swapBuffers();
};
