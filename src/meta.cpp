#include "meta.h"
#include "console_output.h"

void Meta::initGameField() {
    for (int y = 0; y < FIELD_DIM_Y - 2; y++) {
        for (int x = 0; x < FIELD_DIM_X - 2; x++) {
            gameField[y][x] = '-';
        }
    }
}

Meta::Meta() {
    initGameField();
}

void Meta::moveCursorUp() {
    if (cursorY > 1) {
        cursorY--;
    }
}

void Meta::moveCursorDown() {
    if (cursorY < FIELD_DIM_Y - 2) {
        cursorY++;
    }
}

void Meta::moveCursorLeft() {
    if (cursorX > 1) {
        cursorX--;
    }
}

void Meta::moveCursorRight() {
    if (cursorX < FIELD_DIM_X - 2) {
        cursorX++;
    }
}

int Meta::getCursorX() {
    return cursorX;
}

int Meta::getCursorY() {
    return cursorY;
}

char Meta::getGameFieldCharAt(int x, int y) {
    return gameField[y][x];
}
