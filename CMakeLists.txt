cmake_minimum_required (VERSION 3.10.2)
set(PROJECT1 Gameboy)
install(DIRECTORY DESTINATION "build")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
file(GLOB imgui "imgui_lib/*.h" "imgui_lib/*.cpp")
file(GLOB src "src/*.h" "src/*.cpp")
include_directories("imgui_lib")
set(SOURCES ${src} ${imgui})
    
set(EXECUTABLE_OUTPUT_PATH "build")


add_executable(${PROJECT1} ${SOURCES})

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

target_link_libraries(${PROJECT1} SDL2main SDL2)