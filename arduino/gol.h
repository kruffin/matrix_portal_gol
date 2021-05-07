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

#pragma once
#include <Adafruit_Protomatter.h>
#include "viz.h"

#define TYPE_GOL 0

struct gol_cell {
  unsigned char value; // 0 is dead and anything else is alive.
};

class Gol : public Viz {
public:
  Gol(int width, int height, unsigned char max_val, int fps, unsigned long run_time);
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
  unsigned long time_elapsed;
  unsigned long total_time;
  unsigned long sim_time;
  int time_per_iteration;
  
  gol_cell *board;
  gol_cell *old_board;
  gol_cell *old_previous_board;

  void swapBuffers();
  bool isSteadyState();
};
