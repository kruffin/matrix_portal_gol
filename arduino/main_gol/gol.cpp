#include <cstdlib>

#include "gol.h"

Gol::Gol(int width, int height, unsigned char max_val, int fps, unsigned long run_time) {
  this->width = width;
  this->height = height;
  this->max_val = max_val;

  this->board = (gol_cell *)malloc(this->width * this->height * sizeof(gol_cell));
  this->old_board = (gol_cell *)malloc(this->width * this->height * sizeof(gol_cell));
  this->old_previous_board = (gol_cell *)malloc(this->width * this->height * sizeof(gol_cell));
  this->time_elapsed = 0L;
  this->time_per_iteration = ceil((1.0f / fps) * 1000);
  this->total_time = 0L;
  this->sim_time = run_time;
};

Gol::~Gol() {
  delete this->board;
  delete this->old_board;
  delete this->old_previous_board;
}

void Gol::draw(Adafruit_Protomatter *matrix, cell_color *col) {
  for (int y = 0; y < this->height; ++y) {
    for (int x = 0; x < this->width; ++x) {
      int idx = x + y * this->width;
      gol_cell cell = this->old_board[idx];
      cell_color c = col[cell.value];
      matrix->drawPixel(x, y, matrix->color565(c.red, c.green, c.blue));
    }
  }
};

void Gol::update(unsigned long dt) {
  this->time_elapsed += dt;
  this->total_time += dt;
  if (this->time_elapsed < this->time_per_iteration) {
    return;
  } else {
    this->time_elapsed -= this->time_per_iteration;
  }
  
  this->iterations++;
  
  for (int y = 0; y < this->height; ++y) {
    for (int x = 0; x < this->width; ++x) {
      int left = x == 0 ? this->width - 1 : x - 1;
      int right = x == this->width - 1 ? 0 : x + 1;
      int up = y == 0 ? this->height - 1 : y - 1;
      int down = y == this->height - 1 ? 0 : y + 1;

      // count the live neighbors.
      unsigned char old_val = this->old_board[x + y * this->width].value;
      unsigned char neighbors = (this->old_board[left + up * this->width].value > 0 ? 1 : 0) + (this->old_board[x + up * this->width].value > 0 ? 1 : 0) + (this->old_board[right + up * this->width].value > 0 ? 1 : 0) +
                                (this->old_board[left + y * this->width].value > 0 ? 1 : 0) +                                      (this->old_board[right + y * this->width].value > 0 ? 1 : 0) +
                                (this->old_board[left + down * this->width].value > 0 ? 1 : 0) + (this->old_board[x + down * this->width].value > 0 ? 1 : 0) + (this->old_board[right + down * this->width].value > 0 ? 1 : 0);
      if (neighbors == 3 && old_val == 0) {
        this->board[x + y * this->width].value = 1;
      } else if ((neighbors == 3 || neighbors == 2) && old_val > 0) {
        this->board[x + y * this->width].value = old_val == this->max_val ? old_val : old_val + 1;
      } else {
        this->board[x + y * this->width].value = 0;
      }
    }
  }

  if (this->isSteadyState()) {
    this->randomize();
    return;
  }

  this->swapBuffers();
};

bool Gol::isSteadyState() {
  for(int y = 0; y < this->height; ++y) {
    for(int x = 0; x < this->width; ++x) {
      if (this->board[x + y * this->width].value != this->old_previous_board[x + y * this->width].value) {
        return false;
      }
    }
  }
  return true;
};

void Gol::swapBuffers() {
  gol_cell *tmp = this->old_previous_board;
  this->old_previous_board = this->old_board;
  this->old_board = this->board;
  this->board = tmp;
};

void Gol::randomize() {
  this->iterations = 0;
  
  for (int idx = 0; idx < this->width * this->height; ++idx) {
    // Stick the random values in old_board since this is where we draw from.
    this->old_board[idx].value = random(3) == 0 ? 1 : 0;
    // Zero out board to prevent old values from messing with the new.
    this->board[idx].value = 0;
    this->old_previous_board[idx].value = 0;
  }
};

bool Gol::isFinished() {
  return this->total_time >= this->sim_time;
  //return this->iterations >= 400;
};
