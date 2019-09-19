#include "Gameboy.h"
#include <stdio.h>
#include <stdlib.h>

void Gameboy::decode2(uint8_t opcode){
    switch(opcode){
        case 0x00: //RLC B
            left = true;
            op_rotate(BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x01: //RLC C
            left = true;
            op_rotate(BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x02: //RLC D
            left = true;
            op_rotate(DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x03: //RLC E
            left = true;
            op_rotate(DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x04: //RLC H
            left = true;
            op_rotate(HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x05: //RLC L
            left = true;
            op_rotate(HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x06: //RLC (HL)
            left = true;
            op_rotate(read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x07: //RLC A
            left = true;
            op_rotate(AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x08: //RRC B
            right = true;
            op_rotate(BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x09: //RRC C
            right = true;
            op_rotate(BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x0A: //RRC D
            right = true;
            op_rotate(DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x0B: //RRC E
            right = true;
            op_rotate(DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x0C: //RRC H
            right = true;
            op_rotate(HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x0D: //RRC L
            right = true;
            op_rotate(HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x0E: //RRC (HL)
            right = true;
            op_rotate(read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x0F: //RRC A
            right = true;
            op_rotate(AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x10: //RL B
            left = true;
            op_rotate(BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x11: //RL C
            left = true;
            op_rotate(BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x12: //RL D
            left = true;
            op_rotate(DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x13: //RL E
            left = true;
            op_rotate(DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x14: //RL H
            left = true;
            op_rotate(HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x15: //RL L
            left = true;
            op_rotate(HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x16: //RL (HL)
            left = true;
            op_rotate(read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x17: //RL A
            left = true;
            op_rotate(AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x18: //RR B
            right = true;
            op_rotate(BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x19: //RR C
            right = true;
            op_rotate(BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x1A: //RR D
            right = true;
            op_rotate(DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x1B: //RR E
            right = true;
            op_rotate(DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x1C: //RR H
            right = true;
            op_rotate(HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x1D: //RR L
            right = true;
            op_rotate(HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x1E: //RR (HL)
            right = true;
            op_rotate(read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x1F: //RR A
            right = true;
            op_rotate(AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x20: //SLA B
            left = true;
            reset0 = true;
            op_shift(BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x21: //SLA C
            left = true;
            reset0 = true;
            op_shift(BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x22: //SLA D
            left = true;
            reset0 = true;
            op_shift(DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x23: //SLA E
            left = true;
            reset0 = true;
            op_shift(DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x24: //SLA H
            left = true;
            reset0 = true;
            op_shift(HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x25: //SLA L
            left = true;
            reset0 = true;
            op_shift(HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x26: //SLA (HL)
            left = true;
            reset0 = true;
            op_shift(read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x27: //SLA A
            left = true;
            reset0 = true;
            op_shift(AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x28: //SRA B
            left = true;
            op_shift(BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x29: //SRA C
            left = true;
            op_shift(BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x2A: //SRA D
            left = true;
            op_shift(DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x2B: //SRA E
            left = true;
            op_shift(DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x2C: //SRA H
            left = true;
            op_shift(HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x2D: //SRA L
            left = true;
            op_shift(HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x2E: //SRA (HL)
            left = true;
            op_shift(read(HL.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0x2F: //SRA A
            left = true;
            op_shift(AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x30: //SWAP B
            op_swap(BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x31: //SWAP C
            op_swap(BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x32: //SWAP D
            op_swap(DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x33: //SWAP E
            op_swap(DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x34: //SWAP H
            op_swap(HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x35: //SWAP L
            op_swap(HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x36: //SWAP (HL)
            op_swap(read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x37: //SWAP A
            op_swap(AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x38: //SRL B
            right = true;
            reset7 = true;
            op_shift(BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x39: //SRL C
            right = true;
            reset7 = true;
            op_shift(BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x3A: //SRL D
            right = true;
            reset7 = true;
            op_shift(DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x3B: //SRL E
            right = true;
            reset7 = true;
            op_shift(DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x3C: //SRL H
            right = true;
            reset7 = true;
            op_shift(HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x3D: //SRL L
            right = true;
            reset7 = true;
            op_shift(HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x3E: //SRL (HL)
            right = true;
            reset7 = true;
            op_shift(read(HL.full));
            cycles += 8;
            PC.full++;
        break;
    
        case 0x3F: //SRL A
            right = true;
            reset7 = true;
            op_shift(AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x40: //BIT 0 B
            op_bit(0, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x41: //BIT 0 C
            op_bit(0, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x42: //BIT 0 D
            op_bit(0, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x43: //BIT 0 E
            op_bit(0, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x44: //BIT 0 H
            op_bit(0, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x45: //BIT 0 L
            op_bit(0, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x46: //BIT 0 (HL)
            op_bit(0, read(HL.full));
            cycles += 12;
            PC.full++;
        break;
    
        case 0x47: //BIT 0 A
            op_bit(0, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x48: //BIT 1 B
            op_bit(1, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x49: //BIT 1 C
            op_bit(1, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x4A: //BIT 1 D
            op_bit(1, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x4B: //BIT 1 E
            op_bit(1, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x4C: //BIT 1 H
            op_bit(1, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x4D: //BIT 1 L
            op_bit(1, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x4E: //BIT 1 (HL)
            op_bit(1, read(HL.full));
            cycles += 12;
            PC.full++;
        break;
    
        case 0x4F: //BIT 1 A
            op_bit(1, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x50: //BIT 2 B
            op_bit(2, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x51: //BIT 2 C
            op_bit(2, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x52: //BIT 2 D
            op_bit(2, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x53: //BIT 2 E
            op_bit(2, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x54: //BIT 2 H
            op_bit(2, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x55: //BIT 2 L
            op_bit(2, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x56: //BIT 2 (HL)
            op_bit(2, read(HL.full));
            cycles += 12;
            PC.full++;
        break;
    
        case 0x57: //BIT 2 A
            op_bit(2, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x58: //BIT 3 B
            op_bit(3, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x59: //BIT 3 C
            op_bit(3, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x5A: //BIT 3 D
            op_bit(3, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x5B: //BIT 3 E
            op_bit(3, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x5C: //BIT 3 H
            op_bit(3, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x5D: //BIT 3 L
            op_bit(3, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x5E: //BIT 3 (HL)
            op_bit(3, read(HL.full));
            cycles += 12;
            PC.full++;
        break;
    
        case 0x5F: //BIT 3 A
            op_bit(3, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x60: //BIT 4 B
            op_bit(4, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x61: //BIT 4 C
            op_bit(4, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x62: //BIT 4 D
            op_bit(4, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x63: //BIT 4 E
            op_bit(4, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x64: //BIT 4 H
            op_bit(4, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x65: //BIT 4 L
            op_bit(4, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x66: //BIT 4 (HL)
            op_bit(4, read(HL.full));
            cycles += 12;
            PC.full++;
        break;
    
        case 0x67: //BIT 4 A
            op_bit(4, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x68: //BIT 5 B
            op_bit(5, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x69: //BIT 5 C
            op_bit(5, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x6A: //BIT 5 D
            op_bit(5, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x6B: //BIT 5 E
            op_bit(5, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x6C: //BIT 5 H
            op_bit(5, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x6D: //BIT 5 L
            op_bit(5, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x6E: //BIT 5 (HL)
            op_bit(5, read(HL.full));
            cycles += 12;
            PC.full++;
        break;
    
        case 0x6F: //BIT 5 A
            op_bit(5, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x70: //BIT 6 B
            op_bit(6, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x71: //BIT 6 C
            op_bit(6, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x72: //BIT 6 D
            op_bit(6, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x73: //BIT 6 E
            op_bit(6, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x74: //BIT 6 H
            op_bit(6, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x75: //BIT 6 L
            op_bit(6, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x76: //BIT 6 (HL)
            op_bit(6, read(HL.full));
            cycles += 12;
            PC.full++;
        break;
    
        case 0x77: //BIT 6 A
            op_bit(6, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x78: //BIT 7 B
            op_bit(7, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x79: //BIT 7 C
            op_bit(7, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x7A: //BIT 7 D
            op_bit(7, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x7B: //BIT 7 E
            op_bit(7, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x7C: //BIT 7 H
            op_bit(7, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x7D: //BIT 7 L
            op_bit(7, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x7E: //BIT 7 (HL)
            op_bit(7, read(HL.full));
            cycles += 12;
            PC.full++;
        break;
    
        case 0x7F: //BIT 7 A
            op_bit(7, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x80: //RES 0 B
            op_reset(0, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x81: //RES 0 C
            op_reset(0, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x82: //RES 0 D
            op_reset(0, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x83: //RES 0 E
            op_reset(0, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x84: //RES 0 H
            op_reset(0, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x85: //RES 0 L
            op_reset(0, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x86: //RES 0 (HL)
            op_reset(0, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x87: //RES 0 A
            op_reset(0, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x88: //RES 1 B
            op_reset(1, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x89: //RES 1 C
            op_reset(1, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x8A: //RES 1 D
            op_reset(1, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x8B: //RES 1 E
            op_reset(1, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x8C: //RES 1 H
            op_reset(1, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x8D: //RES 1 L
            op_reset(1, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x8E: //RES 1 (HL)
            op_reset(1, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x8F: //RES 1 A
            op_reset(1, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x90: //RES 2 B
            op_reset(2, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x91: //RES 2 C
            op_reset(2, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x92: //RES 2 D
            op_reset(2, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x93: //RES 2 E
            op_reset(2, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x94: //RES 2 H
            op_reset(2, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x95: //RES 2 L
            op_reset(2, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x96: //RES 2 (HL)
            op_reset(2, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x97: //RES 2 A
            op_reset(2, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x98: //RES 3 B
            op_reset(3, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x99: //RES 3 C
            op_reset(3, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x9A: //RES 3 D
            op_reset(3, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x9B: //RES 3 E
            op_reset(3, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x9C: //RES 3 H
            op_reset(3, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x9D: //RES 3 L
            op_reset(3, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0x9E: //RES 3 (HL)
            op_reset(3, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0x9F: //RES 3 A
            op_reset(3, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xA0: //RES 4 B
            op_reset(4, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xA1: //RES 4 C
            op_reset(4, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xA2: //RES 4 D
            op_reset(4, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xA3: //RES 4 E
            op_reset(4, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xA4: //RES 4 H
            op_reset(4, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xA5: //RES 4 L
            op_reset(4, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xA6: //RES 4 (HL)
            op_reset(4, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xA7: //RES 4 A
            op_reset(4, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xA8: //RES 5 B
            op_reset(5, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xA9: //RES 5 C
            op_reset(5, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xAA: //RES 5 D
            op_reset(5, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xAB: //RES 5 E
            op_reset(5, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xAC: //RES 5 H
            op_reset(5, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xAD: //RES 5 L
            op_reset(5, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xAE: //RES 5 (HL)
            op_reset(5, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xAF: //RES 5 A
            op_reset(5, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xB0: //RES 6 B
            op_reset(6, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xB1: //RES 6 C
            op_reset(6, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xB2: //RES 6 D
            op_reset(6, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xB3: //RES 6 E
            op_reset(6, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xB4: //RES 6 H
            op_reset(6, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xB5: //RES 6 L
            op_reset(6, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xB6: //RES 6 (HL)
            op_reset(6, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xB7: //RES 6 A
            op_reset(6, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xB8: //RES 7 B
            op_reset(7, BC.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xB9: //RES 7 C
            op_reset(7, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xBA: //RES 7 D
            op_reset(7, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xBB: //RES 7 E
            op_reset(7, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xBC: //RES 7 H
            op_reset(7, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xBD: //RES 7 L
            op_reset(7, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xBE: //RES 7 (HL)
            op_reset(7, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xBF: //RES 7 A
            op_reset(7, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xC0: //SET 0 B
            op_set(0, BC.high);
            cycles += 8;
            PC.full++;
        break;
        
        case 0xC1: //SET 0 C
            op_set(0, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xC2: //SET 0 D
            op_set(0, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xC3: //SET 0 E
            op_set(0, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xC4: //SET 0 H
            op_set(0, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xC5: //SET 0 L
            op_set(0, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xC6: //SET 0 (HL)
            op_set(0, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xC7: //SET 0 A
            op_set(0, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xC8: //SET 1 B
            op_set(1, BC.high);
            cycles += 8;
            PC.full++;
        break;
        
        case 0xC9: //SET 1 C
            op_set(1, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xCA: //SET 1 D
            op_set(1, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xCB: //SET 1 E
            op_set(1, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xCC: //SET 1 H
            op_set(1, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xCD: //SET 1 L
            op_set(1, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xCE: //SET 1 (HL)
            op_set(1, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xCF: //SET 1 A
            op_set(1, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xD0: //SET 2 B
            op_set(2, BC.high);
            cycles += 8;
            PC.full++;
        break;
        
        case 0xD1: //SET 2 C
            op_set(2, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xD2: //SET 2 D
            op_set(2, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xD3: //SET 2 E
            op_set(2, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xD4: //SET 2 H
            op_set(2, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xD5: //SET 2 L
            op_set(2, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xD6: //SET 2 (HL)
            op_set(2, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xD7: //SET 2 A
            op_set(2, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xD8: //SET 3 B
            op_set(3, BC.high);
            cycles += 8;
            PC.full++;
        break;
        
        case 0xD9: //SET 3 C
            op_set(3, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xDA: //SET 3 D
            op_set(3, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xDB: //SET 3 E
            op_set(3, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xDC: //SET 3 H
            op_set(3, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xDD: //SET 3 L
            op_set(3, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xDE: //SET 3 (HL)
            op_set(3, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xDF: //SET 3 A
            op_set(3, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xE0: //SET 4 B
            op_set(4, BC.high);
            cycles += 8;
            PC.full++;
        break;
        
        case 0xE1: //SET 4 C
            op_set(4, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xE2: //SET 4 D
            op_set(4, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xE3: //SET 4 E
            op_set(4, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xE4: //SET 4 H
            op_set(4, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xE5: //SET 4 L
            op_set(4, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xE6: //SET 4 (HL)
            op_set(4, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xE7: //SET 4 A
            op_set(4, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xE8: //SET 5 B
            op_set(5, BC.high);
            cycles += 8;
            PC.full++;
        break;
        
        case 0xE9: //SET 5 C
            op_set(5, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xEA: //SET 5 D
            op_set(5, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xEB: //SET 5 E
            op_set(5, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xEC: //SET 5 H
            op_set(5, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xED: //SET 5 L
            op_set(5, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xEE: //SET 5 (HL)
            op_set(5, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xEF: //SET 5 A
            op_set(5, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xF0: //SET 6 B
            op_set(6, BC.high);
            cycles += 8;
            PC.full++;
        break;
        
        case 0xF1: //SET 6 C
            op_set(6, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xF2: //SET 6 D
            op_set(6, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xF3: //SET 6 E
            op_set(6, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xF4: //SET 6 H
            op_set(6, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xF5: //SET 6 L
            op_set(6, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xF6: //SET 6 (HL)
            op_set(6, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xF7: //SET 6 A
            op_set(6, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xF8: //SET 7 B
            op_set(7, BC.high);
            cycles += 8;
            PC.full++;
        break;
        
        case 0xF9: //SET 7 C
            op_set(7, BC.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xFA: //SET 7 D
            op_set(7, DE.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xFB: //SET 7 E
            op_set(7, DE.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xFC: //SET 7 H
            op_set(7, HL.high);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xFD: //SET 7 L
            op_set(7, HL.low);
            cycles += 8;
            PC.full++;
        break;
    
        case 0xFE: //SET 7 (HL)
            op_set(7, read(HL.full));
            cycles += 16;
            PC.full++;
        break;
    
        case 0xFF: //SET 7 A
            op_set(7, AF.high);
            cycles += 8;
            PC.full++;
        break;
    
         default:
            printf ("Unkown opcode: 0x%X\n", opcode);
    }
}