#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiManager.h>
#include <time.h> // Built-in ESP32 time library

// Include our modular headers
#include "Constants.h"
#include "Utils.h"
#include "Patterns.h"
#include "Games.h"
#include "Clock.h"  // Added to fix the 'uiMode 1' error
#include "Robot.h"  // Added for Robot Expression Mode
#include "WebUI.h"

// --- Global Object DEFINITIONS ---
Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
WebServer server(80);
Preferences prefs;
WiFiManager wm;
int selectedGame = 0;

// --- Global State DEFINITIONS ---
int brightness = 40;
int speedValue = 50;
int currentPattern = 0;
int uiMode = 0;
unsigned long lastFrameTime = 0;
bool lastWifiConnected = false;

// --- Pattern & Text State DEFINITIONS ---
String scrollText = "Led Matrix by Pradum Pal ";
int scrollX = COLS;
int scrollY = 5;
uint32_t scrollColor = 0x00FFFF;
bool randomScrollColor = false;
uint32_t ringColor = 0x00FFFF;
bool ringAutoColor = false;
int ballCount = 6;

// --- Game State DEFINITIONS ---
int dirX = 1;
int dirY = 0;
bool gameRunning = false;
unsigned long lastGameTick = 0;
int gameSpeed = 180;

void setup() {
  Serial.begin(115200);
  prefs.begin("ledMatrix", false);

  // Load saved values
  currentPattern = prefs.getInt("pattern", 2);
  brightness     = prefs.getInt("bright", 40);
  speedValue     = prefs.getInt("speed", 50);
  uiMode         = prefs.getInt("mode", 0);

  strip.begin();
  strip.setBrightness(brightness);
  strip.show();

  // Initialize NTP Time (India Offset: 19800 seconds / 5:30 hours)
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov");

  wm.setConfigPortalBlocking(false);
  wm.autoConnect("LED-MATRIX-SETUP");

  setupWebHandlers();
  server.begin();
  Serial.println("System Ready.");
}

void loop() {
  wm.process();
  server.handleClient();
  
  bool nowConnected = (WiFi.status() == WL_CONNECTED);
  if(nowConnected && !lastWifiConnected) showIPonMatrix();
  lastWifiConnected = nowConnected;

  unsigned long interval = map(speedValue, 0, 100, 120, 5);

  if(millis() - lastFrameTime >= interval) {
    lastFrameTime = millis();
    
    // --- MODE SWITCHING LOGIC ---
    if(uiMode == 0) {
      // DISPLAY MODE: Run animations only if pattern >= 2
      if(currentPattern >= 2) {
        runPattern(currentPattern);
      }
    } 
    else if(uiMode == 1) {
      // CLOCK MODE: Defined in Clock.h
      displayClock(); 
    } 
    else if(uiMode == 2) {
      // UNIFIED GAME SYSTEM: Defined in Games.h
      runSelectedGame();
    }
    else if(uiMode == 3) {
      // ROBOT MODE: Defined in Robot.h
      runRobotMode();
    }
  }
}
