#include "Gameboy.h"
#include "GPU.h"
#include <iostream>

using std::cout;
using std::endl;

GPU display;
Gameboy emu;

int main(int argc, char* argv[]){
    if(argc < 2) {
        cout << "Please provide a rom file to open." << endl;
        return 1;
    }
    
    if(!display.init()){
        cout << "Failed to initialize display." << endl;
    } else{
        bool quit = false;
        emu.loadGame(argv[1]);
        
        if(!emu.loadGame(argv[1])) return 1;
        
        SDL_Event e;
        while(!quit){
            while( SDL_PollEvent( &e ) != 0 ) {
                
                if( e.type == SDL_QUIT ) {
                    quit = true;
                }
                //input method
            }
            emu.emuLoop();
            display.draw_display();
        }
    }
    display.close();
    return 0;
}
