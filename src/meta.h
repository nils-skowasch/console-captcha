#ifndef meta_h
#define meta_h

class Meta {
  private:
    int cursorX = 5;
    int cursorY = 5;

  public:
    Meta();
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
    int getCursorX();
    int getCursorY();
};

#endif
