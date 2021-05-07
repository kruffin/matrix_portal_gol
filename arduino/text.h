#pragma once

#include <Adafruit_Protomatter.h>

class Text {
public:
  Text();
  ~Text();

  void draw(int x, int y, Adafruit_Protomatter *matrix, uint16_t textColor, uint16_t bgColor=0, unsigned int space = 1);

  String value = "6:27";

  static const unsigned char textHeight = 7;
  static const unsigned char textWidth = 3;
  static const unsigned char numCharacters = 44;
private:
  // DEBT: Only care about 11 characters plus empty/space. Characters are 0-9 and :
  static const bool font[][Text::textHeight * Text::textWidth];
};
