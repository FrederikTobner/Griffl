#ifndef INPUT_STATE_H_
#define INPUT_STATE_H_

#include <stdbool.h>
#include <stdint.h>

#define LEFT_MOUSE_BUTTON 0x01
#define RIGHT_CTRL        0x02
#define LEFT_CTRL         0x04

/// @brief Models the state of the input (the keyboard and mouse)
typedef uint8_t input_state_t;

/// @brief  Creates a new input state
/// @return The new input state or NULL if it could not be created
input_state_t * input_state_new();

void input_state_destroy(input_state_t * input_state);

bool input_state_is_down(input_state_t * input_state, int key);

void input_state_handle_down(input_state_t * input_state, int key);

void input_state_handle_up(input_state_t * input_state, int key);

#endif