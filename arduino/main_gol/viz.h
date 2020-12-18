#pragma once
#include <Adafruit_Protomatter.h>
#include "colors.h"

class Viz {
public:
  Viz() {};
  virtual ~Viz() {};
  virtual void update(unsigned long dt) {};
  virtual void draw(Adafruit_Protomatter *matrix, cell_color *col) {};
  virtual void randomize() {};
  virtual bool isFinished() {};
  virtual int getType() {};
};
