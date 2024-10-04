#include "meta.h"
#include "console_output.h"

Meta::Meta() {}

void Meta::move_cursor_up() {
    if (cursor_y > 1) {
        cursor_y--;
    }
}

void Meta::move_cursor_down() {
    if (cursor_y < FIELD_DIM_Y - 1) {
        cursor_y++;
    }
}

void Meta::move_cursor_left() {
    if (cursor_x > 1) {
        cursor_x--;
    }
}

void Meta::move_cursor_right() {
    if (cursor_x < FIELD_DIM_X - 1) {
        cursor_x++;
    }
}
