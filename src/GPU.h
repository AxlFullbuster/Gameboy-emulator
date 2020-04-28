#ifndef GPU_H
#define GPU_H


#include<SDL2/SDL.h>
#include<bitset>
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_memory_editor.h"
#include "CPU.h"

//! A class for the Gameboy GPU

/*!
 *This class emulates the graphics created in the Gameboy.
 *It inherits public functions from the CPU class.
 */


class GPU : public CPU{
    private:
        //! This is the y coordinate used when the background is selected
        uint8_t scrollY;
        //! This is the x coordinate used when the background is selected
        uint8_t scrollX;
        //! This is the main y coordinate used to render the screen
        uint8_t yCoordinate;
        //! This is a register that is compared with yCoordinate for the stat interrupt
        uint8_t lyCompare;
        //! This is the y coordinate used when the window is selected
        uint8_t winY;
        //! This is the x coordinate used when the windows is selected
        uint8_t winX;
        //! This holds the color data for the background of the gameboy screen
        uint8_t bg_palette;
        //! This holds the color data for the first palette used in the sprites
        uint8_t obj_palette0;
        //! This holds the color data for the second palette used in the sprites
        uint8_t obj_palette1;
        uint8_t DMA;
        
        //! This holds the bits in the lcd control register
        std::bitset<8> lcd_control;
        //! This holds the bits in the lcd status register
        std::bitset<8> lcd_status;
        
        //! Display's the mode the gpu is in
        int mode;
        //! This is the amount of cycles left in the gpu
        int gpu_cycles;
        
        //! The color red
        int red;
        //! The color green
        int green;
        //! The color blue
        int blue;
        
        
        //! Signifies when the background palette is being used
        bool bgColor;
        //! Signifies when the 1st object palette is being used
        bool obj0;
        //! Signifies when the 2nd object palette is being used
        bool obj1;
        //! Signifies when we are using a signed integer
        bool sig;
        
        
        
        
        //! An array that holds the rgb data for the gameboy
        uint8_t gfx[160][144][3];
        
        
        bool check_LCD();
        
        //! Checks to see if the stat interrupt needs to be processed
        void stat_interrupt();
        //! Checks to see what mode the GPU is in
        void check_mode();
        //! Draws a single line on the screen
        void draw_scanline();
        
        void render_data();
        //! Draws the tile data found in the memory map
        void draw_tiles();
        //! Draws the sprite data found in the memory map
        void draw_sprites();
        //! Gets a bit value from a specific position
        int getBit(uint8_t byte,int pos);
        //! Gets a decimal value from two  bits
        int getDecimal(int high, int low);
        //! Gets a color value from two bytes and a specific bit position
        void getColor(uint8_t byte1, uint8_t byte2, int pos);
        
        
        
    
    public:
        //! The window that SDL displays
        SDL_Window* window = NULL;
        //! The renderer used to render images onto the SDL window
        SDL_Renderer* renderer = NULL;
        //! Holds a texture to put on the SDL window
        SDL_Texture* texture = NULL;
        const int SCREEN_WIDTH = 640;
        const int SCREEN_HEIGHT = 320;
        //! Scales the width of the screen so everything fits
        int sx = SCREEN_WIDTH / 160;
        //! Scales the height of the screen so everything fits
        int sy = SCREEN_HEIGHT / 144;
        
        //! An instance of the CPU class
        CPU &emu;
        
        //! Configures everything needed for SDL
        bool init();
        //! Initializes the registers in the GPU
        void init_registers();
        //! Checks what line the scanline is on
        void check_scanline();
        //! Draws all the ImGui windows
        void draw_debugger();
        //! Displays the window and draws the background color
        void draw_display();
        //! Deinitializes the values used in SDL and frees up memory
        void close();
        
        GPU(CPU &emulator);
        ~GPU();
         
};

#endif