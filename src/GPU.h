#ifndef GPU_H
#define GPU_H


#include "Gameboy.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>


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
         void close();
         void draw_display();
         
         GPU();
         ~GPU();
         
    private:
        bool lcd_set();
        
        
};





#endif