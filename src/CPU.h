#ifndef CPU_H
#define CPU_H

#include<stdint.h>
#include<vector>

//! A class for the gameboy CPU

/*!
 This class is designed to mimic the Gameboy's CPU fuctions. It's ALU, Memory Map, and instructions
 are implemented in this class.
 */

class CPU{
    private:
        //! Holds the current operation code value
        uint8_t opcode;
        //! Holds the total amount of clock cycles executed
        int cycles;
        
        //! Represents the entire memory map for the Gameboy
        /*!
         *The Gameboy's memory map is split into diffrent sections, each section is responsible
         *for storing or calculating data.
         *
         *These are the diffrent sections of the memory map:
         *
         *0x0000-0x00FF: This portion of memory is the restart and interrupt vectors in memory.
         *If an interrupt has occured or one of the restart methods have executed. The program counter
         *will jump somewhere into this portion of memory.
         *
         *0x100-0x14F: This is the cartridge header area of memory. It stores memory bank data and the total size of the ROM.
         *It also stores the title of the game in the emulator. Along with a checksum value for the bios ROM.
         *
         *0x150-0x3000: This is the 1st bank of ROM data. It just holds the data stored in this specifc address range of the ROM file
         *
         *0x4000-0x7FFF: This portion of memory holds a certain bank of ROM data. If the ROM is using a memory bank controller
         *you can switch out portions of memory in the rom into this address range. If the ROM doesn't have a memory bank controller
         *the rest of the rom data is just stored into here.
         *
         *0x8000-0x97FF: The bytes stored in this address range represent character data for the GPU.
         *
         *0x9800-0x9BFF: The bytes stored in this address range represent the first portion of background data for the GPU.
         *
         *0x9C00-0x9FFF: The bytes stored in this address range represent the second portion of background data for the GPU.
         *
         *0xA000-0xBFFF: This portion of memory holds RAM stored in the cartridge if there is any.
         *
         *0xC000-0xDFFF: This portion of memory holds internal RAM used by the Gameboy.
         *
         *0XE000-0xFDFF: This portion of memory is echo data for the internal RAM. It's basically a copy of what's in the internal RAM
         *portion of memory.
         *
         *0xFE00-0xFE9F: The bytes stored in this address range represent OAM(Object Attribute Memory) data. AKA sprite data for the GPU.
         *
         *0xFEA0-0xFEFF: Legends say that this portion of memory should never be used. Only our lord and saviour Nintendo can tell us
         *why that is.
         *
         *0xFF00-0xFF7F: This portion of memory contains I/O registers that are used by the CPU.
         *
         *0xFF80-0xFFFE: This portion of memory is the HighRAM area, usually values that are pushed by the push method are stored here. But that
         *is not always the case.
         *
         *0xFFFF: This is the interrupt enable register, it controls interrupts.
         */
        uint8_t memory[0x10000];
        
        
        //! A union type to hold register data
        /*!
         This union type combines two 8-bit long unsigned int values to create
         an unsigned 16-bit integer. There are 8 8-bit integer registers in the gameboy
         CPU, and they can be combined to form a 16-bit registers that holds the value of
         the two registers.
         */
        union Register{
            struct{
                //! Holds the lower order 8-bit value
                uint8_t low;
                //! Holds the high order 8-bit value
                uint8_t high;
            };
            //! The entire 16-bit register value
            uint16_t full;
        };
        
        //! A Register type for the AF register
        Register AF;
        //! A Register type for the BC register
        Register BC;
        //! A Register type for the DE register
        Register DE;
        //! A Register type for the HL register
        Register HL;
        //! A Register type for the Stack Pointer
        Register SP;
        //! A Register type for the Program Counter
        Register PC;        
        
        //! Initializes the CPU to specifc values
        void initialize();
        
        //! Decodes the unprefixed operation codes in the CPU
        void decode1(uint8_t opcode);
        
        //! Decodes the prefixed operation codes in the CPU
        void decode2(uint8_t opcode);
        
        //! Signifies a carry operation
        bool carrying;
        //! Signifies a condition requirement
        bool condition;
        //! Signifies to focus on the left side of an integer
        bool left;
         //! Signifies to focus on the right side of an integer
        bool right;
        //! Signifies to execute an interrupt
        bool IME;
        //! Signifies to reset the 7th bit of a number
        bool reset7;
        //! Signifies to reset the 0th bit of a number
        bool reset0;
        //! Signifies to keep the original value of the 7th bit
        bool keep7;
        //! Signifies the operation is using the A Register
        bool A;         
        //! Signifies th STOP instruction has been executed
        bool stop;
        //! Signifies the HALT instruction has been executed
        bool halt;
        //! Signifies that input will focus on the directional keys
        bool direction;
        //! Signifies that input will focus on the button keys
        bool button;
        
        //! A counter for the Divider Register
        int div_counter;
        
        
        
        //! Executes an interrupt request
        void execute_interrupt(int req);
        
        //! Conducts Direct Memory Access
        void DMA(uint8_t data);
        
        //! Increments the divider register
        void div_inc();
        
        //! loads the bios data into the memory map
        void load_bios();
        //! clears all the memory
        void clearMemory();
        //! processes input
        void input();
        
        
        
        //! sets a flag register
        void set_flag(int f);
        //! unsets a flag register
        void unset_flag(int f);
        //! checks if a flag register is set or unset
        bool check_flag(int f);
        //! flips the flag value
        void flip_flag(int f);
        //! sets flag values to prefixed conditions
        void set_pre_flags(uint8_t &val);
        //! sets flag values to unprefixed condtions
        void set_unpre_flags();
        //! Takes the 1's compliment of the A register 
        void op_cpl();
        //! increments an 8-bit value
        void op_inc(uint8_t &val);
        //! decrements an 8-bit value
        void op_dec(uint8_t &val);
        //! loads an 8-bit integer into another 8-bit integer
        void op_8bit_load(uint8_t &r1, uint8_t r2);
        //! loads a 16-bit integer
        void op_16bit_load(Register &r);
        //! does a addition operation
        void op_8bit_add(uint8_t v);
        //! does a subtraction operaion
        void op_8bit_subtract(uint8_t s);
        //! does an and operation
        void op_8bit_and(uint8_t s);
        //! does an or operation
        void op_8bit_or(uint8_t s);
        //! does an xor operation
        void op_8bit_xor(uint8_t s);
        //! does a compare operation
        void op_8bit_compare(uint8_t s);
        //! adds two 16-bit integers
        void op_16bit_add(uint16_t& r1, uint16_t r2);
        //! rotates the bits in a 8-bit value
        void op_rotate(uint8_t &val);
        //! rotates the bits in the A register
        void op_rotate_A();
        //! shifts the bits in a 8-bit vaalue
        void op_shift(uint8_t &val);
        //! swaps the nibbles in a 8-bit integer
        void op_swap(uint8_t &val);
        //! checks if a specific bit value in a 8-bit integer is set
        void op_bit(int b, uint8_t &val);
        //! sets a specific bit value in a 8-bit integer
        void op_set(int b, uint8_t &val);
        //! unsets a specific bit value in a 8-bit integer
        void op_reset(int b, uint8_t &val);
        //! Has the Program Counter jump to a certain location in memory
        void op_jump();
        //! Has the Program Counter jump to a value specificed by a signed bit value
        void op_jump_signed(int8_t e);
        //! Has the program Counter jump and saves the original value onto the stack
        void op_call();
        //! Resets the Program Counter to its value before a jump
        void op_return();
        //! Resets the Program Counter to the beginning of the memory map
        void op_restart(uint8_t p);
        //! Pushes a value onto the stack
        void op_push(Register qq);
        //! Pops a value from the stack
        void op_pop(Register &qq);
        //! Runs the DAA instruction
        void op_DAA();
        
        
     
    public:
        CPU();
        ~CPU();
        
        //! boolean array that holds button inputs
        bool buttons[8];
        
        //! contains the cycle execution time for gpu and grand_cycles
        int timing;
        
        //! loads a rom file into the ROM portion of the memory map
        bool loadGame(const char* filename);
        //! checks to see if the bios rom was loaded into the memory mape
        bool bios;
        //! checks to see if the user is trying to debug the emulator
        bool debug;
        
        //! emulates the fetch, decode, and execute cycle of the cpu
        void emulateCycle();
        //! adds 1 to the scanline register in the memory map
        void increment_scanline();
        //! updates timer data in the memory map
        void update_timers();
        //! stores an interrupt request in the IF register
        void request_interrupt(int req);
        //! checks to see if an interrupt request was put into the IF register
        void check_interrupt();
    
        //! calculates execution time and stores it in the timer variable
        int get_cycles(int prev);
       
        /** @defgroup debug Debugging Group
         * This group holds debugging methods.
         * They either return register values,
         * show if interrupts are enabled,
         * show the emulator is in a halt state,
         * or returns the previously executed opcode.
         * @{
         */
        uint16_t get_AF();
        uint16_t get_BC();
        uint16_t get_DE();
        uint16_t get_HL();
        uint16_t get_PC();
        uint16_t get_SP();
        uint8_t * get_mem();
        uint8_t get_A();
        uint8_t get_B();
        uint8_t get_C();
        uint8_t get_D();
        uint8_t get_E();
        uint8_t get_H();
        uint8_t get_L();
        uint8_t get_F();
        uint8_t get_OP();
        bool get_ime();
        bool get_halt();
        /** @} */ // end of debug
        
        //! Reads a value in the memory map and returns it
        uint8_t read(uint16_t address);
        //! Writes a value into the memory map
        void write(uint16_t address, uint8_t data);
};

#endif