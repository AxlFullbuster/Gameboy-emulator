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
    } else{
        bool quit = false;
        emu.loadGame(argv[1]);
        
        if(!emu.loadGame(argv[1])) return 1;
        
        
        
        //run the emulator until the user closes the window
        while(!quit){
            ImGuiIO& io = ImGui::GetIO();
            int wheel = 0;
            SDL_Event e;
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
                               
                    case SDL_KEYDOWN:
                    case SDL_KEYUP:{
                        int key = e.key.keysym.scancode;
                        IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
                        io.KeysDown[key] = (e.type == SDL_KEYDOWN);
                        io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
                        io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
                        io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
                    }
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
            
            io.AddInputCharactersUTF8(e.text.text);
            
            io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
            io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
            io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
            io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
            io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
            io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
            io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
            io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
            io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
            io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
            io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
            io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
            io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
            io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
            io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
            io.KeyMap[ImGuiKey_KeyPadEnter] = SDL_SCANCODE_RETURN2;
            io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
            io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
            io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
            io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
            io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
            io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
            
            display.draw_debugger();
            emulate();
        }
    }
    display.close();
    return 0;
}
