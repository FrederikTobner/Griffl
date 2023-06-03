#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define APP_NAME      "Griffl"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define ZOOM_FACTOR   20

#define STROKE_WIDTH  5

void drawPixel(uint32_t * pixels, int x, int y, uint32_t color, uint8_t brushSize, uint16_t canvasHeight, uint16_t canvasWidth);

void handleEvents(uint32_t * pixels, bool * quit, bool * leftMouseButtonDown, uint16_t canvasHeight, uint16_t canvasWidth) {
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
                // Make sure we're in bounds
                if (mouseX < WINDOW_WIDTH && mouseY < WINDOW_HEIGHT && mouseX > 0 && mouseY > 0) {
                    drawPixel(pixels, mouseX, mouseY, 0, STROKE_WIDTH, canvasHeight, canvasWidth);
                }
            }
            break;
        }
    }
}

void drawPixel(uint32_t * pixels, int x, int y, uint32_t color, uint8_t brushSize, uint16_t canvasHeight, uint16_t canvasWidth) {
    // Alter the x and y position to correspond to the canvas
    x = x / ZOOM_FACTOR;
    y = y / ZOOM_FACTOR;
    switch (brushSize) {
    case 1:
        pixels[y * canvasWidth + x] = color;
        break;
    case 3:
        pixels[y * canvasWidth + x] = color;
        if (x < canvasWidth - 1) {
            pixels[y * canvasWidth + x + 1] = color;
        }
        if (x > 0) {
            pixels[y * canvasWidth + x - 1] = color;
        }
        if (y < canvasHeight - 1) {
            pixels[(y + 1) * canvasWidth + x] = color;
        }
        if (y > 0) {
            pixels[(y - 1) * canvasWidth + x] = color;
        }
        break;
    case 5:
        // Bounds check is not working properly
        int_fast8_t startValueY = (y > 2) ? -2 : -y;
        int_fast8_t endValueY = (y < canvasHeight - 2) ? 3 : canvasHeight - y;
        for (int_fast8_t i = startValueY; i < endValueY; i++) {
            if (i == -2 || i == 2) {
                // X Bounds check is not working properly
                int_fast8_t startValueX = (x > 1) ? -1 : -x;
                int_fast8_t endValueX = (x < canvasWidth - 2) ? 2 : canvasWidth - x;
                for (int_fast8_t j = startValueX; j < endValueX; j++) {
                    pixels[(y + i) * canvasWidth + x + j] = color;
                }
            } else {
                // X Bounds check is not working properly
                int_fast8_t startValueX = (x > 2) ? -2 : -x;
                int_fast8_t endValueX = (x < canvasWidth - 2) ? 3 : canvasWidth - x;
                for (int_fast8_t j = -2; j < 3; j++) {
                    pixels[(y + i) * canvasWidth + x + j] = color;
                }
            }
        }
        break;

    default:
        break;
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
    SDL_Texture * canvasTexture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, canvasWidth, canvasHeight);
    uint32_t * pixels = malloc(sizeof(uint32_t) * canvasWidth * canvasHeight);

    memset(pixels, 255, canvasWidth * canvasHeight * sizeof(uint32_t));

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;

    // Preparing rectangles
    SDL_Rect canvasRect = { 0, 0, canvasWidth, canvasHeight };
    SDL_Rect windowRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    while (!quit) {
        SDL_UpdateTexture(canvasTexture, NULL, pixels, canvasWidth * sizeof(uint32_t));
        handleEvents(pixels, &quit, &leftMouseButtonDown, canvasHeight, canvasWidth);
        
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
        printf("Took %f ms to render\n", deltaTime);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, canvasTexture, &canvasRect, &windowRect);
        SDL_RenderPresent(renderer);
    }

    free(pixels);
    SDL_DestroyTexture(canvasTexture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
