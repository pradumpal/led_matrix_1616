#ifndef CLOCK_H
#define CLOCK_H

#include "Constants.h"
#include "Utils.h"
#include <time.h>

/**
 * Fetches the current time from the ESP32 system (synced via NTP)
 * and renders it on the 16x16 matrix using the 3x5 font.
 */
void displayClock() {
  struct tm timeinfo;
  
  // Attempt to get the local time from the system
  if (!getLocalTime(&timeinfo)) {
    // If the time has not synchronized yet, display a "Sync" message
    strip.clear();
    drawChar('S', 1, 5, 0xFF0000); // Red color for sync warning
    drawChar('y', 5, 5, 0xFF0000);
    drawChar('n', 9, 5, 0xFF0000);
    drawChar('c', 13, 5, 0xFF0000);
    strip.show();
    return;
  }

  // Format the time as HH:MM
  char timeStr[6]; 
  strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);

  strip.clear();

  // Draw the time characters using the scrollColor selected in the Web UI
  // Positioning for 16x16 grid: 
  // H(0,5), H(4,5), :(7,5), M(9,5), M(13,5)
  drawChar(timeStr[0], 0, 5, scrollColor);
  drawChar(timeStr[1], 4, 5, scrollColor);
  
  // Blinking colon effect (on for even seconds, off for odd)
  if (timeinfo.tm_sec % 2 == 0) {
    drawChar(':', 7, 5, scrollColor);
  }

  drawChar(timeStr[3], 9, 5, scrollColor);
  drawChar(timeStr[4], 13, 5, scrollColor);
  
  strip.show();
}

#endif