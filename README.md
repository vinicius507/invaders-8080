# Just Another Space Invaders(i8080) Emulator(C)

An Intel 8080 Emulator for the game Space Invaders. In the future I'll build a full blown i8080 emulator.

**REQUIREMENTS**
----
- SDL2
- Space Invaders ROM files

**DEBUGGING**
----
To debug I recommend GDB: 
```sh
cc -g -O0 8080.c -o 8080.out
gdb ./8080.out
```

**TODO**
----
- Finish Space Invaders Emulation(display, audio, input, etc.).
  - Can be done using SDL2. 
- When I'm set to try out full intel 8080 CPU emulation, test using `coudiag` found in this [website](https://brainwagon.org/2011/09/08/more-on-my-8080-emulator/).

**RESOURCES**
----
- [emulator101](http://emulator101.com).
- Intel 8080 Programmers Manual(PDF)
- Intel 8080 Datasheet(PDF)
- Intel 8080 Architecture(PDF)
- Space Invaders ROM
- cpudiag(compiled ASM to test every instruction on the 8080 emulator)  

**AUTHOR**
----
Vínicius Gonçalves de Oliveira
