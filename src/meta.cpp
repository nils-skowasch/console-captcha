#include "meta.h"
#include "console_output.h"
#include <cstdlib>
#include <ctime>

void Meta::initGameField() {
    for (int y = 0; y < FIELD_DIM_Y - 2; y++) {
        for (int x = 0; x < FIELD_DIM_X - 2; x++) {
            gameField[y][x] = ' ';
        }
    }
    // apply a seed to randomizer
    std::srand(static_cast<unsigned int>(std::time(0)));    
    // place randomized start wire
    int startY = std::rand() % (FIELD_DIM_Y - 2);
    gameField[startY][0] = WIRE_CHAR;
    // place randomized end node
    int endY = std::rand() % (FIELD_DIM_Y - 2);
    gameField[endY][FIELD_DIM_X - 3] = TERM_CHAR;
}

Meta::Meta() : continueExecution(true), userSurrendered(false) {
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

void Meta::placeWire() {
    if (hasActionsLeft()) {
        gameField[cursorY - 1][cursorX - 1] = WIRE_CHAR;
        actionsLeft--;
    }
}

void Meta::userWantsToSurrender() {
    userSurrendered = true;
    stopExecution();
}

unsigned char Meta::getGameFieldCharAt(int x, int y) {
    return gameField[y][x];
}

bool Meta::isContinueExecution() {
    return continueExecution;
}

void Meta::stopExecution() {
    continueExecution = false;
}

bool Meta::hasActionsLeft() {
    return actionsLeft > 0;
}

bool Meta::hasUserSurrendered() {
    return userSurrendered;
}
