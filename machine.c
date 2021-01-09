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
          if (py < 16 || py > 118 + 16)
            r = g = b = 255;
          else
            g = 255;
        } else if (px >= 16 && px <= 16 + 56) {
          g = 255;
        } else if (px >= 16 + 56 + 120 && px < 16 + 56 + 120 + 32) {
          r = 255;
        } else {
          r = g = b = 255;
        }
      }

      const uint8_t temp = px;
      px = py;
      py =  (HEIGHT - 1) - temp;
        
      machine->display.buffer[py][px][0] = r;
      machine->display.buffer[py][px][1] = g;
      machine->display.buffer[py][px][2] = b;
    }
  }
  
  const uint32_t pitch = sizeof(uint8_t) * 4 * WIDTH;
  if (SDL_UpdateTexture(
      machine->display.texture,
      NULL,
      &machine->display.buffer,
      pitch) != 0) SDL_Log("SDLError: %s", SDL_GetError());
}

Machine* InitMachine(char* filename) {
  Machine* machine = calloc(1, sizeof(Machine));
  machine->state = InitState();
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
      SDL_WINDOW_SHOWN);

  if (machine->display.window == NULL) {
    SDL_Log("SDLError: %s\n", SDL_GetError());
    exit(1);
  }
  
  SDL_SetWindowMinimumSize(machine->display.window, WIDTH, HEIGHT);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

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

void doCPU(Machine* machine) {
  int cycles = CYCLES_PER_UPDATE;
  int instruction_num = 0;
  while (cycles > 0) {
    cycles -= Emulate(machine->state);
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
      if (e.type == SDL_QUIT)
        quit = 1;
    }

    uint8_t time = SDL_GetTicks() - start;
    if (time < 0) continue;
    uint8_t sleepTime = TIME_PER_FRAME - time;
    
    if (sleepTime > 0) {
      SDL_Delay(sleepTime);
    }
    
    Update(machine);
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
