cmake_minimum_required (VERSION 3.10.2)
set(PROJECT1 Gameboy)
install(DIRECTORY DESTINATION "build")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(SOURCES "src/main.cpp" "src/GPU.h" "src/GPU.cpp" "src/Gameboy.h" "src/Gameboy.cpp" "src/prefixed_codes.cpp" "src/unprefixed_codes.cpp")
    
set(EXECUTABLE_OUTPUT_PATH "build")


add_executable(${PROJECT1} ${SOURCES})

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_ttf REQUIRED SDL2_ttf)

include_directories(${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(${PROJECT1} ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})