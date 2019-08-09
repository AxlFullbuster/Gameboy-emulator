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
    opcode = read(PC.full++);
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
    
    memory[address] = data;
}

void Gameboy::loadGame(){
    //load game into emulator
}

void Gameboy::set_flag(int f){
   bitset<8> flag(AF.low);
   flag.set(f);
   AF.low = flag.to_ulong();
}

void Gameboy::unset_flag(int f){
   bitset<8> flag(AF.low);
   flag.reset(f);
   AF.low = flag.to_ulong();
}

bool Gameboy::check_flag(int f){
   bitset<8> flag(AF.low);
   if(flag.test(f)) return true;
   else return false;
}

void Gameboy::flip_flag(int f){
    bitset<8> flag(AF.low);
    if(check_flag(f)){
       flag.reset(f);
    }else{
       flag.set(f);
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
     unset_flag(6);
     
     if(carrying){
        AF.high++;
     }
     
     if(((AF.high & 0xf) + (v & 0xf)) & 0x10 == 0x10){
            set_flag(5);
     }
     
     if((AF.high + v) > 0xFF){
           set_flag(4);
     }
     
     if((AF.high + v) == 0){
           set_flag(7);
     }
 }
 
 
 void Gameboy::op_8bit_subtract(uint8_t s){
    set_flag(6);
    AF.high -= s;
    
    if(carrying){
      AF.high++;
    }
    
    if( (AF.high - s) == 0){
       set_flag(7);
    }
    
    if(((AF.high & 0xf) + (s & 0xf)) & 0x10 == 0x10){
       set_flag(5);
    }
     
    if((AF.high + s) > 0xFF){
        set_flag(4);
    }
 }
 
 
 void Gameboy::op_8bit_and(uint8_t s){
    AF.high &= s;
    unset_flag(6);
    unset_flag(4); 
    set_flag(5);
    
    if( (AF.high - s) == 0){
       set_flag(7);
    }
 }
 
 void Gameboy::op_8bit_or(uint8_t s){
    AF.high |= s;
    unset_flag(6);
    unset_flag(4);
    unset_flag(5);
    
    
    if( (AF.high - s) == 0){
       set_flag(7);
    }
 }
 
 void Gameboy::op_8bit_xor(uint8_t s){
    AF.high ^= s;
    unset_flag(6);
    unset_flag(4);
    unset_flag(5);
    
    if( (AF.high - s) == 0){
       set_flag(7);
    }
 }
 
 void Gameboy::op_8bit_compare(uint8_t s){
    set_flag(6);
    
    if( (AF.high - s) == 0){
       set_flag(7);
    }
    
    if(((AF.high & 0xf) + (s & 0xf)) & 0x10 == 0x10){
       set_flag(5);
    }
        
    if((AF.high + s) > 0xFF){
       set_flag(4);
    }
}
 
 void Gameboy::op_16bit_add_to_hl(uint16_t ss){
    unset_flag(6);
    HL.full += ss;
 }
 
 void Gameboy::op_jump(){
      Register nn;
      nn.low = read(PC.full);
      nn.high = read(PC.full++);
      PC.full += nn.full;
 }
 
 void Gameboy::op_jump_signed(int8_t e){
      PC.full += e;
 }
 
 void Gameboy::op_rotate(uint8_t val){
     bitset<8> bit(val);
     if(left){
         bit <<= 1;
         val = bit.to_ulong();
     }
     
     if(right){
        bit >>=1;
        val = bit.to_ulong();
     }
 }
 
 void Gameboy::op_call(){
     write(SP.full -1, PC.high);
     write(SP.full -2, PC.low);
     Register nn;
     op_16bit_load(nn, read(PC.full));
     PC.full = nn.full;
     SP.full -= 2;
 }
 
 void Gameboy::op_return(){
     PC.low = read(SP.full);
     PC.high = read(SP.full + 1);
     SP.full += 2;
 }
 
 void Gameboy::op_restart(uint8_t p){
     write(SP.full -1, PC.high);
     write(SP.full -2, PC.low);
     SP.full -= 2;
     PC.high = 0;
     PC.low = p;
 }
 
 void Gameboy::op_push(Register qq){
    write(SP.full -1, qq.high);
    write(SP.full -2, qq.low);
    SP.full -= 2;
 }
 
 void Gameboy::op_pop(Register qq){
    write(SP.full, qq.low);
    write(SP.full +1, qq.high);
    SP.full += 2;
 }
 
 void Gameboy::op_DAA(){
     if(!check_flag(6)){
         if(check_flag(5) || (AF.high & 0x0f) > 0x09){
            AF.high += 0x6;        
         }
         if(check_flag(4) || (AF.high > 0xFF)){
            AF.high += 0x60;
         }
     } else {
        if(check_flag(5)){
           AF.high -= 0x6;
        }
        if(check_flag(4)){
           AF.high -= 0x60;
        }
     }
    
     if(AF.high == 0){
         set_flag(7);
     }
     
     if(AF.high > 0xFF){
         set_flag(4);
     }  
 }
 
 void Gameboy::op_cpl(){
      bitset<8> A(AF.high);
      A.flip();
      AF.high = A.to_ulong();
 }
 
 
 
