#include "fonts.h"

#include "memory.h"

fonts_t * fonts_create(void) {
    fonts_t * fonts = new (fonts_t);
    if (!fonts) {
        return NULL;
    }
    fonts->material_icons = TTF_OpenFont("../fonts/MaterialIcons-Regular.ttf", 20);
    if (!fonts->material_icons) {
        free(fonts);
        return NULL;
    }
    fonts->roboto = TTF_OpenFont("../fonts/Roboto-Medium.ttf", 20);
    if (!fonts->roboto) {
        TTF_CloseFont(fonts->material_icons);
        free(fonts);
        return NULL;
    }
    return fonts;
}

void fonts_destroy(fonts_t * fonts) {
    TTF_CloseFont(fonts->material_icons);
    TTF_CloseFont(fonts->roboto);
    free(fonts);
}