#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "8080.h"

#define DEBUG getenv("DEBUG")

const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 224;

typedef struct Display {
  SDL_Window* window;
  SDL_Surface* screen;
} Display;

typedef struct Machine {
  State* state;
  Display display;
} Machine;

void screenInit(Machine* machine) {
}

Machine* InitMachine() {
  Machine* machine = calloc(1, sizeof(Machine));

  machine->state = InitState();
  machine->display.window = NULL;
  machine->display.screen = NULL;

  return machine;
}

int main (int argc, char* argv[]) {
  Machine* machine = InitMachine();
  ReadFile(machine->state, argv[1]); 
  
  int instruction_num = 0;
  int quit = 0;
  SDL_Event e;
  while(!quit) {
    int cycles = Emulate(machine->state);
    while(SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        quit = 1;
    }

    if (DEBUG){
      instruction_num++;
      printf("Instruction: %d\n", instruction_num);
      printf("\ta %02X bc %02X%02X de %02X%02X hl %02X%02X pc %04X sp %04X memory[sp] %04X\n"
            , machine->state->a
            , machine->state->b
            , machine->state->c
            , machine->state->d
            , machine->state->e
            , machine->state->h
            , machine->state->l
            , machine->state->pc
            , machine->state->sp
            , ((machine->state->memory[machine->state->sp+1]<<8) | (machine->state->memory[machine->state->sp]-2)));
      printf("\t%s %s %s %s %s %s\n"
            , machine->state->cc.z ? "z" : "."
            , machine->state->cc.s ? "s" : "."
            , machine->state->cc.cy ? "cy" : "."
            , machine->state->cc.p ? "p" : "."
            , machine->state->cc.ac ? "ac" : "."
            , machine->state->int_enable ? "i" : ".");
    }
  }
  
  SDL_FreeSurface(machine->display.screen);
  machine->display.screen = NULL;
  SDL_DestroyWindow(machine->display.window);
  SDL_Quit();
  
  return 0;
}
