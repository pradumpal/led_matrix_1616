#ifndef GAMES_H
#define GAMES_H

#include "Constants.h"
#include "Utils.h"

// --- Global Game State Structure ---
struct GPoint { int x; int y; };

// --- SNAKE DATA ---
static GPoint _snake[256];
static int _snakeLen;
static GPoint _food;

// --- FLAPPY BIRD DATA ---
static float birdY, birdV;
static int pipeX, gapY;

// --- BREAKOUT DATA ---
static float bX, bY, bVX, bVY;
static int padX;
static bool bricks[32];

// --- SPACE INVADERS DATA ---
static int shipX = 8;
static bool invaders[24]; 
static int invX = 0, invY = 0, invDir = 1;
static int bulletX = -1, bulletY = -1;

// --- DINO RUN DATA ---
static float dinoY = 14, dinoV = 0;
static int obX = 15;

// --- GAME LOGIC ---

void runSnake() {
  if(!gameRunning) { _snakeLen = 4; for(int i=0; i<4; i++) _snake[i] = {8-i, 8}; _food = {random(16), random(16)}; dirX = 1; dirY = 0; gameRunning = true; }
  if(millis() - lastGameTick < 150) return;
  lastGameTick = millis();
  for(int i = _snakeLen - 1; i > 0; i--) _snake[i] = _snake[i - 1];
  _snake[0].x += dirX; _snake[0].y += dirY;
  if(_snake[0].x < 0) _snake[0].x = 15; if(_snake[0].x > 15) _snake[0].x = 0;
  if(_snake[0].y < 0) _snake[0].y = 15; if(_snake[0].y > 15) _snake[0].y = 0;
  if(_snake[0].x == _food.x && _snake[0].y == _food.y) { _snakeLen = min(_snakeLen + 1, 255); _food = {random(16), random(16)}; }
  for(int i=1; i<_snakeLen; i++) if(_snake[0].x==_snake[i].x && _snake[0].y==_snake[i].y) gameRunning=false;
  strip.clear();
  strip.setPixelColor(getPhysicalLED(_food.y*16+_food.x), 0xFF0000);
  for(int i=0; i<_snakeLen; i++) strip.setPixelColor(getPhysicalLED(_snake[i].y*16+_snake[i].x), 0x00FF00);
  strip.show();
}

void runFlappy() {
  if(!gameRunning) { birdY=8; birdV=0; pipeX=15; gapY=random(2, 10); gameRunning=true; }
  if(millis() - lastGameTick < 70) return;
  lastGameTick = millis();
  birdV += 0.15; birdY += birdV; pipeX--;
  if(pipeX < 0) { pipeX = 15; gapY = random(2, 10); }
  if(birdY < 0 || birdY > 15 || (pipeX == 3 && (birdY < gapY || birdY > gapY+4))) { gameRunning=false; return; }
  strip.clear();
  for(int y=0; y<16; y++) if(y < gapY || y > gapY+4) strip.setPixelColor(getPhysicalLED(y*16+pipeX), 0x0000FF);
  strip.setPixelColor(getPhysicalLED((int)birdY*16+3), 0xFFFF00);
  strip.show();
}

void runBreakout() {
  if(!gameRunning) { bX=8; bY=13; bVX=0.4; bVY=-0.4; padX=6; for(int i=0;i<32;i++) bricks[i]=true; gameRunning=true; }
  if(millis() - lastGameTick < 50) return;
  lastGameTick = millis();
  bX += bVX; bY += bVY;
  if(bX <= 0 || bX >= 15) bVX *= -1;
  if(bY <= 0) bVY *= -1;
  if(bY >= 14 && bX >= padX && bX <= padX + 4) bVY *= -1;
  if(bY >= 15) { gameRunning = false; return; }
  if(bY < 2) { int idx = (int)bY * 16 + (int)bX; if(idx >=0 && idx < 32 && bricks[idx]) { bricks[idx] = false; bVY *= -1; } }
  strip.clear();
  for(int i=0; i<32; i++) if(bricks[i]) strip.setPixelColor(getPhysicalLED(i), Wheel(i*8));
  for(int x=0; x<4; x++) strip.setPixelColor(getPhysicalLED(15*16 + padX + x), 0xFFFFFF);
  strip.setPixelColor(getPhysicalLED((int)bY*16+(int)bX), 0xFFFFFF);
  strip.show();
}

void runSpaceInvaders() {
  if(!gameRunning) { for(int i=0; i<24; i++) invaders[i]=true; invX=0; invY=0; shipX=8; bulletY=-1; gameRunning=true; }
  if(millis() - lastGameTick < 80) return;
  lastGameTick = millis();
  static int moveC = 0;
  if(++moveC > 6) { invX += invDir; if(invX > 7 || invX < 0) { invDir *= -1; invY++; } moveC = 0; }
  if(invY > 10) gameRunning = false;
  if(bulletY >= 0) {
    bulletY--;
    if(bulletY < 3) {
      int ix = bulletX - invX; int iy = bulletY - invY;
      if(ix >=0 && ix < 8 && iy >=0 && iy < 3 && invaders[iy*8+ix]) { invaders[iy*8+ix] = false; bulletY = -1; }
    } else if(bulletY < 0) bulletY = -1;
  }
  strip.clear();
  for(int i=0; i<24; i++) if(invaders[i]) strip.setPixelColor(getPhysicalLED((invY+i/8)*16 + (invX+i%8)), 0xFF00FF);
  if(bulletY >= 0) strip.setPixelColor(getPhysicalLED(bulletY*16 + bulletX), 0xFFFFFF);
  strip.setPixelColor(getPhysicalLED(15*16 + shipX), 0x00FFFF);
  strip.show();
}

void runDino() {
  if(!gameRunning) { dinoY=14; dinoV=0; obX=15; gameRunning=true; }
  if(millis() - lastGameTick < 50) return;
  lastGameTick = millis();
  dinoV += 0.25; dinoY += dinoV; if(dinoY > 14) { dinoY = 14; dinoV = 0; }
  obX--; if(obX < 0) obX = 15;
  if(obX == 2 && dinoY > 12) { gameRunning = false; return; }
  strip.clear();
  for(int x=0; x<16; x++) strip.setPixelColor(getPhysicalLED(15*16+x), 0x222222); 
  strip.setPixelColor(getPhysicalLED(14*16+obX), 0xFF0000); 
  strip.setPixelColor(getPhysicalLED((int)dinoY * 16 + 2), 0x00FF00); 
  strip.show();
}

void runSelectedGame() {
  switch(selectedGame) {
    case 0: runSnake(); break;
    case 1: runFlappy(); break;
    case 2: runBreakout(); break;
    case 3: runSpaceInvaders(); break;
    case 4: runDino(); break;
  }
}

void handleGameInput(String d) {
  if(selectedGame == 0) {
    if(d=="U"){dirX=0;dirY=-1;} else if(d=="D"){dirX=0;dirY=1;}
    else if(d=="L"){dirX=-1;dirY=0;} else if(d=="R"){dirX=1;dirY=0;}
  } else if(selectedGame == 1 || selectedGame == 4) { 
    if(selectedGame == 1) birdV = -1.1; else if(dinoY >= 14) dinoV = -1.3;
  } else if(selectedGame == 2) {
    if(d=="L") padX = max(0, padX-1); else if(d=="R") padX = min(12, padX+1);
  } else if(selectedGame == 3) {
    if(d=="L") shipX = max(0, shipX-1); else if(d=="R") shipX = min(15, shipX+1);
    else if(d=="U" && bulletY < 0) { bulletX = shipX; bulletY = 14; }
  }
}

#endif
