
#define USE_NTP

#include "viz.h"
#include "gol.h"
#include "boid.h"
#include "carviz.h"
#include "rainviz.h"
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

Adafruit_Protomatter matrix(
  WIDTH,          // Width of matrix (or matrix chain) in pixels
  4,           // Bit depth, 1-6
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  true);      // No double-buffering here (see "doublebuffer" example)

Viz *game;

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

#ifdef USE_NTP
  Serial.println("clock viz init()");
  clockViz.init(&matrix);
  Serial.println("clock viz init done.");
#endif

  pinMode(buttonUp.pin, INPUT_PULLUP);
  pinMode(buttonDown.pin, INPUT_PULLUP);

  palette = (cell_color *)malloc(NUM_COLORS * sizeof(cell_color));

  randomSeed(analogRead(a0));
  game = new Gol(WIDTH, HEIGHT, NUM_COLORS - 1);
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
//    lc = 0;
//    NUM_COLORS = NUM_COLORS < MAX_COLORS ? NUM_COLORS + 1 : NUM_COLORS;
    DELAY_MS += DELAY_INCREMENT;
  } else if (is_button_pressed(&buttonDown)) {
//    lc = 0;
//    NUM_COLORS = NUM_COLORS > MIN_COLORS ? NUM_COLORS - 1 : NUM_COLORS;
    DELAY_MS = DELAY_MS > 0 ? DELAY_MS - DELAY_INCREMENT : 0;
  }
  
  if (game->isFinished()) {
    switch (game->getType()) {
      case TYPE_GOL:
        delete game;
        game = new BoidViz(WIDTH, HEIGHT, 5, NUM_COLORS - 1);
        break;
      case TYPE_BOID:
        delete game;
        game = new CarViz(WIDTH, HEIGHT, 5, NUM_COLORS - 1);
        break;
      case TYPE_CAR:
        delete game;
        game = new RainViz(WIDTH, HEIGHT, 5, NUM_COLORS - 1);
        break;
      case TYPE_RAIN:
        delete game;
        if (clockViz.isNight()) {
          // Skip the GOL since it's too bright at night
          game = new BoidViz(WIDTH, HEIGHT, 5, NUM_COLORS - 1);
        } else {
          game = new Gol(WIDTH, HEIGHT, NUM_COLORS - 1);
        }
        break;
      default:
        delete game;
        game = new Gol(WIDTH, HEIGHT, NUM_COLORS - 1);
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
  delay(DELAY_MS);
}
