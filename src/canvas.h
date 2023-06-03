#ifndef CANVAS_H
#define CANVAS_H

#include <SDL.h>

#include <stdint.h>

// Models a canvas of pixels
typedef struct {
        uint16_t width;
        uint16_t height;
        uint32_t * pixels;
} canvas_t;

// Creates a canvas with the given width and height
canvas_t * canvas_new(uint16_t width, uint16_t height);

void canvas_draw(canvas_t * canvas, int x, int y, uint32_t color, uint8_t brushSize);

void canvas_destroy(canvas_t * canvas);

#endif