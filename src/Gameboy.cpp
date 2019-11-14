#include "Gameboy.h"
#include<bitset>
#include<vector>
#include <fstream>
#include <iostream>

using std::bitset;
using std::vector;
using std::streamsize;
using std::ifstream;
using std::ios;
using std::cout;
using std::cin;
using std::endl;



Gameboy::Gameboy(){
    //do nothing
}

Gameboy::~Gameboy(){
    //do nothing
}

//initialize the emulator values to the ones given in the boot rom
void Gameboy::initialize(){
 
    PC.full = 0x100;
    SP.full = 0xFFFE;
    AF.full = 0x01B0;
    BC.full = 0x0013;
    DE.full = 0x00D8;
    HL.full = 0x014D;
    
    cycles = 0;

    memory[0xFF05] = 0x00;
    memory[0xFF06] = 0x00;
    memory[0xFF07] = 0x00;
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
    memory[0xFF21] = 0x00;
    memory[0xFF22] = 0x00;
    memory[0xFF23] = 0xBF; 
    memory[0xFF24] = 0x77; 
    memory[0xFF25] = 0xF3; 
    memory[0xFF26] = 0xF1;
    memory[0xFF40] = 0x91;
    memory[0xFF42] = 0x00;
    memory[0xFF43] = 0x00;
    memory[0xFF45] = 0x00;
    memory[0xFF47] = 0xFC; 
    memory[0xFF48] = 0xFF; 
    memory[0xFF49] = 0xFF;
    memory[0xFF4A] = 0x00;
    memory[0xFF4B] = 0x00;
    memory[0xFFFF] = 0x00;
}

//runs the emulation loop for one frame then refresh the screen
void Gameboy::emuLoop(){
    while (cycles <= 70244){
         emulateCycle();
        //handle interputs
        //other junk
    }
    //refresh screen;
    
    cycles -=70244;
}

void Gameboy::emulateCycle(){
 opcode = read(PC.full);
    if(opcode != 0xCB){
       decode1(opcode);
    }else {
        opcode = read(PC.full + 1);
        decode2(opcode);
    }
}


//read from memory space
uint8_t Gameboy::read(uint16_t address){
    return memory[address];
}

//write to memory space
void Gameboy::write(uint16_t address, uint8_t data){
    if(address >= 0x0000 && address <= 0x7FFF){
       //can't write to rom
    }else if(address >= 0xE000 && address <= 0xFDFF){
       memory[address] = data;
       write(address - 0x2000, data);
    }else if(address >= 0xFEA0 && address <= 0xFEFF){
       //don't write here the gameboy will get mad
    }else{
       memory[address] = data;
    }
}

//load the game into memory
bool Gameboy::loadGame(const char* filename){
    initialize();
    ifstream rom(filename, ios::in | ios::binary | ios::ate);
    streamsize size = rom.tellg();
    rom.seekg(0, ios::beg);
    vector<char> buffer(size);
    
    
    if (rom.read(buffer.data(), size)){
      for (int i = 0; i <= buffer.size(); ++i) {
      	memory[i] = buffer[i];
      }
    } else {
        printf("Couldn't load file");
        return false;
      }
      
    rom.close();
    return true;
}

/*set, unset, check, or flip the flag value given in f
 * 7 = Z flag
 * 6 = N flag
 * 5 = HC flag
 * 4 = C flag
 */

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
    AF.low = flag.to_ulong();
}

void Gameboy::set_pre_flags(uint8_t &val){
   unset_flag(6);
   unset_flag(5);
   
   if(val == 0) set_flag(7);
   else unset_flag(7);
   
   if(val > 0xFF)set_flag(4);
   else unset_flag(4);
}

void Gameboy::set_unpre_flags(){
   unset_flag(7);
   unset_flag(6);
   unset_flag(5);
   
   if(AF.high > 0xFF)set_flag(4);
   else unset_flag(4);
}

void Gameboy::op_8bit_load(uint8_t &r1, uint8_t r2){
    r1 = r2;
}

void Gameboy::op_16bit_load(Register &r){
    r.low = read(PC.full + 1);
    r.high = read(PC.full + 2);
}
 
 void Gameboy::op_8bit_add(uint8_t v){
     AF.high += v;
     unset_flag(6);
  
  
     if(carrying){
        AF.high++;
     }
     
     if((((AF.high & 0xf) + (v & 0xf)) & 0x10) == 0x10){
            set_flag(5);
     }else{
       unset_flag(5);
     }
     
     if((AF.high + v) > 0xFF){
           set_flag(4);
     } else{
       unset_flag(4);
     }
     
     if((AF.high + v) == 0){
           set_flag(7);
     } else {
       unset_flag(7);
     }
 }
 
 
 void Gameboy::op_8bit_subtract(uint8_t s){
    AF.high -= s;
    set_flag(6);
    
    if(carrying){
      AF.high++;
    }
    
    if( (AF.high - s) == 0){
       set_flag(7);
    }else{
      unset_flag(7);
    }
    
    if((((AF.high & 0xf) + (s & 0xf)) & 0x10) == 0x10){
       set_flag(5);
    }else{
      unset_flag(5);
    }
    
    if((AF.high + s) > 0xFF){
        set_flag(4);
    }else{
     unset_flag(4);
    }
 }
 
 
 void Gameboy::op_8bit_and(uint8_t s){
    AF.high &= s;
    unset_flag(6);
    unset_flag(4); 
    set_flag(5);
    
    if( (AF.high & s) == 0){
       set_flag(7);
    }else {
      unset_flag(7);
    }
 }
 
 void Gameboy::op_8bit_or(uint8_t s){
    AF.high |= s;
    unset_flag(6);
    unset_flag(4);
    unset_flag(5);
    
    
    if( (AF.high | s) == 0){
       set_flag(7);
    }else {
      unset_flag(7);
    }
 }
 
 void Gameboy::op_8bit_xor(uint8_t s){
    AF.high ^= s;
    unset_flag(6);
    unset_flag(4);
    unset_flag(5);
    
    if( (AF.high ^ s) == 0){
       set_flag(7);
    }else {
      unset_flag(7);
    }
 }
 
 void Gameboy::op_8bit_compare(uint8_t s){
    set_flag(6);
    
    if(AF.high == s){
       set_flag(7);
    }else {
      unset_flag(7);
    }
    
    if((((AF.high & 0xf) + (s & 0xf)) & 0x10) == 0x10){
       set_flag(5);
    }else {
      unset_flag(5);   
    }
    
    if((AF.high + s) > 0xFF){
       set_flag(4);
    }else {
      unset_flag(4);
    }
}
 
 void Gameboy::op_16bit_add_to_hl(uint16_t ss){
    unset_flag(6);
    HL.full += ss;
    
    if((((HL.full & 0xf) + (ss & 0xf)) & 0x10) == 0x10){
       set_flag(5);
    }else {
      unset_flag(5); 
    }
    if((HL.full + ss) > 0xFF){
       set_flag(4);
    }else{
       unset_flag(4);
    }
}
 
 void Gameboy::op_jump(){
      Register nn;
      nn.low = read(PC.full + 1);
      nn.high = read(PC.full + 2);
      PC.full +=3;
      PC.full = nn.full;
 }
 
 void Gameboy::op_jump_signed(int8_t e){
      PC.full += 2;
      PC.full += e;
 }
 
 void Gameboy::op_rotate(uint8_t &val){
     bitset<8> bit(val);
     bitset<8> flag(AF.low);
     if(left){
         bit = bit << 1 | bit >> (8-1);
     }
     
     if(right){
        bit = bit >> 1 | bit << (8-1);
     }
     
     bit[0] = flag[4];
     flag[4] = bit.test(7);
     
     if(val == AF.high){
        set_unpre_flags();
     }else{
        set_pre_flags(val);
     }
     val = bit.to_ulong();
     AF.low = flag.to_ulong();
 }
 
 void Gameboy::op_shift(uint8_t &val){
    bitset<8> bit(val);
    bitset<8> flag(AF.low);
    unset_flag(6);
    unset_flag(5);
    
    if(left){
      bit <<= 1;
      flag[4] = bit.test(7);
    }
    
    if(right){
     bit >>= 1;
     flag[4] = bit.test(0);
    }
     
    if(reset7) bit.reset(7);
   
    if(reset0) bit.reset(0);
     
    val = bit.to_ulong();
    AF.low = flag.to_ulong();
    set_pre_flags(val);
 }
 
 void Gameboy::op_swap(uint8_t &val){
    bitset<8> bit(val);
    unset_flag(6);
    unset_flag(5);
    unset_flag(4);
    bit = (bit >> 4) | (bit << 4);
    val = bit.to_ulong();
    
    if(val == 0){
       set_flag(7);
    }else{
       unset_flag(7);
    }
 }
 
 void Gameboy::op_bit(int b, uint8_t &val){
    bitset<8> bit(val);
    unset_flag(6);
    set_flag(5);
    if(bit.test(b)){
        unset_flag(7);
    }else{
       set_flag(7);
    }
 }
 
 void Gameboy::op_set(int b, uint8_t &val){
    bitset<8> bit(val);
    bit.set(b);
    val = bit.to_ulong();
 }
 
 void Gameboy::op_reset(int b, uint8_t &val){
   bitset<8> bit(val);
   bit.reset(b);
   val = bit.to_ulong();
 }
 
 void Gameboy::op_call(){
     Register nn;
     op_16bit_load(nn);
     PC.full +=3;
     op_push(PC);
     PC = nn;
 }
 
 void Gameboy::op_return(){
     op_pop(PC); 
 }
 
 void Gameboy::op_restart(uint8_t p){
     PC.full++;
     op_push(PC);
     PC.high = 0;
     PC.low = p;
 }
 
 void Gameboy::op_push(Register qq){
    write(SP.full -1, qq.high);
    write(SP.full -2, qq.low);
    SP.full -= 2;
 }
 
 void Gameboy::op_pop(Register &qq){
    qq.low = read(SP.full);
    qq.high = read(SP.full + 1);
    write(SP.full, 0x0);
    write(SP.full + 1, 0x0);
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
     }else {
       unset_flag(7);
     }
     
     if(AF.high > 0xFF){
         set_flag(4);
     }else {
      unset_flag(7);
     }
 }
 
 void Gameboy::op_cpl(){
      set_flag(6);
      set_flag(5);
      bitset<8> A(AF.high);
      A.flip();
      AF.high = A.to_ulong();
 }
 
uint16_t Gameboy::get_AF(){
   return AF.full;
}

uint16_t Gameboy::get_BC(){
   return BC.full;
}

uint16_t Gameboy::get_DE(){
   return DE.full;
}

uint16_t Gameboy::get_HL(){
   return HL.full;
}

uint16_t Gameboy::get_PC(){
   return PC.full;
}
        
uint16_t Gameboy::get_SP(){
   return SP.full;
}

uint8_t * Gameboy::get_mem(){
   return memory;
}

uint8_t Gameboy::get_A(){
    return AF.high;
}

uint8_t Gameboy::get_B(){
    return BC.high;
}

uint8_t Gameboy::get_C(){
    return BC.low;
}

uint8_t Gameboy::get_D(){
    return DE.high;
}

uint8_t Gameboy::get_E(){
    return DE.low;
}

uint8_t Gameboy::get_H(){
    return HL.high;
}

uint8_t Gameboy::get_L(){
    return HL.low;
}

uint8_t Gameboy::get_F(){
    return AF.low;
}

uint8_t Gameboy::get_OP(){
    return opcode;
}
 
 
 
 

