
#include <Adafruit_Protomatter.h>
#include "text.h"
#include "ntp.h"
#include "secrets.h"

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
#define MAX_COLORS 20
#define MIN_COLORS 2
#define ITERATIONS 400
#define DELAY_INCREMENT 10

unsigned int DELAY_MS = 90;
unsigned char NUM_COLORS = 8;

Adafruit_Protomatter matrix(
  WIDTH,          // Width of matrix (or matrix chain) in pixels
  4,           // Bit depth, 1-6
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  true);      // No double-buffering here (see "doublebuffer" example)

struct gol_cell {
  unsigned char value; // 0 is dead and anything else is alive.
};

struct cell_color {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

struct btn_state {
  int state;
  int lastState;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;
  uint8_t pin;
};

gol_cell *board;
gol_cell *old_board;
cell_color colors[20] = {
  {0,   0,    0},
  {100, 0,    0},
  {0,   100,  0},
  {0,   0,    100},
  {100, 0,    100},
  {0,   1000, 100},
  {0,   100,  100},
  {100, 100,  100},
  {89,  152,  26},
  {219, 164,  14},
  {33,  182,  168},
  {163, 235,  177},
  {24,  165,  88},
  {255, 39,   104},
  {251, 96,   144},
  {255, 197,  208},
  {174, 56,   139},
  {241, 192,  185},
  {53,  66,   124},
  {37,  48,   170}
};
cell_color *palette;

btn_state buttonUp = {.state=0, .lastState=HIGH, .lastDebounceTime=0, .debounceDelay=50, .pin=btnUp};
btn_state buttonDown = {.state=0, .lastState=HIGH, .lastDebounceTime=0, .debounceDelay=50, .pin=btnDwn};
Text timeText = Text();
IPAddress ntpIP = IPAddress(129, 6, 15, 28);
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
Ntp timeGrabber = Ntp(ssid, pass, &ntpIP);

void randomize_board(gol_cell *b) {
  for (int idx = 0; idx < WIDTH*HEIGHT; ++idx) {
    b[idx].value = random(3) == 0 ? 1 : 0;
  }
};

void pick_palette_colors(cell_color *pal, cell_color *avail_colors) {
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
      pick = 1 + random(19); // Never pick the zeroth color.
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

void draw_board(gol_cell *b, cell_color *col) {
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      int idx = x + y * WIDTH;
      gol_cell cell = b[idx];
      cell_color c = col[cell.value];
      matrix.drawPixel(x, y, matrix.color565(c.red, c.green, c.blue));
    }
  }
};

void life(gol_cell *old, gol_cell *b, unsigned char max_val) {
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      int left = x == 0 ? WIDTH - 1 : x - 1;
      int right = x == WIDTH - 1 ? 0 : x + 1;
      int up = y == 0 ? HEIGHT - 1 : y - 1;
      int down = y == HEIGHT - 1 ? 0 : y + 1;

      // count the live neighbors.
      unsigned char old_val = old[x + y * WIDTH].value;
      unsigned char neighbors = (old[left + up * WIDTH].value > 0 ? 1 : 0) + (old[x + up * WIDTH].value > 0 ? 1 : 0) + (old[right + up * WIDTH].value > 0 ? 1 : 0) +
                                (old[left + y * WIDTH].value > 0 ? 1 : 0) +                                      (old[right + y * WIDTH].value > 0 ? 1 : 0) +
                                (old[left + down * WIDTH].value > 0 ? 1 : 0) + (old[x + down * WIDTH].value > 0 ? 1 : 0) + (old[right + down * WIDTH].value > 0 ? 1 : 0);
      if (neighbors == 3 && old_val == 0) {
        b[x + y * WIDTH].value = 1;
      } else if ((neighbors == 3 || neighbors == 2) && old_val > 0) {
        b[x + y * WIDTH].value = old_val == max_val ? old_val : old_val + 1;
      } else {
        b[x + y * WIDTH].value = 0;
      }
    }
  }
}

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

  while (!timeGrabber.init()) {
    Serial.println("Failed to start time grabber.");
    delay(1000);
  }
  timeGrabber.requestNtpPacket();

  pinMode(buttonUp.pin, INPUT_PULLUP);
  pinMode(buttonDown.pin, INPUT_PULLUP);

  board = (gol_cell *)malloc(HEIGHT*WIDTH * sizeof(gol_cell));
  old_board = (gol_cell *)malloc(HEIGHT*WIDTH * sizeof(gol_cell));
  palette = (cell_color *)malloc(NUM_COLORS * sizeof(cell_color));

  randomSeed(analogRead(a0));
  randomize_board(board);
  pick_palette_colors(palette, colors);
}

int lc = ITERATIONS;
bool showTime = true;
bool hasTime = false;
unsigned int textY = 2;
unsigned long lastTime;
unsigned long dt = 0;
void loop() {
  String s;
  if (!hasTime) {
    s = timeGrabber.getNtpResponse();
    if (s.length() != 0) {
      hasTime = true;
      lastTime = millis();
      timeGrabber.disconnect();
    }
  } else {
    // DEBT: This won't work forever since the return from millis() will eventually wrap back to 0. (50 days according
    // to the arduino.cc millis() documentation).
    unsigned long now = millis();
    dt += now - lastTime; // Keep a count of the milliseconds that are building up.
    unsigned long seconds = dt / 1000;
    dt -= seconds * 1000; // subtract off any seconds we took
    lastTime = now;

    timeGrabber.runningEpoch += seconds;
    if (showTime) {
      s = timeGrabber.getTime(timeGrabber.runningEpoch);
    }
  }
  
  if (showTime && s.length() != 0) {
    timeText.value = s;
  }
  gol_cell *tmp = old_board;
  old_board = board;
  board = tmp;

  if (is_button_pressed(&buttonUp)) {
//    lc = 0;
//    NUM_COLORS = NUM_COLORS < MAX_COLORS ? NUM_COLORS + 1 : NUM_COLORS;
    DELAY_MS += DELAY_INCREMENT;
  } else if (is_button_pressed(&buttonDown)) {
//    lc = 0;
//    NUM_COLORS = NUM_COLORS > MIN_COLORS ? NUM_COLORS - 1 : NUM_COLORS;
    DELAY_MS = DELAY_MS > 0 ? DELAY_MS - DELAY_INCREMENT : 0;
  }
  
  if (lc == 0) {
    lc = ITERATIONS;
    randomize_board(old_board);
    pick_palette_colors(palette, colors);
    showTime = !showTime;
    textY += 2;
    if (textY >= HEIGHT - Text::textHeight - 2) {
      textY = 2;
    }
  }
  
  draw_board(board, palette);
  if (showTime) {
    timeText.draw(2,textY, &matrix);
  }
  matrix.show();
  life(old_board, board, NUM_COLORS - 1);
  lc--;
  delay(DELAY_MS);
}
