#include "texture.h"

#include <stdio.h>

#include "common.h"

texture_t * texture_new() {
    texture_t * texture = new (texture_t);
    if (!texture) {
        return NULL;
    }
    // Initialize
    texture->texture = NULL;
    texture->width = 0;
    texture->height = 0;
    return texture;
}

void texture_destroy(texture_t * texture) {
    free(texture);
}

bool texture_loadFromFile(texture_t * texture, char * path, SDL_Renderer * renderer) {
    if (texture->texture) {
        free(texture->texture);
    }

    // The final texture
    SDL_Texture * newTexture = NULL;

    // Load image at specified path
    SDL_Surface * loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (!newTexture) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        } else {
            // Get image dimensions
            texture->width = loadedSurface->w;
            texture->height = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    texture->texture = newTexture;
    return texture->texture != NULL;
}

bool texture_load_from_rendered_text(texture_t * texture, char const * textureText, SDL_Color textColor,
                                     TTF_Font * gFont, SDL_Renderer * renderer) {
    // Get rid of preexisting texture
    texture_free(texture);

    // Render text surface
    SDL_Surface * textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        // Create texture from surface pixels
        texture->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture->texture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            // Get image dimensions
            texture->width = textSurface->w;
            texture->height = textSurface->h;
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    // Return success
    return texture->texture != NULL;
}

void texture_free(texture_t * texture) {
    // Free texture if it exists
    if (texture->texture != NULL) {
        SDL_DestroyTexture(texture->texture);
        texture->texture = NULL;
        texture->width = 0;
        texture->height = 0;
    }
}

void texture_render(texture_t * texture, int x, int y, SDL_Renderer * renderer) {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, texture->width, texture->height};
    SDL_RenderCopy(renderer, texture->texture, NULL, &renderQuad);
}

void texture_render_cliped(texture_t * texture, int x, int y, SDL_Renderer * renderer, SDL_Rect * clip) {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, texture->width, texture->height};

    if (clip) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(renderer, texture->texture, clip, &renderQuad);
}

void texture_render_flipped(texture_t * texture, int x, int y, SDL_Rect * clip, double angle, SDL_Point * center,
                            SDL_RendererFlip flip, SDL_Renderer * renderer) {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, texture->width, texture->height};

    // Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopyEx(renderer, texture->texture, clip, &renderQuad, angle, center, flip);
}

void texture_set_blend_mode(texture_t * texture, SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(texture->texture, blending);
}

void texture_set_alpha(texture_t * texture, uint8_t alpha) {
    SDL_SetTextureAlphaMod(texture->texture, alpha);
}

void texture_set_color(texture_t * texture, uint8_t red, uint8_t green, uint8_t blue) {
    SDL_SetTextureColorMod(texture->texture, red, green, blue);
}