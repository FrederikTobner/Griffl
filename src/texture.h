#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdbool.h>

typedef struct {
        int width;
        int height;
        SDL_Texture * texture;
} texture_t;

texture_t * texture_new();

void texture_destroy(texture_t * texture);

bool texture_loadFromFile(texture_t * texture, char * path, SDL_Renderer * gRenderer);

bool texture_load_from_rendered_text(texture_t * texture, char const * textureText, SDL_Color textColor,
                                     TTF_Font * gFont, SDL_Renderer * renderer);

void texture_free(texture_t * texture);

void texture_render(texture_t * texture, int x, int y, SDL_Renderer * renderer);

void texture_render_cliped(texture_t * texture, int x, int y, SDL_Renderer * renderer, SDL_Rect * clip);

void texture_render_flipped(texture_t * texture, int x, int y, SDL_Rect * clip, double angle, SDL_Point * center,
                            SDL_RendererFlip flip, SDL_Renderer * renderer);

void texture_set_blend_mode(texture_t * texture, SDL_BlendMode blending);

void texture_set_alpha(texture_t * texture, uint8_t alpha);

void texture_set_color(texture_t * texture, uint8_t red, uint8_t green, uint8_t blue);

#endif // TEXTURE_H_