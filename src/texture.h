#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdbool.h>

SDL_Texture * texture_load_from_rendered_text(char const * text, SDL_Color textColor, TTF_Font * font,
                                              SDL_Renderer * renderer);

SDL_Texture * create_texture_from_file(char * path, SDL_Renderer * renderer);

#endif // TEXTURE_H_