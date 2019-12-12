#include "GPU.h"
#include <iostream>
#include <bitset>

using std::cout;
using std::endl;
using std::bitset;

//Note to self: finish commenting out the code 

GPU::GPU(CPU &emulator) : emu(emulator){
    //Do Nothing
}

GPU::~GPU(){
    //Do Nothing
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
    winX = emu.read(0xFF4B) - 7;
    bg_palette = emu.read(0xFF47);
    obj_palette0 = emu.read(0xFF48);
    obj_palette1 = emu.read(0xFF49);
    DMA = emu.read(0xFF46);
    lcd_control = emu.read(0xFF40);
    lcd_status = emu.read(0xFF41);
}

/*change what mode the gpu is in depending on the
 *current value of gpu_cycles
 */
void GPU::check_mode(){
    //holds the current mode the gpu is in
    int currentmode = mode;
    
    /* if the lcd display is disabled go into mode
     * 1, reset the scanline, and reset the gpu cycle
     * count.
     */
    if(!lcd_control.test(7)){
        mode = 1;
        lcd_status.set(0);
        lcd_status.reset(1);
        gpu_cycles = 456;
        emu.write(0xFF44, 0);
        emu.write(0xFF41, lcd_status.to_ulong());
    }

    /*if we're in v-blank enter mode 1 and call an interrupt.
     *otherwise check the current value of gpu_cycles and switch
     *to the specified mode.
     */
    if(yCoordinate >= 144){
        mode = 1;
        lcd_status.set(0);
        lcd_status.reset(1);
        //call a v-blank interrupt
    }else{
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
    }
    //call an interrupt if you changed modes
    //stat_interrupt();
    emu.write(0xFF41, lcd_status.to_ulong());
}

/*checks the scanline value and updates the gpu
 *cycle value.
 */
void GPU::check_scanline(){
    check_mode();
    
    /*if the lcd display is enabled subtract the
     *amount of cycles it took for the last cpu instruction
     *to execute from gpu_cycles. Otherwise exit the function.
     */
    if(lcd_control.test(7)){
        gpu_cycles -= emu.timing;
    }else{
        return;
    }
    
    /*if gpu_cycles is 0 reset the value to 456 and move onto
     *the next line. Afterwards check the scanline value and either
     *draw tile/sprite data, refresh the screen, or reset the value once it
     *reaches 153.
     */
    if(gpu_cycles <= 0){
        gpu_cycles = 456;
        emu.increment_scanline();
        if(yCoordinate < 144){
                draw_scanline();
            }else if(yCoordinate == 144){
                //call a v-blank interrupt
                draw_display();
            }else if (yCoordinate >= 153){
                emu.write(0xFF44, 0);
        }
    }
}

/*draws tile/sprite data depending on the values of
 *the 0 and 1 bit in the lcd control register
 */
void GPU::draw_scanline(){
    if(lcd_control.test(0)){
        draw_tiles();
    }
    
    if(lcd_control.test(1)){
        draw_sprites();
    }
}

/* gets the bit value of a specific position in
 * a byte variable.
 */
int GPU::getBit(uint8_t byte, int pos){
    int bit;
    pos = 7 - (pos % 8);
    
    bit = (byte >> pos) & 0x1;
    return bit;
}

/*return a decimal value depending on the values
 *of the 2 bits given in the function
 */
int GPU::getDecimal(int high, int low){
    if(high == 0 && low == 0) return 0;
    else if(high == 0 && low == 1) return 1;
    else if(high == 1 && low == 0) return 2;
    else return 3;
}

//gets the pixel color data from 2 bytes passed in the function
void GPU::getColor(uint8_t byte1, uint8_t byte2, int pos){
    
    /*get the bit values from a specific position
     *and put their decimal value into an integer called
     *map.
     */
    int bit1 = getBit(byte1, pos);
    int bit2 = getBit(byte2, pos);
    int map = getDecimal(bit1, bit2);
    
    /*palette holds the color data of either the background, ob0,
     *or obj1 depending on the boolean values given.
     */
    bitset<8> palette;
    if(bgColor){
        palette = bg_palette;
    }else if(obj0){
        palette = obj_palette0;
    }else if(obj1){
        palette = obj_palette1;
    }
    
    //two integers that will hold  the positions of the bits in palette
    int hi = 0;
    int lo = 0;
    
    //set hi and low to specific values depending on the value of map
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
    
    //gets the decimal value for the pixel color
    int color = getDecimal(palette[hi], palette[lo]);
    
    /*depending of the value of color, set red, green, and blue
     *to either white, light grey, dark grey, or black.
     */
     switch(color){
        case 0:
           red = 0xFF;
           green = 0xFF;
           blue = 0xFF;
        break;
    
        case 1:
           red = 0xCC;
           green = 0xCC;
           blue = 0xCC;
        break;
    
        case 2:
           red = 0x77;
           green = 0x77;
           blue = 0x77;
        break;
    
        case 3:
           red = 0x00;
           green = 0x00;
           blue = 0x00;
        break;
    }
}

//gets the tile positions from the memory map and stores the data in gfx
void GPU::draw_tiles(){
    //tiles use the background tile data so set bgColor to true
    bgColor = true;
    
    /*values that hold the background memory location, the memory location
     *in VRAM and the tilenum value that will be used to get the tile data
     */
    uint16_t bg_mem = 0;
    uint16_t memloc;
    int16_t tilenum;
    
    /*boolean values used to tell if we are using the window display and if the tile
     *data holds a signed integer value.
     */
    bool window;
    bool sig;

    //test if the window display has been enabled
    if(lcd_control.test(5)){
        if(winY <= yCoordinate){
            window = true;
        }
    }
    
    /*check where in VRAM we will find the tile information.
     *depending on the location we will have to fetch either a signed
     *or unsigned integer
     */
    if(lcd_control.test(4)){
        memloc = 0x8000;
        sig = false;
    }else{
        memloc = 0x8800;
        sig = true;
    }

    /*if the window display is disabled check the 3rd bit lcd control register
     *for the location to start in the background memory.
     */
    if(!window){
        if(lcd_control.test(3)){
            bg_mem = 0x9C00;
        }else{
            bg_mem = 0x9800;
        }
    //otherwise check the 6th bit in the lcd control register.
    }else{
        if(lcd_control.test(6)){
            bg_mem = 0x9C00;
        }else{
            bg_mem = 0x9800;
        }
    }
    
    //y holds the position in the background or window display.
    uint8_t y = scrollY + yCoordinate;
    
    /*if we are using the window display subtract the windowY value
     *from the scanline number to get the correct position on the window display.
     */
    if(window){
      y = yCoordinate - winY;
    }
    
    //the tille row is found by dividing y by 8 and multiplying the value by 32.
    uint16_t tilerow = (((uint8_t) (y/8)) * 32);
    
    //a for loop that iterates over each pixel in the y axis.                
    for(int p = 0; p < 160; p++){
        
        /*x is very similar to y except it holds the position of the x-axis
         *of the display.
         */
        uint8_t x = p + scrollX;
        
        //check if the window display is enabled and set x to the correct position
        if(window){
            if(p >= winX){
                x = p - winX;
            }
        }
        
        //the tile column is found by dividing x by 8.
        uint16_t tilecol = (x/8);
        
        //tileloc and tileAddress are used to find the tile data in VRAM.
        uint16_t tileloc;
        uint16_t tileAddress = bg_mem + tilerow + tilecol;
        
        /*depending on the memory location we will have to fetch either
         *an unsigned integer, or a signed integer.
         */
        if(sig){
            tilenum = (int8_t) emu.read(tileAddress);
        }else{
            tilenum = emu.read(tileAddress);
        }
        
        /*to get the proper tile location we need to add the
         *starting memory location in VRAM with the tile data that
         *we found in memory. We multiply this value by 16 as a tile
         *is 16 bytes long. If we fetched an unsigned value we'll need
         *to add 128 to the tile data in order to get the correct value.
         */
        if(!sig){
            tileloc = memloc + (tilenum * 16);
        }else{
            tileloc = memloc + ((tilenum + 128) * 16);
        }
        
        //this gives us the proper line to render the tile to
        uint8_t line = y % 8;
        //a line is 2 bytes long so we multiply it by 2 to get to the next one
        line *= 2;
        
        //these two bytes will give us the color data for a tile
        uint8_t byte1 = emu.read(tileloc + line);
        uint8_t byte2 = emu.read(tileloc + line + 1);
        getColor(byte1, byte2, p);
        
        /*Once everything is done we need to save the color
         *data into the gfx array so we can use it later to
         *render the proper color data to the screen
         */
        gfx[p][yCoordinate][0] = red;
        gfx[p][yCoordinate][1] = green;
        gfx[p][yCoordinate][2] = blue;
    }
    
    //since we may not fetch tile data for the next line, we'll set the bgcolor boolean to false
    bgColor = false;
}

//fetches sprite data from memory
void GPU::draw_sprites(){
    //y size holds the height of a sprite
    uint8_t ysize;
    
    /*depending on the value in the second bit of the control register
     *the height of the sprite may be either 16 or 8 pixels long.
     */
    if(lcd_control.test(2)){
        ysize = 16;
    }else{
         ysize = 8;
    }
    
   
    /*the gameboy can display up to 40 sprites so
     *we'll need loop over the sprite attribute table to get all
     *the wonderfully drawn pixels.
     */
    for(int i = 0; i < 40; i++){
        
        /*only ten sprites can be displayed per scanline so we need to
         *multiply the index value by 4 so that there aren't any issues.
         */
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
                 
                gfx[pixel][yCoordinate][0] = red;
                gfx[pixel][yCoordinate][1] = green;
                gfx[pixel][yCoordinate][2] = blue;
            }
        }
    }
}


//draw the display onto the screen
void GPU::draw_display(){
    //draw the color data stored in gfx onto the screen
    for(int y = 0; y < 144; y++){
        for(int x = 0; x < 160; x++) {
            red = gfx[x][y][0];
            green = gfx[x][y][1];
            blue = gfx[x][y][2];
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
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
    //set the window background to light blue
    SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
    SDL_RenderClear(renderer);
  
    ImGui::NewFrame();
    
    //get the CPU register values
    bitset<8> flags = emu.get_F();
    uint8_t *mem = emu.get_mem();
    
    //create a new window called "Registers
    ImGui::Begin("Registers");
    
    /*create three columns for the 16 bit registers,
     *the 8 bit registers, and the flags
     */
    ImGui::Columns(3);
    ImGui::Text("16-bit Registers");
    
    ImGui::Text("Program Counter:0x%X", emu.get_PC());
    ImGui::Text("AF Register:0x%X", emu.get_AF());
    ImGui::Text("BC Register:0x%X", emu.get_BC());
    ImGui::Text("DE Register:0x%X", emu.get_DE());
    ImGui::Text("HL Register:0x%X", emu.get_HL());
    ImGui::Text("SP Register:0x%X", emu.get_SP());
    ImGui::NextColumn(); // go to the next column in the window
    
    ImGui::Text("8-bit Registers");
    ImGui::Text("A Register:0x%X", emu.get_A());
    ImGui::Text("B Register:0x%X", emu.get_B());
    ImGui::Text("C Register:0x%X", emu.get_C());
    ImGui::Text("D Register:0x%X", emu.get_D());
    ImGui::Text("E Register:0x%X", emu.get_E());
    ImGui::Text("H Register:0x%X", emu.get_H());
    ImGui::Text("L Register:0x%X", emu.get_L());
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
    ImGui::Text("Code:0x%X", emu.get_OP());
    ImGui::End();
    
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
    
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
