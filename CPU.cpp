#include "Gameboy.h"
#include<bitset>

using std::bitset;


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
    opcode = read(pc++);
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
    
    memory[address] = data;
}

void Gameboy::loadGame(){
    //load game into emulator
}

void Gameboy::flags(){
   bitset<8> flag(AF.low);
   if(setZ){
     flag.set(7);
   }
  
   if(setN){
     flag.set(6);
   }
   
   if(setH){
     flag.set(5);
   }
   
   if(unsetC){
     flag.reset(4);
   }
   
   if(unsetZ){
     flag.reset(7);
   }
   
   if(unsetN){
     flag.reset(6);
   }
   
   if(unsetH){
     flag.reset(5);
   }
   
   if(unsetC){
     flag.reset(4);
   }
   
}

void Gameboy::op_8bit_load(uint8_t r1, uint8_t r2){
    r1 = r2;
    
}

 void Gameboy::op_16bit_load(Register dd, uint16_t nn){
    dd.low = nn;
    dd.high = nn++;
 }
 
 void Gameboy::op_8bit_add(uint8_t v){
     AF.high += v;
     
     if(carrying){
        AF.high++;
     }
     
     if(((AF.high & 0xf) + (v & 0xf)) & 0x10 == 0x10){
            setH = true;
     }
     
     if((AF.high + v) > 0xFF){
           setC = true;
     }
     
     if((AF.high + v) == 0){
           setZ = true;
     }
     
     flags();
 }
 
 
 void Gameboy::op_8bit_subtract(uint8_t s){
    setN = true;
    AF.high -= s;
    
    if(carrying){
      AF.high++;
    }
    
    if( (AF.high - s) == 0){
       setZ = true;
    }
    
    if(((AF.high & 0xf) + (s & 0xf)) & 0x10 == 0x10){
       setH = true;
    }
     
    if((AF.high + s) > 0xFF){
        setC = true;
    }
    
    flags();
 }
 
 
 void Gameboy::op_8bit_and(uint8_t s){
    AF.high &= s;
    unsetN = true;
    unsetC = true;
    setH = true;
    
    if( (AF.high - s) == 0){
       setZ = true;
    }
 }
 
 void Gameboy::op_8bit_or(uint8_t s){
    AF.high |= s;
    unsetC = true;
    unsetH = true;
    unsetN = true;
    
    
    if( (AF.high - s) == 0){
       setZ = true;
    }
    
    flags();
 }
 
 void Gameboy::op_8bit_xor(uint8_t s){
    AF.high ^= s;
    unsetC = true;
    unsetH = true;
    unsetN = true;
    
    if( (AF.high - s) == 0){
       setZ = true;
    }
    
    flags();
 }
 
 void Gameboy::op_8bit_compare(uint8_t s){
    setN = true;
    
    if( (AF.high - s) == 0){
       setZ = true;
    }
    
    if(((AF.high & 0xf) + (s & 0xf)) & 0x10 == 0x10){
       setH = true;
    }
        
    if((AF.high + s) > 0xFF){
       setC = true;
    }
    
    flags();
}
 
 void Gameboy::op_16bit_add_to_hl(uint16_t ss){
    HL.full += ss;
 }
 
 

