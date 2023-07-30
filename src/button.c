#include "button.h"

#include <stdbool.h>

#include "common.h"

button_t * button_new() {
    button_t * button = new (button_t);
    if (!button) {
        return NULL;
    }
    button->position = new (SDL_Point);
    if (!button->position) {
        free(button);
        return NULL;
    }
    button->position->x = 0;
    button->position->y = 0;
    button->width = 0;
    button->height = 0;
    button->texture = NULL;
    button->clip = NULL;
    button->current_state = BUTTON_STATE_MOUSE_OUT;
    return button;
}

void button_set_position(button_t * button, int x, int y) {
    button->position->x = x;
    button->position->y = y;
}

void button_set_size(button_t * button, int width, int height) {
    button->width = width;
    button->height = height;
}

void button_handle_event(button_t * button, SDL_Event * e) {
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
        // Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        // Check if mouse is in button
        bool inside = true;

        // Mouse is left of the button
        if (x < button->position->x) {
            inside = false;
        }
        // Mouse is right of the button
        else if (x > button->position->x + button->width) {
            inside = false;
        }
        // Mouse above the button
        else if (y < button->position->y) {
            inside = false;
        }
        // Mouse below the button
        else if (y > button->position->y + button->height) {
            inside = false;
        }

        // Mouse is outside button
        if (!inside) {
            button->current_state = BUTTON_STATE_MOUSE_OUT;
        }
        // Mouse is inside button
        else {
            // Set mouse over sprite
            switch (e->type) {
            case SDL_MOUSEMOTION:
                button->current_state = BUTTON_STATE_MOUSE_OVER_MOTION;
                break;

            case SDL_MOUSEBUTTONDOWN:
                button->current_state = BUTTON_STATE_MOUSE_DOWN;
                break;

            case SDL_MOUSEBUTTONUP:
                button->current_state = BUTTON_STATE_MOUSE_UP;
                break;
            }
        }
    }
}

void button_render(button_t * button, SDL_Renderer * renderer) {
    if (!button->clip) {
        texture_render(button->texture, button->position->x, button->position->y, renderer);
    } else {
        texture_render_cliped(button->texture, button->position->x, button->position->y, renderer, button->clip);
    }
}

void button_set_clip(button_t * button, SDL_Rect * clip) {
    button->clip = clip;
}

void button_set_texture(button_t * button, texture_t * texture) {
    button->texture = texture;
}