#include "toolbar.h"

#include "button.h"
#include "icons_material_design.h"
#include "memory.h"
#include "texture.h"

typedef struct toolbar_t {
        button_t * colorpicker;
        SDL_Color toolBarBackgroundColor;
        color_picker_t * color_picker;
} toolbar_t;

toolbar_t * toolbar_create(SDL_Renderer * renderer, fonts_t * fonts, color_picker_t * color_picker) {
    toolbar_t * toolbar = new (toolbar_t);
    if (!toolbar) {
        return NULL;
    }
    SDL_Color color = {0xff, 0xff, 0xff, 255};
    SDL_Color buttonBackGroundColor = {60, 60, 60, 0xFF};
    SDL_Color buttonBackGroundColorMouseOver = {255, 0, 0, 255};
    SDL_Color buttonBorderColorMouseOver = {0, 255, 0, 255};
    toolbar->colorpicker =
        button_new(texture_load_from_rendered_text(ICON_MD_COLOR_LENS, color, fonts->material_icons, renderer), 5, 5,
                   40, 40, buttonBackGroundColor, buttonBackGroundColorMouseOver, buttonBorderColorMouseOver, 3);
    if (!toolbar->colorpicker) {
        free(toolbar);
        return NULL;
    }
    toolbar->color_picker = color_picker;
    return toolbar;
}

void toolbar_destroy(toolbar_t * toolbar) {
    free(toolbar);
    button_destroy(toolbar->colorpicker);
}

void toolbar_render(toolbar_t * toolbar, SDL_Renderer * renderer) {
    button_render(toolbar->colorpicker, renderer);
}

void toolbar_handle_mouse_event(toolbar_t * toolbar, SDL_Event * e) {
    if (button_handle_event(toolbar->colorpicker, e)) {
        color_picker_show(toolbar->color_picker);
    }
}