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
    
    pc = 0x100;
    sp = 0xFFFE;
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
    opcode = read(pc);
    if(opcode != 0xCB){
        decode1(opcode);
    } else {
        opcode = read(pc++);
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

