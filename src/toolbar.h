#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include <SDL.h>

#include "fonts.h"

typedef struct toolbar_t toolbar_t;

toolbar_t * toolbar_create(SDL_Renderer * renderer, fonts_t * fonts);

void toolbar_destroy(toolbar_t * toolbar);

void toolbar_render(toolbar_t * toolbar, SDL_Renderer * renderer);

#endif