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
