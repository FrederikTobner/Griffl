#ifndef BUTTON_H_
#define BUTTON_H_

#include "SDL.h"

#include "texture.h"

typedef enum {
    BUTTON_STATE_MOUSE_OUT,
    BUTTON_STATE_MOUSE_OVER_MOTION,
    BUTTON_STATE_MOUSE_DOWN,
    BUTTON_STATE_MOUSE_UP,
    BUTTON_STATE_TOTAL_COUNT
} button_states_t;

typedef struct button_t button_t;

button_t * button_new(SDL_Texture * icon, int x, int y, int w, int h, SDL_Color buttonBackGroundColor,
                      SDL_Color buttonBorderColor, int borderWidth);

void button_destroy(button_t * button);

void button_handle_event(button_t * button, SDL_Event * e);

void button_render(button_t * button, SDL_Renderer * renderer);

#endif // BUTTON_H_