set(GRIFFL_SOURCE_FILES
main.c
canvas.c
color.c
griffl_application.c
input_state.c
)

set(GRIFFL_HEADER_FILES
canvas.h
color.h
draw_mode.h
griffl_application.h
input_state.h
)

add_executable(${PROJECT_NAME} ${GRIFFL_SOURCE_FILES} ${GRIFFL_HEADER_FILES})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static)
target_include_directories(${PROJECT_NAME} PUBLIC ../external/SDL2/include ../external/SDL_image/)