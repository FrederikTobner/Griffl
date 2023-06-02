#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define APP_NAME      "Griffl"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

int main(int argc, char ** argv) {

    bool leftMouseButtonDown = false;
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window =
        SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    uint32_t * pixels = malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

    memset(pixels, 255, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    while (!quit) {
        SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH * sizeof(uint32_t));
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                }
            case SDL_MOUSEMOTION:
                if (leftMouseButtonDown) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
                    printf("Mouse at (%d, %d)\n", mouseX, mouseY);
                    // Make sure we're in bounds
                    if (mouseX < WINDOW_WIDTH && mouseY < WINDOW_HEIGHT && mouseX > 0 && mouseY > 0) {
                        pixels[mouseY * WINDOW_WIDTH + mouseX] = 0;
                    }
                }
                break;
            }
        }

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
        printf("Took %f ms to render\n", deltaTime);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
