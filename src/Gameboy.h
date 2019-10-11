#ifndef GAMEBOY_H
#define GAMEBOY_H

#include<stdint.h>


class Gameboy {
    private:
        uint8_t opcode;
        int cycles;
        
        
        uint8_t memory[0x10000];
        
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
        
        bool carrying;
        bool condition;
        bool left;
        bool right;
        bool halt;
        bool IME;
        bool reset7;
        bool reset0;
        
        
        //opcode functions
        void set_flag(int f);
        void unset_flag(int f);
        bool check_flag(int f);
        void flip_flag(int f);
        void op_cpl();
        void op_8bit_load(uint8_t r1, uint8_t r2);
        void op_16bit_load(Register dd, uint16_t nn);
        void op_8bit_add(uint8_t v);
        void op_8bit_subtract(uint8_t s);
        void op_8bit_and(uint8_t s);
        void op_8bit_or(uint8_t s);
        void op_8bit_xor(uint8_t s);
        void op_8bit_compare(uint8_t s);
        void op_16bit_add_to_hl(uint16_t ss);
        void op_rotate(uint8_t val);
        void op_shift(uint8_t val);
        void op_swap(uint8_t val);
        void op_bit(int b, uint8_t val);
        void op_set(int b, uint8_t val);
        void op_reset(int b, uint8_t val);
        void op_jump();
        void op_jump_signed(int8_t e);
        void op_call();
        void op_return();
        void op_restart(uint8_t p);
        void op_push(Register qq);
        void op_pop(Register qq);
        void op_DAA();
        
    public:
        Gameboy();
        ~Gameboy();
        
        bool lcd_set;
        void lcd_control();
        
        void debug();
        void print_char();
        void tile_data();
        void emuLoop();
        bool loadGame(const char* filename);
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);    
};

#endif