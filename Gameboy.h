#ifndef GAMEBOY_H
#define GAMEBOY_H

#include<stdint.h>

class Gameboy{
    private:
        uint8_t opcode;
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
        Register PC;
        
        void initialize();
        void emulateCycle();
        void decode1(uint8_t opcode);
        void decode2(uint8_t opcode);
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);
        
        //opcode functions (I'll try and find better names for them)
        void op_8bit_load_registers(uint8_t r1, uint8_t r2);
        void op_8bit_load_immediate(uint8_t r, uint8_t n);
        void op_8bit_load_into_A(uint8_t n);
        void op_8bit_load_into_immediate(uint8_t n);
        void op_16bit_load_into_A(uint8_t nn);
        void op_16bit_load_A_into_immediate(uint8_t nn);
        void op_16bit_load(uint16_t dd, uint16_t nn);
        void op_16bit_load_into_sp(uint16_t nn);
        void op_8bit_add_register(uint8_t r);
        void op_8bit_add_immediate(uint8_t n);
        void op_8bit_subtract(uint8_t s);
        void op_8bit_add_carry(uint8_t s);
        void op_8bit_subtract_carry(uint8_t s);
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