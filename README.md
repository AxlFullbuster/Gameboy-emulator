# Information  
This project focuses on creating a Gameboy emulator.

The emulator uses SDL for it's display and input, and is written in C++.  

The goal of this project is to create an emulator that can run games somewhat accurately, and to learn about CPU design.  

There are currently no plans to emulate sound and memory banking past the first type.    

# Current Progress  
Currently debugging timer and MBC switch.

#Issues
###Memory Banking
The MBC switch is currently not working correctly and as of this commit only these 2 roms run on the emulator without issue:
"Tetris", Blargg's "cpu_instrs".

The roms that are not running correctly are the following:
"The Amazing SpiderMan", "Aladdin", "Zelda Link's Awakening".

###Timer
The Timer is also not fully implemented due to issues with TIMA, TMA, and TAC registers. The DIV register is also not
correct according to test roms. But is able to run well enough for Tetris to randomize it's blocks.

# References/Resources    
[Gameboy Development Manual Version 1.1](https://archive.org/details/GameBoyProgManVer1.1)  
[GameLad Development Wiki](https://github.com/Dooskington/GameLad/wiki)  
[Gameboy Operation Code Table](https://izik1.github.io/gbops/)  
[Blargg's Test Roms](https://github.com/retrio/gb-test-roms)  
[Mooneye Test Roms](https://github.com/Gekkio/mooneye-gb)  
[tcbrindle's CMake Scripts](https://github.com/tcbrindle/sdl2-cmake-scripts)  
[imgui library](https://github.com/ocornut/imgui)  
[imgui_sdl library](https://github.com/Tyyppi77/imgui_sdl)  

# Compiling Instructions  
The program uses cmake to compile so here is what you'll need to do.  
In the root directory of this project type in these commands:  
```
cmake .
make
build/Gameboy *Rom file name*
```
  
Note that the rom file should also be in the root directory when entering the commands above.
