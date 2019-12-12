#include "CPU.h"
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



CPU::CPU(){
    //do nothing
}

CPU::~CPU(){
    //do nothing
}

//initialize the emulator values to the ones given in the boot rom
void CPU::initialize(){
 
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


//does the fetch, decode, and execute cycle
void CPU::emulateCycle(){
if(read(0xFF50) == 1){
  return;
}
   int current_cycle = cycles;
     opcode = read(PC.full);
        if(opcode != 0xCB){
           decode1(opcode);
        }else {
            opcode = read(PC.full +1);
            decode2(opcode);
        }
      timing = get_cycles(current_cycle);
}

//read from memory space
uint8_t CPU::read(uint16_t address){
    return memory[address];
}

//write to memory space
void CPU::write(uint16_t address, uint8_t data){
    if(address >= 0x0000 && address <= 0x7FFF){
       //can't write to rom
       return;
    }else if(address >= 0xE000 && address <= 0xFDFF){
       memory[address] = data;
       write(address - 0x2000, data);
    }else if(address >= 0xFEA0 && address <= 0xFEFF){
       //don't write here the gameboy will get mad
       return;
    }else if(address == 0xFF44){
       //all writes to the scanline will reset the value
       memory[address] = 0;
    }else if(address == 0xFF46){
       DMA(data);
    }else{
       memory[address] = data;
    }
}


//A method for the GPU which increments the scaline value by 1
void CPU::increment_scanline(){
   memory[0xFF44]++;
}

void CPU::load_bios(){
 //these values in memory hold the nintendo logo
   memory[0x104] = 0xce;
   memory[0x105] = 0xed;
   memory[0x106] = 0x66;
   memory[0x107] = 0x66;
   memory[0x108] = 0xcc;
   memory[0x109] = 0x0d;
   memory[0x10B] = 0x0b;
   memory[0x10C] = 0x03;
   memory[0x10D] = 0x73;
   memory[0x10F] = 0x83;
   memory[0x111] = 0x0c;
   memory[0x113] = 0x0d;
   memory[0x115] = 0x08;
   memory[0x116] = 0x11;
   memory[0x117] = 0x1f;
   memory[0x118] = 0x88;
   memory[0x119] = 0x89;
   memory[0x11B] = 0x0e;
   memory[0x11C] = 0xdc;
   memory[0x11D] = 0xcc;
   memory[0x11E] = 0x6e;
   memory[0x11F] = 0xe6;
   memory[0x120] = 0xdd;
   memory[0x121] = 0xdd;
   memory[0x122] = 0xd9;
   memory[0x123] = 0x99;
   memory[0x124] = 0xbb;
   memory[0x125] = 0xbb;
   memory[0x126] = 0x67;
   memory[0x127] = 0x63;
   memory[0x128] = 0x6e;
   memory[0x129] = 0x0e;
   memory[0x12A] = 0xec;
   memory[0x12B] = 0xcc;
   memory[0x12C] = 0xdd;
   memory[0x12D] = 0xdc;
   memory[0x12E] = 0x99;
   memory[0x12F] = 0x9f;
   memory[0x130] = 0xbb;
   memory[0x131] = 0xb9;
   memory[0x132] = 0x33;
   memory[0x133] = 0x3e;
  
  /*these values in memory are the checksum and header checksum
    *which are used to make sure that the values in memory
    *are correct.
    */
  memory[0x143] = 0x80;
  memory[0x147] = 0x01;
  memory[0x14D] = 0x66;
   
}


//load the game into memory
bool CPU::loadGame(const char* filename){
  if(bios) load_bios();
  else initialize();
    
    ifstream rom(filename, ios::in | ios::binary | ios::ate);
    streamsize size = rom.tellg();
    rom.seekg(0, ios::beg);
    vector<char> buffer(size);
    
    
    if (rom.read(buffer.data(), size)){
      for (int i = 0; i <= buffer.size(); i++) {
      	memory[i] = buffer[i];
      }
    } else {
        printf("Couldn't load file");
        return false;
      }
    rom.close();
    
    
    return true;
}


void CPU::DMA(uint8_t data){
   uint16_t address = data << 8;
   
   for(int i = 0; i < 0xA0; i++){
    write(0xFE00+i, read(address+i));
   }
}

/*set, unset, check, or flip the flag value given in f
 * 7 = Z flag
 * 6 = N flag
 * 5 = HC flag
 * 4 = C flag
 */

void CPU::set_flag(int f){
   bitset<8> flag(AF.low);
   flag.set(f);
   AF.low = flag.to_ulong();
}

void CPU::unset_flag(int f){
   bitset<8> flag(AF.low);
   flag.reset(f);
   AF.low = flag.to_ulong();
}

bool CPU::check_flag(int f){
   bitset<8> flag(AF.low);
   if(flag.test(f)) return true;
   else return false;
}

void CPU::flip_flag(int f){
    bitset<8> flag(AF.low);
    if(check_flag(f)){
       flag.reset(f);
    }else{
       flag.set(f);
    }
    AF.low = flag.to_ulong();
}

void CPU::set_pre_flags(uint8_t &val){
   unset_flag(6);
   unset_flag(5);
   
   if(val == 0) set_flag(7);
   else unset_flag(7);
   
}

void CPU::set_unpre_flags(){
   unset_flag(7);
   unset_flag(6);
   unset_flag(5);
}

//loads an 8-bit register into another
void CPU::op_8bit_load(uint8_t &r1, uint8_t r2){
    r1 = r2;
}

//loads a 16-bit register into another
void CPU::op_16bit_load(Register &r){
    r.low = read(PC.full + 1);
    r.high = read(PC.full + 2);
}
 
//adds an 8-bit value to the A register
 void CPU::op_8bit_add(uint8_t v){
    uint8_t bef = AF.high;
     AF.high += v;
     unset_flag(6);
  
  
     if(carrying){
        AF.high++;
     }
     
     if((((bef & 0xf) + (v & 0xf)) & 0x10) == 0x10){
            set_flag(5);
     }else{
       unset_flag(5);
     }
     
     if((bef + v) > 0xFF){
           set_flag(4);
     } else{
       unset_flag(4);
     }
     
     if((AF.high) == 0){
         set_flag(7);
     } else {
       unset_flag(7);
     }
     
 }
 

//subtracts an 8-bit value from the A register
 void CPU::op_8bit_subtract(uint8_t s){
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
    
    if((AF.high - s) < 0xFF){
        set_flag(4);
    }else{
     unset_flag(4);
    }
    
    AF.high -= s;
 }
 

//bitwise and an 8-bit value with the A register
 void CPU::op_8bit_and(uint8_t s){
    unset_flag(6);
    unset_flag(4); 
    set_flag(5);
    
    if( (AF.high & s) == 0){
       set_flag(7);
    }else {
      unset_flag(7);
    }
    
    AF.high &= s;
 }
 
//bitwise or an 8-bit value with the A register
 void CPU::op_8bit_or(uint8_t s){
    unset_flag(6);
    unset_flag(4);
    unset_flag(5);
    
    
    if( (AF.high | s) == 0){
       set_flag(7);
    }else {
      unset_flag(7);
    }
    
    AF.high |= s;
 }
 
//xor an 8-bit value with the A register
 void CPU::op_8bit_xor(uint8_t s){
    unset_flag(6);
    unset_flag(4);
    unset_flag(5);
    
    if( (AF.high ^ s) == 0){
       set_flag(7);
    }else {
      unset_flag(7);
    }
    
    AF.high ^= s;
 }
 
//compares A with an 8-bit value
 void CPU::op_8bit_compare(uint8_t s){
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
    
    if((AF.high & 0xFF) < (s & 0xFF)){
       set_flag(4);
    }else {
      unset_flag(4);
    }
}
 
//adds a 16-bit value to the HL register
 void CPU::op_16bit_add_to_hl(uint16_t ss){
    unset_flag(6);
    
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
    
    HL.full += ss;
}
 
//Have the PC jump to the value specified in the next 2 bytes in memory
 void CPU::op_jump(){
      Register nn;
      nn.low = read(PC.full + 1);
      nn.high = read(PC.full + 2);
      PC.full +=3;
      PC.full = nn.full;
 }
 
 
//Have the PC jump to the sum of it's original value and a signed integer
 void CPU::op_jump_signed(int8_t e){
      PC.full += 2;
      PC.full += e;
 }
 
//rotate the bits in an 8-bit value
 void CPU::op_rotate(uint8_t &val){
     bitset<8> bit(val);
     bitset<8> flag(AF.low);
     bool temp;
     
     temp = bit[7];
     if(left){
         bit = bit << 1 | bit >> (8-1);
     }
     
     if(right){
        bit = bit >> 1 | bit << (8-1);
     }
     
     if(!A){
      if(val == 0) set_flag(7);
      else unset_flag(7);
     }
     
     bit[0] = flag[4];
     flag[4] = temp;
     
     val = bit.to_ulong();
     AF.low = flag.to_ulong();
 }
 
 
//shift the bits in an 8-bit value
 void CPU::op_shift(uint8_t &val){
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
 
//swap the lower bit values with the higer bit values
 void CPU::op_swap(uint8_t &val){
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
 
//check if a bit is set
 void CPU::op_bit(int b, uint8_t &val){
    bitset<8> bit(val);
    unset_flag(6);
    set_flag(5);
    if(bit.test(b)){
        unset_flag(7);
    }else{
       set_flag(7);
    }
 }
 
//set a specific bit
 void CPU::op_set(int b, uint8_t &val){
    bitset<8> bit(val);
    bit.set(b);
    val = bit.to_ulong();
 }
 
//unset a specfic bit
 void CPU::op_reset(int b, uint8_t &val){
   bitset<8> bit(val);
   bit.reset(b);
   val = bit.to_ulong();
 }
 
 void CPU::op_call(){
     Register nn;
     op_16bit_load(nn);
     PC.full +=3;
     op_push(PC);
     PC = nn;
 }
 
 void CPU::op_return(){
     op_pop(PC); 
 }
 
 void CPU::op_restart(uint8_t p){
     PC.full++;
     op_push(PC);
     PC.high = 0;
     PC.low = p;
 }
 
 void CPU::op_push(Register qq){
    write(SP.full -1, qq.high);
    write(SP.full -2, qq.low);
    SP.full -= 2;
 }
 
 void CPU::op_pop(Register &qq){
    qq.low = read(SP.full);
    qq.high = read(SP.full + 1);
    write(SP.full, 0x0);
    write(SP.full + 1, 0x0);
    SP.full += 2;
 }
 
 void CPU::op_DAA(){
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
 
 void CPU::op_cpl(){
      set_flag(6);
      set_flag(5);
      bitset<8> A(AF.high);
      A.flip();
      AF.high = A.to_ulong();
 }
 
uint16_t CPU::get_AF(){
   return AF.full;
}

uint16_t CPU::get_BC(){
   return BC.full;
}

uint16_t CPU::get_DE(){
   return DE.full;
}

uint16_t CPU::get_HL(){
   return HL.full;
}

uint16_t CPU::get_PC(){
   return PC.full;
}
        
uint16_t CPU::get_SP(){
   return SP.full;
}

uint8_t * CPU::get_mem(){
   return memory;
}

uint8_t CPU::get_A(){
    return AF.high;
}

uint8_t CPU::get_B(){
    return BC.high;
}

uint8_t CPU::get_C(){
    return BC.low;
}

uint8_t CPU::get_D(){
    return DE.high;
}

uint8_t CPU::get_E(){
    return DE.low;
}

uint8_t CPU::get_H(){
    return HL.high;
}

uint8_t CPU::get_L(){
    return HL.low;
}

uint8_t CPU::get_F(){
    return AF.low;
}

uint8_t CPU::get_OP(){
    return opcode;
}
 
int CPU::get_cycles(int prev){
    return cycles - prev;
}

 
 
 

