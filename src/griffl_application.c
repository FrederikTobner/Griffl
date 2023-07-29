#include "griffl_application.h"

#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct griffl_application_t {
        draw_mode_t draw_mode;
        color_t * selected_color;
        input_state_t * input_state;
        canvas_t * canvas;
        uint8_t brush_size;
        bool quit;
        uint16_t window_width;
        uint16_t window_height;
        uint8_t toolbar_height;
        uint8_t zoom_factor;
} griffl_application_t;

griffl_application_t * griffl_application_new(uint16_t window_width, uint16_t window_height, uint8_t toolbar_height,
                                              uint8_t zoom_factor, uint32_t initial_color) {
    griffl_application_t * griffl_application = malloc(sizeof(griffl_application_t));
    if (griffl_application == NULL) {
        return NULL;
    }
    griffl_application->input_state = input_state_new();
    if (griffl_application->input_state == NULL) {
        free(griffl_application);
        return NULL;
    }
    griffl_application->selected_color = color_new(initial_color >> 24 & 0xFF, initial_color >> 16 & 0xFF,
                                                   initial_color >> 8 & 0xFF, initial_color & 0xFF);
    if (griffl_application->selected_color == NULL) {
        input_state_destroy(griffl_application->input_state);
        free(griffl_application);
        return NULL;
    }
    griffl_application->canvas = canvas_new(window_width / zoom_factor, (window_height - toolbar_height) / zoom_factor);
    if (griffl_application->canvas == NULL) {
        input_state_destroy(griffl_application->input_state);
        free(griffl_application->selected_color);
        free(griffl_application);
        return NULL;
    }
    griffl_application->draw_mode = DRAW_MODE_FREEHAND;
    griffl_application->brush_size = 1;
    griffl_application->quit = false;
    griffl_application->window_width = window_width;
    griffl_application->window_height = window_height;
    griffl_application->toolbar_height = toolbar_height;
    griffl_application->zoom_factor = zoom_factor;
    return griffl_application;
}

void griffl_application_destroy(griffl_application_t * griffl_application) {
    input_state_destroy(griffl_application->input_state);
    free(griffl_application->selected_color);
    free(griffl_application);
}

void griffl_application_handle_events(griffl_application_t * griffl_application) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            griffl_application->quit = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                input_state_handle_up(griffl_application->input_state, LEFT_MOUSE_BUTTON);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                input_state_handle_down(griffl_application->input_state, LEFT_MOUSE_BUTTON);
            }
        case SDL_MOUSEMOTION:
            if (input_state_is_down(griffl_application->input_state, LEFT_MOUSE_BUTTON)) {
                int32_t mouseX = event.motion.x;
                int32_t mouseY = event.motion.y;
                // Make sure the cursor is within the window
                if (mouseX < griffl_application->window_width && mouseY < griffl_application->window_height &&
                    mouseX > 0 && mouseY > griffl_application->toolbar_height) {
                    canvas_draw_free(griffl_application->canvas, mouseX / griffl_application->zoom_factor,
                                     (mouseY - griffl_application->toolbar_height) / griffl_application->zoom_factor,
                                     color_as_argb(griffl_application->selected_color), griffl_application->brush_size);
                }
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_RCTRL) {
                input_state_handle_down(griffl_application->input_state, RIGHT_CTRL);
            } else if (event.key.keysym.sym == SDLK_LCTRL) {
                input_state_handle_down(griffl_application->input_state, LEFT_CTRL);
            } else if (event.key.keysym.sym == SDLK_s &&
                       (input_state_is_down(griffl_application->input_state, RIGHT_CTRL) ||
                        input_state_is_down(griffl_application->input_state, LEFT_CTRL))) {
                canvas_save_as_png("./canvas.png", griffl_application->canvas);
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_RCTRL) {
                input_state_handle_up(griffl_application->input_state, RIGHT_CTRL);
            } else if (event.key.keysym.sym == SDLK_LCTRL) {
                input_state_handle_up(griffl_application->input_state, LEFT_CTRL);
            }
            break;
        default:
            break;
        }
    }
}

int griffl_application_update_screen(griffl_application_t * griffl_application, SDL_Renderer * renderer,
                                     SDL_Texture * canvasTexture) {
    if (canvas_update_texture(griffl_application->canvas, canvasTexture)) {
        printf("Could not update texture!\n");
        return EXIT_FAILURE;
    }
    if (SDL_RenderClear(renderer)) {
        printf("SDL could not clear renderer! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Rect canvasSrcRect = {0, 0, canvas_get_width(griffl_application->canvas),
                              canvas_get_height(griffl_application->canvas)};
    SDL_Rect canvasDstRect = {0, griffl_application->toolbar_height, griffl_application->window_width,
                              griffl_application->window_height - griffl_application->toolbar_height};
    if (SDL_RenderCopy(renderer, canvasTexture, &canvasSrcRect, &canvasDstRect)) {
        printf("SDL could not copy texture to renderer! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_RenderPresent(renderer);
    return 0;
}

void griffl_application_set_brush_size(griffl_application_t * griffl_application, uint8_t brush_size) {
    griffl_application->brush_size = brush_size;
}

canvas_t * griffl_application_get_canvas(griffl_application_t * griffl_application) {
    return griffl_application->canvas;
}

input_state_t * griffl_application_get_input_state(griffl_application_t * griffl_application) {
    return griffl_application->input_state;
}

bool griffl_application_is_quit(griffl_application_t * griffl_application) {
    return griffl_application->quit;
}