#pragma once
#include "viz.h"
#include "text.h"
#include "ntp.h"

#define TYPE_CLOCK 4

class ClockViz : public Viz {
public:
  ClockViz(int width, int height);
  ~ClockViz();

  void init(Adafruit_Protomatter *matrix);
  void update(unsigned long not_used);
  void draw(Adafruit_Protomatter *matrix, cell_color *col);
  void randomize();
  bool isNight() { return this->timeGrabber->isNight(); };
  bool isFinished() { return false; };
  int getType() { return TYPE_CLOCK; };

private:
  int width;
  int height;
  Ntp *timeGrabber = NULL;
  IPAddress *ntpIP = NULL;
  Text timeText;

  char *ssid;
  char *pass;

  bool showTime;
  bool hasTime;
  unsigned int textY;
  unsigned long lastTime;
  unsigned long dt;
  unsigned long lastTimePulled;
  unsigned long backOffTime;
  cell_color timeColor;
};
