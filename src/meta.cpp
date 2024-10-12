#include "meta.h"
#include "color_table.cpp"
#include "console_output.h"
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <typeinfo>

Node::Node(Color color, unsigned char character) : color(color), colorMix(ColorMix::None), character(character){};

Node::Node(ColorMix colorMix, unsigned char character) : color(Color::None), colorMix(colorMix), character(character) {
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

Wire::Wire(int id, Color color) : Node(color, WIRE_CHAR), id(id){};

Wire::Wire(int id, ColorMix colorMix) : Node(colorMix, WIRE_CHAR), id(id){};

Merger::Merger(int x, int y) : Node(Color::None, '+'), x(x), y(y) {
}

Term::Term(ColorMix colorMix) : Node(colorMix, TERM_CHAR){};

WireStart::WireStart(int x, int y, int wireId) : x(x), y(y), wireId(wireId) {
}

int WireStart::getX() {
    return x;
}

int WireStart::getY() {
    return y;
}

int WireStart::getWireId() {
    return wireId;
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
    wireStart0 = new WireStart(0, startY0, wire->getId());
    gameField[startY0][0] = wire;

    // place randomized start wire 1
    int startY1 = startY0;
    while (startY1 == startY0) {
        startY1 = std::rand() % (FIELD_DIM_Y - 2);
    }
    wire = createWire(getRandomColor());
    wireStart1 = new WireStart(0, startY1, wire->getId());
    gameField[startY1][0] = wire;

    // place randomized merger
    int mergerX = (std::rand() % 4) + (FIELD_DIM_X / 2) - 2;
    int mergerY = (std::rand() % 4) + (FIELD_DIM_Y / 2) - 2;
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
        if (node == nullptr || !dynamic_cast<Merger *>(node)) { // the field must be empty or not contain a Merger!
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

int Wire::getId() {
    return id;
}
