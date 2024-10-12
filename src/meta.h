#ifndef meta_h
#define meta_h

#include "color_table.h"
#include <atomic>
#include <vector>

#define OFFSET_X 28
#define OFFSET_Y 4
#define FIELD_DIM_X 16
#define FIELD_DIM_Y 8
#define CURSOR_CHAR 'X'
#define WIRE_CHAR '>'
#define TERM_CHAR '='
#define USER_ACTIONS 24

class Node {
  private:
    Color color;
    ColorMix colorMix;
    unsigned char character;

  public:
    Node(Color color, unsigned char character);
    Node(ColorMix colorMix, unsigned char character);
    virtual ~Node() = default; // make class polymorphic
    unsigned char getCharacter();
    RGB getCharacterRGB();
};

class Wire : public Node {
  private:
    int id;

  public:
    Wire(int id, Color color);
    Wire(int id, ColorMix colorMix);
    int getId();
};

class Term : public Node {
  public:
    Term(ColorMix colorMix);
};

class StartWire {
  private:
    int x;
    int y;
    int wireId;
  public:
    StartWire(int x, int y, int wireId);
    int getX();
    int getY();
    int getWireId();
};

class Meta {
  private:
    // threading
    std::atomic<bool> continueExecution = true;
    std::atomic<bool> userSurrendered = false;
    // cursor
    int cursorX = 5;
    int cursorY = 5;
    // gamefield wiring
    Node *gameField[FIELD_DIM_Y - 2][FIELD_DIM_X - 2];
    int actionsLeft = USER_ACTIONS; // is not enough, if the start and end points are too far away ... but some real world
                          // captcha's are not solveable either ;)
    StartWire *startWire0 = nullptr;
    StartWire *startWire1 = nullptr;
    std::vector<Wire *> wires = {};
    Color selectedColor = Color::None;
    Term term = Term(ColorMix::None);
    // methods
    void initGameField();
    Wire *createWire(Color color);

  public:
    Meta();
    ~Meta();
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
    int getCursorX();
    int getCursorY();
    int getActionsLeft();
    void placeWire();
    void userWantsToSurrender();
    Node *getGameFieldNodeAt(int x, int y);
    bool isContinueExecution();
    void stopExecution();
    bool hasActionsLeft();
    bool hasUserSurrendered();
    StartWire* getStartWire0();
    StartWire* getStartWire1();
};

#endif
