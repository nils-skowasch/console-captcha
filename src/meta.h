#ifndef meta_h
#define meta_h

#define FIELD_DIM_X 12
#define FIELD_DIM_Y 8
#define CURSOR_CHAR 'X'
#define WIRE_CHAR '>'
#define TERM_CHAR '='

class Meta {
  private:
    int cursorX = 5;
    int cursorY = 5;
    unsigned char gameField[FIELD_DIM_Y - 2][FIELD_DIM_X - 2];
    void initGameField();

  public:
    Meta();
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
    int getCursorX();
    int getCursorY();
    void placeWire();
    unsigned char getGameFieldCharAt(int x, int y);
};

#endif
