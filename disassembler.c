#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Disassembler(unsigned char *codebuffer, int pc) {
  unsigned char *code = &codebuffer[pc];
  int opbytes = 1;
  printf("%04x ", pc);
  switch (*code) {
   case 0x00: printf("NOP"); break;
   // group BC
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
  }
}

int main(int argc, char *argv[]) {
  return 0;
}