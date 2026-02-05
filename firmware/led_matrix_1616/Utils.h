#ifndef UTILS_H
#define UTILS_H

#include "Constants.h"

// Physical Mapping for 16x16 Matrix (Snake/Zig-zag pattern handling)
int getPhysicalLED(int step) {
  int row = step / COLS;
  int col = step % COLS;
  int leftBase, rightBase;
  if (row < 8) {
    leftBase  = row * 8;
    rightBase = 192 + row * 8;
  } else {
    leftBase  = 64 + (row - 8) * 8;
    rightBase = 128 + (row - 8) * 8;
  }
  return (col < 8) ? (leftBase + col) : (rightBase + col - 8);
}

// Color Wheel Utility for Rainbow effects
uint32_t Wheel(byte pos) {
  pos = 255 - pos;
  if (pos < 85) return strip.Color(255 - pos * 3, 0, pos * 3);
  if (pos < 170) { 
    pos -= 85; 
    return strip.Color(0, pos * 3, 255 - pos * 3); 
  }
  pos -= 170;
  return strip.Color(pos * 3, 255 - pos * 3, 0);
}

// ================= 5x3 DIGIT FONT =================
// Stored in PROGMEM to save RAM
const uint8_t font3x5[][FONT_H][FONT_W] PROGMEM = {
  // ---------- 0–9 ----------
  {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}}, // 0
  {{0,1,0},{1,1,0},{0,1,0},{0,1,0},{1,1,1}}, // 1
  {{1,1,1},{0,0,1},{1,1,1},{1,0,0},{1,1,1}}, // 2
  {{1,1,1},{0,0,1},{0,1,1},{0,0,1},{1,1,1}}, // 3
  {{1,0,1},{1,0,1},{1,1,1},{0,0,1},{0,0,1}}, // 4
  {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // 5
  {{1,1,1},{1,0,0},{1,1,1},{1,0,1},{1,1,1}}, // 6
  {{1,1,1},{0,0,1},{0,1,0},{0,1,0},{0,1,0}}, // 7
  {{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,1,1}}, // 8
  {{1,1,1},{1,0,1},{1,1,1},{0,0,1},{1,1,1}}, // 9

  // ---------- Space ----------
  {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}}, // ' ' (Index 10)

  // ---------- Symbols ----------
  {{0,0,0},{0,0,0},{0,1,0},{0,0,0},{0,0,0}}, // . (Index 11)
  {{0,1,0},{0,0,0},{0,1,0},{0,0,0},{0,0,0}}, // : (Index 12)
  {{0,0,0},{0,0,0},{1,1,1},{0,0,0},{0,0,0}}, // - (Index 13)
  {{0,1,0},{1,1,1},{0,1,0},{0,0,0},{0,0,0}}, // +
  {{1,0,1},{0,1,0},{1,0,1},{0,0,0},{0,0,0}}, // *
  {{0,0,1},{0,1,0},{1,0,0},{0,0,0},{0,0,0}}, // /
  {{1,0,1},{0,0,1},{0,1,0},{1,0,0},{1,0,1}}, // %
  {{0,1,0},{0,1,0},{0,1,0},{0,0,0},{0,1,0}}, // !
  {{1,1,1},{0,0,1},{0,1,0},{0,0,0},{0,1,0}}, // ?
  {{1,0,1},{1,1,1},{1,0,1},{1,1,1},{1,0,1}}, // #
  {{1,1,1},{1,0,1},{1,1,1},{1,0,0},{0,1,1}}, // @ (Index 21)

  // ---------- A–Z ----------
  {{0,1,0},{1,0,1},{1,1,1},{1,0,1},{1,0,1}}, // A (Index 22)
  {{1,1,0},{1,0,1},{1,1,0},{1,0,1},{1,1,0}}, // B
  {{0,1,1},{1,0,0},{1,0,0},{1,0,0},{0,1,1}}, // C
  {{1,1,0},{1,0,1},{1,0,1},{1,0,1},{1,1,0}}, // D
  {{1,1,1},{1,0,0},{1,1,0},{1,0,0},{1,1,1}}, // E
  {{1,1,1},{1,0,0},{1,1,0},{1,0,0},{1,0,0}}, // F
  {{0,1,1},{1,0,0},{1,0,1},{1,0,1},{0,1,1}}, // G
  {{1,0,1},{1,0,1},{1,1,1},{1,0,1},{1,0,1}}, // H
  {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{1,1,1}}, // I
  {{0,0,1},{0,0,1},{0,0,1},{1,0,1},{0,1,0}}, // J
  {{1,0,1},{1,1,0},{1,0,0},{1,1,0},{1,0,1}}, // K
  {{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1}}, // L
  {{1,0,1},{1,1,1},{1,0,1},{1,0,1},{1,0,1}}, // M
  {{1,0,1},{1,1,1},{1,1,1},{1,0,1},{1,0,1}}, // N
  {{0,1,0},{1,0,1},{1,0,1},{1,0,1},{0,1,0}}, // O
  {{1,1,0},{1,0,1},{1,1,0},{1,0,0},{1,0,0}}, // P
  {{0,1,0},{1,0,1},{1,0,1},{0,1,0},{0,0,1}}, // Q
  {{1,1,0},{1,0,1},{1,1,0},{1,1,0},{1,0,1}}, // R
  {{0,1,1},{1,0,0},{0,1,0},{0,0,1},{1,1,0}}, // S
  {{1,1,1},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // T
  {{1,0,1},{1,0,1},{1,0,1},{1,0,1},{0,1,0}}, // U
  {{1,0,1},{1,0,1},{1,0,1},{0,1,0},{0,1,0}}, // V
  {{1,0,1},{1,0,1},{1,0,1},{1,1,1},{1,0,1}}, // W
  {{1,0,1},{0,1,0},{0,1,0},{0,1,0},{1,0,1}}, // X
  {{1,0,1},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // Y
  {{1,1,1},{0,0,1},{0,1,0},{1,0,0},{1,1,1}}, // Z
};

// Map characters to font indices
int getFontIndex(char ch) {
  if (ch >= '0' && ch <= '9') return ch - '0';
  if (ch == ' ') return 10;
  if (ch == '.') return 11;
  if (ch == ':') return 12;
  if (ch == '-') return 13;
  if (ch == '+') return 14;
  if (ch == '*') return 15;
  if (ch == '/') return 16;
  if (ch == '%') return 17;
  if (ch == '!') return 18;
  if (ch == '?') return 19;
  if (ch == '#') return 20;
  if (ch == '@') return 21;

  if (ch >= 'A' && ch <= 'Z') return 22 + (ch - 'A');
  if (ch >= 'a' && ch <= 'z') return 22 + (ch - 'a');

  return -1;
}

// Low-level drawing function for font indices
void drawSymbol(int index, int startX, int startY, uint32_t color) {
  if (index < 0 || index >= (int)(sizeof(font3x5) / sizeof(font3x5[0])))
    return;

  for (int r = 0; r < FONT_H; r++) {
    for (int c = 0; c < FONT_W; c++) {
      int x = startX + c;
      int y = startY + r;

      if (x < 0 || x >= COLS || y < 0 || y >= ROWS) continue;

      int ledIndex = getPhysicalLED(y * COLS + x);

      // Read pixel from PROGMEM and set LED
      if (pgm_read_byte(&font3x5[index][r][c]))
        strip.setPixelColor(ledIndex, color);
      else
        strip.setPixelColor(ledIndex, 0); // Clear background for readability
    }
  }
}

// High-level drawing function for single characters
// Returns the horizontal space used (width + gap)
int drawChar(char ch, int x, int y, uint32_t color) {
  int index = getFontIndex(ch);
  if (index < 0) return 0;
  
  drawSymbol(index, x, y, color);
  return FONT_W + 1; // Width of char + 1px spacing
}

#endif
