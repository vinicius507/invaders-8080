/*
Arithimetic Group 
  ADD, ADC, ACI, SBB, SUI,
  INX, DCX (effect register pairs, do not effect the flags),
  DAD (register pair function, only affects the carry flag),
  INR, DCR (do not affect the carry flag).
Branch Group
  JMP (unconditionally branches to the target address),
  CALL, RET (CALL will push the adress of the instruction after the call onto the stack, then jumps to target address,
            RET gets an address off the stack and stores it into PC).
  JNZ, JZ, CZ, CNZ, RZ, RNZ for Zero Flag
  JNC, JC, CNC, CC, RNC, RC for Carry Flag
  JPO, JPE, CPO, CPE, RPO, RPE for Parity Flag
  JP, JM, CP, CM, RP, RM for Sign 
  PCHL (move HL to PC)  
  RST (pushes the return address to the stack then jumps to a predetermined
       low-memory address)
Logical Group (bitwise operations)
  AND, ANA, ANI, 
  XOR, XRA, XRI, 
  OR, ORA, ORI, 
  CMP, CPI, (compare) 
  CMA, CMC (affects CY flag!!),
  RLC, RRC, RAL, RAR (shift bits),
  STC (set carry)
IO and Special Group
  EI, DI (enable/disable the processors ability to process interrupts),
  RIM, SIM (used for serial I/O ? gonna look at it after space invaders emulation),
  HLT (halt),
  IN, OUT (implement to skip over its data byte for now), 
  NOP (no operation, one use for it is to pad timing, it takes 4 CPU cycles to execute)
Stack Group
  PUSH, POP (work only on register pairs)
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ConditionCodes {
  uint8_t z : 1;
  uint8_t s : 1;
  uint8_t p : 1;
  uint8_t cy : 1;
  uint8_t ac : 1;
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

int Disassemble(unsigned char *codebuffer, int pc) {
  unsigned char *code = &codebuffer[pc];
  int opbytes = 1;
  printf("%04x ", pc);
  switch (*code) {
   // group BC
   case 0x00: printf("NOP"); break;
   case 0x01: printf("LXI   B,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
   case 0x02: printf("STAX  B"); break;
   case 0x03: printf("INX   B"); break;
   case 0x04: printf("INR   B"); break;
   case 0x05: printf("DCR   B"); break;
   case 0x06: printf("MVI   B,#$%02x", code[1]); opbytes = 2; break;
   case 0x07: printf("RLC"); break;
   case 0x09: printf("DAD  B"); break;
   case 0x0a: printf("LDAX B"); break;
   case 0x0b: printf("DCX  B"); break;
   case 0x0c: printf("INR  C"); break;
   case 0x0d: printf("DCR  C"); break;
   case 0x0e: printf("MVI  C,#$%02X", code[1]); opbytes = 2; break;
   case 0x0f: printf("RRC"); break;
   // group DE
   case 0x10: printf("NOP"); break;
   case 0x11: printf("LXI  D,$%02x", code[1]); opbytes = 2; break;
   case 0x12: printf("STAX D"); break;
   case 0x13: printf("INX  D"); break;
   case 0x14: printf("INR  D"); break;
   case 0x15: printf("DCR  D"); break;
   case 0x16: printf("MVI  D,#$%02X", code[1]); opbytes = 2; break;
   case 0x17: printf("RAL"); break;
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

void UnimplementedInstruction(State *state) {
  printf("Error: Unimplemented Instruction\n");
  exit(1);
}

int Emulate(State *state) {
  unsigned char *opcode = &state->memory[state->pc];
  
  switch (*opcode) {
      case 0x00: break; // NOP
      case 0x01: // LXI  B,word | B <- word  
         state->c = opcode[1];
         state->b = opcode[2];
         state->pc += 2;
         break;
      case 0x02: UnimplementedInstruiction(state); break;
      case 0x03: UnimplementedInstruiction(state); break;
      case 0x04: UnimplementedInstruiction(state); break;
      case 0x05: UnimplementedInstruiction(state); break;
      case 0x06: UnimplementedInstruiction(state); break;
      case 0x07: UnimplementedInstruiction(state); break;
      case 0x08: UnimplementedInstruiction(state); break;
      case 0x09: UnimplementedInstruiction(state); break;
      case 0x0a: UnimplementedInstruiction(state); break;
      case 0x0b: UnimplementedInstruiction(state); break;
      case 0x0c: UnimplementedInstruiction(state); break;
      case 0x0d: UnimplementedInstruiction(state); break;
      case 0x0e: UnimplementedInstruiction(state); break;
      case 0x0f: // RRC | A <- 0bA0A7A6A5A4A3A2A1
        uint8_t aux = state->a;
        state->a = ((aux & 1) << 7) | (aux >> 1);
        state->cc.cy = (1 == (aux & 1));
        break;
      case 0x10: UnimplementedInstruiction(state); break;
      case 0x11: UnimplementedInstruiction(state); break;
      case 0x12: UnimplementedInstruiction(state); break;
      case 0x13: UnimplementedInstruiction(state); break;
      case 0x14: UnimplementedInstruiction(state); break;
      case 0x15: UnimplementedInstruiction(state); break;
      case 0x16: UnimplementedInstruiction(state); break;
      case 0x17: UnimplementedInstruiction(state); break;
      case 0x18: UnimplementedInstruiction(state); break;
      case 0x19: UnimplementedInstruiction(state); break;
      case 0x1a: UnimplementedInstruiction(state); break;
      case 0x1b: UnimplementedInstruiction(state); break;
      case 0x1c: UnimplementedInstruiction(state); break;
      case 0x1d: UnimplementedInstruiction(state); break;
      case 0x1e: UnimplementedInstruiction(state); break;
      case 0x1f: // RAR | A <- 0bCA7A6A5A4A3A2A1
        uint8_t aux = state->a;
        state->a = (state->cc.cy << 7) | (aux >> 1);
        state->cc.cy = (1 == (aux & 1));
        break;
      case 0x20: UnimplementedInstruiction(state); break;
      case 0x21: UnimplementedInstruiction(state); break;
      case 0x22: UnimplementedInstruiction(state); break;
      case 0x23: UnimplementedInstruiction(state); break;
      case 0x24: UnimplementedInstruiction(state); break;
      case 0x25: UnimplementedInstruiction(state); break;
      case 0x26: UnimplementedInstruiction(state); break;
      case 0x27: UnimplementedInstruiction(state); break;
      case 0x28: UnimplementedInstruiction(state); break;
      case 0x29: UnimplementedInstruiction(state); break;
      case 0x2a: UnimplementedInstruiction(state); break;
      case 0x2b: UnimplementedInstruiction(state); break;
      case 0x2c: UnimplementedInstruiction(state); break;
      case 0x2d: UnimplementedInstruiction(state); break;
      case 0x2e: UnimplementedInstruiction(state); break;
      case 0x2f: // CMA (not)
        state->a = ~state->a;
        break;
      case 0x30: UnimplementedInstruiction(state); break;
      case 0x31: UnimplementedInstruiction(state); break;
      case 0x32: UnimplementedInstruiction(state); break;
      case 0x33: UnimplementedInstruiction(state); break;
      case 0x34: UnimplementedInstruiction(state); break;
      case 0x35: UnimplementedInstruiction(state); break;
      case 0x36: UnimplementedInstruiction(state); break;
      case 0x37: UnimplementedInstruiction(state); break;
      case 0x38: UnimplementedInstruiction(state); break;
      case 0x39: UnimplementedInstruiction(state); break;
      case 0x3a: UnimplementedInstruiction(state); break;
      case 0x3b: UnimplementedInstruiction(state); break;
      case 0x3c: UnimplementedInstruiction(state); break;
      case 0x3d: UnimplementedInstruiction(state); break;
      case 0x3e: UnimplementedInstruiction(state); break;
      case 0x3f: UnimplementedInstruiction(state); break;
      case 0x40: UnimplementedInstruiction(state); break;
      case 0x41:  // MOV B,C | B <- C 
         state->b = state->c;
         break;
      case 0x42:  // MOV B,C | B <- D 
         state->b = state->d; 
         break;
      case 0x43:  // MOV B,C | B <- E 
         state->b = state->e; 
         break;
      case 0x44: UnimplementedInstruiction(state); break;
      case 0x45: UnimplementedInstruiction(state); break;
      case 0x46: UnimplementedInstruiction(state); break;
      case 0x47: UnimplementedInstruiction(state); break;
      case 0x48: UnimplementedInstruiction(state); break;
      case 0x49: UnimplementedInstruiction(state); break;
      case 0x4a: UnimplementedInstruiction(state); break;
      case 0x4b: UnimplementedInstruiction(state); break;
      case 0x4c: UnimplementedInstruiction(state); break;
      case 0x4d: UnimplementedInstruiction(state); break;
      case 0x4e: UnimplementedInstruiction(state); break;
      case 0x4f: UnimplementedInstruiction(state); break;
      case 0x50: UnimplementedInstruiction(state); break;
      case 0x51: UnimplementedInstruiction(state); break;
      case 0x52: UnimplementedInstruiction(state); break;
      case 0x53: UnimplementedInstruiction(state); break;
      case 0x54: UnimplementedInstruiction(state); break;
      case 0x55: UnimplementedInstruiction(state); break;
      case 0x56: UnimplementedInstruiction(state); break;
      case 0x57: UnimplementedInstruiction(state); break;
      case 0x58: UnimplementedInstruiction(state); break;
      case 0x59: UnimplementedInstruiction(state); break;
      case 0x5a: UnimplementedInstruiction(state); break;
      case 0x5b: UnimplementedInstruiction(state); break;
      case 0x5c: UnimplementedInstruiction(state); break;
      case 0x5d: UnimplementedInstruiction(state); break;
      case 0x5e: UnimplementedInstruiction(state); break;
      case 0x5f: UnimplementedInstruiction(state); break;
      case 0x60: UnimplementedInstruiction(state); break;
      case 0x61: UnimplementedInstruiction(state); break;
      case 0x62: UnimplementedInstruiction(state); break;
      case 0x63: UnimplementedInstruiction(state); break;
      case 0x64: UnimplementedInstruiction(state); break;
      case 0x65: UnimplementedInstruiction(state); break;
      case 0x66: UnimplementedInstruiction(state); break;
      case 0x67: UnimplementedInstruiction(state); break;
      case 0x68: UnimplementedInstruiction(state); break;
      case 0x69: UnimplementedInstruiction(state); break;
      case 0x6a: UnimplementedInstruiction(state); break;
      case 0x6b: UnimplementedInstruiction(state); break;
      case 0x6c: UnimplementedInstruiction(state); break;
      case 0x6d: UnimplementedInstruiction(state); break;
      case 0x6e: UnimplementedInstruiction(state); break;
      case 0x6f: UnimplementedInstruiction(state); break;
      case 0x70: UnimplementedInstruiction(state); break;
      case 0x71: UnimplementedInstruiction(state); break;
      case 0x72: UnimplementedInstruiction(state); break;
      case 0x73: UnimplementedInstruiction(state); break;
      case 0x74: UnimplementedInstruiction(state); break;
      case 0x75: UnimplementedInstruiction(state); break;
      case 0x76: UnimplementedInstruiction(state); break;
      case 0x77: UnimplementedInstruiction(state); break;
      case 0x78: UnimplementedInstruiction(state); break;
      case 0x79: UnimplementedInstruiction(state); break;
      case 0x7a: UnimplementedInstruiction(state); break;
      case 0x7b: UnimplementedInstruiction(state); break;
      case 0x7c: UnimplementedInstruiction(state); break;
      case 0x7d: UnimplementedInstruiction(state); break;
      case 0x7e: UnimplementedInstruiction(state); break;
      case 0x7f: UnimplementedInstruiction(state); break;
      case 0x80:  // ADD B | A <- A+B | A is a special register sometimes called accumulator.
         // Do the math with higher precision so we can capture the
         // carry out. 
         uint16_t ans = (uint16_t) state->a + (uint16_t) state->b;
         // Flags
         state->cc.z = ((ans & 0xff) == 0); // Zero Flag
         state->cc.s = ((ans & 0x80) != 0);  // Bit 7 Flag
         state->cc.cy = (ans > 0xff); // Carry Flag
         // The parity flag is handled by a subroutine.
         state->cc.p = parity(ans & 0xff, 8);
         
         state->a = ans & 0xff;
         break;
      case 0x81: // ADD C | A <- A+C 
        uint16_t ans= (uint16_t)state->a + (uint16_t)state->c;
        state->cc.z = ((ans & 0xff) == 0);
        state->cc.s = ((ans & 0x80) != 0);
        state->cc.cy = (ans > 0xff);
        state->cc.p = parity(ans & 0xff, 8);
        state->a = ans & 0xff;
        break;
      case 0x82: // ADD D | A <- A+D 
        uint16_t ans= (uint16_t)state->a + (uint16_t)state->d;
        state->cc.z = ((ans & 0xff) == 0);
        state->cc.s = ((ans & 0x80) != 0);
        state->cc.cy = (ans > 0xff);
        state->cc.p = parity(ans & 0xff, 8);
        state->a = ans & 0xff;
        break;
      case 0x83: // ADD D | A <- A+E 
        uint16_t ans= (uint16_t)state->a + (uint16_t)state->e;
        state->cc.z = ((ans & 0xff) == 0);
        state->cc.s = ((ans & 0x80) != 0);
        state->cc.cy = (ans > 0xff);
        state->cc.p = parity(ans & 0xff, 8);
        state->a = ans & 0xff;
        break;
      case 0x84: // ADD D | A <- A+H 
        uint16_t ans= (uint16_t)state->a + (uint16_t)state->h;
        state->cc.z = ((ans & 0xff) == 0);
        state->cc.s = ((ans & 0x80) != 0);
        state->cc.cy = (ans > 0xff);
        state->cc.p = parity(ans & 0xff, 8);
        state->a = ans & 0xff;
        break;
      case 0x85: // ADD D | A <- A+L 
        uint16_t ans= (uint16_t)state->a + (uint16_t)state->l;
        state->cc.z = ((ans & 0xff) == 0);
        state->cc.s = ((ans & 0x80) != 0);
        state->cc.cy = (ans > 0xff);
        state->cc.p = parity(ans & 0xff, 8);
        state->a = ans & 0xff;
        break;
      case 0x86: // ADD M | A <- A+(HL)
        uint16_t offset = (state->h << 8) | (state->l);
        uint16_t ans= (uint16_t)state->a + (uint16_t)state->memory[offset];
        state->cc.z = ((ans & 0xff) == 0);
        state->cc.s = ((ans & 0x80) != 0);
        state->cc.cy = (ans > 0xff);
        state->cc.p = parity(ans & 0xff, 8);
        state->a = ans & 0xff;
        break;
      case 0x87: // ADD A | A <- A+A 
        uint16_t ans= (uint16_t)state->a + (uint16_t)state->a;
        state->cc.z = ((ans & 0xff) == 0);
        state->cc.s = ((ans & 0x80) != 0);
        state->cc.cy = (ans > 0xff);
        state->cc.p = parity(ans & 0xff, 8);
        state->a = ans & 0xff;
        break;
      case 0x88: UnimplementedInstruiction(state); break;
      case 0x89: UnimplementedInstruiction(state); break;
      case 0x8a: UnimplementedInstruiction(state); break;
      case 0x8b: UnimplementedInstruiction(state); break;
      case 0x8c: UnimplementedInstruiction(state); break;
      case 0x8d: UnimplementedInstruiction(state); break;
      case 0x8e: UnimplementedInstruiction(state); break;
      case 0x8f: UnimplementedInstruiction(state); break;
      case 0x90: UnimplementedInstruiction(state); break;
      case 0x91: UnimplementedInstruiction(state); break;
      case 0x92: UnimplementedInstruiction(state); break;
      case 0x93: UnimplementedInstruiction(state); break;
      case 0x94: UnimplementedInstruiction(state); break;
      case 0x95: UnimplementedInstruiction(state); break;
      case 0x96: UnimplementedInstruiction(state); break;
      case 0x97: UnimplementedInstruiction(state); break;
      case 0x98: UnimplementedInstruiction(state); break;
      case 0x99: UnimplementedInstruiction(state); break;
      case 0x9a: UnimplementedInstruiction(state); break;
      case 0x9b: UnimplementedInstruiction(state); break;
      case 0x9c: UnimplementedInstruiction(state); break;
      case 0x9d: UnimplementedInstruiction(state); break;
      case 0x9e: UnimplementedInstruiction(state); break;
      case 0x9f: UnimplementedInstruiction(state); break;
      case 0xa0: UnimplementedInstruiction(state); break;
      case 0xa1: UnimplementedInstruiction(state); break;
      case 0xa2: UnimplementedInstruiction(state); break;
      case 0xa3: UnimplementedInstruiction(state); break;
      case 0xa4: UnimplementedInstruiction(state); break;
      case 0xa5: UnimplementedInstruiction(state); break;
      case 0xa6: UnimplementedInstruiction(state); break;
      case 0xa7: UnimplementedInstruiction(state); break;
      case 0xa8: UnimplementedInstruiction(state); break;
      case 0xa9: UnimplementedInstruiction(state); break;
      case 0xaa: UnimplementedInstruiction(state); break;
      case 0xab: UnimplementedInstruiction(state); break;
      case 0xac: UnimplementedInstruiction(state); break;
      case 0xad: UnimplementedInstruiction(state); break;
      case 0xae: UnimplementedInstruiction(state); break;
      case 0xaf: UnimplementedInstruiction(state); break;
      case 0xb0: UnimplementedInstruiction(state); break;
      case 0xb1: UnimplementedInstruiction(state); break;
      case 0xb2: UnimplementedInstruiction(state); break;
      case 0xb3: UnimplementedInstruiction(state); break;
      case 0xb4: UnimplementedInstruiction(state); break;
      case 0xb5: UnimplementedInstruiction(state); break;
      case 0xb6: UnimplementedInstruiction(state); break;
      case 0xb7: UnimplementedInstruiction(state); break;
      case 0xb8: UnimplementedInstruiction(state); break;
      case 0xb9: UnimplementedInstruiction(state); break;
      case 0xba: UnimplementedInstruiction(state); break;
      case 0xbb: UnimplementedInstruiction(state); break;
      case 0xbc: UnimplementedInstruiction(state); break;
      case 0xbd: UnimplementedInstruiction(state); break;
      case 0xbe: UnimplementedInstruiction(state); break;
      case 0xbf: UnimplementedInstruiction(state); break;
      case 0xc0: UnimplementedInstruiction(state); break;
      case 0xc1: // POP B
        state->c = state->memory[state->sp];
        state->b = state->memory[state->sp + 1];
        state->sp += 2;
        break;
      case 0xc2: // JNZ addr | JMP if zero flag
        if (0 == state->cc.z)
          state->pc = (opcode[2] << 8) | opcode[1];
        else
          state->pc += 2;
          break;
      case 0xc3: // JMP addr 
        state->pc = (opcode[2]<<8) | opcode[1];
        break;
      case 0xc4: UnimplementedInstruiction(state); break;
      case 0xc5: // PUSH B
        state->memory[state->sp-1] = state->b;
        state->memory[state->sp-2] = state->c;
        state->sp -= 2;
        break;
      case 0xc6:
        uint16_t ans = (uint16_t)state->a + (uint16_t)opcode[1];
        state->cc.z = ((ans & 0xff) == 0);
        state->cc.s = ((ans & 0x80) != 0);
        state->cc.cy = (ans > 0xff);
        state->cc.p = parity(ans & 0xff, 8);
        state->a = ans & 0xff;
        break;
      case 0xc7: UnimplementedInstruiction(state); break;
      case 0xc8: UnimplementedInstruiction(state); break;
      case 0xc9: // RET addr
        state->pc = state->memory[state->sp] | (state->memory[state->sp+1]<<8);
        state->sp += 2;
        break;
      case 0xca: UnimplementedInstruiction(state); break;
      case 0xcb: UnimplementedInstruiction(state); break;
      case 0xcc: UnimplementedInstruiction(state); break;
      case 0xcd: // CALL addr
        uint16_t ret = state->pc+2;
        state->memory[state->sp-1] = (ret >> 8) & 0xff;
        state->memory[state->sp-2] = (ret & 0xff);
        state->sp = state->sp - 2;
        state->pc = (opcode[2] << 8) | opcode[1];
        break;
      case 0xce: UnimplementedInstruiction(state); break;
      case 0xcf: UnimplementedInstruiction(state); break;
      case 0xd0: UnimplementedInstruiction(state); break;
      case 0xd1: UnimplementedInstruiction(state); break;
      case 0xd2: UnimplementedInstruiction(state); break;
      case 0xd3: UnimplementedInstruiction(state); break;
      case 0xd4: UnimplementedInstruiction(state); break;
      case 0xd5: UnimplementedInstruiction(state); break;
      case 0xd6: UnimplementedInstruiction(state); break;
      case 0xd7: UnimplementedInstruiction(state); break;
      case 0xd8: UnimplementedInstruiction(state); break;
      case 0xd9: UnimplementedInstruiction(state); break;
      case 0xda: UnimplementedInstruiction(state); break;
      case 0xdb: UnimplementedInstruiction(state); break;
      case 0xdc: UnimplementedInstruiction(state); break;
      case 0xdd: UnimplementedInstruiction(state); break;
      case 0xde: UnimplementedInstruiction(state); break;
      case 0xdf: UnimplementedInstruiction(state); break;
      case 0xe0: UnimplementedInstruiction(state); break;
      case 0xe1: UnimplementedInstruiction(state); break;
      case 0xe2: UnimplementedInstruiction(state); break;
      case 0xe3: UnimplementedInstruiction(state); break;
      case 0xe4: UnimplementedInstruiction(state); break;
      case 0xe5: UnimplementedInstruiction(state); break;
      case 0xe6: // ANI byte
        uint8_t ans = state->a & opcode[1];
        state->cc.z = (ans == 0);
        state->cc.s = (0x80 == (ans & 0x80));
        state->cc.p = parity(ans, 8);
        state->cc.cy = 0;
        state->a = ans;
        state->pc++;
        break;
      case 0xe7: UnimplementedInstruiction(state); break;
      case 0xe8: UnimplementedInstruiction(state); break;
      case 0xe9: UnimplementedInstruiction(state); break;
      case 0xea: UnimplementedInstruiction(state); break;
      case 0xeb: UnimplementedInstruiction(state); break;
      case 0xec: UnimplementedInstruiction(state); break;
      case 0xed: UnimplementedInstruiction(state); break;
      case 0xee: UnimplementedInstruiction(state); break;
      case 0xef: UnimplementedInstruiction(state); break;
      case 0xf0: UnimplementedInstruiction(state); break;
      case 0xf1: // POP PSW
        state->a = state->memory[state->sp+1];
        uint8_t psw = state->memory[state->sp];
        state->cc.cy = (0x01 == (psw & 0x01));
        state->cc.p = (0x04 == (psw & 0x04));
        state->cc.ac = (0x10 == (psw & 0x10));
        state->cc.z = (0x40 == (psw & 0x40));
        state->cc.s = (0x80 == (psw & 0x80)); 
      case 0xf2: UnimplementedInstruiction(state); break;
      case 0xf3: UnimplementedInstruiction(state); break;
      case 0xf4: UnimplementedInstruiction(state); break;
      case 0xf5: UnimplementedInstruiction(state); break;
      case 0xf6: UnimplementedInstruiction(state); break;
      case 0xf7: UnimplementedInstruiction(state); break;
      case 0xf8: UnimplementedInstruiction(state); break;
      case 0xf9: UnimplementedInstruiction(state); break;
      case 0xfa: UnimplementedInstruiction(state); break;
      case 0xfb: UnimplementedInstruiction(state); break;
      case 0xfc: UnimplementedInstruiction(state); break;
      case 0xfd: UnimplementedInstruiction(state); break;
      case 0xfe:
        uint8_t aux = state->a - opcode[1];
        state->cc.z = (aux == 0);
        state->cc.s = (0x80 == (aux & 0x80));
        state->cc.p = parity(aux, 8);
        state->cc.cy = (state->a < opcode[1]);
        state->pc++;
        break;
      case 0xff: UnimplementedInstruiction(state); break;
  }
  
  state->pc+=1;
}

int main(int argc, char *argv[]) {
   FILE *f = fopen(argv[1], "rb");
   if (f==NULL) {
      printf("Error: couldn't open %s\n", argv[1]);
      exit(1);
   }
   
   fseek(f, 0L, SEEK_END);
   int fsize = ftell(f);
   fseek(f, 0L, SEEK_SET);
   
   unsigned char *buffer=malloc(fsize);
   
   fread(buffer, fsize, 1, f);
   fclose(f);
   
   int pc = 0;
   
   while (pc < fsize) {
      pc += Disassemble(buffer, pc);
   }

   return 0;
}