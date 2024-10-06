#include <chrono>
#include <iostream>
#include <thread>

#include "console_output.h"
#include "meta.h"
#include "ansi_codes.cpp"

static bool flip = true;



static void drawBorder() {
    // draw the game field border
    for (int y = 0; y < FIELD_DIM_Y; y++) {
        for (int x = 0; x < FIELD_DIM_X; x++) {
            if (y == 0 || x == 0 || x == FIELD_DIM_X - 1 || y == FIELD_DIM_Y - 1) {
                setColor(AnsiStyle::BOLD, AnsiForegroundColor::MAGENTA, AnsiBackgroundColor::MAGENTA);
                std::cout << (flip ? "#" : "+");
                setColor(AnsiStyle::RESET, AnsiForegroundColor::WHITE, AnsiBackgroundColor::BLACK);
            } else {
                std::cout << " ";
            }
            if (x == FIELD_DIM_X - 1) {
                std::cout << std::endl;
            }
        }
    }

    // flip border character to visualize redraw events
    flip = !flip;
}

static void drawCursor(Meta *meta) {
    moveCursor(meta->getCursorX(), meta->getCursorY());
    std::cout << "X" << std::flush;
}

void drawField(Meta *meta) {
    hideCursor();
    clearConsole();

    drawBorder();
        
    drawCursor(meta);
}

void resetConsole() {
    clearConsole();
    showCursor();
}
