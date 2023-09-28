#ifndef FONT_MANAGER_H_
#define FONT_MANAGER_H_

#include "../external/SDL_ttf/SDL_ttf.h"

typedef struct {
        TTF_Font * material_icons;
        TTF_Font * roboto;
} fonts_t;

fonts_t * fonts_create(void);

void fonts_destroy(fonts_t * fonts);

#endif
