# Information  
This project focuses on creating a Gameboy emulator.

The emulator uses SDL for it's display and input, ImGui for debugging information, and is written in C++.  

The goal of this project is to create an emulator that can run games somewhat accurately, and to learn about CPU design.  

There are currently no plans to emulate sound and memory banking past the first type.

# Current Progress  
Completed for the time being. Check "Issues" to see what is not implemented.

# Issues
### Memory Banking
The MBC switch is currently not working correctly and as of this commit the emulator can only run "Tetris"
without issues.  

The roms that are not running correctly are the following:
"The Amazing SpiderMan", "Aladdin", "Zelda Link's Awakening".

### Timer
The Timer is also not fully implemented due to issues with TIMA, TMA, and TAC registers. The DIV register is also not
correct according to test roms. But is able to run well enough for Tetris to randomize it's blocks.

# Documentation
The documentation of this code was written with Doxygen in mind. A doxyfile is provided if you would like to see it,
please see [Doxygen's](http://www.doxygen.nl/manual/index.html) documentation for installing the program and getting it
to run.

# Controls
This is the current key mapping for the emulator:  
A - Z key.  
B - X key.  
Start - RightShift key.  
Select - Space key.  
Directional Input - Directional Keys.  

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
