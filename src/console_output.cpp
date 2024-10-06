#include <chrono>
#include <iostream>
#include <thread>

#include "ansi_codes.cpp"
#include "console_output.h"
#include "meta.h"

static bool flip = true;

static void drawBorder() {
    // draw the game field border
    setStyle(AnsiStyle::BOLD);
    setColor(AnsiRgbColorMode::BACKGROUND, 80, 80, 80);
    setColor(AnsiRgbColorMode::FOREGROUND, 0, 0, 0);
    for (int y = 0; y < FIELD_DIM_Y; y++) {
        for (int x = 0; x < FIELD_DIM_X; x++) {
            if (y == 0 || x == 0 || x == FIELD_DIM_X - 1 || y == FIELD_DIM_Y - 1) {
                std::cout << (flip ? "#" : "+");
            } else {
                std::cout << " ";
            }
            if (x == FIELD_DIM_X - 1) {
                std::cout << std::endl;
            }
        }
    }
    setStyle(AnsiStyle::RESET);

    // flip border character to visualize redraw events
    flip = !flip;
}

static void drawCursor(Meta *meta) {
    moveCursor(meta->getCursorX(), meta->getCursorY());
    std::cout << "X" << std::flush;
}

static void drawGameField(Meta *meta) {
    for (int y = 0; y < FIELD_DIM_Y - 2; y++) {
        moveCursor(1, y + 1);
        for (int x = 0; x < FIELD_DIM_X - 2; x++) {
            std::cout << meta->getGameFieldCharAt(x, y);
        }
    }
}

void drawField(Meta *meta) {
    hideCursor();
    clearConsole();

    drawBorder();
    drawGameField(meta);

    drawCursor(meta);
}

void resetConsole() {
    clearConsole();
    showCursor();
}
