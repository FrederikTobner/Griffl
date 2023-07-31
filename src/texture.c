#include "texture.h"

#include <stdio.h>

#include "memory.h"

SDL_Texture * create_texture_from_file(char * path, SDL_Renderer * renderer) {

    // Load image at specified path
    SDL_Surface * loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return NULL;
    }
    // Color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    // Create texture from surface pixels
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!texture) {
        fprintf(stderr, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
    return texture;
}

SDL_Texture * texture_load_from_rendered_text(char const * text, SDL_Color textColor, TTF_Font * font,
                                              SDL_Renderer * renderer) {

    // Render text surface
    SDL_Surface * textSurface = TTF_RenderUTF8_Solid(font, text, textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }
    // Create texture from surface pixels
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (texture == NULL) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }

    // Get rid of old surface
    SDL_FreeSurface(textSurface);

    // Return success
    return texture;
}