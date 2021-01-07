#ifndef __8080_H__
#define __8080_H__

#include <stdint.h>

      /* state->cc.cy = (0x01 == (psw & 0x01)); */
      /* state->cc.p = (0x04 == (psw & 0x04)); */
      /* state->cc.ac = (0x10 == (psw & 0x10)); */
      /* state->cc.z = (0x40 == (psw & 0x40)); */
      /* state->cc.s = (0x80 == (psw & 0x80)); */
typedef struct ConditionCodes {
  uint8_t cy : 1;
  uint8_t p : 1;
  uint8_t ac : 1;
  uint8_t z : 1;
  uint8_t s : 1;
  uint8_t pad : 3;
} ConditionCodes;

typedef struct State {
  uint8_t a;
  uint8_t b;
  uint8_t c;
  uint8_t d;
  uint8_t e;
  uint8_t h;
  uint8_t l;
  uint16_t sp;
  uint16_t pc;
  uint8_t *memory;
  ConditionCodes cc;
  uint8_t int_enable;
} State;

int Emulate(State *state);

void ReadFile(State *state, char *filename);

State *InitState();

void GenerateInterrupt(State *state, int interrupt_num);

#endif