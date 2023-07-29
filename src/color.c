#include "color.h"

#include <stdlib.h>

color_t * color_new(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue) {
    color_t * color = malloc(sizeof(color_t));
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