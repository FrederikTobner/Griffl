#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include "../external/SDL2/include/SDL.h"

#include "color_picker.h"
#include "fonts.h"

typedef struct toolbar_t toolbar_t;

toolbar_t * toolbar_create(SDL_Renderer * renderer, fonts_t * fonts, color_picker_t * color_picker);

void toolbar_destroy(toolbar_t * toolbar);

void toolbar_render(toolbar_t * toolbar, SDL_Renderer * renderer);

void toolbar_handle_mouse_event(toolbar_t * toolbar, SDL_Event * e);

#endif
