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

void clearLine(){
    std::cout << ANSI_ESCAPE << "2K"; // 2K = clear the current line
}

void setStyle(AnsiStyle style){
    std::cout << ANSI_ESCAPE << static_cast<int>(style) << "m";
}

void setColor(AnsiStyle style, AnsiForegroundColor fgColor, AnsiBackgroundColor bgColor) {
    std::cout << ANSI_ESCAPE << static_cast<int>(style) << ";" << static_cast<int>(fgColor) << ";"
              << static_cast<int>(bgColor) << "m";
}

void setColor(AnsiRgbColorMode mode, int red, int green, int blue) {
    std::cout << ANSI_ESCAPE << static_cast<int>(mode) << ";2;" << red << ";" << green << ";" << blue << "m"; // ';2;' = 24bit rgb color mode
}
