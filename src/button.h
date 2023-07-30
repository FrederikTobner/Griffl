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

typedef struct {
        SDL_Point * position;
        button_states_t current_state;
        int width;
        int height;
        SDL_Rect * clip;
        // All the states of the rectangle share the same texture -> the image that is shown can be set by setting clip
        texture_t * texture;
} button_t;

button_t * button_new();

void button_set_position(button_t * button, int x, int y);

void button_set_size(button_t * button, int width, int height);

void button_handle_event(button_t * button, SDL_Event * e);

void button_render(button_t * button, SDL_Renderer * renderer);

void button_set_clip(button_t * button, SDL_Rect * clip);

void button_set_texture(button_t * button, texture_t * texture);

#endif // BUTTON_H_