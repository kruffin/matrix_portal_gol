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
#include "viz.h"
#include "text.h"
#include "ntp.h"

#define TYPE_CLOCK 4

class ClockViz : public Viz {
public:
  ClockViz(int width, int height);
  ~ClockViz();

  bool init(Adafruit_Protomatter *matrix);
  void update(unsigned long not_used);
  void draw(Adafruit_Protomatter *matrix, cell_color *col);
  void randomize();
  bool isNight() { return this->timeGrabber->isNight(); };
  bool isFinished() { return false; };
  int getType() { return TYPE_CLOCK; };
  void set_credentials(char *ssid, char *password);

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
