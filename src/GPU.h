#ifndef GPU_H
#define GPU_H


#include<SDL2/SDL.h>
#include<bitset>
#include "imgui.h"
#include "imgui_sdl.h"
#include "CPU.h"


class GPU : public CPU{
    private:
        //the GPU registers
        uint8_t scrollY;
        uint8_t scrollX;
        uint8_t yCoordinate;
        uint8_t lyCompare;
        uint8_t winY;
        uint8_t winX;
        uint8_t bg_palette;
        uint8_t obj_palette0;
        uint8_t obj_palette1;
        uint8_t DMA;
        
    
        std::bitset<8> lcd_control;
        std::bitset<8> lcd_status;
        
        int mode;
        int gpu_cycles;
        int red;
        int green;
        int blue;
        
        
        //boolean values that decide what palette data we will use
        bool bgColor;
        bool obj0;
        bool obj1;
        bool sig;
        
        
        //an array that holds the pixel color data in the Gameboy
        uint8_t gfx[160][144][3];
        
        bool check_LCD();
        void stat_interrupt();
        void check_mode();
        void draw_scanline();
        void render_data();
        void draw_tiles();
        void draw_sprites();
        int getBit(uint8_t byte,int pos);
        int getDecimal(int high, int low);
        void getColor(uint8_t byte1, uint8_t byte2, int pos);
        
        
        
    
    public:
        //various SDL variables used to set up the display.
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_Texture* texture = NULL;
        const int SCREEN_WIDTH = 640;
        const int SCREEN_HEIGHT = 320;
        int sx = SCREEN_WIDTH / 160;
        int sy = SCREEN_HEIGHT / 144;
        
        CPU &emu;
        bool debug;
        
        
        bool init();
        void init_registers();
        void check_scanline();
        void draw_debugger();
        void draw_display();
        void input();
        void close();
        
        GPU(CPU &emulator);
        ~GPU();
         
};

#endif