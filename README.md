# Information  
This project focuses on creating a Gameboy emulator.

The emulator uses SDL for it's display and input, and is written in C++.  

The goal of this project is to create an emulator that can run games somewhat accurately, and to learn about CPU design.  

There are currently no plans to emulate sound and memory banking past the first type.    

# Current Progress  
Timers and Interrupts have been implemented. Currently testing the emulator with Blargg's test roms.  

# References/Resources    
[Gameboy Development Manual Version 1.1](https://archive.org/details/GameBoyProgManVer1.1)  
[GameLad Development Wiki](https://github.com/Dooskington/GameLad/wiki)  
[Gameboy Operation Code Table](https://izik1.github.io/gbops/)  
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
