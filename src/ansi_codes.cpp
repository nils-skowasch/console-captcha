#include "ansi_codes.h"
#include <iostream>

void moveCursor(int x, int y) {
    std::cout << ANSI_ESCAPE << (y + 1) << ";" << (x + 1) << "H";
}

void hideCursor() {
    std::cout << ANSI_ESCAPE << "?25l]"; // ?25l = hide cursor
}

void showCursor() {
    std::cout << ANSI_ESCAPE << "?25h"; // ?25h = show cursor
}

void clearConsole() {
    std::cout << ANSI_ESCAPE << "2J"; // 2J = clear screen;
    moveCursor(0, 0);
}

void setColor(AnsiStyle style, AnsiForegroundColor fgColor, AnsiBackgroundColor bgColor) {
    std::cout << ANSI_ESCAPE << static_cast<int>(style) << ";" << static_cast<int>(fgColor) << ";"
              << static_cast<int>(bgColor) << "m";
}
