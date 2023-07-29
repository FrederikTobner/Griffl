#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>

typedef struct {
        uint8_t alpha;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
} color_t;

color_t * color_new(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);

uint32_t color_as_argb(color_t * color);

#endif