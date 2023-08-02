#include "color_picker.h"

#include <math.h>

#include <SDL_ttf.h>

#include "color.h"
#include "memory.h"

#define COLOR_BAR_WIDTH        30
#define COLOR_BAR_HEIGHT       500
#define COLOR_BAR_WIDTH_SPACE  10
#define COLOR_BAR_SPACE_BOTTOM 40
#define NUMBER_OF_SECTIONS     6
#define SEGMENTED              (COLOR_BAR_HEIGHT / NUMBER_OF_SECTIONS)
#define NUM                    (255.0 / ((float)NUMBER_OF_SECTIONS / (float)NUMBER_OF_SECTIONS))

typedef struct {
        uint32_t x;
        uint32_t y;
} mouse_position_t;

static void color_picker_render_vertical_color_selector(color_picker_t * color_picker);
static void color_picker_draw_horizontal_zone(color_picker_t * color_picker);
static void color_picker_position_color(color_picker_t * color_picker);
static void color_picker_color_square(color_picker_t * color_picker);
static void color_picker_color_square_cross_hairs(color_picker_t * color_picker);
static bool checkBounds(int xMin, int xMax, int yMin, int yMax, mouse_position_t mousePos);

typedef struct color_picker_t {
        SDL_Window * window;
        SDL_Renderer * renderer;
        mouse_position_t mouse_position;
        mouse_position_t bound_check_position;
        color_t * selected_color;
        // Position of mouse (Y-Axis) on vertical color bar
        int32_t position;
        bool left_mouse_button_down;
        bool left_mouse_button_down_square;
} color_picker_t;

/// @brief Create a new color picker
/// @return The newly created color picker
color_picker_t * color_picker_new() {
    color_picker_t * color_picker = new (color_picker_t);
    if (!color_picker) {
        return NULL;
    }
    color_picker->window =
        SDL_CreateWindow("Color Picker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                         COLOR_BAR_HEIGHT + COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE,
                         COLOR_BAR_HEIGHT + COLOR_BAR_SPACE_BOTTOM, SDL_WINDOW_HIDDEN | SDL_WINDOW_ALWAYS_ON_TOP);
    if (!color_picker->window) {
        color_picker_destroy(color_picker);
        return NULL;
    }
    color_picker->renderer = SDL_CreateRenderer(color_picker->window, -1, SDL_RENDERER_ACCELERATED);
    if (!color_picker->renderer) {
        color_picker_destroy(color_picker);
        SDL_DestroyWindow(color_picker->window);
        return NULL;
    }
    color_picker->selected_color = color_new(0, 255, 0, 255);
    if (!color_picker->selected_color) {
        color_picker_destroy(color_picker);
        SDL_DestroyWindow(color_picker->window);
        SDL_DestroyRenderer(color_picker->renderer);
        return NULL;
    }
    color_picker->mouse_position.x = 0;
    color_picker->mouse_position.y = 0;
    color_picker->position = 0;
    return color_picker;
}

void color_picker_destroy(color_picker_t * color_picker) {
    SDL_DestroyRenderer(color_picker->renderer);
    SDL_DestroyWindow(color_picker->window);
    free(color_picker);
}

void color_picker_show(color_picker_t * color_picker) {
    SDL_ShowWindow(color_picker->window);
}

void color_picker_hide(color_picker_t * color_picker) {
    SDL_HideWindow(color_picker->window);
}

bool color_picker_is_shown(color_picker_t * color_picker) {
    return SDL_GetWindowFlags(color_picker->window) & SDL_WINDOW_SHOWN;
}

void color_picker_render(color_picker_t * color_picker) {
    SDL_RenderClear(color_picker->renderer);
    color_picker_render_vertical_color_selector(color_picker);
    color_picker->position = color_picker->position > COLOR_BAR_HEIGHT ? COLOR_BAR_HEIGHT : color_picker->position;
    color_picker_draw_horizontal_zone(color_picker);
    color_picker_position_color(color_picker);
    color_picker_color_square(color_picker);
    color_picker_color_square_cross_hairs(color_picker);
    SDL_SetRenderDrawColor(color_picker->renderer, 100, 100, 100, 255);
    SDL_RenderPresent(color_picker->renderer);
}

void color_picker_handle_event(color_picker_t * color_picker, SDL_Event event) {
    switch (event.type) {
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT) {
            color_picker->left_mouse_button_down = false;
            color_picker->left_mouse_button_down_square = false;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
            // Used for bounds checking (within which section)
            color_picker->bound_check_position.x = event.motion.x;
            color_picker->bound_check_position.y = event.motion.y;

            if (checkBounds(0, COLOR_BAR_WIDTH, 0, COLOR_BAR_HEIGHT, color_picker->bound_check_position)) {
                color_picker->left_mouse_button_down = true;
            }
            if (checkBounds(COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE,
                            COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE + COLOR_BAR_HEIGHT, 0, COLOR_BAR_HEIGHT,
                            color_picker->bound_check_position)) {
                color_picker->left_mouse_button_down_square = true;
            }
            if (checkBounds(COLOR_BAR_HEIGHT - (COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE),
                            COLOR_BAR_HEIGHT + COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE,
                            COLOR_BAR_HEIGHT + COLOR_BAR_WIDTH_SPACE,
                            COLOR_BAR_HEIGHT + COLOR_BAR_WIDTH_SPACE + COLOR_BAR_WIDTH,
                            color_picker->bound_check_position)) {
                color_picker_hide(color_picker);
            }
        }
        break;
    case SDL_MOUSEMOTION:
        if (color_picker->left_mouse_button_down) {
            color_picker->position = event.motion.y;
        }
        if (color_picker->left_mouse_button_down_square) {
            color_picker->mouse_position.x = event.motion.x;
            color_picker->mouse_position.y = event.motion.y;
        }
        break;
    default:
        break;
    }
}

uint32_t color_picker_get_window_id(color_picker_t * color_picker) {
    return SDL_GetWindowID(color_picker->window);
}

static void color_picker_render_vertical_color_selector(color_picker_t * color_picker) {
    float red = 255.0;
    float green = 0.0;
    float blue = 0.0;
    for (int i = 0; i < COLOR_BAR_HEIGHT; i++) {
        SDL_SetRenderDrawColor(color_picker->renderer, (int)red, (int)green, (int)blue, 255);

        SDL_Rect r;
        r.x = 0;
        r.y = i;
        r.w = COLOR_BAR_WIDTH;
        r.h = 1;

        blue = i <= SEGMENTED ? blue + NUM : blue;
        red = (i >= SEGMENTED && i < 2 * SEGMENTED) ? red - NUM : red;
        green = i >= 2 * SEGMENTED && i < 3 * SEGMENTED ? green + NUM : green;
        blue = i >= 3 * SEGMENTED && i < 4 * SEGMENTED ? blue - NUM : blue;
        red = i >= 4 * SEGMENTED && i < 5 * SEGMENTED ? red + NUM : red;
        green = i >= 5 * SEGMENTED ? green - NUM : green;

        red = red > 255.0 ? 255 : round(red);
        green = green > 255.0 ? 255 : round(green);
        blue = blue > 255.0 ? 255 : round(blue);

        red = red < 0 ? 0 : round(red);
        green = green < 0 ? 0 : round(green);
        blue = blue < 0 ? 0 : round(blue);

        SDL_RenderFillRect(color_picker->renderer, &r);
    }
}

static void color_picker_draw_horizontal_zone(color_picker_t * color_picker) {
    SDL_SetRenderDrawColor(color_picker->renderer, 255, 255, 255, 255);

    SDL_Rect r1;
    r1.x = 0;
    r1.y = color_picker->position - 1;
    r1.w = 30;
    r1.h = 3;

    SDL_RenderFillRect(color_picker->renderer, &r1);

    SDL_SetRenderDrawColor(color_picker->renderer, 0, 0, 0, 255);

    SDL_Rect r2;
    r2.x = 0;
    r2.y = color_picker->position;
    r2.w = 30;
    r2.h = 1;

    SDL_RenderFillRect(color_picker->renderer, &r2);
}

static void color_picker_position_color(color_picker_t * color_picker) {
    float tmpRed = 255;
    float tmpGreen = 0;
    float tmpBlue = 0;

    for (int i = 0; i < color_picker->position; i++) {
        tmpBlue = i <= SEGMENTED ? tmpBlue + NUM : tmpBlue;
        tmpRed = (i >= SEGMENTED && i < 2 * SEGMENTED) ? tmpRed - NUM : tmpRed;
        tmpGreen = i >= 2 * SEGMENTED && i < 3 * SEGMENTED ? tmpGreen + NUM : tmpGreen;
        tmpBlue = i >= 3 * SEGMENTED && i < 4 * SEGMENTED ? tmpBlue - NUM : tmpBlue;
        tmpRed = i >= 4 * SEGMENTED && i < 5 * SEGMENTED ? tmpRed + NUM : tmpRed;
        tmpGreen = i >= 5 * SEGMENTED ? tmpGreen - NUM : tmpGreen;

        tmpRed = tmpRed > 255.0 ? 255 : round(tmpRed);
        tmpGreen = tmpGreen > 255.0 ? 255 : round(tmpGreen);
        tmpBlue = tmpBlue > 255.0 ? 255 : round(tmpBlue);

        tmpRed = tmpRed < 0 ? 0 : round(tmpRed);
        tmpGreen = tmpGreen < 0 ? 0 : round(tmpGreen);
        tmpBlue = tmpBlue < 0 ? 0 : round(tmpBlue);
    }
    color_set_red(color_picker->selected_color, tmpRed);
    color_set_green(color_picker->selected_color, tmpGreen);
    color_set_blue(color_picker->selected_color, tmpBlue);
}

static void color_picker_color_square(color_picker_t * color_picker) {
    int topLeft = COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE;

    float xDifferenceRed = 255.0 - color_get_red(color_picker->selected_color);
    float xDifferenceGreen = 255.0 - color_get_green(color_picker->selected_color);
    ;
    float xDifferenceBlue = 255.0 - color_get_blue(color_picker->selected_color);

    float xRedDelta = xDifferenceRed / COLOR_BAR_HEIGHT;
    float xGreenDelta = xDifferenceGreen / COLOR_BAR_HEIGHT;
    float xBlueDelta = xDifferenceBlue / COLOR_BAR_HEIGHT;

    xRedDelta = xDifferenceRed == 0 ? 0 : xRedDelta;
    xGreenDelta = xDifferenceGreen == 0 ? 0 : xGreenDelta;
    xBlueDelta = xDifferenceBlue == 0 ? 0 : xBlueDelta;

    float yDifferenceRed = 255.0;
    float yDifferenceGreen = 255.0;
    float yDifferenceBlue = 255.0;

    float yRedDelta = yDifferenceRed / COLOR_BAR_HEIGHT;
    float yGreenDelta = yDifferenceGreen / COLOR_BAR_HEIGHT;
    float yBlueDelta = yDifferenceBlue / COLOR_BAR_HEIGHT;

    float currentRed = 255.0;
    float currentGreen = 255.0;
    float currentBlue = 255.0;

    for (int y = 0; y < COLOR_BAR_HEIGHT; y++) {
        for (int x = 0; x < COLOR_BAR_HEIGHT; x++) {
            SDL_SetRenderDrawColor(color_picker->renderer, currentRed, currentGreen, currentBlue, 255);
            SDL_RenderDrawPoint(color_picker->renderer, topLeft + x, y);

            currentRed -= xRedDelta;
            currentGreen -= xGreenDelta;
            currentBlue -= xBlueDelta;

            currentRed = currentRed > 255.0 ? 255 : currentRed;
            currentGreen = currentGreen > 255.0 ? 255 : currentGreen;
            currentBlue = currentBlue > 255.0 ? 255 : currentBlue;

            currentRed = currentRed < 0 ? 0 : currentRed;
            currentGreen = currentGreen < 0 ? 0 : currentGreen;
            currentBlue = currentBlue < 0 ? 0 : currentBlue;
        }
        currentRed = 255 - (yRedDelta * (y + 1));
        currentGreen = 255 - (yGreenDelta * (y + 1));
        currentBlue = 255 - (yBlueDelta * (y + 1));
    }
}

static void color_picker_color_square_cross_hairs(color_picker_t * color_picker) {
    // Vertical white outline
    SDL_SetRenderDrawColor(color_picker->renderer, 255, 255, 255, 255);

    SDL_Rect rwv;
    rwv.x = color_picker->mouse_position.x - 1;
    if (color_picker->mouse_position.x < COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE) {
        rwv.x = COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE;
    }
    rwv.y = 0;
    rwv.w = 3;
    rwv.h = COLOR_BAR_HEIGHT;

    SDL_RenderFillRect(color_picker->renderer, &rwv);

    // Horizontal white outline
    SDL_SetRenderDrawColor(color_picker->renderer, 255, 255, 255, 255);

    SDL_Rect rwh;
    rwh.x = COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE;
    rwh.y = color_picker->mouse_position.y - 1;

    if (color_picker->mouse_position.y > COLOR_BAR_HEIGHT) {
        rwh.y = COLOR_BAR_HEIGHT - 2;
    }

    rwh.w = COLOR_BAR_HEIGHT;
    rwh.h = 3;

    SDL_RenderFillRect(color_picker->renderer, &rwh);

    // Vertical black outline
    SDL_SetRenderDrawColor(color_picker->renderer, 0, 0, 0, 255);

    SDL_Rect rbv;
    rbv.x = color_picker->mouse_position.x;

    if (color_picker->mouse_position.x < COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE) {
        rbv.x = COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE;
    }

    rbv.y = 0;
    rbv.w = 1;
    rbv.h = COLOR_BAR_HEIGHT;

    SDL_RenderFillRect(color_picker->renderer, &rbv);

    // Horizontal black outline
    SDL_SetRenderDrawColor(color_picker->renderer, 0, 0, 0, 255);

    SDL_Rect rbh;
    rbh.x = COLOR_BAR_WIDTH + COLOR_BAR_WIDTH_SPACE;
    rbh.y = color_picker->mouse_position.y;

    if (color_picker->mouse_position.y > COLOR_BAR_HEIGHT) {
        rbh.y = COLOR_BAR_HEIGHT - 1;
    }

    rbh.w = COLOR_BAR_HEIGHT;
    rbh.h = 1;

    SDL_RenderFillRect(color_picker->renderer, &rbh);
}

// Checks if mouse position is within bounds of specific zone
static bool checkBounds(int xMin, int xMax, int yMin, int yMax, mouse_position_t mousePos) {
    return (mousePos.x > xMin && mousePos.x < xMax && mousePos.y > yMin && mousePos.y < yMax);
}