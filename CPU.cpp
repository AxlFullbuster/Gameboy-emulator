#include "Gameboy.h"


Gameboy::Gameboy(){
    //do nothing
}

Gameboy::~Gameboy(){
    //do nothing
}

void Gameboy::initialize(){
    //clear memory
    for(int i = 0; i < 65536; i++){
        memory[i] = 0;
    }
    
    PC.full = 0x100;
    SP.full = 0xFFFE;
    AF.full = 0x01B0;
    BC.full = 0x0013;
    DE.full = 0x00D8;
    HL.full = 0x014D;

    memory[0xFF10] = 0x80; 
    memory[0xFF11] = 0xBF; 
    memory[0xFF12] = 0xF3; 
    memory[0xFF14] = 0xBF; 
    memory[0xFF16] = 0x3F; 
    memory[0xFF17] = 0x00; 
    memory[0xFF19] = 0xBF; 
    memory[0xFF1A] = 0x7F; 
    memory[0xFF1B] = 0xFF; 
    memory[0xFF1C] = 0x9F; 
    memory[0xFF1E] = 0xBF; 
    memory[0xFF20] = 0xFF;
    memory[0xFF23] = 0xBF; 
    memory[0xFF24] = 0x77; 
    memory[0xFF25] = 0xF3; 
    memory[0xFF26] = 0xF1;
    memory[0xFF40] = 0x91; 
    memory[0xFF47] = 0xFC; 
    memory[0xFF48] = 0xFF; 
    memory[0xFF49] = 0xFF; 
}


void Gameboy::emuLoop(){
    cycles = 0;
    while (cycles < 70256){
        emulateCycle();
        //draw graphics
        //handle interputs
        //other junk
    }
    
    cycles = 0;
    //refresh screen;
}

void Gameboy::emulateCycle(){
    opcode = read(PC.full);
    if(opcode != 0xCB){
        decode1(opcode);
    } else {
        opcode = read(PC.full++);
        decode2(opcode);
    }
}

uint8_t Gameboy::read(uint16_t address){
    if(address >= 0x0000 && address <= 0x7000){
        //reading from 32kb rom
    }
    
    if(address >= 0x8000 && address <= 0x9FFF){
        //reading from GPU address space
    }
    
    if(address >= 0xA000 && address <= 0xB000){
        //reading from external ram
    }
    
    if(address >= 0xC000 && address <= 0xDFFF){
        //reading from internal ram
    }
    
    if(address >= 0xE000 &&  address <= 0xFDFF){
        //reading from echo ram
    }
    
    if(address >= 0xFE00 && address <=0xFE9F){
        //reading from sprite attribute table
    }
    
    if(address >= 0xFEA0 && address <= 0x0FEFF){
        //don't do anything here the gameboy will get mad
    }
    
    if (address >= 0xFF00 && address <= 0xFF7F){
        //various hardware I/O registers
    }
    
    if (address >= 0xFF80 && address <= 0xFFFE){
        //high ram and where the stack lives
    }
    
    if (address == 0xFFFF){
        //enable interupt flag
    }
    
    return memory[address];
}


void Gameboy::write(uint16_t address, uint8_t data){
    if(address >= 0x0000 && address <= 0x7000){
        //you can't write to the rom
    }
    
    if(address >= 0x8000 && address <= 0x9FFF){
        //write to GPU address space
    }
    
    if(address >= 0xA000 && address <= 0xB000){
        //write to external ram
    }
    
    if(address >= 0xC000 && address <= 0xDFFF){
        //write to internal ram
    }
    
    if(address >= 0xE000 &&  address <= 0xFDFF){
        //write to echo ram
    }
    
    if(address >= 0xFE00 && address <=0xFE9F){
        //write to sprite attribute table
    }
    
    if(address >= 0xFEA0 && address <= 0x0FEFF){
        //don't do anything here the gameboy will get mad
    }
    
    if (address >= 0xFF00 && address <= 0xFF7F){
        //write to hardware I/O registers
    }
    
    if (address >= 0xFF80 && address <= 0xFFFE){
        //write to high ram/stack
    }
    
    if (address == 0xFFFF){
        //enable interupt flag
    }
}

void Gameboy::loadGame(){
    //load game into emulator
}

void Gameboy::op_8bit_load_registers(uint8_t r1, uint8_t r2){
    r1 = r2;
}

void Gameboy::op_8bit_load_immediate(uint8_t r, uint8_t n){
    n = r;
}

 void Gameboy::op_8bit_load_into_A(uint8_t n){
    AF.high = n;
 }
 
 void Gameboy::op_8bit_load_into_immediate(uint8_t n){
    n = AF.high;
 }
 
 void Gameboy::op_16bit_load_into_A(uint8_t nn){
    AF.high = nn;
 }
 
 void Gameboy::op_16bit_load_A_into_immediate(uint8_t nn){
    nn = AF.high;
 }
 
 void Gameboy::op_16bit_load(uint16_t dd, uint16_t nn){
    dd = nn;
 }
 
 void Gameboy::op_16bit_load_into_sp(uint16_t nn){
    SP.low = nn;
    SP.high = nn++;
 }
 
 void Gameboy::op_8bit_add_register(uint8_t r){
    AF.high += r;
 }
 
 void Gameboy::op_8bit_add_immediate(uint8_t n){
    AF.high += n;
 }
 
 void Gameboy::op_8bit_add_carry(uint8_t s){
    if(AF.high + s > 0xFF){
        AF.low |= 4;
        AF.high += s++;
        if( (AF.high + s) == 0){
            AF.low |= 7;
        }
        if(((AF.high & 0xf) + (s & 0xf)) & 0x10 == 0x10){
            AF.low |= 5;
        }
    }else {
        AF.high += s;
        if( (AF.high + s) == 0){
            AF.low |= 7;
        }
        if(((AF.high & 0xf) + (s & 0xf)) & 0x10 == 0x10){
            AF.low |= 5;
        }
    }
 }
 
 void Gameboy::op_8bit_subtract(uint8_t s){
        AF.low |= 6;
        AF.high -= s;
        if( (AF.high - s) == 0){
            AF.low |= 7;
        }
        if(((AF.high & 0xf) + (s & 0xf)) & 0x10 == 0x10){
            AF.low |= 5;
        }
 }
 
 void Gameboy::op_8bit_subtract_carry(uint8_t s){
    AF.low |= 6;
    if(AF.high - s > 0xFF){
        AF.low |= 4;
        AF.high -= s++;
        if( (AF.high - s) == 0){
            AF.low |= 7;
        }
        if(((AF.high & 0xf) - (s & 0xf)) & 0x10 == 0x10){
            AF.low |= 5;
        }
    }else {
        AF.high -= s;
        if( (AF.high - s) == 0){
            AF.low |= 7;
        }
        if(((AF.high & 0xf) - (s & 0xf)) & 0x10 == 0x10){
            AF.low |= 5;
        }
    }
 }
 
 void Gameboy::op_8bit_and(uint8_t s){
    AF.high &= s;
 }
 
 void Gameboy::op_8bit_or(uint8_t s){
    AF.high |= s;
 }
 
 void Gameboy::op_8bit_xor(uint8_t s){
    AF.high ^= s;
 }
 
 void Gameboy::op_8bit_compare(uint8_t s){
    if( (AF.high - s) == 0){
            AF.low |= 7;
    } 
 }
 
 void Gameboy::op_16bit_add_to_hl(uint16_t ss){
    HL.full += ss;
 }
 
 

