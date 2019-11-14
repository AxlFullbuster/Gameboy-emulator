#ifndef GPU_H
#define GPU_H


#include<SDL2/SDL.h>
#include "imgui.h"
#include "imgui_sdl.h"
#include "Gameboy.h"


class GPU : Gameboy{
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
        
        
        /*boolean values for the bits in the
         *lcd control register
         */
        bool enableLCD;
        bool windowTileMap;
        bool enableWindow;
        bool bg_windowTile;
        bool bgTileMap;
        bool spriteSize;
        bool enableSprites;
        bool enablebg;
        
        /*boolean values for the bits in the
         *lcd status register
         */
        bool lyc;
        bool mode2_OAM;
        bool mode1_Vblank;
        bool mode0_Hblank;
        bool coincidence;
        bool flag1;
        bool flag0;
        
    
        bool stat_interrupt();
        void init_registers();
        void lcd_control();
        void lcd_status();
    
    public:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_Texture* texture = NULL;
        const int SCREEN_WIDTH = 640;
        const int SCREEN_HEIGHT = 320;
        int sx = SCREEN_WIDTH / 160;
        int sy = SCREEN_HEIGHT / 144;
        
        Gameboy &emu;
        bool debug;
        
        bool init();
        void input();
        void close();
        void draw_display();
        void draw_debugger();
        void print();
        
        GPU(Gameboy &emulator);
        ~GPU();
         
};

#endif