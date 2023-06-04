#ifndef CANVAS_H
#define CANVAS_H

#include <SDL.h>

#include <stdint.h>

// Models a canvas of pixels
typedef struct canvas_t canvas_t;

// Creates a canvas with the given width and height
canvas_t * canvas_new(uint16_t width, uint16_t height);

void canvas_draw(canvas_t * canvas, int32_t x, int32_t y, uint32_t color, uint8_t brushSize);

void canvas_destroy(canvas_t * canvas);

void canvas_resize(canvas_t * canvas, uint16_t width, uint16_t height);

void canvas_save_as_png(const char * file_name, canvas_t * canvas);

uint16_t canvas_get_width(canvas_t * canvas);

uint16_t canvas_get_height(canvas_t * canvas);

void canvas_update_texture(canvas_t * canvas, SDL_Texture * texture);

#endif