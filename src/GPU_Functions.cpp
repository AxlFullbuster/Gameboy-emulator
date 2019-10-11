#include "Gameboy.h"
#include <bitset>
#include <iostream>

using std::bitset;
using std::cout;
using std::endl;

void Gameboy::lcd_control(){
   bitset<8> lcd_control = read(0xFF40);
   if(lcd_control.test(7)) lcd_set = true;
}
