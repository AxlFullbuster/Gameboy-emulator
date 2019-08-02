#include "Gameboy.h"

void Gameboy::decode1(uint8_t opcode){
    switch(opcode){
        case 0x00: // NOP 
            cycles += 4;
        break;
    
        case 0x01: // LD BC u16 
            op_16bit_load(BC, read(pc));
            cycles += 12;
            pc += 2;
        break;
    
        case 0x02: // LD (BC) A 
            write(BC.full, AF.high);
            cycles += 8;
            pc++;
        break;
    
        case 0x03: //INC BC
            BC.full++;
            cycles += 8;
        break;
    
        case 0x04: //INC B
            BC.high++;
            unsetN = true;
            if(BC.high == 0) setZ = true;
            if(((BC.high & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x05: //DEC B
            BC.high--;
            setN = true;
            if(BC.high == 0) setZ = true;
            if(((BC.high & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x0B: //DEC BC
            BC.full--;
            cycles += 8;
        break;
    
        case 0x0C: //INC C
            BC.low++;
            unsetN = true;
            if(BC.low == 0) setZ = true;
            if(((BC.low & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x0D: //DEC C
            BC.low--;
            setN = true;
            if(BC.low == 0) setZ = true;
            if(((BC.low & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x12: // LD (DE) A
            write(DE.full, AF.high);
            cycles += 8;
            pc++;
        break;
    
        case 0x13: //INC DE
            DE.full++;
            cycles += 8;
        break;
    
        case 0x14: //INC D
            DE.high++;
            unsetN = true;
            if(DE.high == 0) setZ = true;
            if(((DE.high & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x15: //DEC D
            DE.high--;
            setN = true;
            if(DE.high == 0) setZ = true;
            if(((DE.high & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x1B: //DEC DE
            DE.full--;
            cycles += 8;
        break;
    
        case 0x1C: //INC E
            DE.low++;
            unsetN = true;
            if(DE.low == 0) setZ = true;
            if(((DE.low & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x1D: //DEC E
            DE.low--;
            setN = true;
            if(DE.low == 0) setZ = true;
            if(((DE.low & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x22: // LD (HL+) A 
            write(HL.full, AF.high);
            HL.full++;
            cycles += 8;
            pc++;
        break;
    
        case 0x23: //INC HL
            HL.full++;
            cycles += 8;
        break;
    
        case 0x24: //INC H
            HL.high++;
            unsetN = true;
            if(HL.high == 0) setZ = true;
            if(((HL.high & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x25: //DEC H
            HL.high--;
            setN = true;
            if(HL.high == 0) setZ = true;
            if(((HL.high & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x2B: //DEC HL
            HL.full--;
            cycles += 8;
        break;
    
        case 0x2C: //INC L
            HL.low++;
            unsetN = true;
            if(HL.low == 0) setZ = true;
            if(((HL.low & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x2D: //DEC L
            HL.low--;
            setN = true;
            if(HL.low == 0) setZ = true;
            if(((HL.low & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x32: // LD (HL-) A
            write(HL.full, AF.high);
            HL.full--;
            cycles += 8;
            pc++;
        break;
    
        case 0x33: //INC SP
            SP.full++;
            cycles += 8;
        break;
    
        case 0x34:{ //INC (HL)
            uint8_t val = read(HL.full);
            val++;
            unsetN = true;
            if(val == 0) setZ = true;
            if(((val & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 12;
        }
        break;
    
        case 0x35:{ //DEC (HL)
            uint8_t val = read(HL.full);
            val--;
            setN = true;
            if(val == 0) setZ = true;
            if(((val & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 12;
        }
        break;
    
        case 0x3B: //DEC SP
            SP.full--;
            cycles += 8;
        break;
    
        case 0x3C: //INC A
            AF.high++;
            unsetN = true;
            if(AF.high == 0) setZ = true;
            if(((AF.high & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
            cycles += 4;
        break;
    
        case 0x3D: //DEC A
            BC.high--;
            setN = true;
            if(AF.high == 0) setZ = true;
            if(((AF.high & 0xf) & 0x10) == 0x10){
                setH = true;
            }
            
            flags();
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
    
        case 0xC6: //ADD A u8
            op_8bit_add(read(pc));
            cycles += 8;
            pc++;
        break;
    
        case 0xCE: //ADC A u8
            carrying = true;
            op_8bit_add(read(pc));
            cycles += 8;
            pc++;
        break;
    
        case 0xD6: //SUB A u8
            op_8bit_subtract(read(pc));
            cycles += 8;
            pc++;
        break;
    
        case 0xDE: //SBC A u8
            carrying = true;
            op_8bit_subtract(read(pc));
            cycles += 8;
            pc++;
        break;
    
        case 0xE6: //AND A u8
            op_8bit_and(read(pc));
            cycles += 8;
            pc++;
        break;
    
        case 0xEE: //XOR A u8
            op_8bit_xor(read(pc));
            cycles += 8;
            pc++;
        break;
    
        case 0xF6: //OR A u8
            op_8bit_or(read(pc));
            cycles += 8;
            pc++;
        break;
    
        case 0xFE: //CP A u8
            op_8bit_compare(read(pc));
            cycles += 8;
            pc++;
        break;
    }
}