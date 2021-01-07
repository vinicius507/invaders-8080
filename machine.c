#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "8080.h"

#define DEBUG getenv("DEBUG")

const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 224;

int main (int argc, char* argv[]) {
  State* state = InitState();
  ReadFile(state, argv[1]);
  
  int done;
  int instruction_num = 0;
  do {
    instruction_num++;
    printf("Instruction: %d\n", instruction_num);
    done = Emulate(state);
    if (DEBUG){
      printf("\ta %02X bc %02X%02X de %02X%02X hl %02X%02X pc %04X sp %04X memory[sp] %04X\n"
            , state->a
            , state->b
            , state->c
            , state->d
            , state->e
            , state->h
            , state->l
            , state->pc
            , state->sp
            , ((state->memory[state->sp+1]<<8) | (state->memory[state->sp]-2)));
      printf("\t%s %s %s %s %s %s\n"
            , state->cc.z ? "z" : "."
            , state->cc.s ? "s" : "."
            , state->cc.cy ? "cy" : "."
            , state->cc.p ? "p" : "."
            , state->cc.ac ? "ac" : "."
            , state->int_enable ? "i" : ".");
    }
  } while(done != 0);
  
  return 0;
}
