#include "8080.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Disassemble(unsigned char *codebuffer, int pc) {
  unsigned char *code = &codebuffer[pc];
  int opbytes = 1;
  printf("%04x ", pc);
  switch (*code) {
   // group BC
   case 0x00: printf("NOP"); break;
   case 0x01: printf("LXI  B,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
   case 0x02: printf("STAX B"); break;
   case 0x03: printf("INX  B"); break;
   case 0x04: printf("INR  B"); break;
   case 0x05: printf("DCR  B"); break;
   case 0x06: printf("MVI  B,#$%02x", code[1]); opbytes = 2; break;
   case 0x07: printf("RLC"); break;
   case 0x08: printf("NOP"); break;
   case 0x09: printf("DAD  B"); break;
   case 0x0a: printf("LDAX B"); break;
   case 0x0b: printf("DCX  B"); break;
   case 0x0c: printf("INR  C"); break;
   case 0x0d: printf("DCR  C"); break;
   case 0x0e: printf("MVI  C,#$%02X", code[1]); opbytes = 2; break;
   case 0x0f: printf("RRC"); break;
   // group DE
   case 0x10: printf("NOP"); break;
   case 0x11: printf("LXI  D,$%02x%02x", code[2], code[1]); opbytes = 3; break;
   case 0x12: printf("STAX D"); break;
   case 0x13: printf("INX  D"); break;
   case 0x14: printf("INR  D"); break;
   case 0x15: printf("DCR  D"); break;
   case 0x16: printf("MVI  D,#$%02X", code[1]); opbytes = 2; break;
   case 0x17: printf("RAL"); break;
   case 0x18: printf("NOP"); break;
   case 0x19: printf("DAD  D"); break;
   case 0x1a: printf("LDAX D"); break;
   case 0x1b: printf("DCX  D"); break;
   case 0x1c: printf("INR  E"); break;
   case 0x1d: printf("DCR  E"); break;
   case 0x1e: printf("MVI  E,#$%02X", code[1]); opbytes = 2; break;
   case 0x1f: printf("RAR"); break;
   // group HL
   case 0x20: printf("NOP"); break;
   case 0x21: printf("LXI  H,$%02x%02x", code[2], code[1]); opbytes = 3; break;
   case 0x22: printf("SHLD D,$%02x%02x", code[2], code[1]); opbytes = 3; break;
   case 0x23: printf("INX  H"); break;
   case 0x24: printf("INR  H"); break;
   case 0x25: printf("DCR  H"); break;
   case 0x26: printf("MVI  H,#$%02X", code[1]); opbytes = 2; break;
   case 0x27: printf("DAA"); break;
   case 0x28: printf("NOP"); break;
   case 0x29: printf("DAD  H"); break;
   case 0x2a: printf("LHLD H,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
   case 0x2b: printf("DCX  H"); break;
   case 0x2c: printf("INR  L"); break;
   case 0x2d: printf("DCR  L"); break;
   case 0x2e: printf("MVI  L,#$%02X", code[1]); opbytes = 2; break;
   case 0x2f: printf("CMA"); break;
              
   case 0x30: printf("NOP"); break;
   case 0x31: printf("LXI  SP,#$%02x%02x", code[2], code[1]); opbytes=3; break;
   case 0x32: printf("STA  $%02x%02x", code[2], code[1]); opbytes=3; break;
   case 0x33: printf("INX  SP"); break;
   case 0x34: printf("INR  M"); break;
   case 0x35: printf("DCR  M"); break;
   case 0x36: printf("MVI  M,#$%02x", code[1]); opbytes=2; break;
   case 0x37: printf("STC"); break;
   case 0x38: printf("NOP"); break;
   case 0x39: printf("DAD  SP"); break;
   case 0x3a: printf("LDA  $%02x%02x", code[2], code[1]); opbytes=3; break;
   case 0x3b: printf("DCX  SP"); break;
   case 0x3c: printf("INR  A"); break;
   case 0x3d: printf("DCR  A"); break;
   case 0x3e: printf("MVI  A,#$%02x", code[1]); opbytes = 2; break;
   case 0x3f: printf("CMC"); break;   
              
   case 0x40: printf("MOV  B,B"); break;
   case 0x41: printf("MOV  B,C"); break;
   case 0x42: printf("MOV  B,D"); break;
   case 0x43: printf("MOV  B,E"); break;
   case 0x44: printf("MOV  B,H"); break;
   case 0x45: printf("MOV  B,L"); break;
   case 0x46: printf("MOV  B,M"); break;
   case 0x47: printf("MOV  B,A"); break;
   case 0x48: printf("MOV  C,B"); break;
   case 0x49: printf("MOV  C,C"); break;
   case 0x4a: printf("MOV  C,D"); break;
   case 0x4b: printf("MOV  C,E"); break;
   case 0x4c: printf("MOV  C,H"); break;
   case 0x4d: printf("MOV  C,L"); break;
   case 0x4e: printf("MOV  C,M"); break;
   case 0x4f: printf("MOV  C,A"); break;

   case 0x50: printf("MOV  D,B"); break;
   case 0x51: printf("MOV  D,C"); break;
   case 0x52: printf("MOV  D,D"); break;
   case 0x53: printf("MOV  D.E"); break;
   case 0x54: printf("MOV  D,H"); break;
   case 0x55: printf("MOV  D,L"); break;
   case 0x56: printf("MOV  D,M"); break;
   case 0x57: printf("MOV  D,A"); break;
   case 0x58: printf("MOV  E,B"); break;
   case 0x59: printf("MOV  E,C"); break;
   case 0x5a: printf("MOV  E,D"); break;
   case 0x5b: printf("MOV  E,E"); break;
   case 0x5c: printf("MOV  E,H"); break;
   case 0x5d: printf("MOV  E,L"); break;
   case 0x5e: printf("MOV  E,M"); break;
   case 0x5f: printf("MOV  E,A"); break;

   case 0x60: printf("MOV  H,B"); break;
   case 0x61: printf("MOV  H,C"); break;
   case 0x62: printf("MOV  H,D"); break;
   case 0x63: printf("MOV  H.E"); break;
   case 0x64: printf("MOV  H,H"); break;
   case 0x65: printf("MOV  H,L"); break;
   case 0x66: printf("MOV  H,M"); break;
   case 0x67: printf("MOV  H,A"); break;
   case 0x68: printf("MOV  L,B"); break;
   case 0x69: printf("MOV  L,C"); break;
   case 0x6a: printf("MOV  L,D"); break;
   case 0x6b: printf("MOV  L,E"); break;
   case 0x6c: printf("MOV  L,H"); break;
   case 0x6d: printf("MOV  L,L"); break;
   case 0x6e: printf("MOV  L,M"); break;
   case 0x6f: printf("MOV  L,A"); break;

   case 0x70: printf("MOV  M,B"); break;
   case 0x71: printf("MOV  M,C"); break;
   case 0x72: printf("MOV  M,D"); break;
   case 0x73: printf("MOV  M.E"); break;
   case 0x74: printf("MOV  M,H"); break;
   case 0x75: printf("MOV  M,L"); break;
   case 0x76: printf("HLT");      break;
   case 0x77: printf("MOV  M,A"); break;
   case 0x78: printf("MOV  A,B"); break;
   case 0x79: printf("MOV  A,C"); break;
   case 0x7a: printf("MOV  A,D"); break;
   case 0x7b: printf("MOV  A,E"); break;
   case 0x7c: printf("MOV  A,H"); break;
   case 0x7d: printf("MOV  A,L"); break;
   case 0x7e: printf("MOV  A,M"); break;
   case 0x7f: printf("MOV  A,A"); break;

   case 0x80: printf("ADD  B"); break;
   case 0x81: printf("ADD  C"); break;
   case 0x82: printf("ADD  D"); break;
   case 0x83: printf("ADD  E"); break;
   case 0x84: printf("ADD  H"); break;
   case 0x85: printf("ADD  L"); break;
   case 0x86: printf("ADD  M"); break;
   case 0x87: printf("ADD  A"); break;
   case 0x88: printf("ADC  B"); break;
   case 0x89: printf("ADC  C"); break;
   case 0x8a: printf("ADC  D"); break;
   case 0x8b: printf("ADC  E"); break;
   case 0x8c: printf("ADC  H"); break;
   case 0x8d: printf("ADC  L"); break;
   case 0x8e: printf("ADC  M"); break;
   case 0x8f: printf("ADC  A"); break;

   case 0x90: printf("SUB  B"); break;
   case 0x91: printf("SUB  C"); break;
   case 0x92: printf("SUB  D"); break;
   case 0x93: printf("SUB  E"); break;
   case 0x94: printf("SUB  H"); break;
   case 0x95: printf("SUB  L"); break;
   case 0x96: printf("SUB  M"); break;
   case 0x97: printf("SUB  A"); break;
   case 0x98: printf("SBB  B"); break;
   case 0x99: printf("SBB  C"); break;
   case 0x9a: printf("SBB  D"); break;
   case 0x9b: printf("SBB  E"); break;
   case 0x9c: printf("SBB  H"); break;
   case 0x9d: printf("SBB  L"); break;
   case 0x9e: printf("SBB  M"); break;
   case 0x9f: printf("SBB  A"); break;

   case 0xa0: printf("ANA  B"); break;
   case 0xa1: printf("ANA  C"); break;
   case 0xa2: printf("ANA  D"); break;
   case 0xa3: printf("ANA  E"); break;
   case 0xa4: printf("ANA  H"); break;
   case 0xa5: printf("ANA  L"); break;
   case 0xa6: printf("ANA  M"); break;
   case 0xa7: printf("ANA  A"); break;
   case 0xa8: printf("XRA  B"); break;
   case 0xa9: printf("XRA  C"); break;
   case 0xaa: printf("XRA  D"); break;
   case 0xab: printf("XRA  E"); break;
   case 0xac: printf("XRA  H"); break;
   case 0xad: printf("XRA  L"); break;
   case 0xae: printf("XRA  M"); break;
   case 0xaf: printf("XRA  A"); break;

   case 0xb0: printf("ORA  B"); break;
   case 0xb1: printf("ORA  C"); break;
   case 0xb2: printf("ORA  D"); break;
   case 0xb3: printf("ORA  E"); break;
   case 0xb4: printf("ORA  H"); break;
   case 0xb5: printf("ORA  L"); break;
   case 0xb6: printf("ORA  M"); break;
   case 0xb7: printf("ORA  A"); break;
   case 0xb8: printf("CMP  B"); break;
   case 0xb9: printf("CMP  C"); break;
   case 0xba: printf("CMP  D"); break;
   case 0xbb: printf("CMP  E"); break;
   case 0xbc: printf("CMP  H"); break;
   case 0xbd: printf("CMP  L"); break;
   case 0xbe: printf("CMP  M"); break;
   case 0xbf: printf("CMP  A"); break;

   case 0xc0: printf("RNZ"); break;
   case 0xc1: printf("POP  B"); break;
   case 0xc2: printf("JNZ  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xc3: printf("JMP  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xc4: printf("CNZ  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xc5: printf("PUSH B"); break;
   case 0xc6: printf("ADI  #$%02x",code[1]); opbytes = 2; break;
   case 0xc7: printf("RST  0"); break;
   case 0xc8: printf("RZ"); break;
   case 0xc9: printf("RET"); break;
   case 0xca: printf("JZ   $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xcb: printf("JMP  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xcc: printf("CZ   $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xcd: printf("CALL $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xce: printf("ACI  #$%02x",code[1]); opbytes = 2; break;
   case 0xcf: printf("RST  1"); break;

   case 0xd0: printf("RNC"); break;
   case 0xd1: printf("POP  D"); break;
   case 0xd2: printf("JNC  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xd3: printf("OUT  #$%02x",code[1]); opbytes = 2; break;
   case 0xd4: printf("CNC  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xd5: printf("PUSH D"); break;
   case 0xd6: printf("SUI  #$%02x",code[1]); opbytes = 2; break;
   case 0xd7: printf("RST  2"); break;
   case 0xd8: printf("RC");  break;
   case 0xd9: printf("RET"); break;
   case 0xda: printf("JC   $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xdb: printf("IN   #$%02x",code[1]); opbytes = 2; break;
   case 0xdc: printf("CC   $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xdd: printf("CALL $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xde: printf("SBI  #$%02x",code[1]); opbytes = 2; break;
   case 0xdf: printf("RST  3"); break;

   case 0xe0: printf("RPO"); break;
   case 0xe1: printf("POP  H"); break;
   case 0xe2: printf("JPO  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xe3: printf("XTHL");break;
   case 0xe4: printf("CPO  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xe5: printf("PUSH H"); break;
   case 0xe6: printf("ANI  #$%02x",code[1]); opbytes = 2; break;
   case 0xe7: printf("RST  4"); break;
   case 0xe8: printf("RPE"); break;
   case 0xe9: printf("PCHL");break;
   case 0xea: printf("JPE  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xeb: printf("XCHG"); break;
   case 0xec: printf("CPE  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xed: printf("CALL  $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xee: printf("XRI  #$%02x",code[1]); opbytes = 2; break;
   case 0xef: printf("RST  5"); break;
   
   case 0xf0: printf("RP");  break;
   case 0xf1: printf("POP  PSW"); break;
   case 0xf2: printf("JP   $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xf3: printf("DI");  break;
   case 0xf4: printf("CP   $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xf5: printf("PUSH PSW"); break;
   case 0xf6: printf("ORI  #$%02x",code[1]); opbytes = 2; break;
   case 0xf7: printf("RST  6"); break;
   case 0xf8: printf("RM");  break;
   case 0xf9: printf("SPHL");break;
   case 0xfa: printf("JM   $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xfb: printf("EI");  break;
   case 0xfc: printf("CM   $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xfd: printf("CALL $%02x%02x",code[2],code[1]); opbytes = 3; break;
   case 0xfe: printf("CPI  #$%02x",code[1]); opbytes = 2; break;
   case 0xff: printf("RST  7"); break;
  }
  
  printf("\n");

  return opbytes;
}

int parity(int x, int size) {
  int parity = 0;
  for (int i = 0; i < size; i++) {
    parity += x & 1;
    x = x >> 1;
  }
  return (parity % 2 == 0);
}

unsigned char cycles[] = {
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x00..0x0f
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x10..0x1f
	4, 10, 16, 5, 5, 5, 7, 4, 4, 10, 16, 5, 5, 5, 7, 4, //etc
	4, 10, 13, 5, 10, 10, 10, 4, 4, 10, 13, 5, 5, 5, 7, 4,
	
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x40..0x4f
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
	7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 5,
	
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0x80..8x4f
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
	
	11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 10, 17, 7, 11, //0xc0..0xcf
	11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 10, 17, 7, 11, 
	11, 10, 10, 18, 17, 11, 7, 11, 11, 5, 10, 5, 17, 17, 7, 11, 
	11, 10, 10, 4, 17, 11, 7, 11, 11, 5, 10, 4, 17, 17, 7, 11, 
};

void LogicFlags(State *state) {
  state->cc.cy = state->cc.ac = 0;
  state->cc.z = (state->a == 0);
  state->cc.s = ((state->a & 0x80) == 0x80);
  state->cc.p = parity(state->a,8);
}

void ArithFlags(State *state, uint16_t res) {
  state->cc.cy = (res > 0xff);
  state->cc.z = ((res&0xff) == 0);
  state->cc.s = ((res&0x80) == 0x80);
  state->cc.p = parity(res&0xff, 8);
}

void ZSPFlags(State *state, uint16_t res) {
  state->cc.z = (res == 0);
  state->cc.s = (0x80 == (res & 0x80));
  state->cc.p = parity(res, 8);
}

void UnimplementedInstruction(State *state) {
  uint8_t *opcode = &state->memory[state->pc - 1];
  printf("Error: Unimplemented Instruction.\n opcode: 0x%02X ", *opcode);
  Disassemble(state->memory, state->pc-1);
  exit(1);
}

void WriteMem(State *state, uint16_t addr, uint8_t value) {
  if (addr < 0x2000)
    return;
  else if (addr >= 0x4000 && addr < 0x6000) addr -= 0x2000;

  state->memory[addr] = value;
}

uint8_t ReadMem(State *state, uint16_t addr) {
  if (addr >= 0x4000 && addr < 0x6000) addr -= 0x2000;
  return state->memory[addr];
}

uint8_t ReadHL(State *state) {
  uint16_t offset = (state->h<<8) | (state->l);
  return ReadMem(state, offset);
}

void WriteHL(State *state, uint8_t value) {
  uint16_t offset = (state->h<<8) | (state->l);
  WriteMem(state, offset, value);
}

void Push(State *state, uint8_t hi, uint8_t lo) {
  WriteMem(state, state->sp-1, hi);
  WriteMem(state, state->sp-2, lo);
  state->sp -= 2;
}

void Pop(State *state, uint8_t *hi, uint8_t *lo) {
  *lo = ReadMem(state, state->sp);
  *hi = ReadMem(state, state->sp+1);
  state->sp += 2;
}

int Emulate(State *state) {
  unsigned char *opcode = &state->memory[state->pc];
  state->pc+=1;
  switch (*opcode) {
    case 0x00: // NOP's
    case 0x08:
    case 0x10:
    case 0x18:
    case 0x20:
    case 0x28:
    case 0x30:
    case 0x38: break;
    case 0x01: // LXI  B,word | B <- word
      state->b = opcode[2];
      state->c = opcode[1];
      state->pc += 2;
      break;
    case 0x02: // STAX B
    {
      uint16_t offset = (state->b<<8) | (state->c);
      WriteMem(state, offset, state->a);
    } break;
    case 0x03: // INX B
    {
      uint16_t aux = (state->b<<8) | (state->c);
      aux++;
      state->b = (aux>>8 & 0xff);
      state->c = (aux&0xff); 
    } break;
    case 0x04: // INR B
      state->b++;
      ZSPFlags(state, state->b);
      break;
    case 0x05: // DCR B
      state->b -= 1;
      ZSPFlags(state, state->b);
      break;
    case 0x06: // MVI B,byte
      state->b = opcode[1];
      state->pc++;
      break;
    case 0x07: //RLC 
    {
      uint8_t aux = state->a;
      state->a = ((aux & 0x80)>>7) | (aux<<1);
      state->cc.cy = (0x80 == (aux & 0x80));
    } break;
    case 0x09: // DAD B
    {
      uint32_t hl = (state->h<<8) | (state->l);
      uint32_t bc = (state->b<<8) | (state->c); 
      uint32_t aux = hl + bc;
      state->h = (aux & 0xff00)>>8;
      state->l = (aux & 0xff);
      state->cc.cy = ((aux & 0xffff0000) != 0);
    } break;
    case 0x0a: // LDAX B 
    {
      uint16_t offset = (state->b<<8) | (state->c);
      state->a = ReadMem(state, offset);
    } break;
    case 0x0b: // DCX B
    {
      uint16_t aux = (state->b<<8) | (state->c);
      aux--;
      state->b = (aux>>8 & 0xff);
      state->c = (aux & 0xff);      
    } break;
    case 0x0c: // INR C
      state->c++;
      ZSPFlags(state, state->c);
      break;      
    case 0x0d: // DCR C
      state->c -= 1;
      ZSPFlags(state, state->c);
      break;
    case 0x0e: // MVI C,byte
      state->c = opcode[1];
      state->pc++;
      break;
    case 0x0f:  // RRC | A <- 0bA0A7A6A5A4A3A2A1
    {
      uint8_t aux = state->a;
      state->a = ((aux & 1)<<7) | (aux>>1);
      state->cc.cy = (1 == (aux & 1));
    } break;
    case 0x11: // LXI D, word
      state->d = opcode[2];
      state->e = opcode[1]; 
      state->pc += 2;
      break;
    case 0x12: UnimplementedInstruction(state); break;
    case 0x13: // INX D
    {
      uint16_t aux = (state->d<<8) | (state->e);
      aux++;
      state->d = (aux>>8 & 0xff);
      state->e = (aux & 0xff);
    } break;
    case 0x14: // INR D
      state->d++;
      ZSPFlags(state, state->d);
      break;
    case 0x15: // DCR D
      state->d -= 1;
      ZSPFlags(state, state->d);
      break;
    case 0x16: // MVI D,byte
      state->d = opcode[1];
      state->pc++;
      break;
    case 0x17: // RAL
      state->a = (state->a<<1) | (state->cc.cy);
      state->cc.cy = (0x80 == (state->a&0x80));
      break;
    case 0x19: // DAD D
    {
      uint32_t hl = (state->h<<8) | (state->l);
      uint32_t de = (state->d<<8) | (state->e);
      uint32_t aux = hl + de;
      state->h = (aux & 0xff00)>>8;
      state->l = (aux & 0xff);
      state->cc.cy = ((aux & 0xffff0000) != 0);
    } break;
    case 0x1a:  // LDAX D
    {
      uint16_t offset = (state->d<<8) | (state->e);
      state->a = ReadMem(state, offset);
    } break;
    case 0x1b:  // DCX D
    {
      uint16_t aux = (state->d<<8) | (state->e);
      aux--;
      state->d = (aux>>8 & 0xff);
      state->e = (aux & 0xff);
    } break;
    case 0x1c: UnimplementedInstruction(state); break;
    case 0x1d: UnimplementedInstruction(state); break;
    case 0x1e: UnimplementedInstruction(state); break;
    case 0x1f:  // RAR | A <- 0bCA7A6A5A4A3A2A1
    {
        uint8_t aux = state->a;
        state->a = (state->cc.cy << 7) | (aux >> 1);
        state->cc.cy = (1 == (aux & 1));
    } break;
    case 0x21: // LXI H,word
      state->h = opcode[2];
      state->l = opcode[1];
      state->pc += 2;
      break;
    case 0x22: // SHLD word
    {
      uint16_t offset = (opcode[2] << 8) | (opcode[1]);
      WriteMem(state, offset, state->l);
      WriteMem(state, offset + 1, state->h);
      state->pc += 2;
    } break;
    case 0x23: // INX H
    {
      uint16_t aux = (state->h<<8) | (state->l);
      aux+=1;
      state->h = (aux>>8 & 0xff);
      state->l = (aux & 0xff);
    } break;
    case 0x24:  // INR H
      state->h++;
      ZSPFlags(state, state->h);
      break;
    case 0x25: // DCR H
      state->h -= 1;
      ZSPFlags(state, state->h);
      break;
    case 0x26: // MVI H,byte
      state->h = opcode[1];
      state->pc++;
      break;
    case 0x27:  // DAA
    {
      uint16_t aux = state->a;
      if ((state->a & 0xf) > 9) {
        aux += 6;
        ArithFlags(state, aux);
      }
      if ((state->a & 0xf0) > 0x90) {
        aux = (uint16_t)state->a + 0x60;
        ArithFlags(state, aux);
      }
      state->a = aux & 0xff;
    } break;
    case 0x29: // DAD H
    {
      uint32_t hl = (state->h<<8) | (state->l);
      uint32_t aux = hl + hl;
      state->h = (aux>>8 & 0xff);
      state->l = (aux & 0xff);
      state->cc.cy = ((aux & 0xffff0000) != 0);
    } break;
    case 0x2a: // LHLD word
    {
      uint16_t offset = (opcode[2]<<8) | (opcode[1]);
      state->h = ReadMem(state, offset + 1);
      state->l = ReadMem(state, offset);
      state->pc += 2;
    } break;
    case 0x2b: // DCX H
    {
      uint16_t aux = (state->h<<8) | (state->l);
      aux--;
      state->h = (aux>>8 & 0xff);
      state->l = (aux&0xff);
    } break;
    case 0x2c: // INR L
      state->l++;
      ZSPFlags(state, state->l);
      break;
    case 0x2d: UnimplementedInstruction(state); break;
    case 0x2e: // MVI L,byte
      state->l = opcode[1];
      state->pc++;
      break;
    case 0x2f:  // CMA (not)
      state->a = ~state->a;
      break;
    case 0x31: // LXI SP, word 
      state->sp = (opcode[2]<<8) | (opcode[1]);
      state->pc += 2;
      break;
    case 0x32: // STA word
    {
      uint16_t offset = (opcode[2]<<8) | (opcode[1]);
      WriteMem(state, offset, state->a);
      state->pc += 2;
    } break;
    case 0x33: UnimplementedInstruction(state); break;
    case 0x34: // INR M
    {
      uint8_t aux = ReadHL(state) + 1;
      ZSPFlags(state, aux);
      WriteHL(state, aux);
    }
    case 0x35: // DCR M
    {
      uint8_t aux = ReadHL(state) - 1;
      ZSPFlags(state, aux);
      WriteHL(state, aux); 
    } break;
    case 0x36: // MVI M,byte
      WriteHL(state, opcode[1]);
      state->pc++;
      break;
    case 0x37: // STC
      state->cc.cy = 1;
      break;
    case 0x39: // DAD SP
    {
      uint32_t hl = (state->h<<8) | (state->l);
      uint32_t aux = hl + state->sp;
      state->h = (aux & 0xff00)>>8;
      state->l = (aux & 0xff); 
      state->cc.cy = ((aux & 0xffff0000) != 0);
    } break;
    case 0x3a: // LDA word
    {
      uint16_t offset = (opcode[2]<<8) | (opcode[1]); 
      state->a = ReadMem(state, offset);
      state->pc+=2;
    } break;
    case 0x3b: // DCX SP
      state->sp -= 1;
      break;
    case 0x3c: // INR A
      state->a++;
      ZSPFlags(state, state->a);
      break;
    case 0x3d: // DCR A
      state->a -= 1;
      ZSPFlags(state, state->a);
      break;
    case 0x3e: // MVI A,byte
      state->a = opcode[1];
      state->pc++;
      break;
    case 0x3f: UnimplementedInstruction(state); break;
    case 0x40: // MOV B,B 
      state->b = state->b;
      break;
    case 0x41:  // MOV B,C | B <- C
      state->b = state->c;
      break;
    case 0x42:  // MOV B,D | B <- D
      state->b = state->d;
      break;
    case 0x43:  // MOV B,E | B <- E
      state->b = state->e;
      break;
    case 0x44: UnimplementedInstruction(state); break;
    case 0x45: UnimplementedInstruction(state); break;
    case 0x46: // MOV B,M
      state->b = ReadHL(state);
      break;
    case 0x47: // MOV B,A
      state->b = state->a;
      break;
    case 0x48: // MOV C,B
      state->c = state->b;
      break;
    case 0x49: UnimplementedInstruction(state); break;
    case 0x4a: UnimplementedInstruction(state); break;
    case 0x4b: UnimplementedInstruction(state); break;
    case 0x4c: UnimplementedInstruction(state); break;
    case 0x4d: UnimplementedInstruction(state); break;
    case 0x4e: // MOV C,M
      state->c = ReadHL(state);
      break;
    case 0x4f: // MOV C,A
      state->c = state->a;
      break;
    case 0x50: UnimplementedInstruction(state); break;
    case 0x51: UnimplementedInstruction(state); break;
    case 0x52: UnimplementedInstruction(state); break;
    case 0x53: UnimplementedInstruction(state); break;
    case 0x54: UnimplementedInstruction(state); break;
    case 0x55: UnimplementedInstruction(state); break;
    case 0x56: // MOV D,M
      state->d = ReadHL(state);
      break;
    case 0x57: // MOV D,A
      state->d = state->a;
      break;
    case 0x58: UnimplementedInstruction(state); break;
    case 0x59: UnimplementedInstruction(state); break;
    case 0x5a: UnimplementedInstruction(state); break;
    case 0x5b: UnimplementedInstruction(state); break;
    case 0x5c: UnimplementedInstruction(state); break;
    case 0x5d: UnimplementedInstruction(state); break;
    case 0x5e: // MOV E,M
      state->e = ReadHL(state);
      break;
    case 0x5f: // MOV E,A
      state->e = state->a;
      break;
    case 0x60: UnimplementedInstruction(state); break;
    case 0x61: // MOV H,C
      state->h = state->c;
      break;
    case 0x62: UnimplementedInstruction(state); break;
    case 0x63: UnimplementedInstruction(state); break;
    case 0x64: UnimplementedInstruction(state); break;
    case 0x65: // MOV H,L
      state->h = state->l;
      break;
    case 0x66: // MOV H,M
      state->h = ReadHL(state);
      break;
    case 0x67: // MOV H,A 
      state->h = state->a;  
      break;
    case 0x68: // MOV L,B
      state->l = state->b;
      break;
    case 0x69: // MOV L,C
      state->l = state->c;
      break;
    case 0x6a: UnimplementedInstruction(state); break;
    case 0x6b: UnimplementedInstruction(state); break;
    case 0x6c: UnimplementedInstruction(state); break;
    case 0x6d: UnimplementedInstruction(state); break;
    case 0x6e: UnimplementedInstruction(state); break;
    case 0x6f: // MOV L,A
      state->l = state->a;
      break;
    case 0x70: // MOV M,B
      WriteHL(state, state->b);
      break;
    case 0x71: // MOV M,C
      WriteHL(state, state->c);
      break;
    case 0x72: // MOV M,D 
      WriteHL(state, state->d);
      break;
    case 0x73: UnimplementedInstruction(state); break;
    case 0x74: UnimplementedInstruction(state); break;
    case 0x75: UnimplementedInstruction(state); break;
    case 0x76: UnimplementedInstruction(state); break;
    case 0x77: // MOV M,A
      WriteHL(state, state->a);
      break;
    case 0x78: // MOV A,B
      state->a = state->b;
      break;
    case 0x79: // MOV A,C
      state->a = state->c;
      break;
    case 0x7a: // MOV A,D
      state->a = state->d;
      break;
    case 0x7b: // MOV A,E
      state->a = state->e;
      break;
    case 0x7c: // MOV A,H
      state->a = state->h;
      break;
    case 0x7d: // MOV A,L
      state->a = state->l;
      break;
    case 0x7e: // MOV A,M
      state->a = ReadHL(state);
      break;
    case 0x7f: UnimplementedInstruction(state); break;
    case 0x80:  // ADD B | A <- A+B 
    {
        uint16_t res = state->a + state->b;
        ArithFlags(state, res);
        state->a = res & 0xff;
    } break;
    case 0x81:  // ADD C | A <- A+C
    {
      uint16_t res = state->a + state->c;
      ArithFlags(state, res);
      state->a = res & 0xff;
    } break;
    case 0x82:  // ADD D | A <- A+D
    {
      uint16_t res = state->a + state->d;
      ArithFlags(state, res);
      state->a = res & 0xff;
    } break;
    case 0x83:  // ADD D | A <- A+E
    {
      uint16_t res = state->a + state->e;
      ArithFlags(state, res);
      state->a = res & 0xff;
    } break;
    case 0x84:  // ADD D | A <- A+H
    {
      uint16_t res = state->a + state->h;
      ArithFlags(state, res);
      state->a = res & 0xff;
    } break;
    case 0x85:  // ADD D | A <- A+L
    {
      uint16_t res = state->a + state->l;
      ArithFlags(state, res);
      state->a = res & 0xff;
    } break;
    case 0x86:  // ADD M | A <- A+(HL)
    {
      uint16_t res = state->a + ReadHL(state);
      ArithFlags(state, res);
      state->a = res & 0xff;
    } break;
    case 0x87:  // ADD A | A <- A+A
    {
      uint16_t res = state->a + state->a;
      ArithFlags(state, res);
      state->a = res & 0xff;
    } break;
    case 0x88: UnimplementedInstruction(state); break;
    case 0x89: UnimplementedInstruction(state); break;
    case 0x8a: // ADC D
    {
      uint16_t aux = state->a + state->d + state->cc.cy;
      ArithFlags(state, aux);
      state->a = (aux & 0xff);
    } break;
    case 0x8b: UnimplementedInstruction(state); break;
    case 0x8c: UnimplementedInstruction(state); break;
    case 0x8d: UnimplementedInstruction(state); break;
    case 0x8e: UnimplementedInstruction(state); break;
    case 0x8f: UnimplementedInstruction(state); break;
    case 0x90: UnimplementedInstruction(state); break;
    case 0x91: UnimplementedInstruction(state); break;
    case 0x92: UnimplementedInstruction(state); break;
    case 0x93: UnimplementedInstruction(state); break;
    case 0x94: UnimplementedInstruction(state); break;
    case 0x95: UnimplementedInstruction(state); break;
    case 0x96: UnimplementedInstruction(state); break;
    case 0x97: // SUB A
    {
      uint16_t aux = state->a - state->a;
      ArithFlags(state, aux);
      state->a = (aux & 0xff);
    } break;
    case 0x98: UnimplementedInstruction(state); break;
    case 0x99: UnimplementedInstruction(state); break;
    case 0x9a: UnimplementedInstruction(state); break;
    case 0x9b: UnimplementedInstruction(state); break;
    case 0x9c: UnimplementedInstruction(state); break;
    case 0x9d: UnimplementedInstruction(state); break;
    case 0x9e: UnimplementedInstruction(state); break;
    case 0x9f: UnimplementedInstruction(state); break;
    case 0xa0: // ANA B
      state->a = state->a & state->b;
      LogicFlags(state);
      break;
    case 0xa1: UnimplementedInstruction(state); break;
    case 0xa2: UnimplementedInstruction(state); break;
    case 0xa3: UnimplementedInstruction(state); break;
    case 0xa4: UnimplementedInstruction(state); break;
    case 0xa5: UnimplementedInstruction(state); break;
    case 0xa6: // ANA M
      state->a = (state->a & ReadHL(state));
      LogicFlags(state);
      break;
    case 0xa7: // ANA A
      state->a = (state->a & state->a);
      LogicFlags(state);
      break;
    case 0xa8: // XRA B
      state->a = (state->a ^ state->b);
      break;
    case 0xa9: UnimplementedInstruction(state); break;
    case 0xaa: UnimplementedInstruction(state); break;
    case 0xab: UnimplementedInstruction(state); break;
    case 0xac: UnimplementedInstruction(state); break;
    case 0xad: UnimplementedInstruction(state); break;
    case 0xae: UnimplementedInstruction(state); break;
    case 0xaf: // XRA A
      state->a = (state->a ^ state->a);
      LogicFlags(state);
      break;
    case 0xb0: // ORA B
      state->a = (state->a | state->b);
      LogicFlags(state);
      break;
    case 0xb1: UnimplementedInstruction(state); break;
    case 0xb2: UnimplementedInstruction(state); break;
    case 0xb3: UnimplementedInstruction(state); break;
    case 0xb4: // ORA H
      state->a = (state->a | state->h);
      LogicFlags(state);
      break;
    case 0xb5: UnimplementedInstruction(state); break;
    case 0xb6: // ORA M
      state->a = (state->a | ReadHL(state));
      LogicFlags(state);
      break;
    case 0xb7: UnimplementedInstruction(state); break;
    case 0xb8:  // CMP B
    {
      uint16_t aux = state->a - state->b;
      ArithFlags(state, aux);
    } break;
    case 0xb9: UnimplementedInstruction(state); break;
    case 0xba: UnimplementedInstruction(state); break;
    case 0xbb: UnimplementedInstruction(state); break;
    case 0xbc: // CMP H
    {
      uint16_t aux = state->a - state->h;
      ArithFlags(state, aux);
    } break;
    case 0xbd: UnimplementedInstruction(state); break;
    case 0xbe: // CMP M
    {
      uint16_t aux = state->a - ReadHL(state);
      ArithFlags(state, aux);
    } break;
    case 0xbf: UnimplementedInstruction(state); break;
    case 0xc0: // RNZ
      if (state->cc.z == 0) {
        state->pc = ReadMem(state,state->sp) | (ReadMem(state, state->sp+1)<<8);
        state->sp += 2;
      }
      break;
    case 0xc1:  // POP B
      Pop(state, &state->b, &state->c);
      break;
    case 0xc2:  // JNZ addr | JMP if no zero flag
      if (state->cc.z == 0){  
        state->pc = (opcode[2]<<8) | opcode[1];
      } else {
        state->pc += 2;
      }
      break;
    case 0xc3:  // JMP addr
      state->pc = (opcode[2]<<8) | opcode[1];
      break;
    case 0xc4: // CNZ addr
      if (state->cc.z == 0) {
        uint16_t ret = state->pc + 2;
        WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
        WriteMem(state, state->sp - 2, ret & 0xff);
        state->sp = state->sp - 2;
        state->pc = (opcode[2] << 8) | opcode[1];
      } else {
        state->pc += 2;
      }
      break;
    case 0xc5:  // PUSH B
      Push(state, state->b, state->c);
      break;
    case 0xc6:  //ADI byte
    {
      uint16_t res = state->a + opcode[1];
      ArithFlags(state, res);
      state->a = res & 0xff;
      state->pc++;
    } break;
    case 0xc7: UnimplementedInstruction(state); break;
    case 0xc8: // RZ 
      if (state->cc.z != 0) {
        state->pc = ReadMem(state, state->sp+1)<<8 | ReadMem(state, state->sp);
        state->sp += 2;
      }
      break;
    case 0xc9:  // RET 
      state->pc = ReadMem(state, state->sp) | (ReadMem(state,state->sp + 1)<<8);
      state->sp += 2;
      break;
    case 0xca: // JZ addr
      if (state->cc.z != 0)
        state->pc = (opcode[2]<<8) | opcode[1];
      else {
        state->pc += 2; 
      }
      break;
    case 0xcb: UnimplementedInstruction(state); break;
    case 0xcc: // CZ addr
      if (state->cc.z != 0) {
        uint16_t ret = state->pc + 2;
        WriteMem(state, state->sp - 1, (ret >> 8) & 0xff);
        WriteMem(state, state->sp - 2, (ret & 0xff));
        state->sp -= 2;
        state->pc = (opcode[2] << 8) | opcode[1];
      } else {
        state->pc += 2;
      }
      break;
    case 0xcd:  // CALL addr
    {
      uint16_t ret = state->pc + 2;
      WriteMem(state, state->sp - 1, (ret>>8) & 0xff);
      WriteMem(state, state->sp - 2, ret & 0xff);
      state->sp -= 2;
      state->pc = (opcode[2]<<8) | opcode[1];
    } break;
    case 0xce: UnimplementedInstruction(state); break;
    case 0xcf: UnimplementedInstruction(state); break;
    case 0xd0: // RNC
      if (state->cc.cy == 0) {
        state->pc = ReadMem(state, state->sp) | (ReadMem(state, state->sp + 1) << 8);
        state->sp += 2;
      }
      break;
    case 0xd1: // POP D
      Pop(state, &state->d, &state->e);
      break;
    case 0xd2: // JNC addr
      if (state->cc.cy == 0)
        state->pc = (opcode[2]<<8) | opcode[1];
      else
        state->pc += 2;
      break;
    case 0xd3: // OUT byte
      break;
    case 0xd4: // CNC word
      if (state->cc.cy == 0) {
        uint16_t ret = state->pc + 2;
        WriteMem(state, state->sp-1, (ret>>8)&0xff);
        WriteMem(state, state->sp-2, (ret&0xff));
        state->sp -= 2;
        state->pc = (opcode[2]<<8) | (opcode[1]);
      } else {
        state->pc += 2;
      }
      break;
    case 0xd5: // PUSH D
      Push(state, state->d, state->e);
      break;
    case 0xd6: // SUI byte 
    {
      uint16_t aux = state->a - opcode[1];
      state->a = aux&0xff;
      ArithFlags(state, aux);
      state->pc++;
    } break;
    case 0xd7: UnimplementedInstruction(state); break;
    case 0xd8: // RC
      if (state->cc.cy != 0) {
        state->pc = ReadMem(state, state->sp) | (ReadMem(state, state->sp + 1) << 8);
        state->sp += 2;
      }
      break;
    case 0xd9: UnimplementedInstruction(state); break;
    case 0xda: //JC addr 
      if (state->cc.cy != 0)
        state->pc = (opcode[2]<<8) | (opcode[1]);
      else 
        state->pc += 2;
      break;
    case 0xdb: // IN, byte
      break;
    case 0xdc: UnimplementedInstruction(state); break;
    case 0xdd: UnimplementedInstruction(state); break;
    case 0xde: // SBI byte
    {
      uint16_t aux = state->a - opcode[1] -state->cc.cy;
      ArithFlags(state, aux);
      state->a = (aux & 0xff);
      state->pc++;
    } break; 
    case 0xdf: UnimplementedInstruction(state); break;
    case 0xe0: UnimplementedInstruction(state); break;
    case 0xe1: // POP H
      Pop(state, &state->h, &state->l);
      break;
    case 0xe2: UnimplementedInstruction(state); break;
    case 0xe3:  // XTHL
    {
      uint16_t aux = (state->h<<8) | (state->l);
      state->h = ReadMem(state, state->sp + 1);
      state->l = ReadMem(state, state->sp);
      WriteMem(state, state->sp, aux&0xff);
      WriteMem(state, state->sp+1, (aux&0xff00)>>8);
    } break;
    case 0xe4: UnimplementedInstruction(state); break;
    case 0xe5: // PUSH H
      Push(state, state->h, state->l);
      break;
    case 0xe6:  // ANI byte
      state->a = state->a & opcode[1];
      LogicFlags(state);
      state->pc++;
      break;
    case 0xe7: UnimplementedInstruction(state); break;
    case 0xe8: UnimplementedInstruction(state); break;
    case 0xe9: // PCHL
      state->pc = (state->h<<8) | (state->l);
      break;
    case 0xea: UnimplementedInstruction(state); break;
    case 0xeb: // XCHG
    {
      uint16_t aux = (state->h<<8) | (state->l);
      state->h = state->d;
      state->l = state->e;
      state->d = ((aux>>8) & 0xff);
      state->e = (aux & 0xff);
    } break;
    case 0xec: UnimplementedInstruction(state); break;
    case 0xed: UnimplementedInstruction(state); break;
    case 0xee: UnimplementedInstruction(state); break;
    case 0xef: UnimplementedInstruction(state); break;
    case 0xf0: UnimplementedInstruction(state); break;
    case 0xf1:  // POP PSW
    {
      uint8_t psw;
      Pop(state, &state->a, &psw);
      state->cc.cy = (0x01 == (psw & 0x01));
      state->cc.p = (0x04 == (psw & 0x04));
      state->cc.ac = (0x10 == (psw & 0x10));
      state->cc.z = (0x40 == (psw & 0x40));
      state->cc.s = (0x80 == (psw & 0x80));
    } break;
    case 0xf2: UnimplementedInstruction(state); break;
    case 0xf3: UnimplementedInstruction(state); break;
    case 0xf4: UnimplementedInstruction(state); break;
    case 0xf5:  // PUSH PSW
    {
      uint8_t psw = ( state->cc.cy<<0 
                    | 0x01<<1
                    | state->cc.p<<2 
                    | 0x00<<3
                    | state->cc.ac<<4 
                    | 0x00<<5
                    | state->cc.z<<6 
                    | state->cc.s<<7);
      Push(state, state->a, psw);
    } break;
    case 0xf6: // ORI byte
      state->a = (state->a | opcode[1]);
      ZSPFlags(state, state->a);
      state->cc.cy = 0;
      state->pc++;
      break;
    case 0xf7: UnimplementedInstruction(state); break;
    case 0xf8: UnimplementedInstruction(state); break;
    case 0xf9: UnimplementedInstruction(state); break;
    case 0xfa: // JM addr
      if (state->cc.s != 0)
        state->pc = (opcode[2]<<8) | opcode[1];
      else
        state->pc += 2;
      break;
    case 0xfb: // EI
      state->int_enable = 1;
      break;
    case 0xfc: UnimplementedInstruction(state); break;
    case 0xfd: UnimplementedInstruction(state); break;
    case 0xfe:  // CPI byte
    {
      uint8_t aux = state->a - opcode[1];
      ZSPFlags(state, aux);
      state->cc.cy = (state->a < opcode[1]);
      state->pc++;
    } break;
    case 0xff: UnimplementedInstruction(state); break;
    default: UnimplementedInstruction(state); break;
  }
  
  return cycles[*opcode];
}

void ReadFile(State *state, char *filename) {
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    printf("Error: couldn't open %s\n", filename);
    exit(1);
  }

  fseek(f, 0L, SEEK_END);
  int fsize = ftell(f);
  fseek(f, 0L, SEEK_SET);

  uint8_t *buffer = &state->memory[0];
  fread(buffer, fsize, 1, f);
  fclose(f);
}

State *InitState() {
  State* state = calloc(1,sizeof(State));
  state->memory = malloc(0x10000); // 16K
  return state;
}

void GenerateInterrupt(State *state, int nextInterrupt) {
  state->int_enable = 0;
  Push(state, (state->pc & 0xff00) >> 8, (state->pc & 0xff));
  state->pc = (uint16_t)nextInterrupt;
}
