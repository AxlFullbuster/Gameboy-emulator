#include "GPU.h"
#include <iostream>
#include <bitset>

using std::cout;
using std::endl;
using std::bitset;


GPU::GPU(Gameboy &emulator) : emu(emulator){
    mode = 1;
    gpu_cycles = 456;
    emu.write(0xFF44, 0);
}

GPU::~GPU(){
    //Do Nothing
}

void GPU::gpuloop(){
    int cycles = emu.get_cycles();
        if(check_LCD()){
            init_registers();
            check_scanline();
        }
        draw_display();
}

bool GPU::check_LCD(){
    if(!lcd_control.test(7))return false;
    else return true;
}

/*compares these two registers, depending on their values
 *the GPU will trigger a stat interrupt
 */
void GPU::stat_interrupt(){
    if(yCoordinate == lyCompare){
        lcd_status.set(2);
        //do a stat interrupt
    }else{
        lcd_status.reset(2);
    }
}

//initialize various registers in the Gameboy GPU
void GPU::init_registers(){
    scrollY = emu.read(0xFF42);
    scrollX = emu.read(0xFF43);
    yCoordinate = emu.read(0xFF44);
    lyCompare = emu.read(0xFF45);
    winY = emu.read(0xFF4A);
    winX = emu.read(0xFF4B);
    bg_palette = emu.read(0xFF47);
    obj_palette0 = emu.read(0xFF48);
    obj_palette1 = emu.read(0xFF49);
    DMA = emu.read(0xFF46);
    lcd_control = emu.read(0xFF40);
    lcd_status = emu.read(0xFF41);
}

void GPU::check_mode(){
    int currentmode = mode;
    if(gpu_cycles >= 376){
        mode = 2;
        lcd_status.reset(0);
        lcd_status.set(1);
    }else if(gpu_cycles >= 204){
        mode = 3;
        lcd_status.set(0);
        lcd_status.set(1);
    }else{
        mode = 0;
        lcd_status.reset(0);
        lcd_status.reset(1);
    }
    //call an interrupt if you changed modes
    stat_interrupt();
}

void GPU::check_scanline(){
    check_mode();
    if(lcd_control.test(7)){
        gpu_cycles = 456;
        if(yCoordinate < 144){
            draw_scanline();
            emu.increment_scanline();
        }else if(yCoordinate == 144){
            //call a v-blank interrupt
        }else{
            emu.write(0xFF44, 0);
        }
    }
}

void GPU::draw_scanline(){
    if(lcd_control.test(0)){
        draw_tiles();
    }
    
    if(lcd_control.test(1)){
        draw_sprites();
    }
}

int GPU::getBit(uint8_t byte, int pos){
    int bit;
    pos = 7 - (pos % 8);
    
    bit = (byte >> pos) & 0x1;
    return bit;
}

int GPU::getDecimal(int high, int low){
    if(high == 0 && low == 0) return 0;
    else if(high == 0 && low == 1) return 1;
    else if(high == 1 && low == 0) return 2;
    else return 3;
}

void GPU::getColor(uint8_t byte1, uint8_t byte2, int pos){
    int bit1 = getBit(byte1, pos);
    int bit2 = getBit(byte2, pos);
    int map = getDecimal(bit1, bit2);
    bitset<8> palette;
    
    if(bgColor){
        palette = bg_palette;
    }else if(obj0){
        palette = obj_palette0;
    }else if(obj1){
        palette = obj_palette1;
    }
    
    int hi = 0;
    int lo = 0;
    
    switch(map){
        case 0:
            hi = 1;
            lo = 0;
        break;
    
        case 1:
            hi = 3;
            lo = 2;
        break;
    
        case 2:
            hi = 5;
            lo = 4;
        break;
    
        case 3:
            hi = 7;
            lo = 6;
        break;
    }
    
    int color = getDecimal(palette[hi], palette[lo]);
    
     switch(color){
        case 0:
           red = 0xFF;
           green = 0xFF;
           blue = 0xFF;
        break;
    
        case 1:
           red = 0xD3;
           green = 0xD3;
           blue = 0xD3;
        break;
    
        case 2:
           red = 0xA9;
           green = 0xA9;
           blue = 0xA9;
        break;
    
        case 3:
           red = 0x00;
           green = 0x00;
           blue = 0x00;
        break;
    }
}

uint16_t GPU::getTileData(uint8_t tileAddress){
    uint16_t memloc;
    int16_t tilenum;
    uint16_t tileData;
    
    if(lcd_control.test(4)){
        memloc = 0x8000;
        tilenum = (uint8_t)emu.read(tileAddress);
        tileData = memloc + (tilenum * 16);
        return tileData;
    }
    
    memloc = 0x8000;
    tilenum = (int8_t)emu.read(tileAddress);
    tileData = memloc + ((tilenum + 128) * 16);
    return tileData;
}

void GPU::draw_tiles(){
    bgColor = true;
    uint16_t bg_mem = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    bool window;
    
    if(lcd_control.test(5)){
        if(winY <= yCoordinate){
            window = true;
        }
    }
    
    if(!window){
        if(lcd_control.test(3)){
            bg_mem = 0x9C00;
        }else{
            bg_mem = 0x9800;
        }
        y = scrollY + yCoordinate;
    }else{
        if(lcd_control.test(6)){
            bg_mem = 0x9C00;
        }else{
            bg_mem = 0x9800;
        }
        y = yCoordinate - winY;
    }
    
    uint16_t tilerow = (((uint8_t) (y/8)) * 32);
                        
    for(int p = 0; p < 160; p++){
        x = p + scrollX;
        
        if(window){
            if(p >= winX){
                x = p - winX;
            }
        }
        
        uint16_t tilecol = (x/8);
        uint16_t tileloc;
        uint16_t tileAddress = bg_mem + tilerow + tilecol;
        tileloc = getTileData(tileAddress);
        
        uint8_t line = y % 8;
        line *= 2;
        uint8_t byte1 = emu.read(tileloc + line);
        uint8_t byte2 = emu.read(tileloc + line + 1);
        getColor(byte1, byte2, p);
        
        gfx[p * yCoordinate * 1] = red;
        gfx[p * yCoordinate * 2] = green;
        gfx[p * yCoordinate * 3] = blue;
    }
    bgColor = false;
}

void GPU::draw_sprites(){
    uint8_t ysize;
    if(lcd_control.test(2)){
        ysize = 16;
    }
    
    ysize = 8;
    
    for(int i = 0; i < 40; i++){
        uint8_t index = i*4;
        uint8_t ypos = emu.read(0xFE00 + index) - 16;
        uint8_t xpos = emu.read(0xFE00 + index + 1) - 8;
        uint8_t tiledata = emu.read(0xFE00 + index + 2);
        uint8_t atri = emu.read(0xFE00 + index + 3);
        
        bitset<8> attributes(atri);
        bool flipY = attributes.test(6);
        bool flipX = attributes.test(5);
        
        if((yCoordinate >= ypos) && (yCoordinate < (ypos + ysize))){
            int line = yCoordinate - ypos;
            if(flipY){
                line -= ysize;
                line *= -1;
            }
            
            line *= 2;
            uint16_t address = (0x8000 + (tiledata * 16)) + line;
            uint8_t byte1 = emu.read(address);
            uint8_t byte2 = emu.read(address + 1);
            
            for(int j = 0; j < 8; j++){
                int color = j;
                
                if(flipX){
                    color = 7 - j;
                }
                
                if(attributes.test(4)){
                    obj0 = true;
                    obj1 = false;
                }else{
                    obj0 = false;
                    obj1 = true;
                }
                
                getColor(byte1, byte2, color);
                int xpix = j;
                int pixel = xpos + xpix;
                
                gfx[pixel * yCoordinate * 1] = red;
                gfx[pixel * yCoordinate * 2] = green;
                gfx[pixel * yCoordinate * 3] = blue;
            }
        }
    }
}



//draw onto the screen
void GPU::draw_display(){
    //set to light blue
    SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
    SDL_RenderClear(renderer);
    
    
    //draw the color data stored in gfx onto the screen
    for(int y = 0; y < 144; ++y){
        for(int x = 0; x < 160; ++x) {
            red = gfx[x * y * 1];
            green = gfx[x * y * 2];
            blue = gfx[x * y * 3];
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    
    if(debug){
        draw_debugger();
    }
    //present the drawn canvas
    SDL_RenderPresent(renderer);
}

/*
 *prints out the values of all registers,
 *and values stored in memory
 *using the ImGui Library.
 */
void GPU::draw_debugger(){
     
    
    ImGui::NewFrame();
    
    //get the CPU register values
    bitset<8> flags = emu.get_F();
    uint16_t pc = emu.get_PC();
    uint16_t af = emu.get_AF();
    uint16_t bc = emu.get_BC();
    uint16_t de = emu.get_DE();
    uint16_t hl = emu.get_HL();
    uint16_t sp = emu.get_SP();
    
    uint8_t a = emu.get_A();
    uint8_t b = emu.get_B();
    uint8_t c = emu.get_C();
    uint8_t d = emu.get_D();
    uint8_t e = emu.get_E();
    uint8_t h = emu.get_H();
    uint8_t l = emu.get_L();
    uint8_t op = emu.get_OP();
    uint8_t *mem = emu.get_mem();
    
    //create a new window called "Registers
    ImGui::Begin("Registers");
    
    /*create three columns for the 16 bit registers,
     *the 8 bit registers, and the flags
     */
    ImGui::Columns(3);
    ImGui::Text("16-bit Registers");
    
    ImGui::Text("Program Counter:0x%X", pc);
    ImGui::Text("AF Register:0x%X", af);
    ImGui::Text("BC Register:0x%X", bc);
    ImGui::Text("DE Register:0x%X", de);
    ImGui::Text("HL Register:0x%X", hl);
    ImGui::Text("SP Register:0x%X", sp);
    ImGui::NextColumn(); // go to the next column in the window
    
    ImGui::Text("8-bit Registers");
    ImGui::Text("A Register:0x%X", a);
    ImGui::Text("B Register:0x%X", b);
    ImGui::Text("C Register:0x%X", c);
    ImGui::Text("D Register:0x%X", d);
    ImGui::Text("E Register:0x%X", e);
    ImGui::Text("H Register:0x%X", h);
    ImGui::Text("L Register:0x%X", l);
    ImGui::NextColumn();
    
    ImGui::Text("Flags");
    ImGui::Separator(); //seperate the first word put in a column with a line
    ImGui::Text("Z:%d", flags.test(7));
    ImGui::Text("N:%d", flags.test(6));
    ImGui::Text("HC:%d", flags.test(5));
    ImGui::Text("C:%d", flags.test(4));
    
    
    ImGui::End();
    
    ImGui::Begin("Memory");
    ImGui::Columns(2);
    ImGui::Text("Memory Location");
    for(int i = 0; i < 0x10000; i++){
        //print the memory location
        ImGui::Text("0x%X", i);
    }
    ImGui::NextColumn();
    
    ImGui::Text("Memory Value");
    ImGui::Separator();
    for(int i = 0; i < 0x10000; i++){
        //print the value stored in a specific memory location
        ImGui::Text("0x%X", *(mem+i));
    }
    
    ImGui::End();
    
    //print out the executed operation code
    ImGui::Begin("Executed Codes");
    ImGui::Text("Code:0x%X", op);
    ImGui::End();
    
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
    
    SDL_RenderPresent(renderer);
    SDL_Delay(50);
}


void GPU::input(){
    //do nothing for now
}

//initialize the SDL window
bool GPU::init(){
    bool success = true;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf ( "SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
	}
    
    
    window = SDL_CreateWindow("Gameboy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    }
    
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, 640,320);
    
    return success;
}



//free memory space the window uses after closing it
void GPU::close(){
    ImGuiSDL::Deinitialize();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;
    
	ImGui::DestroyContext();
    SDL_Quit();
}
