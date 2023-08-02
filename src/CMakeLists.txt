set(GRIFFL_SOURCE_FILES
button.c
canvas.c
color.c
color_picker.c
fonts.c
griffl_application.c
input_state.c
main.c
texture.c
toolbar.c
)

set(GRIFFL_HEADER_FILES
button.h
canvas.h
color.h
color_picker.h
draw_mode.h
fonts.h
griffl_application.h
input_state.h
memory.h
texture.h
toolbar.h
)

include(../cmake/detect.cmake)
Detect_Platform()
Detect_Compiler()

add_executable(${PROJECT_NAME} ${GRIFFL_SOURCE_FILES} ${GRIFFL_HEADER_FILES})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static)
target_include_directories(${PROJECT_NAME} PUBLIC ../external/SDL2/include ../external/SDL_image/ ../external/SDL_ttf/)