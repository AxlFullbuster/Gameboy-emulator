#include "CPU.h"
#include "GPU.h"
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;

CPU emu;
GPU display(emu);



//runs the emulation loop for one frame then refresh the screen
void emulate(){
    int grand_cycles = 0;
    while(grand_cycles < 70244){
        emu.emulateCycle();
        grand_cycles += emu.timing;
        emu.update_timers();
        display.check_scanline();
        emu.check_interrupt();
       //other junk  
    }
    display.draw_display();
}

int main(int argc, char* argv[]){
    //check if rom file name was entered
    if(argc < 2) {
        cout << "Please provide a rom file to open." << endl;
        return 1;
    }
    
    cout << "Would you like to display the debugger? Yes(1), No(0): ";
    cin >> display.debug;
    cout << "Are you trying to run the bios rom? Yes(1), No(0): ";
    cin >> emu.bios;
    
    //check to make sure SDL initialized the window
    if(!display.init()){
        cout << "Failed to initialize display." << endl;
    }else{
        bool quit = false;
        emu.loadGame(argv[1]);
        
        if(!emu.loadGame(argv[1])) return 1;
        
        
        
        //run the emulator until the user closes the window
        while(!quit){
            ImGuiIO& io = ImGui::GetIO();
            int wheel = 0;
            SDL_Event e;
            while( SDL_PollEvent( &e ) != 0 ){
                if (e.type == SDL_QUIT) quit = true;
                
                else if (e.type == SDL_WINDOWEVENT){
                    if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                        io.DisplaySize.x = static_cast<float>(e.window.data1);
                        io.DisplaySize.y = static_cast<float>(e.window.data2);
                    }
                }
                
                
                else if (e.type == SDL_MOUSEWHEEL){
                    wheel = e.wheel.y;
                }
                
                else if (e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
                    switch(e.key.keysym.sym){
                        case SDLK_RIGHT:
                            emu.buttons[0] = 0;
                        break; 
                        
                        case SDLK_LEFT:
                            emu.buttons[1] = 0;
                        break;
                    
                        case SDLK_UP:
                            emu.buttons[2] = 0;
                        break;

                        case SDLK_DOWN:
                            emu.buttons[3] = 0;
                        break;
    
                        case SDLK_z:
                            emu.buttons[4] = 0;
                        break;
                    
                        case SDLK_x:
                            emu.buttons[5] = 0;
                        break;
                    
                        case SDLK_SPACE:
                            emu.buttons[6] = 0;
                        break;

                        case SDLK_RSHIFT:
                            emu.buttons[7] = 0;
                        break;
                    }
                }
            
                
                else if(e.type == SDL_KEYUP && e.key.repeat == 0){
                    switch(e.key.keysym.sym){
                        case SDLK_RIGHT:
                            emu.buttons[0] = 1;
                        break; 
                        
                        case SDLK_LEFT:
                            emu.buttons[1] = 1;
                        break;
                    
                        case SDLK_UP:
                            emu.buttons[2] = 1;
                        break;

                        case SDLK_DOWN:
                            emu.buttons[3] = 1;
                        break;

                        case SDLK_z:
                            emu.buttons[4] = 1;
                        break;
                    
                        case SDLK_x:
                            emu.buttons[5] = 1;
                        break;
                    
                        case SDLK_SPACE:
                            emu.buttons[6] = 1;
                        break;

                        case SDLK_RSHIFT:
                            emu.buttons[7] = 1;
                        break;
                    }
                }
            }

                
            int mouseX, mouseY;
            const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
            
            io.DeltaTime = 1.0f / 60.0f;
            io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
            io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
            io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
            io.MouseWheel = static_cast<float>(wheel);
            
            
            display.draw_debugger();
            emulate();
        }
    }
    display.close();
    return 0;
}
