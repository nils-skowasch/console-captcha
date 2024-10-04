
#include "console_output.h"


class Meta
{
private:
    int cursor_x = 0;
    int cursor_y = 0;

public:
    void move_cursor_up();
    void move_cursor_down();
    void move_cursor_left();
    void move_cursor_right();

    void Meta::move_cursor_up()
    {
        if (cursor_y > 0)
        {
            cursor_y--;
        }
    }

    void Meta::move_cursor_down()
    {
        if (cursor_y < FIELD_DIM_Y)
        {
            cursor_y++;
        }
    }

    void Meta::move_cursor_left()
    {
        if (cursor_x > 0)
        {
            cursor_x--;
        }
    }

    void Meta::move_cursor_right()
    {
        if (cursor_x < FIELD_DIM_X)
        {
            cursor_x++;
        }
    }
};
