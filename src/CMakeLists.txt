# Looks up all the files in this directory that end with .c or .h (Recursively)
file(GLOB_RECURSE GRIFFL_SOURCES *.c *.h)

include(../cmake/detect.cmake)
Detect_Platform()
Detect_Compiler()

add_executable(${PROJECT_NAME} ${GRIFFL_SOURCES})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static)
target_include_directories(${PROJECT_NAME} PUBLIC ../external/SDL2/include ../external/SDL_image/ ../external/SDL_ttf/)