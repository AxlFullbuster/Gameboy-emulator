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


//check if the lcd control register has the 7th bit set
bool GPU::lcd_set(){
    bitset<8> lcd_control (read(0xFF40)); //holds address 0xFF40's bit values
    
    //uncomment the line below to manually set the value to 1
    //lcd_control.set(7);
    
    if(lcd_control.test(7)) return true;
    else {
        cout << lcd_control << endl;
        exit(1);
        return false;
    }
}

//draw onto the screen
void GPU::draw_display(){
    if(lcd_set()){
        //set to white
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        //set to black
        SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0x00);
        SDL_Rect pixel;
        
        /* draw white pixels onto the 144*166 canvas
         * draw black for everything else
         * set the pixels to be 8x8 in size and then scale
         * them to the current screen width and height
         */
        
        for(int y = 0; y < 144; ++y){
            for(int x = 0; x< 160; ++x) {
                pixel.x = x*sx;
                pixel.y = y*sy;
                pixel.w = 8;
                pixel.h = 8;
                SDL_RenderFillRect(renderer,&pixel);  
            }
        }
        //present the drawn canvas
        SDL_RenderPresent(renderer);
    } 
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
    
    
    return success;
}

//free memory space the window uses after closing it
void GPU::close(){
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    
    SDL_Quit();
}
