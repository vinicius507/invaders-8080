#ifndef __MACHINE_INVADERS__
#define __MACHINE_INVADERS__

#include <SDL2/SDL.h>
#include "8080.h"

#define DEBUG getenv("DEBUG")
#define HEIGHT 256
#define WIDTH 224 
#define FPS 60
#define CYCLES_PER_UPDATE 2000000 / FPS
#define TIME_PER_FRAME 1000/60
#define VRAM_ADDR 0x2400

typedef struct Display {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  uint8_t buffer[HEIGHT][WIDTH][4];
} Display;

typedef struct Machine {
  State* state;
  Display display;
} Machine;

#endif