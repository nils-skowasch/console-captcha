#ifndef meta_h
#define meta_h

class Meta {
  private:
    int cursor_x = 0;
    int cursor_y = 0;

  public:
    Meta();
    void move_cursor_up();
    void move_cursor_down();
    void move_cursor_left();
    void move_cursor_right();
};

#endif
