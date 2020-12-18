#include <cstdlib>

#include "gol.h"

Gol::Gol(int width, int height, unsigned char max_val) {
  this->width = width;
  this->height = height;
  this->max_val = max_val;

  this->board = (gol_cell *)malloc(this->width * this->height * sizeof(gol_cell));
  this->old_board = (gol_cell *)malloc(this->width * this->height * sizeof(gol_cell));
};

Gol::~Gol() {
  delete this->board;
  delete this->old_board;
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

  this->swapBuffers();
};


void Gol::swapBuffers() {
  gol_cell *tmp = this->board;
  this->board = this->old_board;
  this->old_board = tmp;
};

void Gol::randomize() {
  this->iterations = 0;
  
  for (int idx = 0; idx < this->width * this->height; ++idx) {
    // Stick the random values in old_board since this is where we draw from.
    this->old_board[idx].value = random(3) == 0 ? 1 : 0;
    // Zero out board to prevent old values from messing with the new.
    this->board[idx].value = 0;
  }
};

bool Gol::isFinished() {
  return this->iterations >= 400;
};
