// SDL dependencies
#include <SDL.h>
#include <SDL_image.h>
// Standard library dependencies
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Internal dependencies
#include "canvas.h"
#include "input_state.h"

#define APP_NAME       "Griffl"
#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  650
#define TOOLBAR_HEIGHT 50
#define ZOOM_FACTOR    5
#define BRUSH_COLOR    0xFF000000

#define STROKE_WIDTH   5

void handleEvents(canvas_t * canvas, bool * quit, input_state_t * input_state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            *quit = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                input_state_handle_up(input_state, LEFT_MOUSE_BUTTON);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                input_state_handle_down(input_state, LEFT_MOUSE_BUTTON);
            }
        case SDL_MOUSEMOTION:
            if (input_state_is_down(input_state, LEFT_MOUSE_BUTTON)) {
                int32_t mouseX = event.motion.x;
                int32_t mouseY = event.motion.y;
                // Make sure the cursor is within the window
                if (mouseX < WINDOW_WIDTH && mouseY < WINDOW_HEIGHT && mouseX > 0 && mouseY > TOOLBAR_HEIGHT) {
                    canvas_draw(canvas, mouseX / ZOOM_FACTOR, (mouseY - TOOLBAR_HEIGHT) / ZOOM_FACTOR, BRUSH_COLOR,
                                STROKE_WIDTH);
                }
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_RCTRL) {
                input_state_handle_down(input_state, RIGHT_CTRL);
            } else if (event.key.keysym.sym == SDLK_LCTRL) {
                input_state_handle_down(input_state, LEFT_CTRL);
            } else if (event.key.keysym.sym == SDLK_s &&
                       (input_state_is_down(input_state, RIGHT_CTRL) || input_state_is_down(input_state, LEFT_CTRL))) {
                canvas_save_as_png("./canvas.png", canvas);
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_RCTRL) {
                input_state_handle_up(input_state, RIGHT_CTRL);
            } else if (event.key.keysym.sym == SDLK_LCTRL) {
                input_state_handle_up(input_state, LEFT_CTRL);
            }
            break;
        default:
            break;
        }
    }
}

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

    canvas_t * canvas = canvas_new(WINDOW_WIDTH / ZOOM_FACTOR, (WINDOW_HEIGHT - TOOLBAR_HEIGHT) / ZOOM_FACTOR);
    input_state_t * input_state = input_state_new();

    SDL_Window * window =
        SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                    canvas_get_width(canvas), canvas_get_height(canvas));

    uint64_t NOW = SDL_GetPerformanceCounter();
    uint64_t LAST = 0;
    double deltaTime = 0;
    double fpsdisplayTimeCounter = 0;
    uint16_t fps = 0;

    // Preparing rectangles
    SDL_Rect canvasSrcRect = {0, 0, canvas_get_width(canvas), canvas_get_height(canvas)};
    SDL_Rect canvasDstRect = {0, TOOLBAR_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - TOOLBAR_HEIGHT};
    // Rendering the first frame
    canvas_update_texture(canvas, canvasTexture);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, canvasTexture, &canvasSrcRect, &canvasDstRect);
    SDL_RenderPresent(renderer);

    while (!quit) {
        handleEvents(canvas, &quit, input_state);

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        // Calculating delta time in milliseconds
        deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
        fpsdisplayTimeCounter += deltaTime;
        fps++;
        if (fpsdisplayTimeCounter > 1000) {
            fpsdisplayTimeCounter = 0;
            printf("%i FPS\n", fps);
            fps = 0;
        }
        if (input_state_is_down(input_state, LEFT_MOUSE_BUTTON)) {
            // We only need to update our rendering if we are drawing
            canvas_update_texture(canvas, canvasTexture);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, canvasTexture, &canvasSrcRect, &canvasDstRect);
            SDL_RenderPresent(renderer);
        }
    }

    canvas_destroy(canvas);
    input_state_destroy(input_state);
    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    return 0;
}
