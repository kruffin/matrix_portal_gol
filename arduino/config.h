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

struct configuration {
  char version[7];
  int gmtOffset;
  char wifiName[32];
  char wifiPass[32];
  int golFps;
  int simTime;
};

class Config {
  public:
    static configuration read_config();
    static bool write_config(configuration conf);
    static const char config_file_name[];
    static bool is_initialized;
    static bool init();
};
