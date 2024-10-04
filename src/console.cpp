#include <iostream>
#include <thread>
#include <chrono>

#define FIELD_DIM_X 24
#define FIELD_DIM_Y 8

bool flip = true;

void draw_field()
{
    // 2J = clear screen; 1;1H = move cursor to row 1, column 1
    std::cout << "\033[2J\033[1;1H";

    // draw the game field border
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

    // flip border character to visualize redraw events
    flip = !flip;
}

void console_playground()
{
    // Clear the screen
    std::cout << "\033[2J\033[1;1H"; // Clear the screen and move cursor to (1,1)

    // Print some text
    std::cout << "Hello, World!" << std::endl;

    // Move the cursor to row 5, column 3
    std::cout << "\033[5;3H"; // Move cursor to row 5, column 10
    std::cout << "This is at (5, 3)" << std::endl;

    // Move the cursor up by one line
    std::cout << "\033[A\033[A"; // Move cursor up, twice
    std::cout << "Moved up!" << std::endl;

    // Move the cursor down by one line
    std::cout << "\033[B\033[0G"; // Move cursor down "B" and to the start of the line "0G"
    std::cout << "Moved down!" << std::endl;

    // Move the cursor back to the beginning
    std::cout << "\033[1;1H"; // Move cursor back to (1,1)
}
