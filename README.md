# Just Another Space Invaders(i8080) Emulator(C)

An Intel 8080 Emulator for the game Space Invaders. In the future I'll build a full blown i8080 emulator based on the one in here.

**REQUIREMENTS**
----
- SDL2
- Space Invaders ROM files

**DEBUGGING**
----
To debug I recommend GDB: 
```sh
# To debug the Space Invaders Machine
cc -g -O0 machine.c 8080.c -l SDL2 -o machine.out
gdb ./machine.out
```
Or run `8080.out` or `machine.out` with DEBUG=1 to set verbose.
```sh
# Machine out
DEBUG=1 ./machine.out path/to/invaders 
```

**TODO**
----
- Finish Space Invaders Emulation(display, audio, input, etc.).
  - Managed to finish display. Now going to input, audio, etc. 
  - Gotta change IN/OUT instructions.
- When I'm set to try out full intel 8080 CPU emulation, test using `coudiag` found in this [website](https://brainwagon.org/2011/09/08/more-on-my-8080-emulator/).

**RESOURCES**
----
- [emulator101](http://emulator101.com)
- Intel 8080 Programmers Manual(PDF)
- Intel 8080 Datasheet(PDF)
- Intel 8080 Architecture(PDF)
- Space Invaders ROM
- [r/EmuDev](https://www.reddit.com/r/emudev)  
- [SDL2 Wiki](https://wiki.libsdl.org/)
- [superzazu/invaders](https://github.com/superzazu/invaders)(a lot usefull when I was trying to get SDL2 to work with Surfaces, now I'm using Textures like him)

**AUTHOR**
----
Vínicius Gonçalves de Oliveira
