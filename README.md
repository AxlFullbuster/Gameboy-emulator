# Information  
This project focuses on creating a Gameboy emulator.

The emulator uses SDL for it's display and input, and is written in C++.  

The goal of this project is to create an emulator that can run games somewhat accurately, and to learn about CPU design.  

There are currently no plans to emulate sound and memory banking past the first type.    

# Current Progress  
Finished opcode implementation. Will need to test them using Blargg's test roms.  

# References  
[Gameboy Development Manual Version 1.1](https://archive.org/details/GameBoyProgManVer1.1)  
[GameLad Development Wiki](https://github.com/Dooskington/GameLad/wiki)  
[Gameboy Operation Code Table](https://izik1.github.io/gbops/)

# Compiling Instructions  
The program uses cmake to compile so here is what you'll need to do.  
In the directory "CMakeLists.txt" is located enter these commands:  
```
cmake .
make
build/Gameboy *Rom file name*
```
  
Note that the rom file should be in the same directory as the "CMakeLists.txt" file.
