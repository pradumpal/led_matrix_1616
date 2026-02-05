#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WebServer.h>
#include <Preferences.h>

#define PIN 19
#define NUM_LEDS 256
#define ROWS 16
#define COLS 16
#define FONT_W 3
#define FONT_H 5

extern Adafruit_NeoPixel strip;
extern WebServer server;
extern Preferences prefs;

extern int brightness;
extern int speedValue;
extern int currentPattern;
extern int uiMode;

extern String scrollText;
extern int scrollX;
extern int scrollY;
extern uint32_t scrollColor;
extern bool randomScrollColor;
extern uint32_t ringColor;
extern bool ringAutoColor;
extern int ballCount;

extern int dirX;
extern int dirY;
extern bool gameRunning;
extern unsigned long lastGameTick;
extern int gameSpeed;

// --- NEW: Unified Game Selector ---
extern int selectedGame; // 0: Snake, 1: Flappy Bird, 2: Breakout

#endif
