#pragma once
#define MAX_COLORS 20
#define MIN_COLORS 2

struct cell_color {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

class Colors {
public:
  static const cell_color *get_colors(bool isNight) {
    if (isNight) {
      return Colors::night_colors;
    } else {
      return Colors::colors;
    }
  };
private:
  static const cell_color colors[20];
  static const cell_color night_colors[20];
};
