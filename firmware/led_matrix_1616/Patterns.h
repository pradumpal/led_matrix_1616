#ifndef PATTERNS_H
#define PATTERNS_H

#include "Constants.h"
#include "Utils.h"


void scrollTextPattern() {
  strip.clear();
  int x = scrollX;
  for (int i = 0; i < (int)scrollText.length(); i++) {
    uint32_t col = randomScrollColor ? Wheel((millis() / 10) % 255) : scrollColor;
    x += drawChar(scrollText[i], x, scrollY, col);
  }
  strip.show();

  scrollX--;
  // Reset scroll when text has completely passed the screen
  int textWidth = scrollText.length() * (FONT_W + 1);
  if (scrollX < -textWidth) {
    scrollX = COLS;
  }
}

// ================= BASIC PATTERNS =================

void rainbowRain() {
  const int N = 10;
  static int r[N], c[N]; 
  static uint8_t col[N]; 
  static bool init = false;
  if (!init) { 
    for (int i = 0; i < N; i++) { 
      r[i] = random(ROWS); 
      c[i] = random(COLS); 
      col[i] = random(255); 
    } 
    init = true; 
  }
  strip.clear();
  for (int i = 0; i < N; i++) {
    strip.setPixelColor(getPhysicalLED(r[i] * COLS + c[i]), Wheel(col[i]));
    r[i]++; 
    if (r[i] >= ROWS) { 
      r[i] = 0; 
      c[i] = random(COLS); 
      col[i] = random(255); 
    }
    col[i] += 3;
  }
  strip.show();
}

void tetrisCascade() {
  static int board[ROWS][COLS];
  static int fallX = random(COLS);
  static int fallY = 0;
  static uint32_t blockColor = Wheel(random(255));

  for (int y = ROWS - 1; y >= 0; y--) {
    bool full = true;
    for (int x = 0; x < COLS; x++) if (board[y][x] == 0) full = false;
    if (full) {
      for (int ty = y; ty > 0; ty--)
        for (int x = 0; x < COLS; x++) board[ty][x] = board[ty - 1][x];
      for (int x = 0; x < COLS; x++) board[0][x] = 0;
    }
  }

  strip.clear();
  for (int y = 0; y < ROWS; y++)
    for (int x = 0; x < COLS; x++)
      if (board[y][x]) strip.setPixelColor(getPhysicalLED(y * COLS + x), board[y][x]);

  strip.setPixelColor(getPhysicalLED(fallY * COLS + fallX), blockColor);
  strip.show();

  fallY++;
  if (fallY >= ROWS - 1 || board[fallY + 1][fallX]) {
    board[fallY][fallX] = blockColor;
    fallY = 0;
    fallX = random(COLS);
    blockColor = Wheel(random(255));
  }
}

void milkyWayGalaxy() {
  static float t = 0;
  int cx = COLS / 2;
  int cy = ROWS / 2;
  strip.clear();
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      float dx = x - cx; float dy = y - cy;
      float dist = sqrt(dx * dx + dy * dy);
      float angle = atan2(dy, dx);
      float spiral = angle + dist * 0.45 - t;
      float arm = sin(spiral * 2.0);
      float bVal = constrain(max(0.0f, arm) * (1.0f - dist / 12.0f), 0.0, 1.0);
      strip.setPixelColor(getPhysicalLED(y * COLS + x), strip.Color(bVal * 80, bVal * 180, bVal * 255));
    }
  }
  strip.show();
  t += 0.05;
}

// ================= ADVANCED PATTERNS =================

void matrixRain() {
  static int head[COLS];
  static bool init = false;
  if (!init) { for (int i = 0; i < COLS; i++) head[i] = random(ROWS); init = true; }
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t c = strip.getPixelColor(getPhysicalLED(i));
    strip.setPixelColor(getPhysicalLED(i), c >> 1);
  }
  for (int c = 0; c < COLS; c++) {
    strip.setPixelColor(getPhysicalLED(head[c] * COLS + c), strip.Color(0, 255, 0));
    head[c] = (head[c] + 1) % ROWS;
  }
  strip.show();
}

void bouncingBalls() {
  const int MAX_BALLS = 12;
  static float x[MAX_BALLS], y[MAX_BALLS], vx[MAX_BALLS], vy[MAX_BALLS];
  static bool init = false;
  if (!init) {
    for (int i = 0; i < MAX_BALLS; i++) {
      x[i] = random(COLS); y[i] = random(ROWS);
      vx[i] = random(-15, 15) / 10.0; vy[i] = random(-15, 15) / 10.0;
    }
    init = true;
  }
  strip.clear();
  for (int i = 0; i < ballCount; i++) {
    x[i] += vx[i]; y[i] += vy[i];
    if (x[i] <= 0 || x[i] >= COLS - 1) vx[i] = -vx[i];
    if (y[i] <= 0 || y[i] >= ROWS - 1) vy[i] = -vy[i];
    strip.setPixelColor(getPhysicalLED(constrain((int)y[i], 0, ROWS - 1) * COLS + constrain((int)x[i], 0, COLS - 1)), Wheel(i * 35));
  }
  strip.show();
}

void lavaLamp() {
  static uint16_t t = 0;
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      int v = sin((x + t) * 0.2) * 60 + sin((y + t) * 0.3) * 60 + 120;
      strip.setPixelColor(getPhysicalLED(y * COLS + x), Wheel(constrain(v, 0, 255)));
    }
  }
  strip.show(); t++;
}

void expandingRings() {
  static int radius = 0;
  static uint8_t hue = 0;
  int cx = COLS / 2; int cy = ROWS / 2;
  strip.clear();
  uint32_t drawColor = ringAutoColor ? Wheel(hue += 4) : ringColor;
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      int d = sqrt(pow(x - cx, 2) + pow(y - cy, 2));
      if (abs(d - radius) <= 1) strip.setPixelColor(getPhysicalLED(y * COLS + x), drawColor);
    }
  }
  strip.show();
  if (++radius > max(cx, cy)) radius = 0;
}

void auroraWave() {
  static uint16_t t = 0;
  for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(getPhysicalLED(i), Wheel(sin((i * 0.15) + (t * 0.05)) * 127 + 128));
  strip.show(); t++;
}

void fireFlicker() {
  for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(getPhysicalLED(i), strip.Color(random(180, 255), random(30, 120), 0));
  strip.show();
}

void glitterRain() {
  strip.clear();
  for (int i = 0; i < 25; i++) strip.setPixelColor(getPhysicalLED(random(NUM_LEDS)), strip.Color(255, 255, 255));
  strip.show();
}

void oceanRipple() {
  static float phase = 0; static float tide = ROWS - 1; static bool goingUp = true;
  strip.clear();
  for (int x = 0; x < COLS; x++) {
    float waterLine = tide + sin((x / 5.0) + phase) * 2.0;
    for (int y = 0; y < ROWS; y++) {
      if (y >= waterLine) {
        int depth = y - waterLine;
        strip.setPixelColor(getPhysicalLED(y * COLS + x), strip.Color(0, constrain(60 + depth * 5, 40, 160), constrain(180 + depth * 10, 150, 255)));
      }
      float edgeDist = abs(y - waterLine);
      if (edgeDist < 1.2) {
        int foam = constrain(255 - edgeDist * 180, 0, 255);
        strip.setPixelColor(getPhysicalLED(y * COLS + x), strip.Color(foam, foam, foam));
      }
    }
  }
  strip.show();
  if (goingUp) { tide -= 0.04; if (tide <= 3) goingUp = false; }
  else { tide += 0.04; if (tide >= ROWS - 1) goingUp = true; }
  phase += 0.1;
}

void meteorTrail() {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t c = strip.getPixelColor(getPhysicalLED(i));
    strip.setPixelColor(getPhysicalLED(i), c >> 1);
  }
  static int pos = 0;
  strip.setPixelColor(getPhysicalLED(pos), strip.Color(255, 80, 50));
  strip.show();
  pos = (pos + 1) % NUM_LEDS;
}

void fireworkShow() {
  const int P = 25;
  static float px[P], py[P], vx[P], vy[P];
  static int rocketY = ROWS - 1, rocketX = COLS / 2, blastY = random(2, ROWS / 2), life = 0;
  static bool exploding = false, colorful = true;
  static uint32_t baseColor;

  if (!exploding) {
    strip.clear();
    strip.setPixelColor(getPhysicalLED(rocketY * COLS + rocketX), strip.Color(255, 255, 255));
    strip.show();
    if (--rocketY <= blastY) {
      exploding = true; colorful = random(2); baseColor = Wheel(random(255));
      for (int i = 0; i < P; i++) {
        px[i] = rocketX; py[i] = rocketY;
        float angle = random(360) * DEG_TO_RAD; float speed = random(5, 15) / 10.0;
        vx[i] = cos(angle) * speed; vy[i] = sin(angle) * speed;
      }
      life = 28;
    }
    return;
  }
  strip.clear();
  for (int i = 0; i < P; i++) {
    px[i] += vx[i]; py[i] += vy[i]; vy[i] += 0.04;
    strip.setPixelColor(getPhysicalLED(constrain(py[i], 0, ROWS - 1) * COLS + constrain(px[i], 0, COLS - 1)), colorful ? Wheel((i * 30 + millis()) % 255) : baseColor);
  }
  strip.show();
  if (--life <= 0) { exploding = false; rocketY = ROWS - 1; rocketX = random(COLS); blastY = random(2, ROWS / 2); }
}

void colorPulse() {
  static int b = 0, dir = 1;
  b += dir * 6; if (b > 255 || b < 0) dir = -dir;
  for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(getPhysicalLED(i), strip.Color(b, 0, 255 - b));
  strip.show();
}

void spiralSpin() {
  static int off = 0;
  for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(getPhysicalLED(i), Wheel((i * 7 + off) % 255));
  strip.show(); off += 4;
}

void vortexSpin() {
  static float t = 0;
  int cx = COLS / 2; int cy = ROWS / 2;
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      float dx = x - cx; float dy = y - cy;
      strip.setPixelColor(getPhysicalLED(y * COLS + x), Wheel((int)(atan2(dy, dx) * 40 + sqrt(dx * dx + dy * dy) * 25 + t * 50) % 255));
    }
  }
  strip.show(); t += 0.05;
}

void galaxyTwinkle() {
  strip.setPixelColor(getPhysicalLED(random(NUM_LEDS)), Wheel(random(255)));
  strip.show();
}

// ================= PATTERN ENGINE =================

void runPattern(int p) {
  switch (p) {
    case 2:  scrollTextPattern(); break;
    case 3:  rainbowRain(); break;
    case 4:  fireworkShow(); break;
    case 5:  tetrisCascade(); break;
    case 6:  auroraWave(); break;
    case 7:  fireFlicker(); break;
    case 8:  glitterRain(); break;
    case 9:  oceanRipple(); break;
    case 10: meteorTrail(); break;
    case 11: milkyWayGalaxy(); break;
    case 12: colorPulse(); break;
    case 13: spiralSpin(); break;
    case 14: vortexSpin(); break;
    case 15: galaxyTwinkle(); break;
    case 16: matrixRain(); break;
    case 17: bouncingBalls(); break;
    case 18: lavaLamp(); break;
    case 19: expandingRings(); break; // Added missing case

    default: strip.clear(); strip.show(); break;
  }
}

void showIPonMatrix() {
  if (WiFi.status() == WL_CONNECTED) {
    scrollText = "IP: " + WiFi.localIP().toString();
    scrollX = COLS;
    currentPattern = 2;
  }
}

#endif
