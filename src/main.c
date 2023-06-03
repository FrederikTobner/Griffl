#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Internal dependencies
#include "canvas.h"

#define APP_NAME      "Griffl"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define ZOOM_FACTOR   5
#define BRUSH_COLOR   0xFF000000

#define STROKE_WIDTH  5

void handleEvents(canvas_t * canvas, bool * quit, bool * leftMouseButtonDown) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            *quit = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                *leftMouseButtonDown = false;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                *leftMouseButtonDown = true;
            }
        case SDL_MOUSEMOTION:
            if (*leftMouseButtonDown) {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                printf("Mouse at (%d, %d)\n", mouseX, mouseY);
                // Make sure the cursor is within the window
                if (mouseX < WINDOW_WIDTH && mouseY < WINDOW_HEIGHT && mouseX > 0 && mouseY > 0) {
                    canvas_draw(canvas, mouseX / ZOOM_FACTOR, mouseY / ZOOM_FACTOR, BRUSH_COLOR, STROKE_WIDTH);
                }
            }
            break;
        }
    }
}

int main(int argc, char ** argv) {

    bool leftMouseButtonDown = false;
    bool quit = false;
    uint16_t canvasHeight = WINDOW_HEIGHT / ZOOM_FACTOR;
    uint16_t canvasWidth = WINDOW_WIDTH / ZOOM_FACTOR;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window =
        SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    canvas_t * canvas = canvas_new(canvasWidth, canvasHeight);
    SDL_Texture * canvasTexture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, canvasWidth, canvasHeight);

    uint64_t NOW = SDL_GetPerformanceCounter();
    uint64_t LAST = 0;
    double deltaTime = 0;

    // Preparing rectangles
    SDL_Rect canvasRect = {0, 0, canvasWidth, canvasHeight};
    SDL_Rect windowRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    while (!quit) {
        SDL_UpdateTexture(canvasTexture, NULL, canvas->pixels, canvasWidth * sizeof(uint32_t));
        handleEvents(canvas, &quit, &leftMouseButtonDown);

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
        printf("Took %f ms to render\n", deltaTime);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, canvasTexture, &canvasRect, &windowRect);
        SDL_RenderPresent(renderer);
    }

    canvas_destroy(canvas);
    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
