set(GRIFFL_SOURCE_FILES
main.c
canvas.c
)

set(GRIFFL_HEADER_FILES
canvas.h
)

add_executable(${PROJECT_NAME} ${GRIFFL_SOURCE_FILES} ${GRIFFL_HEADER_FILES})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static)
target_include_directories(${PROJECT_NAME} PUBLIC ../external/SDL2/include)