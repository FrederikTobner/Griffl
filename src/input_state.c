#include "input_state.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct input_state_t {
        uint8_t state;
} input_state_t;

input_state_t * input_state_new() {
    input_state_t * input_state = malloc(sizeof(input_state_t));
    if (input_state == NULL) {
        return NULL;
    }
    input_state->state = 0;
    return input_state;
}

void input_state_destroy(input_state_t * input_state) {
    free(input_state);
}

bool input_state_is_down(input_state_t * input_state, int key) {
    if (input_state == NULL) {
        return false;
    }
    return input_state->state & key;
}

void input_state_handle_down(input_state_t * input_state, int key) {
    if (input_state == NULL) {
        return;
    }
    input_state->state |= key;
}

void input_state_handle_up(input_state_t * input_state, int key) {
    if (input_state == NULL) {
        return;
    }
    input_state->state &= ~key;
}