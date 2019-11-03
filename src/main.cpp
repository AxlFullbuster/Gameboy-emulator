#include "Gameboy.h"
#include "GPU.h"
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;

Gameboy emu;
GPU display;

int main(int argc, char* argv[]){
    //check if rom file name was entered
    if(argc < 2) {
        cout << "Please provide a rom file to open." << endl;
        return 1;
    }
    
    //check to make sure SDL initialized the window
    if(!display.init()){
        cout << "Failed to initialize display." << endl;
    } else{
        bool quit = false;
        emu.loadGame(argv[1]);
        
        if(!emu.loadGame(argv[1])) return 1;
        
        SDL_Event e;
        //run the emulator until the user closes the window
        while(!quit){
            ImGuiIO& io = ImGui::GetIO();
            int wheel = 0;
            while( SDL_PollEvent( &e ) != 0 ) {
                switch(e.type){
                    case SDL_QUIT:
                        quit = true;
                    break;
                
                    case SDL_WINDOWEVENT:
                        switch(e.window.event){
                            case SDL_WINDOWEVENT_SIZE_CHANGED:
                                io.DisplaySize.x = static_cast<float>(e.window.data1);
                                io.DisplaySize.y = static_cast<float>(e.window.data2);
                            break;
                        
                            default:
                                //do nothing
                            break;
                        }
                    break;
                
                    case SDL_MOUSEWHEEL:
                        wheel = e.wheel.y;
                    break;
                
                    default:
                        //do nothing
                    break;
                } 
            }
            int mouseX, mouseY;
            const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
            
            io.DeltaTime = 1.0f / 60.0f;
            io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
            io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
            io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
            io.MouseWheel = static_cast<float>(wheel);
            
            emu.emuLoop();
            display.draw_display(emu);
        }
    }
    display.close();
    return 0;
}
