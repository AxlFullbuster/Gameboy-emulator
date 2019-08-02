#ifndef GAMEBOY_H
#define GAMEBOY_H

#include<stdint.h>


class Gameboy{
    private:
        uint8_t opcode;
        uint16_t pc;
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
        Register SP;
        
        void initialize();
        void emulateCycle();
        void decode1(uint8_t opcode);
        void decode2(uint8_t opcode);
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);
        
        bool carrying;
        bool setZ;    bool unsetZ;
        bool setN;    bool unsetN;
        bool setH;    bool unsetH;
        bool setC;    bool unsetC;
        
        
       
        //opcode functions
        void flags();
        void op_8bit_load(uint8_t r1, uint8_t r2);
        void op_16bit_load(Register dd, uint16_t nn);
        void op_8bit_add(uint8_t v);
        void op_8bit_subtract(uint8_t s);
        void op_8bit_and(uint8_t s);
        void op_8bit_or(uint8_t s);
        void op_8bit_xor(uint8_t s);
        void op_8bit_compare(uint8_t s);
        void op_16bit_add_to_hl(uint16_t ss);
    
    public:
        Gameboy();
        ~Gameboy();
        
        unsigned char gfx[160*144];
        
        void emuLoop();
        void loadGame();
       
};

#endif