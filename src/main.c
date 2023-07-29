// SDL dependencies
#include <SDL.h>
#include <SDL_image.h>
// Standard library dependencies
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Internal dependencies
#include "canvas.h"
#include "griffl_application.h"
#include "input_state.h"

#define APP_NAME       "Griffl"
#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  630
#define TOOLBAR_HEIGHT 30
#define ZOOM_FACTOR    5
#define BRUSH_COLOR    0xFF0000FF

#define STROKE_WIDTH   5

int main(int argc, char ** argv) {
    bool quit = false;
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    atexit(SDL_Quit);
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }
    atexit(IMG_Quit);
    griffl_application_t * application =
        griffl_application_new(WINDOW_WIDTH, WINDOW_HEIGHT, TOOLBAR_HEIGHT, ZOOM_FACTOR, BRUSH_COLOR);
    griffl_application_set_brush_size(application, STROKE_WIDTH);
    if (!application) {
        printf("Could not create application!\n");
        return EXIT_FAILURE;
    }
    SDL_Window * window =
        SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("SDL could not create window! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("SDL could not create renderer! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Texture * canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                    canvas_get_width(griffl_application_get_canvas(application)),
                                                    canvas_get_height(griffl_application_get_canvas(application)));
    if (!canvasTexture) {
        printf("SDL could not create texture! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    uint64_t NOW = SDL_GetPerformanceCounter();
    uint64_t LAST = 0;
    double fpsdisplayTimeCounter = 0;
    uint16_t fps = 0;
    // Rendering the first frame
    if (griffl_application_update_screen(application, renderer, canvasTexture)) {
        return EXIT_FAILURE;
    }
    while (!griffl_application_is_quit(application)) {
        griffl_application_handle_events(application);
        if (input_state_is_down(griffl_application_get_input_state(application), LEFT_MOUSE_BUTTON)) {
            // We only need to update our rendering if we are drawing
            if (griffl_application_update_screen(application, renderer, canvasTexture)) {
                return EXIT_FAILURE;
            }
        }
    }
    griffl_application_destroy(application);
    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return EXIT_SUCCESS;
}
