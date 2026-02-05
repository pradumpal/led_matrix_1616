#ifndef ROBOT_H
#define ROBOT_H

#include "Constants.h"
#include "Utils.h"

static int robotExpression = 0; // 0:Neutral, 1:Happy, 2:Angry, 3:Surprise, 4:Heart
static unsigned long lastBlinkTime = 0;
static bool isBlinking = false;
static int blinkDuration = 150;
static int nextBlinkInterval = 3000;

// Helper to draw a single eye
void drawEye(int startX, int startY, int w, int h, uint32_t color, int type) {
  if (isBlinking) {
    // Draw a flat line for blinking
    for (int x = startX; x < startX + w; x++) {
      strip.setPixelColor(getPhysicalLED((startY + h / 2) * 16 + x), color);
    }
    return;
  }

  for (int y = startY; y < startY + h; y++) {
    for (int x = startX; x < startX + w; x++) {
      bool paint = false;
      switch (type) {
        case 0: paint = true; break; // Neutral Square
        case 1: // Happy (Arch)
          if (!(y == startY + h - 1 && (x == startX || x == startX + w - 1))) paint = true;
          if (y < startY + h / 2) paint = false;
          break;
        case 2: // Angry (Slant)
          paint = true;
          if (startX < 8) { if (y == startY && x == startX) paint = false; } // Left eye
          else { if (y == startY && x == startX + w - 1) paint = false; }    // Right eye
          break;
        case 3: // Surprise (Circle)
          if (!((x == startX || x == startX + w - 1) && (y == startY || y == startY + h - 1))) paint = true;
          break;
        case 4: // Hearts
          if (!((y == startY && (x == startX || x == startX + w - 1)) || (y == startY + h - 1 && (x != startX + w / 2)))) paint = true;
          break;
      }
      if (paint) strip.setPixelColor(getPhysicalLED(y * 16 + x), color);
    }
  }
}

void runRobotMode() {
  // Blink Logic
  if (!isBlinking && millis() - lastBlinkTime > nextBlinkInterval) {
    isBlinking = true;
    lastBlinkTime = millis();
  }
  if (isBlinking && millis() - lastBlinkTime > blinkDuration) {
    isBlinking = false;
    lastBlinkTime = millis();
    nextBlinkInterval = random(2000, 5000);
  }

  strip.clear();
  uint32_t eyeColor = scrollColor; // Uses the text color from Web UI

  // Draw Left Eye (starting at x=2, y=5, size 5x5)
  drawEye(2, 5, 5, 5, eyeColor, robotExpression);
  
  // Draw Right Eye (starting at x=9, y=5, size 5x5)
  drawEye(9, 5, 5, 5, eyeColor, robotExpression);
  
  strip.show();
}

#endif