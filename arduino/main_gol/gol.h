#pragma once
#include <Adafruit_Protomatter.h>

struct gol_cell {
  unsigned char value; // 0 is dead and anything else is alive.
};

struct cell_color {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

class Gol {
public:
  Gol(int width, int height);
  ~Gol();

  // Draws from the old_board buffer; assumes life() has been called prior.
  void draw(cell_color *col, Adafruit_Protomatter *matrix);
  // Ticks the simulation one round and then flips the buffers.
  void life(unsigned char max_val);

  void randomize();
private:
  int width;
  int height;
  
  gol_cell *board;
  gol_cell *old_board;

  void swapBuffers();
};
