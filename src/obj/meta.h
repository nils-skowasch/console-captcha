#ifndef meta_h
#define meta_h

#include "../io/color_table.h"
#include <atomic>
#include <vector>

#define OFFSET_X 28
#define OFFSET_Y 4
#define FIELD_DIM_X 16
#define FIELD_DIM_Y 8
#define CURSOR_CHAR 'X'
#define WIRE_CHAR '>'
#define MERGER_CHAR '+'
#define TERM_CHAR '='
#define USER_ACTIONS 24

class Node {
  private:
    unsigned char character;
    ColorMix colorMix;

  protected:
    Color color;

  public:
    Node(unsigned char character, Color color);
    Node(unsigned char character, ColorMix colorMix);
    virtual ~Node() = default; // make class polymorphic
    unsigned char getCharacter();
    RGB getCharacterRGB();
    bool hasMatchingRGB(RGB rgb);
};

class Wire : public Node {
  private:
    int id;

  public:
    Wire(int id, Color color);
    Wire(int id, ColorMix colorMix);
    int getId();
    Color getColor();
};

class WireStart {
  private:
    int x;
    int y;
    Wire *wire;

  public:
    WireStart(int x, int y, Wire *wire);
    int getX();
    int getY();
    Wire *getWire();
};

class Merger : public Node {
  private:
    int x;
    int y;

  public:
    Merger(int x, int y);
    int getX();
    int getY();
};

class Term : public Node {
  public:
    Term(ColorMix colorMix);
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
    int actionsLeft = USER_ACTIONS; // is not enough, if the start and end points are too far away ... but some real
                                    // world captcha's are not solveable either ;)
    // the two entry wires (heap)
    WireStart *wireStart0 = nullptr;
    WireStart *wireStart1 = nullptr;
    // merger object
    Merger *merger = nullptr;
    // terminus object
    Term term = Term(ColorMix::None);
    // created wire objects (heap)
    std::vector<Wire *> wires = {};
    // user selects colors
    Color selectedColor = Color::None;
    ColorMix selectedColorMix = ColorMix::None;
    // methods
    void initGameField();
    Wire *createWire(Color color);
    Wire *createWire(ColorMix colorMix);

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
    WireStart *getWireStart0();
    WireStart *getWireStart1();
    Merger *getMerger();
    Color getSelectedColor();
    ColorMix getSelectedColorMix();
    void setSelectedColor(Color color);
    void setSelectedColorMix(ColorMix colorMix);
    bool isInGameFieldDimension(int x, int y);
};

#endif
