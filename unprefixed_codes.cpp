#include "Gameboy.h"

void Gameboy::decode1(uint8_t opcode){
    switch(opcode){
        case 0x00: // NOP 
            cycles += 4;
            PC.full++;
        break;
    
        case 0x01: // LD BC u16 
            op_16bit_load(BC, read(PC.full));
            cycles += 12;
            PC.full += 2;
        break;
    
        case 0x02: // LD (BC) A 
            write(BC.full, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x03: //INC BC
            BC.full++;
            cycles += 8;
        break;
    
        case 0x04: //INC B
            BC.high++;
            unset_flag(6);
            if(BC.high == 0) set_flag(7);
            if(((BC.high & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x05: //DEC B
            BC.high--;
            set_flag(6);
            if(BC.high == 0) set_flag(7);
            if(((BC.high & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            
            cycles += 4;
        break;
    
        case 0x06: // LD B u8
            op_8bit_load(BC.high, read(PC.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x07: //RLCA
            left = true;
            op_rotate(AF.high);
            cycles += 4;
        break;
    
        case 0x08:{ //LD (u16) SP
            Register nn;
            nn.low = read(PC.full);
            nn.high = read(PC.full++);
            nn.full = SP.full;
            cycles += 20;
            PC.full += 2;
        }
        break;
    
        case 0x09: //ADD HL BC
            op_16bit_add_to_hl(BC.full);
            cycles += 8;
        break;
    
        case 0x0A: //LD A (BC)
            op_8bit_load(AF.high, read(BC.full));
            cycles += 8;
        break;
    
        case 0x0B: //DEC BC
            BC.full--;
            cycles += 8;
        break;
    
        case 0x0C: //INC C
            BC.low++;
            unset_flag(6);
            if(BC.low == 0) set_flag(7);
            if(((BC.low & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x0D: //DEC C
            BC.low--;
            set_flag(6);
            if(BC.low == 0) set_flag(7);
            if(((BC.low & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            
            cycles += 4;
        break;
    
        case 0x0E: //LD C u8
            op_8bit_load(BC.low, read(PC.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x0F: //RRCA
            op_rotate(AF.high);
            cycles += 4;
        break;
    
        case 0x10: //STOP
            halt = true;
        break;
    
        case 0x11: //LD DE u16
            op_16bit_load(DE, read(PC.full));
            PC.full += 2;
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
        break;
    
        case 0x14: //INC D
            DE.high++;
            unset_flag(6);
            if(DE.high == 0) set_flag(7);
            if(((DE.high & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x15: //DEC D
            DE.high--;
            set_flag(6);
            if(DE.high == 0) set_flag(7);
            if(((DE.high & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x16: //LD D u8
            op_8bit_load(DE.high, read(PC.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x17: //RLA
            left = true;
            op_rotate(AF.high);
            cycles += 4;
        break;
    
        case 0x18: //JR i8
            op_jump_signed(read(PC.full));
            cycles += 12;
        break;
    
        case 0x19: //ADD HL DE
            op_16bit_add_to_hl(DE.full);
            cycles += 8;
        break;
    
        case 0x1A: //LD A (DE)
            op_8bit_load(AF.high, read(DE.full));
            cycles += 8;
        break;
    
        case 0x1B: //DEC DE
            DE.full--;
            cycles += 8;
        break;
    
        case 0x1C: //INC E
            DE.low++;
            unset_flag(6);
            if(DE.low == 0) set_flag(7);
            if(((DE.low & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x1D: //DEC E
            DE.low--;
            set_flag(6);
            if(DE.low == 0) set_flag(7);
            if(((DE.low & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x1E: //LD E u8
            op_8bit_load(DE.low, read(PC.full));
            PC.full++;
            cycles += 8;
        break;
    
        case 0x1F: //RRA
            right = true;
            op_rotate(AF.high);
            cycles += 4;
        break;
    
        case 0x21: //JR NZ i8
            if(!check_flag(7)){
                op_jump_signed(read(PC.full));
                cycles += 12;
            }else{
                cycles += 8;
                PC.full++;
            }
        break;
    
        case 0x22: // LD (HL+) A 
            write(HL.full, AF.high);
            HL.full++;
            cycles += 8;
            PC.full++;
        break;
    
        case 0x23: //INC HL
            HL.full++;
            cycles += 8;
        break;
    
        case 0x24: //INC H
            HL.high++;
            unset_flag(6);
            if(HL.high == 0) set_flag(7);
            if(((HL.high & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x25: //DEC H
            HL.high--;
            set_flag(6);
            if(HL.high == 0) set_flag(7);
            if(((HL.high & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x26: //LD H u8
            op_8bit_load(HL.high, read(PC.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0x27: //DAA
            op_DAA();
            cycles += 4;
        break;
    
        case 0x28: //JR Z i8
            if(check_flag(7)){
                cycles += 12;
                op_jump_signed(read(PC.full));
            }else{
                cycles += 8;
                PC.full++;
            }
        break;
    
        case 0x29: //ADD HL HL
            op_16bit_add_to_hl(HL.full);
            cycles += 8;
        break;
    
        case 0x2A: //LD A (HL+)
            op_8bit_load(AF.high, read(HL.full));
            HL.full++;
            cycles += 8;
        break;
    
        case 0x2B: //DEC HL
            HL.full--;
            cycles += 8;
        break;
    
        case 0x2C: //INC L
            HL.low++;
            unset_flag(6);
            if(HL.low == 0) set_flag(7);
            if(((HL.low & 0xf) & 0x10) == 0x10){
                set_flag(7);
            }
            cycles += 4;
        break;
    
        case 0x2D: //DEC L
            HL.low--;
            set_flag(6);
            if(HL.low == 0) set_flag(7);
            if(((HL.low & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x2E: //LD L u8
            op_8bit_load(HL.low, read(PC.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0x2F: //CPL
            op_cpl();
            cycles += 4;
        break;
    
        case 0x30: //JR NC i8
            if(!check_flag(4)){
                cycles += 12;
                op_jump_signed(read(PC.full));
            }else{
                cycles += 8;
                PC.full++;
            }
        break;
    
        case 0x31:{ //LD SP u16
            Register u16;
            u16.low = PC.full++;
            u16.high = PC.full += 2;
            op_16bit_load(SP, read(u16.full));
            cycles += 12;
            PC.full += 2;
        }
        break;
    
        case 0x32: // LD (HL-) A
            write(HL.full, AF.high);
            HL.full--;
            cycles += 8;
            PC.full++;
        break;
    
        case 0x33: //INC SP
            SP.full++;
            cycles += 8;
        break;
    
        case 0x34:{ //INC (HL)
            uint8_t val = read(HL.full);
            val++;
            unset_flag(6);
            if(val == 0) set_flag(7);
            if(((val & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 12;
        }
        break;
    
        case 0x35:{ //DEC (HL)
            uint8_t val = read(HL.full);
            val--;
            set_flag(6);
            if(val == 0) set_flag(7);
            if(((val & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 12;
        }
        break;
    
        case 0x36: // LD (HL) u8
            write(HL.full, read(PC.full));
            cycles += 12;
            PC.full++;
        break;
    
        case 0x37: //SCF
            set_flag(4);
            cycles += 4;
        break;
    
        case 0x38: //JR C i8
            if(check_flag(4)){
                cycles += 12;
                op_jump_signed(read(PC.full));
            }else{
                cycles += 8;
                PC.full++;
            }
        break;
    
        case 0x39: //ADD HL SP
            op_16bit_add_to_hl(SP.full);
            cycles += 8;
        break;
    
        case 0x3A: //LD A (HL-)
            op_8bit_load(AF.high, read(HL.full));
            HL.full--;
            cycles += 8;
        break;
    
        case 0x3B: //DEC SP
            SP.full--;
            cycles += 8;
        break;
    
        case 0x3C: //INC A
            AF.high++;
            unset_flag(6);
            if(AF.high == 0) set_flag(7);
            if(((AF.high & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x3D: //DEC A
            BC.high--;
            set_flag(6);
            if(AF.high == 0) set_flag(7);
            if(((AF.high & 0xf) & 0x10) == 0x10){
                set_flag(5);
            }
            cycles += 4;
        break;
    
        case 0x3E: //LD A u8
            op_8bit_load(AF.high, read(PC.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0x3F: // CCF
            flip_flag(4);
            cycles += 4;
        break;
            
        case 0x40: // LD B B
            op_8bit_load(BC.high, BC.high);
            cycles += 4;
        break;
    
        case 0x41: // LD B C
            op_8bit_load(BC.high, BC.low);
            cycles += 4;
        break;
    
        case 0x42: // LD B D
            op_8bit_load(BC.high, DE.high);
            cycles += 4;
        break;
    
        case 0x43: // LD B E
            op_8bit_load(BC.high, DE.low);
            cycles += 4;
        break;
    
        case 0x44: // LD B H
            op_8bit_load(BC.high, HL.high);
            cycles += 4;
        break;
    
        case 0x45: // LD B L
            op_8bit_load(BC.high, HL.low);
            cycles += 4;
        break;
    
        case 0x46: //LD B (HL)
            op_8bit_load(BC.high, read(HL.full));
            cycles += 8;
        break;
    
        case 0x47: //LD B A
            op_8bit_load(BC.high, AF.high);
            cycles += 4;
        break;
    
        case 0x48: // LD C B
            op_8bit_load(BC.low, BC.high);
            cycles += 4;
        break;
    
        case 0x49: // LD C C
            op_8bit_load(BC.low, BC.low);
            cycles += 4;
        break;
    
        case 0x4A: // LD C D
            op_8bit_load(BC.low, DE.high);
            cycles += 4;
        break;
    
        case 0x4B: // LD C E
            op_8bit_load(BC.low, DE.low);
            cycles += 4;
        break;
    
        case 0x4C: // LD C H
            op_8bit_load(BC.low, HL.high);
            cycles += 4;
        break;
    
        case 0x4D: // LD C L
            op_8bit_load(BC.low, HL.low);
            cycles += 4;
        break;
    
        case 0x4E:{ // LD C (HL)
            uint8_t val = read(HL.full);
            op_8bit_load(BC.low, val);
            cycles += 8;
        }
        break;
    
        case 0x4F: // LD C A
            op_8bit_load(BC.low, AF.high);
            cycles += 4;
        break;
    
        case 0x50: // LD D B
            op_8bit_load(DE.high, BC.high);
            cycles += 4;
        break;
    
        case 0x51: // LD D C
            op_8bit_load(DE.high, BC.low);
            cycles += 4;
        break;
    
        case 0x52: // LD D D
            op_8bit_load(DE.high, DE.high);
            cycles += 4;
        break;
    
        case 0x53: // LD D E
            op_8bit_load(DE.high, DE.low);
            cycles += 4;
        break;
    
        case 0x54: // LD D H
            op_8bit_load(DE.high, HL.high);
            cycles += 4;
        break;
    
        case 0x55: // LD D L
            op_8bit_load(DE.high, HL.low);
            cycles += 4;
        break;
    
        case 0x56: //LD D (HL)
            op_8bit_load(DE.high, read(HL.full));
            cycles += 8;
        break;
    
        case 0x57: //LD D A
            op_8bit_load(DE.high, AF.high);
            cycles += 4;
        break;
    
        case 0x58: // LD E B
            op_8bit_load(DE.low, BC.high);
            cycles += 4;
        break;
    
        case 0x59: // LD E C
            op_8bit_load(DE.low, BC.low);
            cycles += 4;
        break;
    
        case 0x5A: // LD E D
            op_8bit_load(DE.low, DE.high);
            cycles += 4;
        break;
    
        case 0x5B: // LD E E
            op_8bit_load(DE.low, DE.low);
            cycles += 4;
        break;
    
        case 0x5C: // LD E H
            op_8bit_load(DE.low, HL.high);
            cycles += 4;
        break;
    
        case 0x5D: // LD E L
            op_8bit_load(DE.low, HL.low);
            cycles += 4;
        break;
    
        case 0x5E:{ // LD E (HL)
            uint8_t val = read(HL.full);
            op_8bit_load(DE.low, val);
            cycles += 8;
        }
        break;
    
        case 0x5F: // LD E A
            op_8bit_load(DE.low, AF.high);
            cycles += 4;
        break;
    
        case 0x60: // LD H B
            op_8bit_load(HL.high, BC.high);
            cycles += 4;
        break;
    
        case 0x61: // LD H C
            op_8bit_load(HL.high, BC.low);
            cycles += 4;
        break;
    
        case 0x62: // LD H D
            op_8bit_load(HL.high, DE.high);
            cycles += 4;
        break;
    
        case 0x63: // LD H E
            op_8bit_load(HL.high, DE.low);
            cycles += 4;
        break;
    
        case 0x64: // LD H H
            op_8bit_load(HL.high, HL.high);
            cycles += 4;
        break;
    
        case 0x65: // LD H L
            op_8bit_load(HL.high, HL.low);
            cycles += 4;
        break;
    
        case 0x66: //LD H (HL)
            op_8bit_load(HL.high, read(HL.full));
            cycles += 8;
        break;
    
        case 0x67: //LD HL A
            op_8bit_load(HL.high, AF.high);
            cycles += 4;
        break;
    
        case 0x68: // LD L B
            op_8bit_load(HL.low, BC.high);
            cycles += 4;
        break;
    
        case 0x69: // LD L C
            op_8bit_load(HL.low, BC.low);
            cycles += 4;
        break;
    
        case 0x6A: // LD L D
            op_8bit_load(HL.low, DE.high);
            cycles += 4;
        break;
    
        case 0x6B: // LD L E
            op_8bit_load(HL.low, DE.low);
            cycles += 4;
        break;
    
        case 0x6C: // LD L H
            op_8bit_load(HL.low, HL.high);
            cycles += 4;
        break;
    
        case 0x6D: // LD L L
            op_8bit_load(HL.low, HL.low);
            cycles += 4;
        break;
    
        case 0x6E:{ // LD L (HL)
            uint8_t val = read(HL.full);
            op_8bit_load(HL.low, val);
            cycles += 8;
        }
        break;
    
        case 0x6F: // LD L A
            op_8bit_load(HL.low, AF.high);
            cycles += 4;
        break;
    
        case 0x70: // LD (HL) B
            write(HL.full, BC.high);
            cycles += 8;
        break;
    
        case 0x71: // LD (HL) C
            write(HL.full, BC.low);
            cycles += 8;
        break;
    
        case 0x72: // LD (HL) D
            write(HL.full, DE.high);
            cycles += 8;
        break;
    
        case 0x73: // LD (HL) E
            write(HL.full, DE.low);
            cycles += 8;
        break;
    
        case 0x74: // LD (HL) H
            write(HL.full, HL.high);
            cycles += 8;
        break;
    
        case 0x75: // LD (HL) L
            write(HL.full, HL.low);
            cycles += 8;
        break;
    
        case 0x76: //LD B (HL)
            halt = true;
        break;
    
        case 0x77: //LD (HL) A
            write(HL.full, AF.high);
            cycles += 8;
        break;
    
         case 0x78: // LD A B
            op_8bit_load(AF.high, BC.high);
            cycles += 4;
        break;
    
        case 0x79: // LD A C
            op_8bit_load(AF.high, BC.low);
            cycles += 4;
        break;
    
        case 0x7A: // LD A D
            op_8bit_load(AF.high, DE.high);
            cycles += 4;
        break;
    
        case 0x7B: // LD A E
            op_8bit_load(AF.high, DE.low);
            cycles += 4;
        break;
    
        case 0x7C: // LD A H
            op_8bit_load(AF.high, HL.high);
            cycles += 4;
        break;
    
        case 0x7D: // LD A L
            op_8bit_load(AF.high, HL.low);
            cycles += 4;
        break;
    
        case 0x7E:{ // LD A (HL)
            uint8_t val = read(HL.full);
            op_8bit_load(AF.high, val);
            cycles += 8;
        }
        break;
    
        case 0x7F: // LD A A
            op_8bit_load(AF.high, AF.high);
            cycles += 4;
        break;
    
        case 0x80: // ADD A B
            op_8bit_add(BC.high);
            cycles += 4;
        break;
    
        case 0x81: // ADD A C
            op_8bit_add(BC.low);
            cycles += 4;
        break;
    
        case 0x82: // ADD A D
            op_8bit_add(DE.high);
            cycles += 4;
        break;
    
        case 0x83: // ADD A E
            op_8bit_add(DE.low);
            cycles += 4;
        break;
    
        case 0x84: // ADD A H
            op_8bit_add(HL.high);
            cycles += 4;
        break;
    
        case 0x85: // ADD A L
            op_8bit_add(HL.low);
            cycles += 4;
        break;
    
        case 0x86: // ADD A (HL)
            op_8bit_add(read(HL.full));
            cycles += 8;
        break;
    
        case 0x87: // ADD A A
            carrying = true;
            op_8bit_add(AF.high);
            cycles += 4;
        break;
    
        case 0x88: // ADC A B
            carrying = true;
            op_8bit_add(BC.high);
            cycles += 4;
        break;
    
        case 0x89: // ADC A C
            carrying = true;
            op_8bit_add(BC.low);
            cycles += 4;
        break;
    
        case 0x8A: // ADC A D
            carrying = true;
            op_8bit_add(DE.high);
            cycles += 4;
        break;
    
        case 0x8B: // ADC A E
            carrying = true;
            op_8bit_add(DE.low);
            cycles += 4;
        break;
    
        case 0x8C: // ADC A H
            carrying = true;
            op_8bit_add(HL.high);
            cycles += 4;
        break;
    
        case 0x8D: // ADC A L
            carrying = true;
            op_8bit_add(HL.low);
            cycles += 4;
        break;
    
        case 0x8E: // ADC A (HL)
            carrying = true;
            op_8bit_add(read(HL.full));
            cycles += 8;
        break;
    
        case 0x8F: // ADC A A
            carrying = true;
            op_8bit_add(AF.high);
            cycles += 4;
        break;
    
        case 0x90: // SUB A B
            op_8bit_subtract(BC.high);
            cycles += 4;
        break;
    
        case 0x91: // SUB A C
            op_8bit_subtract(BC.low);
            cycles += 4;
        break;
    
        case 0x92: // SUB A D
            op_8bit_subtract(DE.high);
            cycles += 4;
        break;
    
        case 0x93: // SUB A E
            op_8bit_subtract(DE.low);
            cycles += 4;
        break;
    
        case 0x94: // SUB A H
            op_8bit_subtract(HL.high);
            cycles += 4;
        break;
    
        case 0x95: // SUB A L
            op_8bit_subtract(HL.low);
            cycles += 4;
        break;
    
        case 0x96: // SUB A (HL)
            op_8bit_subtract(read(HL.full));
            cycles += 8;
        break;
    
        case 0x97: // SUB A A
            op_8bit_subtract(AF.high);
            cycles += 4;
        break;
    
        case 0x98: //SBC A B
            carrying = true;
            op_8bit_subtract(BC.high);
            cycles += 4;
        break;
    
        case 0x99: //SBC A C
            carrying = true;
            op_8bit_subtract(BC.low);
            cycles += 4;
        break;
    
        case 0x9A: //SBC A D
            carrying = true;
            op_8bit_subtract(DE.high);
            cycles += 4;
        break;
    
        case 0x9B: //SBC A E
            carrying = true;
            op_8bit_subtract(DE.low);
            cycles += 4;
        break;
    
        case 0x9C: //SBC A H
            carrying = true;
            op_8bit_subtract(HL.high);
            cycles += 4;
        break;
    
        case 0x9D: //SBC A L
            carrying = true;
            op_8bit_subtract(HL.low);
            cycles += 4;
        break;
    
        case 0x9E: //SBC A (HL)
            carrying = true;
            op_8bit_subtract(read(HL.full));
            cycles += 8;
        break;
    
        case 0x9F: //SBC A A
            carrying = true;
            op_8bit_subtract(AF.high);
            cycles += 4;
        break;
    
        case 0xA0: //AND A B
            op_8bit_and(BC.high);
            cycles += 4;
        break;
    
        case 0xA1: //AND A C
            op_8bit_and(BC.low);
            cycles += 4;
        break;
    
        case 0xA2: //AND A D
            op_8bit_and(DE.high);
            cycles += 4;
        break;
    
        case 0xA3: //AND A E
            op_8bit_and(DE.low);
            cycles += 4;
        break;
    
        case 0xA4: //AND A H
            op_8bit_and(HL.high);
            cycles += 4;
        break;
    
        case 0xA5: //AND A L
            op_8bit_and(HL.low);
            cycles += 4;
        break;
    
        case 0xA6: //AND A (HL)
            op_8bit_and(read(HL.full));
            cycles += 8;
        break;
    
        case 0xA7: //AND A A
            op_8bit_and(AF.high);
            cycles += 4;
        break;
    
        case 0xA8: //XOR A B
            op_8bit_xor(BC.high);
            cycles += 4;
        break;
    
        case 0xA9: //XOR A C
            op_8bit_xor(BC.low);
            cycles += 4;
        break;
    
        case 0xAA: //XOR A D
            op_8bit_xor(DE.high);
            cycles += 4;
        break;
    
        case 0xAB: //XOR A E
            op_8bit_xor(DE.low);
            cycles += 4;
        break;
    
        case 0xAC: //XOR A H
            op_8bit_xor(HL.high);
            cycles += 4;
        break;
    
        case 0xAD: //XOR A L
            op_8bit_xor(HL.low);
            cycles += 4;
        break;
    
        case 0xAE: //XOR A (HL)
            op_8bit_xor(read(HL.full));
            cycles += 8;
        break;
    
        case 0xAF: //XOR A A
            op_8bit_xor(AF.high);
            cycles += 4;
        break;
    
        case 0xB0: //OR A B
            op_8bit_or(BC.high);
            cycles += 4;
        break;
    
        case 0xB1: //OR A C
            op_8bit_or(BC.low);
            cycles += 4;
        break;
    
        case 0xB2: //OR A D
            op_8bit_or(DE.high);
            cycles += 4;
        break;
    
        case 0xB3: //OR A E
            op_8bit_or(DE.low);
            cycles += 4;
        break;
    
        case 0xB4: //OR A H
            op_8bit_or(HL.high);
            cycles += 4;
        break;
    
        case 0xB5: //OR A L
            op_8bit_or(HL.low);
            cycles += 4;
        break;
    
        case 0xB6: //OR A (HL)
            op_8bit_or(read(HL.full));
            cycles += 8;
        break;
    
        case 0xB7: //OR A A
            op_8bit_or(AF.high);
            cycles += 4;
        break;
    
        case 0xB8: //CP A B
            op_8bit_compare(BC.high);
            cycles += 4;
        break;
    
        case 0xB9: //CP A C
            op_8bit_compare(BC.low);
            cycles += 4;
        break;
    
        case 0xBA: //CP A D
            op_8bit_compare(DE.high);
            cycles += 4;
        break;
    
        case 0xBB: //CP A E
            op_8bit_compare(DE.low);
            cycles += 4;
        break;
    
        case 0xBC: //CP A H
            op_8bit_compare(HL.high);
            cycles += 4;
        break;
    
        case 0xBD: //CP A L
            op_8bit_compare(HL.low);
            cycles += 4;
        break;
    
        case 0xBE: //CP A (HL)
            op_8bit_compare(read(HL.full));
            cycles += 8;
        break;
    
        case 0xBF: //CP A A
            op_8bit_compare(AF.high);
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
            cycles += 12;
        break;
    
        case 0xC2: //JP NZ u16
            if(!check_flag(7)){
                cycles += 16;
                op_jump();
            }else{
                cycles += 12;
                PC.full++;
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
                PC.full += 2;
            }
        break;
    
        case 0xC5: //PUSH BC
            op_push(BC);
            cycles += 16;
        break;
    
        case 0xC6: //ADD A u8
            op_8bit_add(read(PC.full));
            cycles += 8;
            PC.full++;
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
                PC.full += 2;
            }
        break;
        
        case 0xCC: //CALL Z u16
            if(check_flag(7)){
                cycles += 24;
                op_call();
            }else{
                cycles += 12;
                PC.full += 2;
            }
        break;
    
        case 0xCD: //CALL u16
            cycles += 24;
            op_call();
        break;
    
        case 0xCE: //ADC A u8
            carrying = true;
            op_8bit_add(read(PC.full));
            cycles += 8;
            PC.full++;
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
            }
        break;
    
        case 0xD1: //POP DE
            op_pop(DE);
            cycles += 12;
        break;
    
        case 0xD4: //CALL NC u16
            if(!check_flag(4)){
                cycles += 24;
                op_call();
            }else{
                cycles += 12;
                PC.full += 2;
            }
        break;
    
        case 0xD5: //PUSH DE
            op_push(DE);
            cycles += 16;
        break;
    
        case 0xD6: //SUB A u8
            op_8bit_subtract(read(PC.full));
            cycles += 8;
            PC.full++;
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
            }
        break;
    
        case 0xD9: //RETI
            /* if interupt return
                cycles += 20;
                op_return();
            */
            cycles += 8;
        break;
    
        case 0xDA: //JP C u16
            if(check_flag(4)){
                cycles += 16;
                op_jump();
            }else{
                cycles += 12;
                PC.full += 2;
            }
        break;
    
        case 0xDC: //CALL C u16
            if(check_flag(4)){
                cycles += 24;
                op_call();
            }else{
                cycles += 12;
                PC.full += 2;
            }
        break;
    
        case 0xDE: //SBC A u8
            carrying = true;
            op_8bit_subtract(read(PC.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0xDF: //RST 18h
            cycles += 16;
            op_restart(0x18);
        break;
    
        case 0xE0: //LD(FF00+u8) A
            write((0xFF00 + read(PC.full)), AF.high);
            cycles += 12;
            PC.full++;
        break;
    
        case 0xE1: //POP HL
            op_pop(HL);
            cycles += 12;
        break;
    
        case 0xE2: //LD(FF00+C) A
            write((0xFF00 + BC.low), AF.high);
            cycles += 8;
        break;
    
        case 0xE5: //PUSH HL
            op_push(HL);
            cycles += 16;
        break;
    
        case 0xE6: //AND A u8
            op_8bit_and(read(PC.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0xE7: //RST 20h
            cycles += 16;
            op_restart(0x20);
        break;
    
        case 0xE8:{ //ADD SP i8
            unset_flag(7);
            unset_flag(6);
            
            int8_t val = read(PC.full);
            
            if(((SP.full & 0xf) + (val & 0xf)) & 0x10 == 0x10){
                    set_flag(5);
            }
            
            if((SP.full + val) > 0xFF){
                  set_flag(4);
            }
            
            SP.full += val;
            cycles += 16;
            PC.full++;
        }
        break;
    
        case 0xE9: //JP HL
            cycles += 4;
            PC.full += HL.full;
        break;
    
        case 0xEA: //LD (u16) A
            write(read(PC.full), AF.high);
            write(read(PC.full++), AF.high);
            cycles += 16;
            PC.full += 2;
    
        case 0xEE: //XOR A u8
            op_8bit_xor(read(PC.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0xEF: //RST 28h
            cycles += 16;
            op_restart(0x28);
        break;
    
        case 0xF0: //LD A (FF00+u8)
            op_8bit_load(AF.high, (0xFF00 + read(PC.full)));
            cycles += 12;
            PC.full++;
        break;
    
        case 0xF1: //POP AF
            op_pop(AF);
            cycles += 12;
        break;
    
        case 0xF2: //LD A (FF00+C)
            op_8bit_load(AF.high, (0xFF00 + BC.low));
            cycles += 8;
        break;
    
        case 0xF3: //DI
            IME = false;
            cycles += 4;
        break;
    
        case 0xF5: //PUSH AF
            op_push(AF);
            cycles += 16;
        break;
    
        case 0xF6: //OR A u8
            op_8bit_or(read(PC.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0xF7: //RST 30h
            cycles += 16;
            op_restart(0x30);
        break;
    
        case 0xF8:{ //LD HL SP+i8
            unset_flag(7);
            unset_flag(6);
            
            int8_t val = read(PC.full);
            
            if(((SP.full & 0xf) + (val & 0xf)) & 0x10 == 0x10){
                    set_flag(5);
            }
            
            if((SP.full + val) > 0xFF){
                  set_flag(4);
            }
            
            SP.full += val;
            HL.full = SP.full;
            cycles += 16;
            PC.full++;
        }
        break;
    
        case 0xF9: //LD SP HL
            cycles += 8;
            SP.full == HL.full;
        break;
    
        case 0xFA: //LD A (u16)
            AF.high = read(PC.full++);
            AF.high = read(PC.full += 2);
            cycles += 16;
            PC.full += 2;
        break;
    
        case 0xFB: //EI
            IME = true;
            cycles += 4;
        break;
    
        case 0xFE: //CP A u8
            op_8bit_compare(read(PC.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0xFF: //RST 38h
            cycles += 16;
            op_restart(0x38);
        break;
    }
}