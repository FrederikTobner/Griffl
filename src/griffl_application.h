#ifndef APPLICATION_STATE_H_
#define APPLICATION_STATE_H_

#include "canvas.h"
#include "color.h"
#include "draw_mode.h"
#include "input_state.h"

typedef struct griffl_application_t griffl_application_t;

griffl_application_t * griffl_application_new(uint16_t window_width, uint16_t window_height, uint8_t zoom_factor,
                                              uint32_t initial_color, uint8_t brush_size);

void griffl_application_destroy(griffl_application_t * griffl_application);

int griffl_application_run(griffl_application_t * griffl_application);

#endif