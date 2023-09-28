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
        SDL_Color buttonBorderColorOutside;
        SDL_Color buttonBorderColorMouseOver;
        int borderWidth;
} button_t;

button_t * button_new(SDL_Texture * icon, int x, int y, int width, int height, SDL_Color buttonBackGroundColor,
                      SDL_Color buttonBorderColorMouseOver, SDL_Color buttonBorderColor, int borderWidth) {
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
    if (width == 0) {
        width = button->iconRect->w;
    } else {
        assert(width >= button->iconRect->w);
    }
    if (height == 0) {
        height = button->iconRect->h;
    } else {
        assert(height >= button->iconRect->h);
    }
    // Center the icon
    button->buttonRect->x = x;
    button->buttonRect->y = y;
    button->buttonRect->w = width;
    button->buttonRect->h = height;
    button->iconRect->x = x + (width - button->iconRect->w) / 2;
    button->iconRect->y = y + (height - button->iconRect->h) / 2;
    button->icon = icon;
    button->buttonBackGroundColor = buttonBackGroundColor;
    button->buttonBorderColor = buttonBorderColor;
    button->buttonBorderColorOutside = buttonBorderColor;
    button->buttonBorderColorMouseOver = buttonBorderColorMouseOver;
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

bool button_handle_event(button_t * button, SDL_Event * e) {
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
            button->buttonBorderColor = button->buttonBorderColorOutside;
        }
        // Mouse is inside button
        else {
            // Set mouse over sprite
            switch (e->type) {
            case SDL_MOUSEMOTION:
                if(button->current_state != BUTTON_STATE_MOUSE_DOWN) {
                button->current_state = BUTTON_STATE_MOUSE_OVER_MOTION;
                }
                button->buttonBorderColor = button->buttonBorderColorMouseOver;
                break;

            case SDL_MOUSEBUTTONDOWN:
                button->current_state = BUTTON_STATE_MOUSE_DOWN;
                button->buttonBorderColor = button->buttonBorderColorOutside;
                break;
            case SDL_MOUSEBUTTONUP:
                {
                    if (button->current_state == BUTTON_STATE_MOUSE_DOWN) {
                        button->current_state = BUTTON_STATE_MOUSE_UP;
                        return true;
                    }
                    break;
                }
            default:
                break;
            }
        }
    }
    switch(button->current_state) {
            case BUTTON_STATE_MOUSE_OUT:
                SDL_Log("BUTTON_STATE_MOUSE_OUT\n");
                break;
            case BUTTON_STATE_MOUSE_OVER_MOTION:
                SDL_Log("BUTTON_STATE_MOUSE_OVER_MOTION\n");
                break;
            case BUTTON_STATE_MOUSE_DOWN:
                SDL_Log("BUTTON_STATE_MOUSE_DOWN\n");
                break;
            case BUTTON_STATE_MOUSE_UP:
                SDL_Log("BUTTON_STATE_MOUSE_UP\n");
                break;
        }
    return false;
}

void button_render(button_t * button, SDL_Renderer * renderer) {
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
    SDL_SetRenderDrawColor(renderer, button->buttonBorderColor.r, button->buttonBorderColor.g,
                           button->buttonBorderColor.b, button->buttonBorderColor.a);
    SDL_Rect border = {button->buttonRect->x, button->buttonRect->y, button->buttonRect->w, button->buttonRect->h};
    SDL_RenderFillRect(renderer, &border);
    SDL_Rect buttonBackground = {
        button->buttonRect->x + button->borderWidth, button->buttonRect->y + button->borderWidth,
        button->buttonRect->w - button->borderWidth * 2, button->buttonRect->h - button->borderWidth * 2};
    SDL_SetRenderDrawColor(renderer, button->buttonBackGroundColor.r, button->buttonBackGroundColor.g,
                           button->buttonBackGroundColor.b, button->buttonBackGroundColor.a);
    SDL_RenderFillRect(renderer, &buttonBackground);
    SDL_RenderCopy(renderer, button->icon, NULL, button->iconRect);
    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}
