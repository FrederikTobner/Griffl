#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>

typedef struct color_t color_t;

color_t * color_new(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);

uint32_t color_as_argb(color_t * color);

uint8_t color_get_alpha(color_t * color);

uint8_t color_get_red(color_t * color);

uint8_t color_get_green(color_t * color);

uint8_t color_get_blue(color_t * color);

void color_set_alpha(color_t * color, uint8_t alpha);

void color_set_red(color_t * color, uint8_t red);

void color_set_green(color_t * color, uint8_t green);

void color_set_blue(color_t * color, uint8_t blue);

#endif