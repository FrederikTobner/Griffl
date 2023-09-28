#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include <stdbool.h>
#include <stdint.h>

#include "../external/SDL2/include/SDL.h"

#include "color.h"
#include "fonts.h"

typedef struct color_picker_t color_picker_t;

color_picker_t * color_picker_new(fonts_t * fonts, color_t * selected_color);

void color_picker_destroy(color_picker_t * color_picker);

void color_picker_show(color_picker_t * color_picker);

void color_picker_hide(color_picker_t * color_picker);

bool color_picker_is_shown(color_picker_t * color_picker);

void color_picker_render(color_picker_t * color_picker);

void color_picker_handle_event(color_picker_t * color_picker, SDL_Event event);

uint32_t color_picker_get_window_id(color_picker_t * color_picker);

#endif
