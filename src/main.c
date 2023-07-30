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

#define APP_NAME      "Griffl"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 630
#define ZOOM_FACTOR   5
#define BRUSH_COLOR   0xFF0000FF

#define STROKE_WIDTH  5

/// @brief Main entry point of the application.
/// @param argc The number of command line arguments.
/// @param argv The command line arguments.
/// @return 0 on success, 1 on failure.
int main(int argc, char ** argv) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    atexit(SDL_Quit);
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }
    atexit(IMG_Quit);

    griffl_application_t * application =
        griffl_application_new(WINDOW_WIDTH, WINDOW_HEIGHT, ZOOM_FACTOR, BRUSH_COLOR, STROKE_WIDTH);
    if (!application) {
        fprintf(stderr, "Could not create application!\n");
        return EXIT_FAILURE;
    }
    if (griffl_application_run(application)) {
        return EXIT_FAILURE;
    }

    griffl_application_destroy(application);
    return EXIT_SUCCESS;
}
