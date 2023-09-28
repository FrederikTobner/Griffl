#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../external/SDL2/include/SDL.h"
#include "../external/SDL_image/SDL_image.h"
#include "../external/SDL_ttf/SDL_ttf.h"
#include <stdbool.h>

SDL_Texture * texture_load_from_rendered_text(char const * text, SDL_Color textColor, TTF_Font * font,
                                              SDL_Renderer * renderer);

SDL_Texture * create_texture_from_file(char * path, SDL_Renderer * renderer);

#endif // TEXTURE_H_
