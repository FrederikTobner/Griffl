#include "canvas.h"

canvas_t * canvas_new(uint16_t width, uint16_t height) {
    canvas_t * canvas = malloc(sizeof(canvas_t));
    canvas->width = width;
    canvas->height = height;
    canvas->pixels = malloc(sizeof(uint32_t) * width * height);
    memset(canvas->pixels, 255, width * height * sizeof(uint32_t));
    return canvas;
}

void canvas_draw(canvas_t * canvas, int32_t x, int32_t y, uint32_t color, uint8_t brushSize) {
    switch (brushSize) {
    case 1:
        canvas->pixels[y * canvas->width + x] = color;
        break;
    case 3:
        canvas->pixels[y * canvas->width + x] = color;
        if (x < canvas->width - 1) {
            canvas->pixels[y * canvas->width + x + 1] = color;
        }
        if (x > 0) {
            canvas->pixels[y * canvas->width + x - 1] = color;
        }
        if (y < canvas->height - 1) {
            canvas->pixels[(y + 1) * canvas->width + x] = color;
        }
        if (y > 0) {
            canvas->pixels[(y - 1) * canvas->width + x] = color;
        }
        break;
    case 5:
        {
            int8_t endValueY = (y < canvas->height - 2) ? 3 : canvas->height - y;
            for (int8_t i = (y > 2) ? -2 : -y; i < endValueY; i++) {
                if (i == -2 || i == 2) {
                    for (int8_t j = (x > 1) ? -1 : -x; j < 2 && j + x < canvas->width; j++) {
                        canvas->pixels[(y + i) * canvas->width + x + j] = color;
                    }
                } else {
                    for (int8_t j = (x > 2) ? -2 : -x; j < 3 && j + x < canvas->width; j++) {
                        canvas->pixels[(y + i) * canvas->width + x + j] = color;
                    }
                }
            }
            break;
        }

    default:
        break;
    }
}

void canvas_resize(canvas_t * canvas, uint16_t width, uint16_t height) {
    canvas->width = width;
    canvas->height = height;
    uint32_t * newPixels = malloc(sizeof(uint32_t) * width * height);
    if(newPixels != NULL) {
        memcpy(newPixels, canvas->pixels, sizeof(uint32_t) * width * height);
        free(canvas->pixels);
        canvas->pixels = newPixels;
    }
}

void canvas_destroy(canvas_t * canvas) {
    free(canvas->pixels);
    free(canvas);
}

void canvas_save_as_png(const char * file_name, canvas_t * canvas) {
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(canvas->pixels, canvas->width, canvas->height, 32,
                                                     canvas->width * 4, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    IMG_SavePNG(surface, file_name);
    SDL_FreeSurface(surface);
}
