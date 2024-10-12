#include "meta.h"
#include "color_table.cpp"
#include "console_output.h"
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <typeinfo>

Node::Node(unsigned char character, Color color) : character(character), colorMix(ColorMix::None), color(color){};

Node::Node(unsigned char character, ColorMix colorMix) : character(character), colorMix(colorMix), color(Color::None) {
}

unsigned char Node::getCharacter() {
    return character;
}

RGB Node::getCharacterRGB() {
    if (color != Color::None) {
        return getRGB(color);
    }
    return getRGB(colorMix);
}

bool Node::hasMatchingRGB(RGB rgb) {
    return rgb.equals(getRGB(color)) || rgb.equals(getRGB(colorMix));
}

Wire::Wire(int id, Color color) : Node(WIRE_CHAR, color), id(id){};

Wire::Wire(int id, ColorMix colorMix) : Node(WIRE_CHAR, colorMix), id(id){};

Merger::Merger(int x, int y) : Node(MERGER_CHAR, Color::None), x(x), y(y) {
}

int Merger::getX() {
    return x;
}

int Merger::getY() {
    return y;
}

Term::Term(ColorMix colorMix) : Node(TERM_CHAR, colorMix){};

WireStart::WireStart(int x, int y, Wire *wire) : x(x), y(y), wire(wire) {
}

int WireStart::getX() {
    return x;
}

int WireStart::getY() {
    return y;
}

Wire *WireStart::getWire() {
    return wire;
}

void Meta::initGameField() {
    for (int y = 0; y < FIELD_DIM_Y - 2; y++) {
        for (int x = 0; x < FIELD_DIM_X - 2; x++) {
            gameField[y][x] = nullptr;
        }
    }

    // apply a seed to randomizer
    std::srand(static_cast<unsigned int>(std::time(0)));

    // place randomized start wire 0
    int startY0 = std::rand() % (FIELD_DIM_Y - 2);
    Wire *wire = createWire(getRandomColor());
    wireStart0 = new WireStart(0, startY0, wire);
    gameField[startY0][0] = wire;

    // place randomized start wire 1
    int startY1 = startY0;
    while (startY1 == startY0) {
        startY1 = std::rand() % (FIELD_DIM_Y - 2);
    }
    wire = createWire(getRandomColor());
    wireStart1 = new WireStart(0, startY1, wire);
    gameField[startY1][0] = wire;

    // place randomized merger
    int mergerX = (std::rand() % 4) + (FIELD_DIM_X / 4);
    int mergerY = (std::rand() % 2) + (FIELD_DIM_Y / 4);
    merger = new Merger(mergerX, mergerY);
    gameField[mergerY][mergerX] = merger;

    // place randomized end node
    int endY = std::rand() % (FIELD_DIM_Y - 2);
    gameField[endY][FIELD_DIM_X - 3] = &term;
}

Wire *Meta::createWire(Color color) {
    // create new Wire object
    int wireCount = wires.size();
    Wire *wire = new Wire(wireCount, color);
    // push to vector
    wires.push_back(wire);
    return wire;
}

Wire *Meta::createWire(ColorMix colorMix) {
    // create new Wire object
    int wireCount = wires.size();
    Wire *wire = new Wire(wireCount, colorMix);
    // push to vector
    wires.push_back(wire);
    return wire;
}

Meta::Meta() {
    initGameField();
}

Meta::~Meta() {
    // delete all StartWire objects
    if (wireStart0 != nullptr) {
        delete wireStart0;
        wireStart0 = nullptr;
    }
    if (wireStart1 != nullptr) {
        delete wireStart1;
        wireStart1 = nullptr;
    }
    // delete merger
    if (merger != nullptr) {
        delete merger;
        merger = nullptr;
    }
    // delete all wires
    for (Wire *wire : wires) {
        delete wire;
    }
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

int Meta::getActionsLeft() {
    return actionsLeft;
}

void Meta::placeWire() {
    if (hasActionsLeft()) {
        int fieldX = cursorX - 1;
        int fieldY = cursorY - 1;
        Node *node = gameField[fieldY][fieldX];
        if (node == nullptr || (!dynamic_cast<Merger *>(node) &&
                                !dynamic_cast<Term *>(node))) { // the field must be empty or neither contain a Merger nor Terminus!
            if (selectedColorMix != ColorMix::None) {
                gameField[fieldY][fieldX] = createWire(selectedColorMix);
            } else {
                gameField[fieldY][fieldX] = createWire(selectedColor);
            }
            actionsLeft--;
        }
    }
}

void Meta::userWantsToSurrender() {
    userSurrendered = true;
    stopExecution();
}

Node *Meta::getGameFieldNodeAt(int x, int y) {
    Node *node = gameField[y][x];
    if (node == nullptr) {
        return nullptr;
    } else {
        if (!dynamic_cast<Node *>(node)) {
            throw std::runtime_error("Pointer is not pointing to a Node!");
        }
        return node;
    }
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

WireStart *Meta::getWireStart0() {
    return wireStart0;
}

WireStart *Meta::getWireStart1() {
    return wireStart1;
}

Merger *Meta::getMerger() {
    return merger;
}

Color Meta::getSelectedColor() {
    return selectedColor;
}

ColorMix Meta::getSelectedColorMix() {
    return selectedColorMix;
}

void Meta::setSelectedColor(Color color) {
    selectedColor = color;
    selectedColorMix = ColorMix::None;
}

void Meta::setSelectedColorMix(ColorMix colorMix) {
    selectedColor = Color::None;
    selectedColorMix = colorMix;
}

bool Meta::isInGameFieldDimension(int x, int y) {
    return x >= 0 && x < (FIELD_DIM_X - 2) && y >= 0 && y < (FIELD_DIM_Y - 2);
}

int Wire::getId() {
    return id;
}

Color Wire::getColor() {
    return color;
}
