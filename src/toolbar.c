#include "toolbar.h"

#include "button.h"
#include "icons_material_design.h"
#include "memory.h"
#include "texture.h"

typedef struct toolbar_t {
        button_t * colorpicker;
} toolbar_t;

toolbar_t * toolbar_create(SDL_Renderer * renderer, fonts_t * fonts) {
    toolbar_t * toolbar = new (toolbar_t);
    if (!toolbar) {
        return NULL;
    }
    SDL_Color color = {0, 0, 0, 255};
    SDL_Color buttonBackGroundColor = {135, 206, 235, 255};
    SDL_Color buttonBorderColor = {255, 0, 0, 255};
    toolbar->colorpicker =
        button_new(texture_load_from_rendered_text(ICON_MD_COLOR_LENS, color, fonts->material_icons, renderer), 10, 10,
                   30, 30, buttonBackGroundColor, buttonBorderColor, 1);
    if (!toolbar->colorpicker) {
        free(toolbar);
        return NULL;
    }
    return toolbar;
}

void toolbar_destroy(toolbar_t * toolbar) {
    free(toolbar);
    button_destroy(toolbar->colorpicker);
}

void toolbar_render(toolbar_t * toolbar, SDL_Renderer * renderer) {
    button_render(toolbar->colorpicker, renderer);
}