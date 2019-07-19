#ifndef GAMEBOY_H
#define GAMEBOY_H

#include<stdint.h>

class Gameboy{
    private:
        uint8_t opcode;
        uint16_t pc;
        uint16_t sp;
        int cycles;
        
        uint8_t memory[65536];
        uint8_t rom[2097152];
        uint8_t ram[32768];
        
        union Register{
            struct{
                uint8_t low;
                uint8_t high;
            };
            uint16_t full;
        };
        
        Register AF;
        Register BC;
        Register DE;
        Register HL;
        
        void initialize();
        void emulateCycle();
        void decode1(uint8_t opcode);
        void decode2(uint8_t opcode);
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);
    
    public:
        Gameboy();
        ~Gameboy();
        
        unsigned char gfx[160*144];
        
        void emuLoop();
        void loadGame();
       
};

#endif