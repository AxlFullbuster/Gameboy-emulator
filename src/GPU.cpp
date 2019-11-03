#include "GPU.h"
#include <iostream>
#include <bitset>

using std::cout;
using std::endl;
using std::bitset;


GPU::GPU(){
    //Do Nothing
}

GPU::~GPU(){
    //Do Nothing
}

//draw onto the screen
void GPU::draw_display(Gameboy &emu){
    //set to light blue
    SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect pixel;
    
    /* draw white pixels onto the 144*166 canvas
     * draw black for everything else
     * set the pixels to be 8x8 in size and then scale
     * them to the current screen width and height
     */
    for(int y = 0; y < 144; ++y){
        for(int x = 0; x < 160 ; ++x) {
            pixel.x = x*sx;
            pixel.y = y*sy;
            pixel.w = 8;
            pixel.h = 8;
            SDL_RenderFillRect(renderer,&pixel);  
        }
    }
    draw_debugger(emu);
    
    //present the drawn canvas
    SDL_RenderPresent(renderer);
    SDL_Delay(50);
}

/*
 *prints out the values of all registers
 *and values stored in memory
 *using the ImGui Library.
 */
void GPU::draw_debugger(Gameboy &emu){
    ImGui::NewFrame();
    
    //get bits in flag register
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
    
    ImGui::Begin("Executed Codes");
    ImGui::Text("Code:0x%X", op);
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
