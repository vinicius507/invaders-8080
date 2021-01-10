#include "machine.h"
#include <stdio.h>
#include <stdlib.h>

void Update(Machine* machine) {
  for (int i = 0; i < 256 * 224 / 8; i++) {
    const int y = i * 8 / 256;
    const int baseX = (i * 8) % 256;
    const uint8_t byte = machine->state->memory[VRAM_ADDR + i];

    for (uint8_t bit = 0; bit < 8; bit++) {
      int px = baseX + bit;
      int py = y;
      const uint8_t isSet = (byte >> bit) & 1;
      uint8_t r = 0, g = 0, b = 0;

      if (isSet) {
        if (px < 16) {
          if (py < 16 || py > 118 + 16) {
            r = 255;
            g = 255;
            b = 255;
          } else {
            g = 255;
          }
        } else if (px >= 16 && px <= 16 + 56) {
          g = 255;
        } else if (px >= 16 + 56 + 120 && px < 16 + 56 + 120 + 32) {
          r = 255;
        } else {
          r = 255;
          g = 255;
          b = 255;
        }
      }

      const uint8_t temp = px;
      px = py;
      py = (HEIGHT - 1) - temp;

      machine->display.buffer[py][px][0] = r;
      machine->display.buffer[py][px][1] = g;
      machine->display.buffer[py][px][2] = b;
    }
  }

  const uint32_t pitch = sizeof(uint8_t) * 4 * WIDTH;
  SDL_UpdateTexture(
      machine->display.texture,
      NULL,
      &machine->display.buffer,
      pitch) != 0;
}

Machine* InitMachine(char* filename) {
  Machine* machine = calloc(1, sizeof(Machine));
  machine->state = InitState();
  machine->nextInterrupt = 0x08;
  machine->port1 = 1<<3;
  machine->port2 = 0;
  machine->shift0 = 0;
  machine->shift1 = 0;
  machine->shiftOffset = 0;
  machine->lastOutPort3 = 0;
  machine->lastOutPort5 = 0;
  ReadFile(machine->state, filename);

  machine->display.window = NULL;
  machine->display.renderer = NULL;
  machine->display.texture = NULL;
  memset(machine->display.buffer, 0, sizeof(machine->display.buffer));

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("SDLError: %s\n", SDL_GetError());
    exit(1);
  }

  machine->display.window = SDL_CreateWindow(
      "Space Invaders",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WIDTH * 2,
      HEIGHT * 2,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (machine->display.window == NULL) {
    SDL_Log("SDLError: %s\n", SDL_GetError());
    exit(1);
  }
  
  SDL_SetWindowMinimumSize(machine->display.window, WIDTH, HEIGHT);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");

  machine->display.renderer = SDL_CreateRenderer(
      machine->display.window, -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (machine->display.renderer == NULL) {
    SDL_Log("SDLError: %s\n", SDL_GetError());
    exit(1);
  }
  
  SDL_RenderSetLogicalSize(machine->display.renderer, WIDTH, HEIGHT);
  SDL_RendererInfo renderer_info;
  SDL_GetRendererInfo(machine->display.renderer, &renderer_info);
  SDL_Log("Using renderer %s", renderer_info.name);

  machine->display.texture = SDL_CreateTexture(
      machine->display.renderer,
      SDL_PIXELFORMAT_RGBA32,
      SDL_TEXTUREACCESS_STREAMING,
      WIDTH,
      HEIGHT);
  
  if (machine->display.texture == NULL) {
    SDL_Log("SDLError: %s\n", SDL_GetError());
    exit(1);
  }
  
  Update(machine);

  return machine;
}

void KillMachine(Machine* machine) {
  SDL_DestroyTexture(machine->display.texture);
  SDL_DestroyRenderer(machine->display.renderer);
  SDL_DestroyWindow(machine->display.window);
  SDL_Quit();
}

void MachineIN (Machine* machine, uint8_t port) {
  switch(port) {
    case 1: 
      machine->state->a = machine->port1;
      break;
    case 2:
      machine->state->a = machine->port2;
      break;
    case 3:
    {
      uint16_t v = (machine->shift1<<8) | machine->shift0;
      machine->state->a = (v>>(8-machine->shiftOffset)) & 0xff;
    } break;
    default:
      printf("Error: Unknown IN port %02X\n", port);
      break;
  }

  machine->state->pc++;
}

void MachineOUT (Machine* machine, uint8_t port) {
  switch(port) {
    case 2:
      machine->shiftOffset = machine->state->a&0x7;
      break;
    case 3: break;
    case 4:
      machine->shift0 = machine->shift1;
      machine->shift1 = machine->state->a;
      break;
    case 5: break;
    case 6: break;
    default:
      printf("Error: Unknown OUT port %02X\n", port);
      break;
  }

  machine->state->pc++;
}

void doCPU(Machine* machine) {
  int cycles = CYCLES_PER_UPDATE;
  int instruction_num = 0;
  while (cycles > 0) {
    uint8_t* opcode = &machine->state->memory[machine->state->pc];
    switch (*opcode) {
      case 0xdb: // IN port 
      {
        uint8_t port = opcode[1];
        MachineIN(machine, port);
        cycles -= 3;
      } break;
      case 0xd3: // OUT port
      {
        uint8_t port = opcode[1];
        MachineOUT(machine, port);
        machine->state->pc++;
        cycles -= 3;
      } break;
      default:
        cycles -= Emulate(machine->state);
        break;
    }

    if (DEBUG) {
      instruction_num++;
      printf("Instruction: %d\n", instruction_num);
      printf("\ta %02X bc %02X%02X de %02X%02X hl %02X%02X pc %04X sp %04X memory[sp] %04X\n", machine->state->a, machine->state->b, machine->state->c, machine->state->d, machine->state->e, machine->state->h, machine->state->l, machine->state->pc, machine->state->sp, ((machine->state->memory[machine->state->sp + 1] << 8) | (machine->state->memory[machine->state->sp] - 2)));
      printf("\t%s %s %s %s %s %s\n", machine->state->cc.z ? "z" : ".", machine->state->cc.s ? "s" : ".", machine->state->cc.cy ? "cy" : ".", machine->state->cc.p ? "p" : ".", machine->state->cc.ac ? "ac" : ".", machine->state->int_enable ? "i" : ".");
    }
  }
}

int main (int argc, char* argv[]) {
  Machine* machine = InitMachine(argv[1]);
  
  int quit = 0;
  SDL_Event e;

  while (!quit) {
    uint8_t start = SDL_GetTicks();
    doCPU(machine);

    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_c:
            machine->port1 = machine->port1 | 1;  // coin
            break;
          case SDLK_2:
            machine->port1 = machine->port1 | 1<<1;  // P2 start butotn
            break;
          case SDLK_RETURN:
            machine->port1 = machine->port1 | 1<<2;  // P1 start button
            break;
          case SDLK_SPACE:
            machine->port1 = machine->port1 | 1<<4;  // P1,2 shoot
            machine->port2 = machine->port2 | 1<<4;
            break;
          case SDLK_LEFT:
            machine->port1 = machine->port1 | 1<<5;  // P1,2 joystick left
            machine->port2 = machine->port2 | 1<<5;
            break;
          case SDLK_RIGHT:
            machine->port1 = machine->port1 | 1<<6;  // P1,2 joystick right
            machine->port2 = machine->port2 | 1<<6;
            break;
          case SDLK_t:
            machine->port2 |= 1 << 2;  //tilt (?)
            break;
        }
      } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
          case SDLK_c:
            machine->port1 = machine->port1 & 0xfe;  // coin
            break;
          case SDLK_2:
            machine->port1 = machine->port1 & 0xfd;  // P2 start butotn
            break;
          case SDLK_RETURN:
            machine->port1 = machine->port1 & 0xfb;  // P1 start button
            break;
          case SDLK_SPACE:
            machine->port1 = machine->port1 & 0xef;  // P1,2 shoot
            machine->port2 = machine->port2 & 0xef;
            break;
          case SDLK_LEFT:
            machine->port1 = machine->port1 & 0xdf;  // P1,2 joystick left
            machine->port2 = machine->port2 & 0xdf;
            break;
          case SDLK_RIGHT:
            machine->port1 = machine->port1 & 0xbf;  // P1,2 joystick right
            machine->port2 = machine->port2 & 0xbf;
            break;
          case SDLK_t:
            machine->port2 = machine->port2 & 0xfb;  //tilt (?)
            break;
        }
      }
    }

    uint8_t time = SDL_GetTicks() - start;
    if (time < 0) exit(1);
    uint8_t sleepTime = TIME_PER_FRAME - time;

    if (sleepTime > 0) {
      SDL_Delay(sleepTime);
      Update(machine);
      if (machine->state->int_enable) {
        GenerateInterrupt(machine->state, machine->nextInterrupt);
        machine->nextInterrupt = machine->nextInterrupt == 0x08 ? 0x10 : 0x08;
      }
    }

    SDL_RenderClear(machine->display.renderer);
    SDL_RenderCopy(
        machine->display.renderer,
        machine->display.texture,
        NULL,
        NULL);
    SDL_RenderPresent(machine->display.renderer);
  }
  
  KillMachine(machine);

  return 0;
}
