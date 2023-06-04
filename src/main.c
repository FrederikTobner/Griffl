// SDL dependencies
#include <SDL.h>
// Standard library dependencies
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Internal dependencies
#include "canvas.h"

#define APP_NAME       "Griffl"
#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  650
#define TOOLBAR_HEIGHT 50
#define ZOOM_FACTOR    5
#define BRUSH_COLOR    0xFF000000

#define STROKE_WIDTH   5

void handleEvents(canvas_t * canvas, bool * quit, bool * leftMouseButtonDown, bool * rightCTRLDown,
                  bool * leftCTRLDown) {
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
                *rightCTRLDown = true;
            } else if (event.key.keysym.sym == SDLK_LCTRL) {
                *leftCTRLDown = true;
            } else if (event.key.keysym.sym == SDLK_s && (*rightCTRLDown || *leftCTRLDown)) {
                canvas_save_as_png("./canvas.png", canvas);
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_RCTRL) {
                *rightCTRLDown = false;
            } else if (event.key.keysym.sym == SDLK_LCTRL) {
                *leftCTRLDown = false;
            }
            break;
        default:
            break;
        }
    }
}

int main(int argc, char ** argv) {

    bool leftMouseButtonDown = false;
    bool quit = false;
    bool rightCTRLDown = false;
    bool leftCTRLDown = false;
    uint16_t canvasHeight = (WINDOW_HEIGHT - TOOLBAR_HEIGHT) / ZOOM_FACTOR;
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
    double fpsdisplayTimeCounter = 0;
    uint16_t fps = 0;

    // Preparing rectangles
    SDL_Rect canvasSrcRect = {0, 0, canvasWidth, canvasHeight};
    SDL_Rect canvasDstRect = {0, TOOLBAR_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - TOOLBAR_HEIGHT};
    // Rendering the first frame
    SDL_UpdateTexture(canvasTexture, NULL, canvas->pixels, canvasWidth * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, canvasTexture, &canvasSrcRect, &canvasDstRect);
    SDL_RenderPresent(renderer);

    while (!quit) {
        handleEvents(canvas, &quit, &leftMouseButtonDown, &rightCTRLDown, &leftCTRLDown);

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
        if (leftMouseButtonDown) {
            // We only need to update our rendering if we are drawing
            SDL_UpdateTexture(canvasTexture, NULL, canvas->pixels, canvasWidth * sizeof(uint32_t));
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, canvasTexture, &canvasSrcRect, &canvasDstRect);
            SDL_RenderPresent(renderer);
        }
    }

    canvas_destroy(canvas);
    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
