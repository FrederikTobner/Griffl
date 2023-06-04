set(GRIFFL_SOURCE_FILES
main.c
canvas.c
input_state.c
)

set(GRIFFL_HEADER_FILES
canvas.h
input_state.h
)

add_executable(${PROJECT_NAME} ${GRIFFL_SOURCE_FILES} ${GRIFFL_HEADER_FILES})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static)
target_include_directories(${PROJECT_NAME} PUBLIC ../external/SDL2/include ../external/SDL_image/)