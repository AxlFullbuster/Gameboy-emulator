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

/*!
 *initializes the memory map and the registers to their state after
 *the bios rom has finished execution. This is run for every single
 *gameboy rom.
 */
void CPU::initialize(){
    PC.full = 0x0100;
    SP.full = 0xFFFE;
    AF.full = 0x01B0;
    BC.full = 0x0013;
    DE.full = 0x00D8;
    HL.full = 0x014D;
    
    cycles = 0;
    div_counter = 0;

    memory[0xFF00] = 0xFF;
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
    
    for(int i = 0; i < 8; i++){
      buttons[i] = 1;
    }
}

void CPU::clearMemory(){
  for(int i = 0; i < 0x10000; i++){
    memory[i] = 0x00;
  }
}

/*!
 *Depending on the value of the opcode this method will call either
 *decode1 or decode2 to execute a prefixed or unprefixed instruction.
 *if the emulator is in a halt state this method will just add 4 clock
 *cycles to the total cycle count and return. If the emulator is in
 *a debugging state it will do nothing.
 */
void CPU::emulateCycle(){
  if(halt){
    cycles += 4;
    return;
  }
  
  if(debug) return;
  
  int current_cycle = cycles;
  opcode = read(PC.full);
  if(opcode != 0xCB){
     decode1(opcode);
  }else {
      opcode = read(PC.full + 1);
      cycles += 4;
      decode2(opcode);
  }
  timing = get_cycles(current_cycle);
}

/*!
 *The exception to this rule is when the
 *emulator reads a value in the input register. Instead of returning
 *the value it will call the input method.
 *
 *@param address the address we want to read from
 */
uint8_t CPU::read(uint16_t address){
  if(address == 0xFF00) input();
  return memory[address];
}


/*!
 *Writing a value into the memory map is tricky. Since there
 *are important registers that are part of the cpu's functionality
 *the emulator should not be allowed to write into everything.
 *To enforce this rule this method will check where data is being
 *written to and will then decide what to do next.
 *
 *@param address the memory address we want to write to
 *@param data the data we are trying to put into the memory map
 *
 *This is what will happen for specific address ranges:
 *
 *Address <= 0x7FFF: This address portion stores ROM data so we return from the function.
 *
 *Address >= 0xA000 && Address <= 0xBFFF: This address range stores cartridge RAM data,
 *this isn't emulated in this version so we just return from the function.
 *
 *Address >=0xE000 && address <= 0xFDFF: This address range stores echo data from the RAM portion
 *of the memory map so anything that is written here will be written to the RAM portion as well.
 *
 *Address >= 0xFEA0 && Address <= 0xFEFF: This address range isn't used for anything, I don't know why
 *but all the references I used recommended not writing to this portion of memory. So we just return
 *from the function.
 *
 *Address == 0xFF04: This address is the divider register, if the emulator tries to write something here
 *we reset the value to 0.
 *
 *Address == 0xFF46: This address is responsible for direct memory access so I call a method that does
 *that here.
 *
 *Address == anything else: By this point it should be safe to write into the memory map since the more critical
 *features have been caught.
 */
void CPU::write(uint16_t address, uint8_t data){
    if(address < 0x8000){
        return;
    }
    else if(address >= 0xA000 && address <= 0xBFFF){
        return;
    }
    else if(address >= 0xE000 && address <= 0xFDFF){
       memory[address] = data;
       write(address - 0x2000, data);
    }
    else if(address >= 0xFEA0 && address <= 0xFEFF){
       return;
    }
    else if(address == 0xFF04){
      memory[address] = 0x00;
    }
    else if(address == 0xFF44){
       memory[address] = 0;
    }
    else if(address == 0xFF46){
       DMA(data);
    }else{
       memory[address] = data;
    }
}

/*!
 *When the address 0xFF00 is read by the emulator it will jump to this method.
 *The method is responsible for checking which type of inputs we will accept as input.
 *There are two sets of inputs:
 *
 *Directional inputs(left, right, down, up)
 *
 *Button inputs (A,B, Start, Select)
 *
 *two global boolean values (direction, button) are used to identify which input type is accepted. Their values
 *are determined by the 4th and 5th bit in 0xFF00. The method then processes what button was pressed by the
 *buttons array and then stores the change in bits into 0xFF00. It also checks if an input interrupt has
 *occured.
 */
void CPU::input(){
  bitset<8> input(memory[0xFF00]);
  direction = input.test(4);
  button = input.test(5);
  bool interrupt;

if(!direction){
  input[0] = buttons[0];
  input[1] = buttons[1];
  input[2] = buttons[2];
  input[3] = buttons[3];
}else if(!button){
  input[0] = buttons[4];
  input[1] = buttons[5];
  input[2] = buttons[6];
  input[3] = buttons[7];
}
  
  memory[0xFF00] = input.to_ulong();
  
  for(int i = 0; i < 4; i++){
    if(input[i] == 0) interrupt = true;
  }
  
  if(interrupt) request_interrupt(4);
  
}


void CPU::update_timers(){
  div_inc();
}

//! increments the divider timer register every 256 clock cycles
void CPU::div_inc(){
  div_counter += timing;
  
  if(div_counter >= 0xFF){
    memory[0xFF04]++;
    div_counter = 0;
  }
}

/*!
 *@param req  the request number that is used to identify what interrupt needs to be processed.
 *
 *0: vertical blank interrupt
 *
 *1: LCDC status interrupt
 *
 *2: Timer interrupt
 *
 *3: Serial transfer interrupt
 *
 *4: Input interrupt
 */
void CPU::request_interrupt(int req){
  bitset<8> IF (read(0xFF0F));
  IF.set(req);
  write(0xFF0F, IF.to_ulong());
}

void CPU::check_interrupt(){
  bitset<8> IF (read(0xFF0F));
  for(int i = 0; i < 5; i++){
    if(IF.test(i)){
      execute_interrupt(i);
    }
  }
}

void CPU::execute_interrupt(int req){
  bitset<8> IE (read(0xFFFF));
  bitset<8> IF (read(0xFF0F));
  if(IE.test(req) && IF.test(req)){
    halt = false;
    if(req == 4) stop = false;
    if(IME){
      switch(req){
        case 0:
            IF.reset(0);
            write(0xFF0F, IF.to_ulong());
            IME = false;
            op_push(PC);
            PC.full = 0x0040;
        break;
      
        case 1:
            IF.reset(1);
            write(0xFF0F, IF.to_ulong());
            IME = false;
            op_push(PC);
            PC.full = 0x0048;
        break;
      
        case 2:
            IF.reset(2);
            write(0xFF0F, IF.to_ulong());
            IME = false;
            op_push(PC);
            PC.full = 0x0050;
        break;
      
        case 3:
            IF.reset(3);
            write(0xFF0F, IF.to_ulong());
            IME = false;
            op_push(PC);
            PC.full = 0x0058;
        break;
      
        case 4:
            IF.reset(4);
            write(0xFF0F, IF.to_ulong());
            IME = false;
            op_push(PC);
            PC.full = 0x0060;
        break;
      }
    }
  }
}

void CPU::increment_scanline(){
   memory[0xFF44]++;
}

/*!
 *The method will load the byte data for the nintendo logo
 *and the cheksum values found in the bios data.
 */
void CPU::load_bios(){
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



bool CPU::loadGame(const char* filename){
    clearMemory();
    ifstream rom(filename, ios::in | ios::binary | ios::ate);
    streamsize size = rom.tellg();
    rom.seekg(0, ios::beg);
    vector<char> buffer(size);
    
    
    if (rom.read(buffer.data(), size)){
      
      for (int i = 0; i <= buffer.size(); i++) {
      	memory[i] = buffer[i];
      }
      
    }else {
        printf("Couldn't load file");
        return false;
    }
    rom.close();

    
    if(bios) load_bios();
    else initialize();
    
    return true;
}


void CPU::DMA(uint8_t data){
   uint16_t address = data << 8;
   
   for(int i = 0; i < 0xA0; i++){
    write(0xFE00+i, read(address+i));
   }
}

/** @defgroup flags Flag Control Group
 * This group is responisble for changing flag values.
 * They all use the same parameter to update the value.
 * 
 * @param f The bit value in the flag register that needs to be changed.
 *
 * 7: Z - Zero flag
 * 
 * 6: N - Negative flag
 * 
 * 5: H - Half Carry flag
 * 
 * 4: C - Carry flag
 *
 * @{
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

/** @} */ // end of flags

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

void CPU::op_8bit_load(uint8_t &r1, uint8_t r2){
    r1 = r2;
}

void CPU::op_16bit_load(Register &r){
    r.low = read(PC.full + 1);
    r.high = read(PC.full + 2);
}

void CPU::op_inc(uint8_t &val){
  unset_flag(6);
  
  if((((val & 0xf) + (1 & 0xf)) & 0x10) == 0x10){
        set_flag(5);
     }else{
       unset_flag(5);
     }
  
  val++;
  
  if(val == 0) set_flag(7);
  else unset_flag(7);
}

void CPU::op_dec(uint8_t &val){
  set_flag(6);
  
  if((((val & 0xf) - (1 & 0xf)) & 0x10) == 0x10){
        set_flag(5);
     }else{
       unset_flag(5);
     }
  
  val--;
  
  if(val == 0) set_flag(7);
  else unset_flag(7);
}

void CPU::op_8bit_add(uint8_t v){
     //used for carry operations
     int c = 0;
     if(carrying && check_flag(4)){
        c = 1;
     }
     
     if((((AF.high & 0xf) + (v & 0xf) + (c & 0xf) ) & 0x10) == 0x10){
            set_flag(5);
     }else{
       unset_flag(5);
     }
     
     if((AF.high + v + c) > 0xFF){
           set_flag(4);
     } else{
       unset_flag(4);
     }
     
     AF.high = AF.high + v + c;
     unset_flag(6);

     if((AF.high) == 0){
         set_flag(7);
     } else {
       unset_flag(7);
     }
}
 


void CPU::op_8bit_subtract(uint8_t s){
    int c = 0;
    if(carrying && check_flag(4)){
        c = 1;
    }
    
    if((((AF.high & 0xf) - (s & 0xf) - (c & 0xf) ) & 0x10) == 0x10){
            set_flag(5);
     }else{
       unset_flag(5);
     }
     
     if((AF.high - s - c) < 0){
        set_flag(4);
     }else{
       unset_flag(4);
     }
     
    
    AF.high = AF.high - s - c;
    set_flag(6);
    
    if( (AF.high) == 0){
       set_flag(7);
    }else{
      unset_flag(7);
    }
    
    
}
 


void CPU::op_8bit_and(uint8_t s){
    unset_flag(6);
    unset_flag(4); 
    set_flag(5);
    
    AF.high &= s;
    
    if( (AF.high) == 0){
       set_flag(7);
    }else {
      unset_flag(7);
    }
}
 

void CPU::op_8bit_or(uint8_t s){
    unset_flag(6);
    unset_flag(4);
    unset_flag(5);
    
    
    AF.high |= s;
    
    if( (AF.high) == 0){
       set_flag(7);
    }else {
      unset_flag(7);
    }
}
 

void CPU::op_8bit_xor(uint8_t s){
    unset_flag(6);
    unset_flag(4);
    unset_flag(5);
    
    AF.high ^= s;
    
    if( (AF.high) == 0){
       set_flag(7);
    }else {
      unset_flag(7);
    }
}
 

void CPU::op_8bit_compare(uint8_t s){
    set_flag(6);
    
    if(AF.high == s){
       set_flag(7);
    }else {
      unset_flag(7);
    }
    
    if((((AF.high & 0xf) - (s & 0xf)) & 0x10) == 0x10){
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
 

void CPU::op_16bit_add(uint16_t& r1, uint16_t r2){
    unset_flag(6);
    if((((r1 & 0xfff) + (r2 & 0xfff)) & 0x1000) == 0x1000){
       set_flag(5);
    }else {
      unset_flag(5); 
    }
    
    if((r1 + r2) > 0xFFFF){
       set_flag(4);
    }else{
       unset_flag(4);
    }
    
   r1 += r2;
}
 

void CPU::op_jump(){
      Register nn;
      nn.low = read(PC.full + 1);
      nn.high = read(PC.full + 2);
      PC.full += 3;
      PC.full = nn.full;
}
 
 

void CPU::op_jump_signed(int8_t e){
      PC.full += 2;
      PC.full += e;
}
 
void CPU::op_rotate_A(){
    bitset<8> A(AF.high);
    bitset<8> F(AF.low);
    bool temp;
    temp = F[4];
    
    if(left) F[4] = A[7];
    if(right)F[4] = A[0];
    
    if(left){
        A = A << 1 | A >> (8-1);
    }
  
    if(right){
       A = A >> 1 | A << (8-1);
    }
    
    if(!carrying){
      if(left) A[0] = temp;
      if(right) A[7] = temp;
    }
    
    AF.high = A.to_ulong();
    AF.low = F.to_ulong();  
}


void CPU::op_rotate(uint8_t &val){
     bitset<8> bit(val);
     bitset<8> flag(AF.low);
     bool temp;
     temp = flag[4];
     
     if(left) flag[4] = bit[7];
     if(right) flag[4] = bit[0];
     
     if(left){
        bit = bit << 1 | bit >> (8-1);
     }
     
     
     if(right){
      bit = bit >> 1 | bit << (8-1);
     }
     
     if(!carrying){
      if(left) bit[0] = temp;
      if(right) bit[7] = temp;
    }
     
     val = bit.to_ulong();
     AF.low = flag.to_ulong();
}
 
void CPU::op_shift(uint8_t &val){
    bitset<8> bit(val);
    bitset<8> flag(AF.low);
    bool temp = bit[7];
    
    if(left){
      flag[4] = bit[7];
      bit <<= 1;
    }
    
    if(right){
    flag[4] = bit[0];
     bit >>= 1;
     
    }
     
    if(reset7) bit.reset(7);
    if(reset0) bit.reset(0);
    if(keep7) bit[7] = temp;
     
    val = bit.to_ulong();
    AF.low = flag.to_ulong();
    set_pre_flags(val);
}
 
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
 
void CPU::op_set(int b, uint8_t &val){
    bitset<8> bit(val);
    bit.set(b);
    val = bit.to_ulong();
}
 
void CPU::op_reset(int b, uint8_t &val){
   bitset<8> bit(val);
   bit.reset(b);
   val = bit.to_ulong();
}
 
void CPU::op_call(){
     Register nn;
     op_16bit_load(nn);
     PC.full += 3;
     op_push(PC);
     PC = nn;
}
 
void CPU::op_return(){
     op_pop(PC); 
}
 
void CPU::op_restart(uint8_t p){
     PC.full++;
     op_push(PC);
     PC.high = 0x00;
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
    
    if(A){
      qq.low &= 0xF0;
    }
    
    SP.full += 2;
}
 
void CPU::op_DAA(){
     if(!check_flag(6)){
         if(check_flag(4) || (AF.high > 0x99)){
            AF.high += 0x60;
            set_flag(4);
         }
      
         if(check_flag(5) || (AF.high & 0x0f) > 0x09){
            AF.high += 0x6;        
         }
         
     } else {
        if(check_flag(4)){
           AF.high -= 0x60;
           set_flag(4);
        }
      
        if(check_flag(5)){
           AF.high -= 0x6;
        }
     }
    
     if(AF.high == 0){
       set_flag(7);
     }else{
       unset_flag(7);
     }
     
     
     unset_flag(5);
     
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

bool CPU::get_ime(){
  return IME;
}

bool CPU::get_halt(){
  return halt;
}


 
 
 

