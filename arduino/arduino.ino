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


#define USE_NTP

#include "viz.h"
#include "gol.h"
#include "boid.h"
#include "carviz.h"
#include "rainviz.h"
#include "sprite.h"
#include "spriteviz.h"
#include "config.h"
#ifdef USE_NTP
#include "clockviz.h"
#endif

#if defined(_VARIANT_MATRIXPORTAL_M4_) // MatrixPortal M4
  uint8_t rgbPins[]  = {7, 8, 9, 10, 11, 12};
  uint8_t addrPins[] = {17, 18, 19, 20};
  uint8_t clockPin   = 14;
  uint8_t latchPin   = 15;
  uint8_t oePin      = 16;
  uint8_t btnUp      = 2;
  uint8_t btnDwn     = 3;
  uint8_t a0         = PIN_A0;
#endif

#define WIDTH 64
#define HEIGHT 32
#define ITERATIONS 400
#define DELAY_INCREMENT 10

unsigned int DELAY_MS = 10;//80;//90;
unsigned char NUM_COLORS = 8;
int GOL_FPS = 15;
unsigned long SIM_TIME = 30L * 1000L;
unsigned long gmt_offset_changed_time = 0L;

#define MAX_SPEED 10

Adafruit_Protomatter matrix(
  WIDTH,          // Width of matrix (or matrix chain) in pixels
  4,           // Bit depth, 1-6
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  true);      // No double-buffering here (see "doublebuffer" example)

Viz *game;
cell_color tree_pal[] = {{200, 30, 100}, {0, 150, 0}};
Sprite tree = Sprite("0020000200022202222200100", 5, 5, (cell_color *)tree_pal, 2);
Sprite flower = Sprite("010121010", 3, 3, (cell_color *)tree_pal, 2);
Sprite snowflake = Sprite("0010010101011101010100100", 5, 5, (cell_color *)tree_pal, 2);
Sprite face = Sprite("0111012021100011020101110", 5, 5, (cell_color *)tree_pal, 2);
Sprite snowman = Sprite("00200232320232022122222222212202220", 5, 7, (cell_color *)tree_pal, 3);
Sprite AVAIL_SPRITES[] = { tree, flower, snowflake, face, snowman };
unsigned char NUM_SPRITES = 5;

struct btn_state {
  int state;
  int lastState;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;
  uint8_t pin;
};

cell_color *palette;

btn_state buttonUp = {.state=0, .lastState=HIGH, .lastDebounceTime=0, .debounceDelay=50, .pin=btnUp};
btn_state buttonDown = {.state=0, .lastState=HIGH, .lastDebounceTime=0, .debounceDelay=50, .pin=btnDwn};

#ifdef USE_NTP
ClockViz clockViz = ClockViz(WIDTH, HEIGHT);
#endif

void pick_palette_colors(cell_color *pal, const cell_color *avail_colors) {
//  deque<cell_color> to_pick;
//  for (int idx = 0; idx < 20; ++idx) {
//    to_pick.push_back(avail_colors[idx]);
//  }
//
//  pal[0] = avail_colors[0];
//  for (int pi = 1; pi < NUM_COLORS; ++pi) {
//    int di = random(deque.size())
//    cell_color c = deque[di];
//    deque.erase(deque.begin() + di);
//    pal[pi] = c;
//  }
  
  pal[0] = avail_colors[0];
  int pick;
  bool good_pick = false;
  for (int pi = 1; pi < NUM_COLORS; ++pi) {
    do {
      pick = 1 + random(MAX_COLORS-1); // Never pick the zeroth color.
      good_pick = true;

      for (int pii = 1; pii < pi; ++pii) {
        cell_color c = pal[pii];
        cell_color a = avail_colors[pick];
        if (c.red == a.red && c.green == a.green && c.blue == a.blue) {
          good_pick = false;
          break;
        }
      }
    } while(!good_pick);

    pal[pi] = avail_colors[pick];
  }
};

bool is_button_pressed(btn_state *btn) {
  int s = digitalRead(btn->pin);
//  Serial.print("pin ");
//  Serial.print(btn->pin);
//  Serial.print("button state: ");
//  Serial.println(s);
  if (s != btn->lastState) {
    btn->lastDebounceTime = millis();
  }

  bool changed = false;
  if ((millis() - btn->lastDebounceTime) > btn->debounceDelay) {
    if (s != btn->state) {
      btn->state = s;
      changed = true;
    }
  }
  btn->lastState = s;

  return (changed && s == HIGH);
}

configuration conf;
bool is_clock_inited = false;

void setup() {
  Serial.begin(9600);
  
  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if(status != PROTOMATTER_OK) {
    // DO NOT CONTINUE if matrix setup encountered an error.
    for(;;);
  }

  pinMode(buttonUp.pin, INPUT_PULLUP);
  pinMode(buttonDown.pin, INPUT_PULLUP);

  palette = (cell_color *)malloc(NUM_COLORS * sizeof(cell_color));

  randomSeed(analogRead(a0));
  

#ifdef USE_NTP
  conf = Config::read_config();
  Ntp::OFFSET_HOURS = conf.gmtOffset;
  if (strcmp(conf.wifiName, "") != 0) {
    clockViz.set_credentials(conf.wifiName, conf.wifiPass);
  }
  if (conf.golFps > 0) {
    GOL_FPS = conf.golFps;
  }
  if (conf.simTime > 0) {
    SIM_TIME = conf.simTime;
  }

  Serial.println("clock viz init()");
  is_clock_inited = clockViz.init(&matrix);
  Serial.println("clock viz init done.");
#endif

  game = new Gol(WIDTH, HEIGHT, NUM_COLORS - 1, GOL_FPS, SIM_TIME);
//  game = new BoidViz(WIDTH, HEIGHT, 5, NUM_COLORS - 1);
  Serial.println("viz randomize start");
  game->randomize();
  Serial.println("viz randomize done.");
  pick_palette_colors(palette, Colors::get_colors(clockViz.isNight()));
}

unsigned long loopTime = millis();

void loop() {
#ifdef USE_NTP
  clockViz.update(0);
#endif

  if (is_button_pressed(&buttonUp)) {
    Ntp::OFFSET_HOURS += 1;
    if (Ntp::OFFSET_HOURS != conf.gmtOffset) {
      gmt_offset_changed_time = millis();
    }
  } else if (is_button_pressed(&buttonDown)) {
    Ntp::OFFSET_HOURS -= 1;
    if (Ntp::OFFSET_HOURS != conf.gmtOffset) {
      gmt_offset_changed_time = millis();
    }
  }
  
  if (game->isFinished()) {
    switch (game->getType()) {
      case TYPE_GOL:
        delete game;
        game = new BoidViz(WIDTH, HEIGHT, MAX_SPEED, NUM_COLORS - 1, SIM_TIME);
        break;
      case TYPE_BOID:
        delete game;
        game = new CarViz(WIDTH, HEIGHT, MAX_SPEED, NUM_COLORS - 1, SIM_TIME);
        break;
      case TYPE_CAR:
        delete game;
        if (clockViz.isNight()) {
          // Skip the GOL since it's too bright at night
          game = new RainViz(WIDTH, HEIGHT, MAX_SPEED, NUM_COLORS - 1, SIM_TIME);
        } else {
          game = new SpriteViz(WIDTH, HEIGHT, MAX_SPEED, NUM_COLORS - 1, AVAIL_SPRITES, random(1, NUM_SPRITES+1), 10, SIM_TIME);
        }
        break;
      case TYPE_SPRITE:
        delete game;
        game = new RainViz(WIDTH, HEIGHT, MAX_SPEED, NUM_COLORS - 1, SIM_TIME);
        break;
      case TYPE_RAIN:
        delete game;
        if (clockViz.isNight()) {
          // Skip the GOL since it's too bright at night
          game = new BoidViz(WIDTH, HEIGHT, MAX_SPEED, NUM_COLORS - 1, SIM_TIME);
        } else {
          game = new Gol(WIDTH, HEIGHT, NUM_COLORS - 1, GOL_FPS, SIM_TIME);
        }
        break;
      default:
        delete game;
        game = new Gol(WIDTH, HEIGHT, NUM_COLORS - 1, GOL_FPS, SIM_TIME);
        break;
    }

    game->randomize();
    pick_palette_colors(palette, Colors::get_colors(clockViz.isNight()));
#ifdef USE_NTP
//    showTime = !showTime;
    clockViz.randomize();
#endif
  }

  game->draw(&matrix, palette);
#ifdef USE_NTP
  clockViz.draw(&matrix, palette);
#endif
  matrix.show();
  unsigned long t = millis();
  game->update(t - loopTime);
  loopTime = t;
//  delay(DELAY_MS);

#ifdef USE_NTP
  if (Serial.available() >= sizeof(configuration)) {
    for(int i = 0; i < sizeof(configuration); i++) {
      *((char*)&conf + i) = Serial.read();
    }

    if (strcmp(conf.version, "GOL001") == 0) {
      Ntp::OFFSET_HOURS = conf.gmtOffset;
      clockViz.set_credentials(conf.wifiName, conf.wifiPass);
      if (conf.golFps > 0) {
        GOL_FPS = conf.golFps;
        Serial.print("New FPS is: ");
        Serial.println(conf.golFps);
      }
      if (conf.simTime > 0) {
        SIM_TIME = conf.simTime;
        Serial.print("New sim time is: ");
        Serial.println(conf.simTime);
      }
      if (Config::write_config(conf)) {
        Serial.println("New configuration written.");
      } else {
        Serial.println("Failed to write the configuration.");
      }
    }
    clockViz.update(0);
  }
  if (!is_clock_inited) {
    is_clock_inited = clockViz.init(&matrix);
  }
  if (gmt_offset_changed_time != 0L && millis() - gmt_offset_changed_time > 5000 && conf.gmtOffset != Ntp::OFFSET_HOURS) {
    // Update the config
    gmt_offset_changed_time = 0L;
    conf.gmtOffset = Ntp::OFFSET_HOURS;
    if (Config::write_config(conf)) {
      Serial.println("New configuration written.");
    } else {
      Serial.println("Failed to write the configuration.");
    }
  }
//  Serial.print("gmt offset: ");
//  Serial.println((int)conf.gmtOffset);
#endif
}
