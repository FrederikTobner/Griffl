#include "color.h"

#include <stdlib.h>

#include "memory.h"

typedef struct color_t {
        uint8_t alpha;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
} color_t;

color_t * color_new(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue) {
    color_t * color = new (color_t);
    if (!color) {
        return color;
    }
    color->alpha = alpha;
    color->red = red;
    color->green = green;
    color->blue = blue;
    return color;
}

uint32_t color_as_argb(color_t * color) {
    return (color->alpha << 24) | (color->red << 16) | (color->green << 8) | color->blue;
}

uint8_t color_get_alpha(color_t * color) {
    return color->alpha;
}

uint8_t color_get_red(color_t * color) {
    return color->red;
}

uint8_t color_get_green(color_t * color) {
    return color->green;
}

uint8_t color_get_blue(color_t * color) {
    return color->blue;
}

void color_set_alpha(color_t * color, uint8_t alpha) {
    color->alpha = alpha;
}

void color_set_red(color_t * color, uint8_t red) {
    color->red = red;
}

void color_set_green(color_t * color, uint8_t green) {
    color->green = green;
}

void color_set_blue(color_t * color, uint8_t blue) {
    color->blue = blue;
}