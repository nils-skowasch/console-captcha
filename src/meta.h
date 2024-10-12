#ifndef meta_h
#define meta_h

#include <atomic>

#define OFFSET_X 28
#define OFFSET_Y 4
#define FIELD_DIM_X 16
#define FIELD_DIM_Y 8
#define CURSOR_CHAR 'X'
#define WIRE_CHAR '>'
#define TERM_CHAR '='

class Meta {
  private:
    std::atomic<bool> continueExecution;
    std::atomic<bool> userSurrendered;
    int cursorX = 5;
    int cursorY = 5;
    unsigned char gameField[FIELD_DIM_Y - 2][FIELD_DIM_X - 2];
    void initGameField();
    int actionsLeft = 16; // is not enough, if the start and end points are too far away ... but some real world captcha's are not solveable either ;)

  public:
    Meta();
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
    int getCursorX();
    int getCursorY();
    void placeWire();
    void userWantsToSurrender();
    unsigned char getGameFieldCharAt(int x, int y);
    bool isContinueExecution();
    void stopExecution();
    bool hasActionsLeft();
    bool hasUserSurrendered();
};

#endif
