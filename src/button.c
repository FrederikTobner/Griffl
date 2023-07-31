#include "button.h"

#include <assert.h>
#include <stdbool.h>

#include "memory.h"

typedef struct button_t {
        button_states_t current_state;
        SDL_Color buttonBackGroundColor;
        SDL_Rect * buttonRect;
        SDL_Rect * iconRect;
        SDL_Texture * icon;
        SDL_Color buttonBorderColor;
        int borderWidth;
} button_t;

button_t * button_new(SDL_Texture * icon, int x, int y, int w, int h, SDL_Color buttonBackGroundColor,
                      SDL_Color buttonBorderColor, int borderWidth) {
    button_t * button = new (button_t);
    if (!button) {
        return NULL;
    }
    button->buttonRect = new (SDL_Rect);
    if (!button->buttonRect) {
        free(button);
        return NULL;
    }
    button->iconRect = new (SDL_Rect);
    if (!button->iconRect) {
        free(button->buttonRect);
        free(button);
        return NULL;
    }
    SDL_QueryTexture(icon, NULL, NULL, &button->iconRect->w, &button->iconRect->h);
    if (w == 0) {
        w = button->iconRect->w;
    } else {
        assert(w >= button->iconRect->w);
    }
    if (h == 0) {
        h = button->iconRect->h;
    } else {
        assert(h >= button->iconRect->h);
    }
    // Center the icon
    button->iconRect->x = x + (w - button->iconRect->w) / 2;
    button->iconRect->y = y + (h - button->iconRect->h) / 2;
    button->buttonRect->x = x;
    button->buttonRect->y = y;
    button->buttonRect->w = w;
    button->buttonRect->h = h;
    button->icon = icon;
    button->buttonBackGroundColor = buttonBackGroundColor;
    button->buttonBorderColor = buttonBorderColor;
    button->borderWidth = borderWidth;
    button->current_state = BUTTON_STATE_MOUSE_OUT;
    return button;
}

void button_destroy(button_t * button) {
    if (button->icon != NULL) {
        SDL_DestroyTexture(button->icon);
    }
    free(button->buttonRect);
    free(button);
}

void button_handle_event(button_t * button, SDL_Event * e) {
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
        // Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        // Check if mouse is in button
        bool inside = true;

        // Mouse is left of the button
        if (x < button->buttonRect->x) {
            inside = false;
        }
        // Mouse is right of the button
        else if (x > button->buttonRect->x + button->buttonRect->w) {
            inside = false;
        }
        // Mouse above the button
        else if (y < button->buttonRect->y) {
            inside = false;
        }
        // Mouse below the button
        else if (y > button->buttonRect->y + button->buttonRect->h) {
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
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
    SDL_SetRenderDrawColor(renderer, button->buttonBorderColor.r, button->buttonBorderColor.g,
                           button->buttonBorderColor.b, button->buttonBorderColor.a);
    SDL_Rect border = {button->buttonRect->x - button->borderWidth, button->buttonRect->y - button->borderWidth,
                       button->buttonRect->w + button->borderWidth * 2,
                       button->buttonRect->h + button->borderWidth * 2};
    SDL_RenderFillRect(renderer, &border);
    SDL_SetRenderDrawColor(renderer, button->buttonBackGroundColor.r, button->buttonBackGroundColor.g,
                           button->buttonBackGroundColor.b, button->buttonBackGroundColor.a);
    SDL_RenderFillRect(renderer, button->buttonRect);
    SDL_RenderCopy(renderer, button->icon, NULL, button->iconRect);
    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}