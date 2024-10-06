#ifndef meta_h
#define meta_h

#define FIELD_DIM_X 24
#define FIELD_DIM_Y 8

class Meta {
  private:
    int cursorX = 5;
    int cursorY = 5;
    char gameField[FIELD_DIM_Y - 2][FIELD_DIM_X - 2];
    void initGameField();

  public:
    Meta();
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
    int getCursorX();
    int getCursorY();
    char getGameFieldCharAt(int x, int y);
};

#endif
