#include "griffl_application.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "fonts.h"
#include "memory.h"
#include "toolbar.h"

#define TOOLBAR_HEIGHT 60

typedef struct griffl_application_t {
        draw_mode_t draw_mode;
        color_t * selected_color;
        input_state_t * input_state;
        canvas_t * canvas;
        uint8_t brush_size;
        bool quit;
        uint16_t window_width;
        uint16_t window_height;
        uint8_t zoom_factor;
        SDL_Texture * canvas_texture;
        SDL_Renderer * renderer;
        SDL_Window * window;
        toolbar_t * toolbar;
        fonts_t * fonts;
} griffl_application_t;

static void griffl_application_handle_events(griffl_application_t * griffl_application);

static int griffl_application_update_screen(griffl_application_t * griffl_application);

griffl_application_t * griffl_application_new(uint16_t window_width, uint16_t window_height, uint8_t zoom_factor,
                                              uint32_t initial_color, uint8_t brush_size) {
    griffl_application_t * griffl_application = new (griffl_application_t);
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
    griffl_application->canvas = canvas_new(window_width / zoom_factor, (window_height - TOOLBAR_HEIGHT) / zoom_factor);
    if (griffl_application->canvas == NULL) {
        input_state_destroy(griffl_application->input_state);
        free(griffl_application->selected_color);
        free(griffl_application);
        return NULL;
    }
    griffl_application->window = SDL_CreateWindow("Griffl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                                  window_width, window_height, SDL_WINDOW_RESIZABLE);
    if (!griffl_application->window) {
        fprintf(stderr, "SDL could not create window! SDL_Error: %s\n", SDL_GetError());
        canvas_destroy(griffl_application->canvas);
        input_state_destroy(griffl_application->input_state);
        free(griffl_application->selected_color);
        free(griffl_application);
        return NULL;
    }

    griffl_application->renderer = SDL_CreateRenderer(griffl_application->window, -1, 0);
    if (!griffl_application->renderer) {
        fprintf(stderr, "SDL could not create renderer! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(griffl_application->window);
        canvas_destroy(griffl_application->canvas);
        input_state_destroy(griffl_application->input_state);
        free(griffl_application->selected_color);
        free(griffl_application);
        return NULL;
    }
    griffl_application->canvas_texture =
        SDL_CreateTexture(griffl_application->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                          canvas_get_width(griffl_application->canvas), canvas_get_height(griffl_application->canvas));
    if (!griffl_application->canvas_texture) {
        fprintf(stderr, "SDL could not create texture! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(griffl_application->renderer);
        SDL_DestroyWindow(griffl_application->window);
        canvas_destroy(griffl_application->canvas);
        input_state_destroy(griffl_application->input_state);
        free(griffl_application->selected_color);
        free(griffl_application);
        return NULL;
    }
    griffl_application->fonts = fonts_create();
    if (!griffl_application->fonts) {
        fprintf(stderr, "Could not create fonts!\n");
        SDL_DestroyTexture(griffl_application->canvas_texture);
        SDL_DestroyRenderer(griffl_application->renderer);
        SDL_DestroyWindow(griffl_application->window);
        canvas_destroy(griffl_application->canvas);
        input_state_destroy(griffl_application->input_state);
        free(griffl_application->selected_color);
        free(griffl_application);
        return NULL;
    }
    griffl_application->toolbar = toolbar_create(griffl_application->renderer, griffl_application->fonts);
    if (!griffl_application->toolbar) {
        fprintf(stderr, "Could not create toolbar!\n");
        fonts_destroy(griffl_application->fonts);
        SDL_DestroyTexture(griffl_application->canvas_texture);
        SDL_DestroyRenderer(griffl_application->renderer);
        SDL_DestroyWindow(griffl_application->window);
        canvas_destroy(griffl_application->canvas);
        input_state_destroy(griffl_application->input_state);
        free(griffl_application->selected_color);
        free(griffl_application);
        return NULL;
    }
    griffl_application->draw_mode = DRAW_MODE_FREEHAND;
    griffl_application->brush_size = brush_size;
    griffl_application->quit = false;
    griffl_application->window_width = window_width;
    griffl_application->window_height = window_height;
    griffl_application->zoom_factor = zoom_factor;
    // Setting the draw color to a light gray
    SDL_SetRenderDrawColor(griffl_application->renderer, 190, 190, 190, 255);
    return griffl_application;
}

void griffl_application_destroy(griffl_application_t * griffl_application) {
    SDL_DestroyTexture(griffl_application->canvas_texture);
    SDL_DestroyRenderer(griffl_application->renderer);
    SDL_DestroyWindow(griffl_application->window);
    fonts_destroy(griffl_application->fonts);
    canvas_destroy(griffl_application->canvas);
    input_state_destroy(griffl_application->input_state);
    free(griffl_application->selected_color);
    free(griffl_application);
}

int griffl_application_run(griffl_application_t * griffl_application) {
    // Rendering the first frame
    if (griffl_application_update_screen(griffl_application)) {
        return -1;
    }
    while (!griffl_application->quit) {
        griffl_application_handle_events(griffl_application);
        if (griffl_application_update_screen(griffl_application)) {
            return -1;
        }
    }
    return 0;
}

static void griffl_application_handle_events(griffl_application_t * griffl_application) {
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
                    mouseX > 0 && mouseY > TOOLBAR_HEIGHT) {
                    canvas_draw_free_hand(griffl_application->canvas, mouseX / griffl_application->zoom_factor,
                                          (mouseY - TOOLBAR_HEIGHT) / griffl_application->zoom_factor,
                                          color_as_argb(griffl_application->selected_color),
                                          griffl_application->brush_size);
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

static int griffl_application_update_screen(griffl_application_t * griffl_application) {
    if (canvas_update_texture(griffl_application->canvas, griffl_application->canvas_texture)) {
        printf("Could not update texture!\n");
        return -1;
    }
    if (SDL_RenderClear(griffl_application->renderer)) {
        printf("SDL could not clear renderer! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    toolbar_render(griffl_application->toolbar, griffl_application->renderer);
    SDL_Rect canvasSrcRect = {0, 0, canvas_get_width(griffl_application->canvas),
                              canvas_get_height(griffl_application->canvas)};
    SDL_Rect canvasDstRect = {0, TOOLBAR_HEIGHT, griffl_application->window_width,
                              griffl_application->window_height - TOOLBAR_HEIGHT};
    if (SDL_RenderCopy(griffl_application->renderer, griffl_application->canvas_texture, &canvasSrcRect,
                       &canvasDstRect)) {
        printf("SDL could not copy texture to renderer! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_RenderPresent(griffl_application->renderer);
    return 0;
}