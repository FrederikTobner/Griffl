#include "input_state.h"

#include <stdlib.h>

#include "memory.h"

input_state_t * input_state_new() {
    input_state_t * input_state = new (input_state_t);
    if (input_state == NULL) {
        return NULL;
    }
    *input_state = 0;
    return input_state;
}

void input_state_destroy(input_state_t * input_state) {
    free(input_state);
}

bool input_state_is_down(input_state_t * input_state, int key) {
    return *input_state & key;
}

void input_state_handle_down(input_state_t * input_state, int key) {
    *input_state |= key;
}

void input_state_handle_up(input_state_t * input_state, int key) {
    *input_state &= ~key;
}