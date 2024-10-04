#include <iostream>

#define FIELD_DIM_X 48
#define FIELD_DIM_Y 16

bool flip = true;

void draw_field()
{
    for (int y = 0; y < FIELD_DIM_Y; y++)
    {
        for (int x = 0; x < FIELD_DIM_X; x++)
        {
            if (y == 0 || x == 0 || x == FIELD_DIM_X - 1 || y == FIELD_DIM_Y - 1)
            {
                std::cout << (flip ? "#" : "+");
            }
            else
            {
                std::cout << " ";
            }
            if (x == FIELD_DIM_X - 1)
            {
                std::cout << std::endl;
            }
        }
    }
    flip = !flip;
}
