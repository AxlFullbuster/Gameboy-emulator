#include "CPU.h"
#include <stdio.h>
#include <stdlib.h>

void CPU::decode1(uint8_t opcode){
    switch(opcode){
        case 0x00: // NOP 
            cycles += 4;
            PC.full++;
        break;
    
        case 0x01: // LD BC u16 
            op_16bit_load(BC);
            cycles += 12;
            PC.full += 3;
        break;
    
        case 0x02: // LD (BC) A 
            write(BC.full, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x03: //INC BC
            BC.full++;
            cycles += 8;
            PC.full++;
        break;
    
        case 0x04: //INC B
            op_inc(BC.high);
            cycles += 4;
            PC.full++;
        break;
    
        case 0x05: //DEC B
            op_dec(BC.high);
            cycles += 4;
            PC.full++;
        break;
    
        case 0x06: // LD B u8
            op_8bit_load(BC.high, read(PC.full + 1));
            PC.full += 2;
            cycles += 8;
        break;
    
        case 0x07: //RLCA
            left = true;
            carrying = true;
            op_rotate_A();
            set_unpre_flags();
            left = false;
            carrying = false;
            cycles += 4;
            PC.full++;
        break;
    
        case 0x08:{ //LD (u16) SP
            Register nn;
            nn.low = read(PC.full + 1);
            nn.high = read(PC.full + 2);
            write(nn.full, SP.low);
            write(nn.full + 1, SP.high);
            cycles += 20;
            PC.full += 3;
        }
        break;
    
        case 0x09: //ADD HL BC
            op_16bit_add(HL.full, BC.full);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x0A: //LD A (BC)
            op_8bit_load(AF.high, read(BC.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0x0B: //DEC BC
            BC.full--;
            cycles += 8;
            PC.full++;
        break;
    
        case 0x0C: //INC C
            op_inc(BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x0D: //DEC C
            op_dec(BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x0E: //LD C u8
            op_8bit_load(BC.low, read(PC.full + 1));
            PC.full += 2;
            cycles += 8;
        break;
    
        case 0x0F: //RRCA
            right = true;
            carrying = true;
            op_rotate_A();
            set_unpre_flags();
            right = false;
            carrying = false;
            cycles += 4;
            PC.full++;
        break;
    
        case 0x10: //STOP
            stop = true;
            cycles += 4;
            PC.full += 2;
        break;
    
        case 0x11: //LD DE u16
            op_16bit_load(DE);
            PC.full += 3;
            cycles += 12;
        break;
    
        case 0x12: // LD (DE) A
            write(DE.full, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x13: //INC DE
            DE.full++;
            cycles += 8;
            PC.full++;
        break;
    
        case 0x14: //INC D
            op_inc(DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x15: //DEC D
            op_dec(DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x16: //LD D u8
            op_8bit_load(DE.high, read(PC.full + 1));
            PC.full += 2;
            cycles += 8;
        break;
    
        case 0x17: //RLA
            left = true;
            op_rotate_A();
            set_unpre_flags();
            left = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x18: //JR i8
            op_jump_signed(read(PC.full + 1));
            cycles += 12;
        break;
    
        case 0x19: //ADD HL DE
            op_16bit_add(HL.full,DE.full);
            PC.full++;
            cycles += 8;
        break;
    
        case 0x1A: //LD A (DE)
            op_8bit_load(AF.high, read(DE.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x1B: //DEC DE
            DE.full--;
            PC.full++;
            cycles += 8;
        break;
    
        case 0x1C: //INC E
            op_inc(DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x1D: //DEC E
            op_dec(DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x1E: //LD E u8
            op_8bit_load(DE.low, read(PC.full + 1));
            PC.full += 2;
            cycles += 8;
        break;
    
        case 0x1F: //RRA
            right = true;
            op_rotate_A();
            set_unpre_flags();
            right = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x20: //JR NZ i8
            if(!check_flag(7)){
                op_jump_signed(read(PC.full + 1));
                cycles += 12;
            }else{
                cycles += 8;
                PC.full +=2;
            }
        break;
    
        case 0x21: //LD HL u16
            op_16bit_load(HL);
            cycles += 12;
            PC.full += 3;
        break;
    
        case 0x22: // LD (HL+) A 
            write(HL.full, AF.high);
            HL.full++;
            cycles += 8;
            PC.full ++;
        break;
    
        case 0x23: //INC HL
            HL.full++;
            PC.full++;
            cycles += 8;
        break;
    
        case 0x24: //INC H
            op_inc(HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x25: //DEC H
            op_dec(HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x26: //LD H u8
            op_8bit_load(HL.high, read(PC.full + 1));
            cycles += 8;
            PC.full += 2;;
        break;
    
        case 0x27: //DAA
            op_DAA();
            PC.full++;
            cycles += 4;
        break;
    
        case 0x28: //JR Z i8
            if(check_flag(7)){
                cycles += 12;
                op_jump_signed(read(PC.full + 1));
            }else{
                cycles += 8;
                PC.full+= 2;
            }
        break;
    
        case 0x29: //ADD HL HL
            op_16bit_add(HL.full,HL.full);
            PC.full++;
            cycles += 8;
        break;
    
        case 0x2A: //LD A (HL+)
            op_8bit_load(AF.high, read(HL.full));
            HL.full++;
            PC.full++;
            cycles += 8;
        break;
    
        case 0x2B: //DEC HL
            HL.full--;
            PC.full++;
            cycles += 8;
        break;
    
        case 0x2C: //INC L
           op_inc(HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x2D: //DEC L
            op_dec(HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x2E: //LD L u8
            op_8bit_load(HL.low, read(PC.full + 1));
            cycles += 8;
            PC.full += 2;
        break;
    
        case 0x2F: //CPL
            op_cpl();
            PC.full++;
            cycles += 4;
        break;
    
        case 0x30: //JR NC i8
            if(!check_flag(4)){
                cycles += 12;
                op_jump_signed(read(PC.full + 1));
            }else{
                cycles += 8;
                PC.full += 2;
            }
        break;
    
        case 0x31: //LD SP u16
            op_16bit_load(SP);
            cycles += 12;
            PC.full += 3;
        break;
    
        case 0x32:// LD (HL-) A
            write(HL.full, AF.high);
            HL.full--;
            cycles += 8;
            PC.full++;
        break;
    
        case 0x33: //INC SP
            SP.full++;
            PC.full++;
            cycles += 8;
        break;
    
        case 0x34:{ //INC (HL)
            uint8_t val = read(HL.full);
            op_inc(val);
            write(HL.full, val);
            PC.full++;
            cycles += 12;
        }
        break;
    
        case 0x35:{ //DEC (HL)
            uint8_t val = read(HL.full);
            op_dec(val);
            write(HL.full, val);
            PC.full++;
            cycles += 12;
        }
        break;
    
        case 0x36: // LD (HL) u8
            write(HL.full, read(PC.full + 1));
            cycles += 12;
            PC.full += 2;
        break;
    
        case 0x37: //SCF
            set_flag(4);
            unset_flag(6);
            unset_flag(5);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x38: //JR C i8
            if(check_flag(4)){
                cycles += 12;
                op_jump_signed(read(PC.full + 1));
            }else{
                cycles += 8;
                PC.full += 2;
            }
        break;
    
        case 0x39: //ADD HL SP
            op_16bit_add(HL.full, SP.full);
            PC.full++;
            cycles += 8;
        break;
    
        case 0x3A: //LD A (HL-)
            op_8bit_load(AF.high, read(HL.full));
            HL.full--;
            PC.full++;
            cycles += 8;
        break;
    
        case 0x3B: //DEC SP
            SP.full--;
            PC.full++;
            cycles += 8;
        break;
    
        case 0x3C: //INC A
            op_inc(AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x3D: //DEC A
            op_dec(AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x3E: //LD A u8
            op_8bit_load(AF.high, read(PC.full + 1));
            cycles += 8;
            PC.full += 2;
        break;
    
        case 0x3F: // CCF
            unset_flag(6);
            unset_flag(5);
            flip_flag(4);
            PC.full++;
            cycles += 4;
        break;
            
        case 0x40: // LD B B
            op_8bit_load(BC.high, BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x41: // LD B C
            op_8bit_load(BC.high, BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x42: // LD B D
            op_8bit_load(BC.high, DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x43: // LD B E
            op_8bit_load(BC.high, DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x44: // LD B H
            op_8bit_load(BC.high, HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x45: // LD B L
            op_8bit_load(BC.high, HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x46: //LD B (HL)
            op_8bit_load(BC.high, read(HL.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x47: //LD B A
            op_8bit_load(BC.high, AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x48: // LD C B
            op_8bit_load(BC.low, BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x49: // LD C C
            op_8bit_load(BC.low, BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x4A: // LD C D
            op_8bit_load(BC.low, DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x4B: // LD C E
            op_8bit_load(BC.low, DE.low);
            PC.full += 2;
            cycles += 4;
        break;
    
        case 0x4C: // LD C H
            op_8bit_load(BC.low, HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x4D: // LD C L
            op_8bit_load(BC.low, HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x4E:{ // LD C (HL)
            op_8bit_load(BC.low, read(HL.full));
            PC.full++;
            cycles += 8;
        }
        break;
    
        case 0x4F: // LD C A
            op_8bit_load(BC.low, AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x50: // LD D B
            op_8bit_load(DE.high, BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x51: // LD D C
            op_8bit_load(DE.high, BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x52: // LD D D
            op_8bit_load(DE.high, DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x53: // LD D E
            op_8bit_load(DE.high, DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x54: // LD D H
            op_8bit_load(DE.high, HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x55: // LD D L
            op_8bit_load(DE.high, HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x56: //LD D (HL)
            op_8bit_load(DE.high, read(HL.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x57: //LD D A
            op_8bit_load(DE.high, AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x58: // LD E B
            op_8bit_load(DE.low, BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x59: // LD E C
            op_8bit_load(DE.low, BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x5A: // LD E D
            op_8bit_load(DE.low, DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x5B: // LD E E
            op_8bit_load(DE.low, DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x5C: // LD E H
            op_8bit_load(DE.low, HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x5D: // LD E L
            op_8bit_load(DE.low, HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x5E:{ // LD E (HL)
            op_8bit_load(DE.low, read(HL.full));
            PC.full++;
            cycles += 8;
        }
        break;
    
        case 0x5F: // LD E A
            op_8bit_load(DE.low, AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x60: // LD H B
            op_8bit_load(HL.high, BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x61: // LD H C
            op_8bit_load(HL.high, BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x62: // LD H D
            op_8bit_load(HL.high, DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x63: // LD H E
            op_8bit_load(HL.high, DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x64: // LD H H
            op_8bit_load(HL.high, HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x65: // LD H L
            op_8bit_load(HL.high, HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x66: //LD H (HL)
            op_8bit_load(HL.high, read(HL.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x67: //LD H A
            op_8bit_load(HL.high, AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x68: // LD L B
            op_8bit_load(HL.low, BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x69: // LD L C
            op_8bit_load(HL.low, BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x6A: // LD L D
            op_8bit_load(HL.low, DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x6B: // LD L E
            op_8bit_load(HL.low, DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x6C: // LD L H
            op_8bit_load(HL.low, HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x6D: // LD L L
            op_8bit_load(HL.low, HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x6E:{ // LD L (HL)
            op_8bit_load(HL.low, read(HL.full));
            PC.full++;
            cycles += 8;
        }
        break;
    
        case 0x6F: // LD L A
            op_8bit_load(HL.low, AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x70: // LD (HL) B
            write(HL.full, BC.high);
            PC.full++;
            cycles += 8;
        break;
    
        case 0x71: // LD (HL) C
            write(HL.full, BC.low);
            PC.full++;
            cycles += 8;
        break;
    
        case 0x72: // LD (HL) D
            write(HL.full, DE.high);
            PC.full++;
            cycles += 8;
        break;
    
        case 0x73: // LD (HL) E
            write(HL.full, DE.low);
            PC.full++;
            cycles += 8;
        break;
    
        case 0x74: // LD (HL) H
            write(HL.full, HL.high);
            PC.full++;
            cycles += 8;
        break;
    
        case 0x75: // LD (HL) L
            write(HL.full, HL.low);
            PC.full++;
            cycles += 8;
        break;
    
        case 0x76: //HALT
           halt = true;
           cycles += 4;
           PC.full++;
        break;
    
        case 0x77: //LD (HL) A
            write(HL.full, AF.high);
            PC.full++;
            cycles += 8;
        break;
    
         case 0x78: // LD A B
            op_8bit_load(AF.high, BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x79: // LD A C
            op_8bit_load(AF.high, BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x7A: // LD A D
            op_8bit_load(AF.high, DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x7B: // LD A E
            op_8bit_load(AF.high, DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x7C: // LD A H
            op_8bit_load(AF.high, HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x7D: // LD A L
            op_8bit_load(AF.high, HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x7E:{ // LD A (HL)
            op_8bit_load(AF.high, read(HL.full));
            PC.full++;
            cycles += 8;
        }
        break;
    
        case 0x7F: // LD A A
            op_8bit_load(AF.high, AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x80: // ADD A B
            op_8bit_add(BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x81: // ADD A C
            op_8bit_add(BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x82: // ADD A D
            op_8bit_add(DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x83: // ADD A E
            op_8bit_add(DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x84: // ADD A H
            op_8bit_add(HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x85: // ADD A L
            op_8bit_add(HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x86: // ADD A (HL)
            op_8bit_add(read(HL.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x87: // ADD A A
            op_8bit_add(AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x88: // ADC A B
            carrying = true;
            op_8bit_add(BC.high);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x89: // ADC A C
            carrying = true;
            op_8bit_add(BC.low);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x8A: // ADC A D
            carrying = true;
            op_8bit_add(DE.high);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x8B: // ADC A E
            carrying = true;
            op_8bit_add(DE.low);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x8C: // ADC A H
            carrying = true;
            op_8bit_add(HL.high);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x8D: // ADC A L
            carrying = true;
            op_8bit_add(HL.low);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x8E:// ADC A (HL)
            carrying = true;
            op_8bit_add(read(HL.full));
            carrying = false;
            PC.full++;
            cycles += 8;
        break;
    
        case 0x8F: // ADC A A
            carrying = true;
            op_8bit_add(AF.high);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x90: // SUB A B
            op_8bit_subtract(BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x91: // SUB A C
            op_8bit_subtract(BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x92: // SUB A D
            op_8bit_subtract(DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x93: // SUB A E
            op_8bit_subtract(DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x94: // SUB A H
            op_8bit_subtract(HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x95: // SUB A L
            op_8bit_subtract(HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x96: // SUB A (HL)
            op_8bit_subtract(read(HL.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x97: // SUB A A
            op_8bit_subtract(AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0x98: //SBC A B
            carrying = true;
            op_8bit_subtract(BC.high);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x99: //SBC A C
            carrying = true;
            op_8bit_subtract(BC.low);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x9A: //SBC A D
            carrying = true;
            op_8bit_subtract(DE.high);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x9B: //SBC A E
            carrying = true;
            op_8bit_subtract(DE.low);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x9C: //SBC A H
            carrying = true;
            op_8bit_subtract(HL.high);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x9D: //SBC A L
            carrying = true;
            op_8bit_subtract(HL.low);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0x9E: //SBC A (HL)
            carrying = true;
            op_8bit_subtract(read(HL.full));
            carrying = false;
            PC.full++;
            cycles += 8;
        break;
    
        case 0x9F: //SBC A A
            carrying = true;
            op_8bit_subtract(AF.high);
            carrying = false;
            PC.full++;
            cycles += 4;
        break;
    
        case 0xA0: //AND A B
            op_8bit_and(BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xA1: //AND A C
            op_8bit_and(BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xA2: //AND A D
            op_8bit_and(DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xA3: //AND A E
            op_8bit_and(DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xA4: //AND A H
            op_8bit_and(HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xA5: //AND A L
            op_8bit_and(HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xA6: //AND A (HL)
            op_8bit_and(read(HL.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0xA7: //AND A A
            op_8bit_and(AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xA8: //XOR A B
            op_8bit_xor(BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xA9: //XOR A C
            op_8bit_xor(BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xAA: //XOR A D
            op_8bit_xor(DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xAB: //XOR A E
            op_8bit_xor(DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xAC: //XOR A H
            op_8bit_xor(HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xAD: //XOR A L
            op_8bit_xor(HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xAE: //XOR A (HL)
            op_8bit_xor(read(HL.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0xAF: //XOR A A
            op_8bit_xor(AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xB0: //OR A B
            op_8bit_or(BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xB1: //OR A C
            op_8bit_or(BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xB2: //OR A D
            op_8bit_or(DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xB3: //OR A E
            op_8bit_or(DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xB4: //OR A H
            op_8bit_or(HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xB5: //OR A L
            op_8bit_or(HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xB6: //OR A (HL)
            op_8bit_or(read(HL.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0xB7: //OR A A
            op_8bit_or(AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xB8: //CP A B
            op_8bit_compare(BC.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xB9: //CP A C
            op_8bit_compare(BC.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xBA: //CP A D
            op_8bit_compare(DE.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xBB: //CP A E
            op_8bit_compare(DE.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xBC: //CP A H
            op_8bit_compare(HL.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xBD: //CP A L
            op_8bit_compare(HL.low);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xBE: //CP A (HL)
            op_8bit_compare(read(HL.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0xBF: //CP A A
            op_8bit_compare(AF.high);
            PC.full++;
            cycles += 4;
        break;
    
        case 0xC0: //RET NZ
            if(!check_flag(7)){
                cycles += 20;
                op_return();
            }else{
                cycles += 8;
                PC.full++;
            }
        break;
    
        case 0xC1: //POP BC
            op_pop(BC);
            PC.full++;
            cycles += 12;
        break;
    
        case 0xC2: //JP NZ u16
            if(!check_flag(7)){
                cycles += 16;
                op_jump();
            }else{
                cycles += 12;
                PC.full += 3;
            }
        break;
    
        case 0xC3: //JP u16
            cycles += 16;
            op_jump();
        break;
    
        case 0xC4: //CALL NZ u16
            if(!check_flag(7)){
                cycles += 24;
                op_call();
            }else{
                cycles += 12;
                PC.full += 3;
            }
        break;
    
        case 0xC5: //PUSH BC
            op_push(BC);
            PC.full++;
            cycles += 16;
        break;
    
        case 0xC6: //ADD A u8
            op_8bit_add(read(PC.full + 1));
            cycles += 8;
            PC.full += 2;
        break;
    
        case 0xC7: //RST 00h
            cycles += 16;
            op_restart(0x00);
        break;
    
        case 0xC8: //RET Z
            if(check_flag(7)){
                cycles += 20;
                op_return();
            }else{
                cycles += 8;
                PC.full++;
            }
        break;
    
        case 0xC9: //RET
            cycles += 16;
            op_return();
        break;
        
        case 0xCA: //JP Z u16
            if(check_flag(7)){
                cycles += 16;
                op_jump();
            }else{
                cycles += 12;
                PC.full += 3;
            }
        break;
        
        case 0xCC: //CALL Z u16
            if(check_flag(7)){
                cycles += 24;
                op_call();
            }else{
                cycles += 12;
                PC.full += 3;
            }
        break;
    
        case 0xCD: //CALL u16
            cycles += 24;
            op_call();
        break;
    
        case 0xCE: //ADC A u8
            carrying = true;
            op_8bit_add(read(PC.full + 1));
            carrying = false;
            cycles += 8;
            PC.full += 2;
        break;
    
        case 0xCF: //RST 08h
            cycles += 16;
            op_restart(0x08);
        break;
    
        case 0xD0: //RET NC
            if(!check_flag(4)){
                cycles += 20;
                op_return();
            }else{
                cycles += 8;
                PC.full++;
            }
        break;
    
        case 0xD1: //POP DE
            op_pop(DE);
            cycles += 12;
            PC.full++;
        break;
    
        case 0xD2: //JP NC u16
            if(!check_flag(4)){
                cycles += 16;
                op_jump();
            }else{
                cycles += 12;
                PC.full += 3;
            }
        break;
    
        case 0xD3: //NOP
            cycles += 4;
            PC.full++;
        break;
            
    
        case 0xD4: //CALL NC u16
            if(!check_flag(4)){
                cycles += 24;
                op_call();
            }else{
                cycles += 12;
                PC.full += 3;
            }
        break;
    
        case 0xD5: //PUSH DE
            op_push(DE);
            cycles += 16;
            PC.full++;
        break;
    
        case 0xD6: //SUB A u8
            op_8bit_subtract(read(PC.full + 1));
            cycles += 8;
            PC.full += 2;
        break;
    
        case 0xD7: //RST 10h
            cycles += 16;
            op_restart(0x10);
        break;
    
        case 0xD8: //RET C
            if(check_flag(4)){
                cycles += 20;
                op_return();
            }else{
                cycles += 8;
                PC.full++;
            }
        break;
    
        case 0xD9: //RETI
            IME = true; 
            op_return();
            cycles += 16;
        break;
    
        case 0xDA: //JP C u16
            if(check_flag(4)){
                cycles += 16;
                op_jump();
            }else{
                cycles += 12;
                PC.full += 3;
            }
        break;
    
        case 0xDB: //NOP
            cycles += 4;
            PC.full++;
        break;
    
        case 0xDC: //CALL C u16
            if(check_flag(4)){
                cycles += 24;
                op_call();
            }else{
                cycles += 12;
                PC.full += 3;
            }
        break;
    
        case 0xDD: //NOP
            cycles += 4;
            PC.full++;
        break;
        
    
        case 0xDE: //SBC A u8
            carrying = true;
            op_8bit_subtract(read(PC.full + 1));
            carrying = false;
            cycles += 8;
            PC.full += 2;
        break;
    
        case 0xDF: //RST 18h
            cycles += 16;
            op_restart(0x18);
        break;
    
        case 0xE0:{ //LD(FF00+u8) A
            uint16_t val = 0xFF00 + read(PC.full + 1);
            write(val, AF.high);
            cycles += 12;
            PC.full +=2;
        }
        break;
    
        case 0xE1: //POP HL
            op_pop(HL);
            cycles += 12;
            PC.full++;
        break;
    
        case 0xE2:{ //LD(FF00+C) A
            uint16_t val = 0xFF00 + BC.low;
            write(val, AF.high);
            cycles += 8;
            PC.full++;
        }
        break;
    
        case 0xE3: //NOP
            cycles += 4;
            PC.full++;
        break;
    
        case 0xE4: //NOP
            cycles += 4;
            PC.full++;
        break;
    
        case 0xE5: //PUSH HL
            op_push(HL);
            cycles += 16;
            PC.full++;
        break;
    
        case 0xE6: //AND A u8
            op_8bit_and(read(PC.full + 1));
            cycles += 8;
            PC.full +=2;
        break;
    
        case 0xE7: //RST 20h
            cycles += 16;
            op_restart(0x20);
        break;
    
        case 0xE8:{ //ADD SP i8
            unset_flag(7);
            unset_flag(6);
            
            uint8_t uval = read(PC.full + 1);
            int8_t sval = uval;
            
            if((((SP.low & 0xf) + (uval & 0xf)) & 0x10) == 0x10){
                set_flag(5);
            }else{
                unset_flag(5);
            }
            
            if((SP.low + uval ) > 0xFF){
                  set_flag(4);
            }else{
                unset_flag(4);
            }
            
            SP.full += sval;
            cycles += 16;
            PC.full += 2;
        }
        break;
    
        case 0xE9: //JP HL
            cycles += 4;
            PC.full++;
            PC.full = HL.full;
        break;
    
        case 0xEA:{ //LD (u16) A
            Register val;
            op_16bit_load(val);
            write(val.full, AF.high);
            cycles += 16;
            PC.full += 3;
        }
        break;
        
        case 0xEB: //NOP
            cycles += 4;
            PC.full++;
        break;
    
        case 0xEC: //NOP
            cycles += 4;
            PC.full++;
        break;
    
        case 0xED: //NOP
            cycles +=4;
            PC.full++;
        break;
    
        case 0xEE: //XOR A u8
            op_8bit_xor(read(PC.full + 1));
            cycles += 8;
            PC.full += 2;
        break;
    
        case 0xEF: //RST 28h
            cycles += 16;
            op_restart(0x28);
        break;
    
        case 0xF0: //LD A (FF00+u8)
            op_8bit_load(AF.high, read(0xFF00 + read(PC.full + 1)));
            cycles += 12;
            PC.full +=2;
        break;
    
        case 0xF1:{ //POP AF
            A = true;
            op_pop(AF);
            A = false;
            cycles += 12;
            PC.full++;
        }
        break;
    
        case 0xF2: //LD A (FF00+C)
            op_8bit_load(AF.high, read(0xFF00 + BC.low));
            cycles += 8;
            PC.full++;
        break;
    
        case 0xF3: //DI
            IME = false;
            cycles += 4;
            PC.full++;
        break;
    
        case 0xF4: //NOP
            cycles += 4;
            PC.full++;
        break;
    
        case 0xF5: //PUSH AF
            op_push(AF);
            cycles += 16;
            PC.full++;
        break;
    
        case 0xF6: //OR A u8
            op_8bit_or(read(PC.full + 1));
            cycles += 8;
            PC.full +=2;
        break;
    
        case 0xF7: //RST 30h
            cycles += 16;
            op_restart(0x30);
        break;
    
        case 0xF8:{ //LD HL SP+i8
            unset_flag(7);
            unset_flag(6);
            
            uint8_t uval = read(PC.full + 1);
            int8_t sval = uval;
            
            
            if((((SP.low & 0xf) + (uval & 0xf)) & 0x10) == 0x10){
                    set_flag(5);
            }else{
                unset_flag(5);
            }
            
            if((SP.low + uval) > 0xFF){
                  set_flag(4);
            }else{
                unset_flag(4);
            }
            
            HL.full = SP.full + sval;
            cycles += 12;
            PC.full +=2;
        }
        break;
    
        case 0xF9: //LD SP HL
            cycles += 8;
            SP.full = HL.full;
            PC.full++;
        break;
    
        case 0xFA:{ //LD A (u16)
            Register value;
            op_16bit_load(value);
            AF.high = read(value.full);
            cycles += 16;
            PC.full += 3;
        }
        break;
    
        case 0xFB: //EI
            IME = true;
            cycles += 4;
            PC.full++;
        break;
    
        case 0xFC: //NOP
            cycles += 4;
            PC.full++;
        break;
    
        case 0xFD: //NOP
            cycles += 4;
            PC.full++;
        break;
    
        case 0xFE: //CP A u8
            op_8bit_compare(read(PC.full + 1));
            cycles += 8;
            PC.full +=2;
        break;
    
        case 0xFF: //RST 38h
            cycles += 16;
            op_restart(0x30);
        break;
    
        default:
            printf ("Unkown opcode: 0x%X\n", opcode);
    }
}