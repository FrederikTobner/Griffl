#ifndef APPLICATION_STATE_H_
#define APPLICATION_STATE_H_

#include "canvas.h"
#include "color.h"
#include "draw_mode.h"
#include "input_state.h"

typedef struct griffl_application_t griffl_application_t;

griffl_application_t * griffl_application_new(uint16_t window_width, uint16_t window_height, uint8_t toolbar_height,
                                              uint8_t zoom_factor, uint32_t initial_color);

void griffl_application_destroy(griffl_application_t * griffl_application);

void griffl_application_handle_events(griffl_application_t * griffl_application);

int griffl_application_update_screen(griffl_application_t * griffl_application, SDL_Renderer * renderer,
                                     SDL_Texture * canvasTexture);

void griffl_application_set_brush_size(griffl_application_t * griffl_application, uint8_t brush_size);

canvas_t * griffl_application_get_canvas(griffl_application_t * griffl_application);

input_state_t * griffl_application_get_input_state(griffl_application_t * griffl_application);

bool griffl_application_is_quit(griffl_application_t * griffl_application);

#endif