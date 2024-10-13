#include <chrono>
#include <iostream>
#include <thread>

#include "ansi_codes.cpp"
#include "../obj/meta.h"

static bool flip = true;

/**
 * Print an intro header to explain the captcha task.
 */
static void printHeader() {
    moveCursor(0, 0);
    setStyle(AnsiStyle::BOLD);
    setStyle(AnsiStyle::UNDERLINED);
    std::cout << "You shall not pass!! Well.. unless you can proove, that you are a human." << std::endl;
    setStyle(AnsiStyle::RESET);
    std::cout << "Please fix the wiring, the AI screwed up!" << std::endl;
    std::cout << "The wires have to get from the entry nodes (" << WIRE_CHAR << ") to the Merger (" << MERGER_CHAR << ") and then to the exit node (" << TERM_CHAR << ")."
              << std::endl;
}

/**
 * Prints the name in colored form at the current cursor position
 */
static void printColorName(RGB &rgb, const char *name) {
    // set ANSI style and color accordingly
    setStyle(AnsiStyle::BOLD);
    setColor(AnsiRgbColorMode::FOREGROUND, rgb.red, rgb.green, rgb.blue);
    setColor(AnsiRgbColorMode::BACKGROUND, 0, 0, 0);

    // print name
    std::cout << name;

    // reset styles
    setStyle(AnsiStyle::RESET);
}

/**
 * Prints the Color name in colored form at the current cursor position
 */
static void printColorName(Color color) {
    RGB rgb = getRGB(color);
    const char *name = getName(color);
    printColorName(rgb, name);
}

/**
 * Prints the ColorMix name in colored form at the current cursor position
 */
static void printColorName(ColorMix colorMix) {
    RGB rgb = getRGB(colorMix);
    const char *name = getName(colorMix);
    printColorName(rgb, name);
}

/**
 * Print a line for the given color
 */
static void printWireColor(int absoluteX, int absoluteY, Color color) {
    moveCursor(absoluteX, absoluteY);
    std::cout << "[" << toChar(color) << "] "; // convert color index to ascii chars A-D
    printColorName(color);
}

/**
 * Print a line for the given color mix
 */
static void printWireColor(int absoluteX, int absoluteY, ColorMix color) {
    moveCursor(absoluteX, absoluteY);
    std::cout << "[" << static_cast<int>(color) << "] ";
    printColorName(color);
}

/**
 * Print the number of action the user (or AI..) can do, until the captcha fails
 */
static void printLeftSide(Meta *meta) {
    int localOffsetX = 0;
    int localOffsetY = 0;

    // print base colors
    moveCursor(localOffsetX, OFFSET_Y + localOffsetY++);
    std::cout << "Select base wire color:";
    for (int i = 0; i < COLOR_COUNT; i++) {
        printWireColor(localOffsetX, OFFSET_Y + localOffsetY++, static_cast<Color>(i + 1));
    }

    localOffsetY++;

    // print remaining wires
    moveCursor(localOffsetX, OFFSET_Y + localOffsetY++);
    std::cout << "Wire left: "; 
    setColor(AnsiStyle::BOLD, AnsiForegroundColor::WHITE, AnsiBackgroundColor::BLACK);
    std::cout << meta->getActionsLeft() << "m";
    setStyle(AnsiStyle::RESET);

    // print selected color (or mix)
    moveCursor(localOffsetX, OFFSET_Y + localOffsetY++);
    std::cout << "Selected color: ";
    ColorMix colorMix = meta->getSelectedColorMix();
    if (colorMix != ColorMix::None) {
        printColorName(colorMix);
        std::cout << " (mix)";
    } else {
        printColorName(meta->getSelectedColor());
    }
}

static void printRightSide(Meta *meta) {
    int localOffsetX = 8;
    int localOffsetY = 0;

    // print mixed colors
    moveCursor(OFFSET_X + FIELD_DIM_X + localOffsetX, OFFSET_Y + localOffsetY++);
    std::cout << "Select mixed wire color:";
    for (int i = 0; i < COLOR_MIX_COUNT; i++) {
        printWireColor(OFFSET_X + FIELD_DIM_X + localOffsetX, OFFSET_Y + localOffsetY++, static_cast<ColorMix>(i + 1));
    }
}

/**
 * Draw the game field border
 */
static void printBorder() {
    setStyle(AnsiStyle::BOLD);
    setColor(AnsiRgbColorMode::BACKGROUND, 80, 80, 80);
    setColor(AnsiRgbColorMode::FOREGROUND, 255, 255, 255);
    for (int y = 0; y < FIELD_DIM_Y; y++) {
        moveCursor(OFFSET_X, OFFSET_Y + y);
        for (int x = 0; x < FIELD_DIM_X; x++) {
            if (y == 0 || x == 0 || x == FIELD_DIM_X - 1 || y == FIELD_DIM_Y - 1) {
                std::cout << '#';
            } else {
                std::cout << ' ';
            }
            if (x == FIELD_DIM_X - 1) {
                std::cout << std::endl;
            }
        }
    }
    setStyle(AnsiStyle::RESET);
}

/**
 * Print the game cursor
 */
static void printCursor(Meta *meta) {
    moveCursor(OFFSET_X + meta->getCursorX(), OFFSET_Y + meta->getCursorY());
    setColor(AnsiStyle::BOLD, AnsiForegroundColor::WHITE, AnsiBackgroundColor::BLACK);
    std::cout << CURSOR_CHAR << std::flush;
    setStyle(AnsiStyle::RESET);
}

/**
 * Print the current state of the game field
 */
static void printGameField(Meta *meta) {
    setColor(AnsiStyle::RESET, AnsiForegroundColor::WHITE, AnsiBackgroundColor::BLACK);
    for (int y = 0; y < FIELD_DIM_Y - 2; y++) {
        moveCursor(OFFSET_X + 1, OFFSET_Y + y + 1);
        for (int x = 0; x < FIELD_DIM_X - 2; x++) {
            Node *node = meta->getGameFieldNodeAt(x, y);
            if (node == nullptr) {
                std::cout << ' ';
            } else {
                // get character and color from node
                unsigned char character = node->getCharacter();
                RGB characterRGB = node->getCharacterRGB();

                // set ANSI style and color accordingly
                setStyle(AnsiStyle::BOLD);
                setColor(AnsiRgbColorMode::FOREGROUND, characterRGB.red, characterRGB.green, characterRGB.blue);
                setColor(AnsiRgbColorMode::BACKGROUND, 0, 0, 0);

                // print character to screen
                std::cout << character;

                // reset ANSI codes
                setColor(AnsiStyle::RESET, AnsiForegroundColor::WHITE, AnsiBackgroundColor::BLACK);
            }
        }
    }
    setStyle(AnsiStyle::RESET);
    std::cout << std::flush; // flush output stream
}

/**
 * Print the legend, with explenations, how to play
 */
static void printLegend() {
    moveCursor(0, OFFSET_Y + FIELD_DIM_Y + 1);
    setStyle(AnsiStyle::BOLD);
    std::cout << "How to captcha:" << std ::endl;
    setStyle(AnsiStyle::RESET);
    std::cout << "Arrow-Keys:\tNavigate the cursor." << std::endl;
    std::cout << "Q:\t\tQuit and surrender ..." << std::endl;
    std::cout << "Space:\t\tPlace a wire at the cursor's position." << std::endl;
}

/**
 * Print the complete output
 */
void printOutput(Meta *meta) {
    hideCursor();
    clearConsole();

    printHeader();

    printLeftSide(meta);
    printBorder();
    printGameField(meta);
    printRightSide(meta);

    printLegend();

    if (flip) {
        printCursor(meta);
    }
    flip = !flip; // flip border character to visualize redraw events
}

/**
 * Erase the legend section of the output
 */
void clearLegend() {
    moveCursor(0, OFFSET_Y + FIELD_DIM_Y + 1);
    clearLine();
    moveCursor(0, OFFSET_Y + FIELD_DIM_Y + 2);
    clearLine();
    moveCursor(0, OFFSET_Y + FIELD_DIM_Y + 3);
    clearLine();
    moveCursor(0, OFFSET_Y + FIELD_DIM_Y + 4);
    clearLine();
}

/**
 * Reset the terminal to the default state
 */
void resetConsole() {
    clearConsole();
    showCursor();
    std::cout << std::flush;
}
