#ifndef GPU_H
#define GPU_H


#include<SDL2/SDL.h>
#include "imgui.h"
#include "imgui_sdl.h"
#include "Gameboy.h"


class GPU : Gameboy{
    public:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_Texture* texture = NULL;
        const int SCREEN_WIDTH = 640;
        const int SCREEN_HEIGHT = 320;
        int sx = SCREEN_WIDTH / 160;
        int sy = SCREEN_HEIGHT / 144;
        
         bool init();
         void input();
         void close();
         void draw_display();
         void draw_debugger();
         
         GPU();
         ~GPU();
         
};

#endif